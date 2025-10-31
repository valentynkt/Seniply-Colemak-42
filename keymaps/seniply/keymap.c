#include QMK_KEYBOARD_H
#include "oneshot.h"

// Layer definitions
enum layers {
    _BASE = 0,
    _EXTEND,
    _SYM,
    _NUM,
    _FUN,
};

// Custom keycodes
enum custom_keycodes {
    // Dual-function FUN layer access (moved to top - far from MY_SAVE to prevent enum collision)
    FUN_KEY = SAFE_RANGE,  // Tap = one-shot FUN, Hold = momentary FUN

    // One-shot modifiers (Flow system with timers)
    OS_SHFT,
    OS_CTRL,
    OS_ALT,
    OS_GUI,
    OS_ALTGR,  // AltGr / Right Alt (custom name to avoid QMK conflict)

    // Oneshot layers (Flow system)
    OS_FUN,    // Oneshot FUN layer - tap once, use once

    // Clipboard keys (macOS - hardcoded)
    MY_COPY,   // Cmd+C
    MY_PASTE,  // Cmd+V
    MY_CUT,    // Cmd+X

    // Undo/Redo (macOS - hardcoded)
    MY_UNDO,   // Cmd+Z
    MY_REDO,   // Cmd+Shift+Z

    // Common shortcuts (macOS - hardcoded)
    MY_SAVE,   // Cmd+S
};

// Layer lock key (provided by QMK)
// QK_LAYER_LOCK (alias QK_LLCK) locks the current layer when pressed
#define LLOCK QK_LLCK

// Layer tap keys
#define ESC_EXT LT(_EXTEND, KC_ESC)  // Esc taps to Esc, hold for EXTEND layer
#define TAB_SYM LT(_SYM, KC_TAB)     // Tab taps to Tab, hold for SYM layer

// Vim line navigation keys
#define VIM_END KC_DLR   // $ - end of line
#define VIM_STA KC_0     // 0 - start of line
#define VIM_FST KC_CIRC  // ^ - first non-blank

// Hyper and Meh modifiers (simple hold keys in outer columns)
#define MY_HYPR KC_HYPR  // All modifiers: Ctrl+Shift+Alt+Gui
#define MY_MEH  KC_MEH   // Ctrl+Shift+Alt

