#pragma once

#include QMK_KEYBOARD_H

// Flow system: 5 states for oneshot keys with timer support
typedef enum {
    os_up_unqueued,
    os_up_queued,
    os_down_unused,
    os_down_used,
    os_up_queued_used,  // New state: queued but already used (waiting for timeout)
} oneshot_state;

// Per-modifier timer struct (PR #16174 inspired)
// Each oneshot modifier/layer gets independent timers to prevent interference
typedef struct {
    uint16_t wait_timer;      // Hold detection: started on keydown, checked on keyup
    uint16_t timeout_timer;   // Auto-timeout: started when queued, checked in matrix_scan
    bool timeout_active;      // Flag: is timeout timer running?
} oneshot_timers_t;

// Timer configuration defaults (can be overridden in config.h)
#ifndef FLOW_ONESHOT_TERM
#define FLOW_ONESHOT_TERM 500  // Auto-release after 500ms
#endif

#ifndef FLOW_ONESHOT_WAIT_TERM
#define FLOW_ONESHOT_WAIT_TERM 500  // Hold detection: >500ms = normal mod
#endif

// Original Callum oneshot implementation (no timers, no auto-timeout)
// Pure sticky behavior: tap queues modifier until next key, no timeout
// Perfect for shift with caps word integration
void update_oneshot_callum(
    oneshot_state *state,
    uint16_t mod,
    uint16_t trigger,
    uint16_t keycode,
    keyrecord_t *record
);

// Flow system oneshot mod implementation with independent timer support
// Adds auto-timeout and hold detection to prevent accidental oneshots
void update_oneshot(
    oneshot_state *state,
    oneshot_timers_t *timers,  // Independent timer struct for this modifier
    uint16_t mod,
    uint16_t trigger,
    uint16_t keycode,
    keyrecord_t *record
);

// Flow system oneshot layer implementation with layer origin detection
// Only consumes oneshot when pressing keys FROM the oneshot layer
void update_oneshot_layer(
    oneshot_state *state,
    oneshot_timers_t *timers,  // Independent timer struct for this layer
    uint8_t layer,
    uint16_t trigger,
    uint16_t keycode,
    keyrecord_t *record
);

// Check if oneshot modifier should timeout (call from matrix_scan_user)
bool check_oneshot_timeout(oneshot_state *state, oneshot_timers_t *timers, uint16_t mod);

// Check if oneshot layer should timeout (call from matrix_scan_user)
bool check_oneshot_layer_timeout(oneshot_state *state, oneshot_timers_t *timers, uint8_t layer);

// To be implemented by the consumer. Defines keys to cancel oneshot mods.
bool is_oneshot_cancel_key(uint16_t keycode);

// To be implemented by the consumer. Defines keys to ignore when determining
// whether a oneshot mod has been used. Setting this to modifiers and layer
// change keys allows stacking multiple oneshot modifiers, and carrying them
// between layers.
bool is_oneshot_ignored_key(uint16_t keycode);
