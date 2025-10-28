#include "oneshot.h"

// ============================================================================
// ORIGINAL CALLUM ONESHOT IMPLEMENTATION (No Timers)
// ============================================================================
// Pure sticky behavior: tap queues modifier until consumed by next keypress
// No timeout, no hold detection - simple and predictable
// Perfect for shift with caps word double-tap integration

void update_oneshot_callum(
    oneshot_state *state,
    uint16_t mod,
    uint16_t trigger,
    uint16_t keycode,
    keyrecord_t *record
) {
    // Special handling for shift + caps word integration
    // If caps word just activated (double-tap shift), clear oneshot state
    if (keycode == trigger && mod == KC_LSFT && !record->event.pressed) {
        if (is_caps_word_on()) {
            *state = os_up_unqueued;
            unregister_code(mod);
            return;
        }
    }

    if (keycode == trigger) {
        if (record->event.pressed) {
            // Trigger keydown
            if (*state == os_up_unqueued) {
                register_code(mod);
            }
            *state = os_down_unused;
        } else {
            // Trigger keyup
            switch (*state) {
            case os_down_unused:
                // Not used while held - queue oneshot
                *state = os_up_queued;
                break;
            case os_down_used:
                // Used while held - release normally, no oneshot
                *state = os_up_unqueued;
                unregister_code(mod);
                break;
            default:
                break;
            }
        }
    } else {
        if (record->event.pressed) {
            // KEYDOWN of other key
            if (is_oneshot_cancel_key(keycode) && *state != os_up_unqueued) {
                // Cancel oneshot on designated cancel keydown
                *state = os_up_unqueued;
                unregister_code(mod);
            } else if (!is_oneshot_ignored_key(keycode)) {
                // Mark as "will be consumed" immediately on KEYDOWN (fixes fast typing)
                if (*state == os_down_unused) {
                    *state = os_down_used;
                } else if (*state == os_up_queued) {
                    // First key pressed after oneshot - mark as used
                    *state = os_up_queued_used;
                }
            }
        } else {
            // KEYUP of other key
            if (!is_oneshot_ignored_key(keycode)) {
                // Release modifier on keyup if marked as used
                if (*state == os_up_queued_used) {
                    *state = os_up_unqueued;
                    unregister_code(mod);
                }
            }
        }
    }
}

// ============================================================================
// ONESHOT MODIFIER IMPLEMENTATION (PR #16174 Inspired)
// ============================================================================
// Each modifier gets independent timers to prevent interference
// Dual timer system: wait_timer (hold detection) + timeout_timer (auto-cancel)

void update_oneshot(
    oneshot_state *state,
    oneshot_timers_t *timers,
    uint16_t mod,
    uint16_t trigger,
    uint16_t keycode,
    keyrecord_t *record
) {
    if (keycode == trigger) {
        if (record->event.pressed) {
            // Trigger keydown
            if (*state == os_up_unqueued) {
                register_code(mod);
            }
            *state = os_down_unused;
            timers->wait_timer = timer_read();  // Start wait timer for hold detection
        } else {
            // Trigger keyup
            uint16_t hold_time = timer_elapsed(timers->wait_timer);

            // Hold detection: if held >FLOW_ONESHOT_WAIT_TERM, treat as normal mod
            if (hold_time > FLOW_ONESHOT_WAIT_TERM) {
                *state = os_up_unqueued;
                unregister_code(mod);
            } else {
                switch (*state) {
                case os_down_unused:
                    // Quick tap - queue oneshot
                    *state = os_up_queued;
                    timers->timeout_timer = timer_read();  // Start timeout timer
                    timers->timeout_active = true;         // Enable auto-timeout
                    break;
                case os_down_used:
                    // Used while held - release normally
                    *state = os_up_unqueued;
                    unregister_code(mod);
                    break;
                default:
                    break;
                }
            }
        }
    } else {
        if (record->event.pressed) {
            // KEYDOWN of other key
            if (is_oneshot_cancel_key(keycode) && *state != os_up_unqueued) {
                // Cancel oneshot on designated cancel keydown
                *state = os_up_unqueued;
                unregister_code(mod);
                timers->timeout_active = false;
            } else if (!is_oneshot_ignored_key(keycode)) {
                // Mark as "will be consumed" immediately on KEYDOWN (fixes fast typing)
                if (*state == os_down_unused) {
                    *state = os_down_used;
                } else if (*state == os_up_queued) {
                    // First key pressed after oneshot - mark as used, disable timeout
                    *state = os_up_queued_used;
                    timers->timeout_active = false;
                }
            }
        } else {
            // KEYUP of other key
            if (!is_oneshot_ignored_key(keycode)) {
                // Release modifier on keyup if marked as used
                if (*state == os_up_queued_used) {
                    *state = os_up_unqueued;
                    unregister_code(mod);
                }
            }
        }
    }
}