// ============================================================================
// KEYMAP LAYOUT - 42 keys (3x6+3 split)
// ============================================================================

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // ========================================================================
    // Layer 0: BASE (Gallium Colstag xq swap with j↔z and c↔w)
    // ========================================================================
    // Left: B L D W V | N R T S G | Q X M C J
    // Right: Z Y O U , | P H A E I | K F ' / .
    // Outer columns: --/DEL/HYPR (left), FUN/--/MEH (right)
    // Thumbs: OS_SHFT (Callum) | SPACE (normal) | ESC→EXTEND (hold)  ---  TAB→SYM (hold) | BSPC | ENTER
    // Note: OS_SHFT uses Callum implementation (no timeout, double-tap for caps word)
    [_BASE] = LAYOUT_split_3x6_3(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
      KC_NO,   KC_B,    KC_L,    KC_D,    KC_W,    KC_V,                         KC_Z,    KC_Y,    KC_O,    KC_U,    KC_COMM, FUN_KEY,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      MY_HYPR, KC_N,    KC_R,    KC_T,    KC_S,    KC_G,                         KC_P,    KC_H,    KC_A,    KC_E,    KC_I,    MY_MEH,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_DEL,  KC_Q,    KC_X,    KC_M,    KC_C,    KC_J,                         KC_K,    KC_F,    KC_QUOT, KC_SLSH, KC_DOT,  KC_NO,
  //└────────┴────────┴────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┴────────┴────────┘
                                 OS_SHFT, KC_SPC,  ESC_EXT,                      TAB_SYM, KC_BSPC, KC_ENT
  //                            └────────┴────────┴────────┘                    └────────┴────────┴────────┘
    ),

    // ========================================================================
    // Layer 1: EXTEND (Navigation - Seniply JSON-aligned)
    // ========================================================================
    // Left: Esc(outer), Ctrl+F, Vim($^0), INS, one-shot mods R=Alt/S=Gui/T=Ctrl/D=AltGr
    //       A=Save (Cmd+S/Ctrl+S) - prioritized over Shift (BASE OS_SHFT accessible via left thumb)
    // Right: EXACT Seniply JSON - PgUp/Home/↑/End/Caps, PgDn/←/↓/→/Del, Ent/Bspc/Tab/F13/Prt
    // Clipboard: Undo/Cut/Copy/Paste/Redo on bottom row
    [_EXTEND] = LAYOUT_split_3x6_3(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
      KC_ESC,  LCTL(KC_F),VIM_END,VIM_STA,VIM_FST, KC_INS,                       KC_PGUP, KC_HOME, KC_UP,   KC_END,  KC_CAPS, FUN_KEY,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      MY_HYPR, MY_SAVE, OS_ALT,  OS_GUI,  OS_CTRL, OS_ALTGR,                     KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_DEL,  MY_MEH,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      LLOCK,   MY_UNDO, MY_CUT,  MY_COPY, MY_PASTE,MY_REDO,                      KC_ENT,  KC_BSPC, KC_TAB,  KC_F13,  KC_PSCR, LLOCK,
  //└────────┴────────┴────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┴────────┴────────┘
                                 _______, _______, _______,                      _______, _______, _______
  //                            └────────┴────────┴────────┘                    └────────┴────────┴────────┘
    ),

    // ========================================================================
    // Layer 2: SYM (Symbols - Seniply-aligned)
    // ========================================================================
    // Left: Shifted numbers (top), one-shot mods A=Shift/R=Alt/S=Gui/T=Ctrl (consistent), symbols (bottom)
    //       Bottom row cleaned: no duplicates - only unique symbols \ | &
    // Right: Exact Seniply layout - = ` : ; + / * ( { [ - / ~ ) } ] _
    // Operators = * + - match NUM layer positions
    [_SYM] = LAYOUT_split_3x6_3(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
      KC_ESC,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                      KC_EQL,  KC_GRV,  KC_COLN, KC_SCLN, KC_PLUS, FUN_KEY,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      MY_HYPR, OS_SHFT, OS_ALT,  OS_GUI,  OS_CTRL, KC_CIRC,                      KC_ASTR, KC_LPRN, KC_LCBR, KC_LBRC, KC_MINS, MY_MEH,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      LLOCK,   KC_NO,   KC_NO,   KC_BSLS, KC_PIPE, KC_AMPR,                      KC_TILD, KC_RPRN, KC_RCBR, KC_RBRC, KC_UNDS, LLOCK,
  //└────────┴────────┴────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┴────────┴────────┘
                                 _______, _______, _______,                      _______, _______, _______
  //                            └────────┴────────┴────────┘                    └────────┴────────┴────────┘
    ),

    // ========================================================================
    // Layer 3: NUM (Numpad - Seniply-aligned) - Tri-layer EXTEND+SYM
    // ========================================================================
    // Left: Clean numpad support - space, . , _ on top, modifiers, utilities on bottom
    //       Top: Space, dot/comma for decimals (1.5, 1,000), underscore for literals (1_000_000)
    //       Home: A=Shift/R=Alt/S=Gui/T=Ctrl/D=AltGr - all 5 modifiers (thumbs occupied in tri-layer)
    //       Bottom: F13, TAB(C), ENTER(V), BSPC(B) - Seniply-aligned editing keys
    // Right: Standard numpad - = 7 8 9 + / * 4 5 6 - / 0 1 2 3 /
    [_NUM] = LAYOUT_split_3x6_3(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
      KC_ESC,  KC_SPC,  KC_DOT,  KC_COMM, KC_UNDS, KC_NO,                        KC_EQL,  KC_7,    KC_8,    KC_9,    KC_PLUS, FUN_KEY,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      MY_HYPR, OS_SHFT, OS_ALT,  OS_GUI,  OS_CTRL, OS_ALTGR,                     KC_ASTR, KC_4,    KC_5,    KC_6,    KC_MINS, MY_MEH,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      LLOCK,   KC_NO,   KC_F13,  KC_TAB,  KC_ENT,  KC_BSPC,                      KC_0,    KC_1,    KC_2,    KC_3,    KC_SLSH, LLOCK,
  //└────────┴────────┴────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┴────────┴────────┘
                                 _______, _______, _______,                      _______, _______, _______
  //                            └────────┴────────┴────────┘                    └────────┴────────┴────────┘
    ),

    // ========================================================================
    // Layer 4: FUN (F-keys + Media - Seniply-aligned)
    // ========================================================================
    // Left: Media controls (bottom), one-shot mods (home row) - consistent with EXTEND/SYM/NUM
    //       Home row: A=Shift/R=Alt/S=Gui/T=Ctrl/D=AltGr - all 5 modifiers for consistency
    // Right: Numpad-style F-keys (F7-9, F4-6, F1-3)
    [_FUN] = LAYOUT_split_3x6_3(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                        KC_NO,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      MY_HYPR, OS_SHFT, OS_ALT,  OS_GUI,  OS_CTRL, OS_ALTGR,                     KC_NO,   KC_F4,   KC_F5,   KC_F6,   KC_F11,  MY_MEH,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      LLOCK,   KC_MPRV, KC_MPLY, KC_MNXT, KC_VOLU, KC_VOLD,                      KC_NO,   KC_F1,   KC_F2,   KC_F3,   KC_F12,  LLOCK,
  //└────────┴────────┴────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┴────────┴────────┘
                                 _______, _______, _______,                      _______, _______, _______
  //                            └────────┴────────┴────────┘                    └────────┴────────┴────────┘
    ),
};

// ============================================================================
// ONESHOT IMPLEMENTATION (Callum style, no timers)
// ============================================================================
//
// Why Callum style instead of QMK's OSM():
// - NO TIMEOUT: Modifiers queue until used or cancelled (not 3 seconds)
// - More predictable: No surprise timeout while thinking
// - Stackable: Can activate multiple one-shot mods (Shift+Ctrl+A)
// - Layer-aware: Modifiers persist across layer switches
//
// Behavior:
// - Tap modifier: Queues modifier for next non-modifier key
// - Hold modifier: Acts as normal modifier while held
// - Cancel: Press layer-tap key (ESC_EXT or TAB_SYM) to cancel queued mods
// - Ignored keys: Other modifiers and layer switches don't consume the mod
//
// Example: Tap OS_CTRL, switch to EXTEND layer, press C = Ctrl+C (copy)

// Initialize oneshot states
oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_gui_state = os_up_unqueued;
oneshot_state os_altgr_state = os_up_unqueued;

// Flow system oneshot layer states
oneshot_state os_fun_state = os_up_unqueued;

// Per-modifier independent timer structs (PR #16174 inspired)
// Prevents timer interference between multiple oneshot modifiers
// Note: os_shft uses Callum implementation (no timers needed)
static oneshot_timers_t os_ctrl_timers = {0};
static oneshot_timers_t os_alt_timers = {0};
static oneshot_timers_t os_gui_timers = {0};
static oneshot_timers_t os_altgr_timers = {0};
static oneshot_timers_t os_fun_timers = {0};

// ============================================================================
// FUN_KEY DUAL-FUNCTION STATE (Tap = one-shot, Hold = momentary)
// ============================================================================
// Tap FUN_KEY = one-shot FUN layer (stays active until next key release)
// Hold FUN_KEY = momentary FUN layer (active while held)

static bool fun_key_held = false;
static bool fun_oneshot_active = false;
static uint16_t fun_key_timer = 0;

// Define keys that cancel oneshot mods
// Note: Layer-tap keys (ESC_EXT, TAB_SYM) are NOT cancel keys
// They should consume oneshot mods when tapped, not cancel them
bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
    // Currently no keys cancel oneshot mods
    // Oneshot mods timeout automatically after FLOW_ONESHOT_TERM (500ms)
    default:
        return false;
    }
}

// Define keys that are ignored by oneshot (don't consume the mod)
// This allows stacking multiple one-shot modifiers and carrying them between layers
bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    // Oneshot modifiers (allow stacking)
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_GUI:
    case OS_ALTGR:
    case KC_LSFT:
    case KC_RSFT:
    case KC_LCTL:
    case KC_RCTL:
    case KC_LALT:
    case KC_RALT:
    case KC_LGUI:
    case KC_RGUI:
    // Oneshot layers (allow carrying mods to other layers)
    case OS_FUN:
    // Layer-tap keys (allow carrying mods to other layers)
    case ESC_EXT:
    case TAB_SYM:
    // Other layer switching keys (FUN_KEY and LLOCK don't send characters, so safe to ignore)
    case FUN_KEY:
    case LLOCK:
        return true;
    default:
        return false;
    }
}