// Check if oneshot modifier should timeout (call from matrix_scan_user)
bool check_oneshot_timeout(oneshot_state *state, oneshot_timers_t *timers, uint16_t mod) {
    if (timers->timeout_active &&
        (*state == os_up_queued || *state == os_up_queued_used) &&
        timer_elapsed(timers->timeout_timer) > FLOW_ONESHOT_TERM) {
        *state = os_up_unqueued;
        unregister_code(mod);
        timers->timeout_active = false;
        return true;
    }
    return false;
}

// ============================================================================
// ONESHOT LAYER IMPLEMENTATION (PR #16174 Inspired + Layer Origin Detection)
// ============================================================================
// Only consumes oneshot when pressing keys FROM the oneshot layer

void update_oneshot_layer(
    oneshot_state *state,
    oneshot_timers_t *timers,
    uint8_t layer,
    uint16_t trigger,
    uint16_t keycode,
    keyrecord_t *record
) {
    if (keycode == trigger) {
        if (record->event.pressed) {
            // Trigger keydown
            if (*state == os_up_unqueued) {
                layer_on(layer);
            }
            *state = os_down_unused;
            timers->wait_timer = timer_read();  // Start wait timer
        } else {
            // Trigger keyup
            uint16_t hold_time = timer_elapsed(timers->wait_timer);

            // Hold detection: if held >FLOW_ONESHOT_WAIT_TERM, treat as normal layer
            if (hold_time > FLOW_ONESHOT_WAIT_TERM) {
                *state = os_up_unqueued;
                layer_off(layer);
            } else {
                switch (*state) {
                case os_down_unused:
                    // Quick tap - queue oneshot layer
                    *state = os_up_queued;
                    timers->timeout_timer = timer_read();
                    timers->timeout_active = true;
                    break;
                case os_down_used:
                    // Used while held - release normally
                    *state = os_up_unqueued;
                    layer_off(layer);
                    break;
                default:
                    break;
                }
            }
        }
    } else {
        if (record->event.pressed) {
            // KEYDOWN of other key
            if (is_oneshot_cancel_key(keycode) && *state != os_up_unqueued) {
                // Cancel oneshot layer
                *state = os_up_unqueued;
                layer_off(layer);
                timers->timeout_active = false;
            } else if (!is_oneshot_ignored_key(keycode)) {
                // Layer origin detection: check if key is FROM the oneshot layer
                uint8_t source_layer = read_source_layers_cache(record->event.key);

                // Only consume oneshot if key is from the oneshot layer
                // Mark as "will be consumed" immediately on KEYDOWN (fixes fast typing)
                if (source_layer == layer) {
                    if (*state == os_down_unused) {
                        *state = os_down_used;
                    } else if (*state == os_up_queued) {
                        // First key pressed from layer - mark as used, disable timeout
                        *state = os_up_queued_used;
                        timers->timeout_active = false;
                    }
                }
                // If source_layer != layer, don't consume oneshot (layer stays active)
            }
        } else {
            // KEYUP of other key
            if (!is_oneshot_ignored_key(keycode)) {
                // Layer origin detection: check if key is FROM the oneshot layer
                uint8_t source_layer = read_source_layers_cache(record->event.key);

                // Only release if key is from the oneshot layer and marked as used
                if (source_layer == layer && *state == os_up_queued_used) {
                    *state = os_up_unqueued;
                    layer_off(layer);
                }
            }
        }
    }
}

// Check if oneshot layer should timeout (call from matrix_scan_user)
bool check_oneshot_layer_timeout(oneshot_state *state, oneshot_timers_t *timers, uint8_t layer) {
    if (timers->timeout_active &&
        (*state == os_up_queued || *state == os_up_queued_used) &&
        timer_elapsed(timers->timeout_timer) > FLOW_ONESHOT_TERM) {
        *state = os_up_unqueued;
        layer_off(layer);
        timers->timeout_active = false;
        return true;
    }
    return false;
}