// ============================================================================
// PROCESS RECORD USER (Custom key handling)
// ============================================================================

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // ========================================================================
    // LLOCK (QMK Layer Lock) - Pass through to QMK for native handling
    // ========================================================================
    // QMK's layer_lock: Press once to lock current layer, press again to unlock
    // This is NOT a oneshot - it's a persistent lock/unlock toggle
    if (keycode == LLOCK) {
        return true;  // Let QMK's process_layer_lock() handle it
    }

    // ========================================================================
    // ONESHOT SYSTEM (Custom implementation)
    // ========================================================================
    // Shift uses original Callum implementation (no timers, pure sticky behavior)
    update_oneshot_callum(&os_shft_state, KC_LSFT, OS_SHFT, keycode, record);

    // Other modifiers use PR #16174 implementation (with timers and auto-timeout)
    update_oneshot(&os_ctrl_state, &os_ctrl_timers, KC_LCTL, OS_CTRL, keycode, record);
    update_oneshot(&os_alt_state, &os_alt_timers, KC_LALT, OS_ALT, keycode, record);
    update_oneshot(&os_gui_state, &os_gui_timers, KC_LGUI, OS_GUI, keycode, record);
    update_oneshot(&os_altgr_state, &os_altgr_timers, KC_RALT, OS_ALTGR, keycode, record);

    // Oneshot layer with PR #16174 (independent timer and layer origin detection)
    update_oneshot_layer(&os_fun_state, &os_fun_timers, _FUN, OS_FUN, keycode, record);

    // Prevent oneshot trigger keys from being processed further by QMK
    // The update_oneshot* functions handle the modifier registration/unregistration
    switch (keycode) {
        case OS_SHFT:
        case OS_CTRL:
        case OS_ALT:
        case OS_GUI:
        case OS_ALTGR:
        case OS_FUN:
            return false;  // Don't let QMK process these custom keycodes
    }

    // Handle custom keycodes
    switch (keycode) {
        // ====================================================================
        // macOS clipboard & shortcut operations
        // ====================================================================
        case MY_COPY:
            if (record->event.pressed) {
                tap_code16(LGUI(KC_C));  // Cmd+C
            }
            return false;

        case MY_PASTE:
            if (record->event.pressed) {
                tap_code16(LGUI(KC_V));  // Cmd+V
            }
            return false;

        case MY_CUT:
            if (record->event.pressed) {
                tap_code16(LGUI(KC_X));  // Cmd+X
            }
            return false;

        case MY_UNDO:
            if (record->event.pressed) {
                tap_code16(LGUI(KC_Z));  // Cmd+Z
            }
            return false;

        case MY_REDO:
            if (record->event.pressed) {
                tap_code16(SGUI(KC_Z));  // Cmd+Shift+Z
            }
            return false;

        case MY_SAVE:
            if (record->event.pressed) {
                tap_code16(LGUI(KC_S));  // Cmd+S
            }
            return false;

        // ====================================================================
        // FUN_KEY: Dual-function (Tap = one-shot FUN, Hold = momentary FUN)
        // ====================================================================
        case FUN_KEY:
            if (record->event.pressed) {
                // Key pressed - start timer and activate layer
                fun_key_timer = timer_read();
                fun_key_held = true;
                layer_on(_FUN);
            } else {
                // Key released
                if (fun_key_held && timer_elapsed(fun_key_timer) < TAPPING_TERM) {
                    // Quick tap - activate one-shot layer
                    fun_oneshot_active = true;
                    // Layer stays on for next keypress
                } else {
                    // Hold released - deactivate immediately
                    layer_off(_FUN);
                }
                fun_key_held = false;
            }
            return false;
    }

    // ========================================================================
    // Handle one-shot FUN layer deactivation
    // ========================================================================
    if (fun_oneshot_active && !record->event.pressed) {
        // A key was released while one-shot FUN is active
        if (keycode != FUN_KEY && !IS_QK_LAYER_TAP(keycode) && !IS_QK_MOD_TAP(keycode)) {
            // Deactivate one-shot after this key is released (better UX - full keypress completes)
            fun_oneshot_active = false;
            layer_off(_FUN);
        }
    }

    return true;
}

// ============================================================================
// TRI-LAYER ACTIVATION (EXTEND + SYM = NUM)
// ============================================================================

void matrix_scan_user(void) {
    // Shift uses Callum (no timeout check needed - waits indefinitely)

    // Check other modifier timeouts with independent timers
    // Auto-release after FLOW_ONESHOT_TERM (500ms) if unused
    check_oneshot_timeout(&os_ctrl_state, &os_ctrl_timers, KC_LCTL);
    check_oneshot_timeout(&os_alt_state, &os_alt_timers, KC_LALT);
    check_oneshot_timeout(&os_gui_state, &os_gui_timers, KC_LGUI);
    check_oneshot_timeout(&os_altgr_state, &os_altgr_timers, KC_RALT);

    // Check oneshot layer timeout with independent timer
    check_oneshot_layer_timeout(&os_fun_state, &os_fun_timers, _FUN);
}

// ============================================================================
// TRI-LAYER ACTIVATION (EXTEND + SYM = NUM)
// ============================================================================

layer_state_t layer_state_set_user(layer_state_t state) {
    // When both EXTEND and SYM are active, activate NUM layer
    return update_tri_layer_state(state, _EXTEND, _SYM, _NUM);
}
