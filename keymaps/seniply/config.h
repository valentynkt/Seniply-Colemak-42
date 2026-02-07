#pragma once

// Split keyboard hand configuration
#define SPLIT_HAND_PIN B5  // Keep existing from base keyboard

// Tapping and timing settings
#undef TAPPING_TERM
#define TAPPING_TERM 180              // 180ms tap/hold threshold for layer switches (fast typing optimized)
#define QUICK_TAP_TERM 0              // Disable quick tap
#define HOLD_ON_OTHER_KEY_PRESS       // Activate hold immediately when another key is pressed (optimal for fast typing)
#define ONESHOT_TAP_TOGGLE 2          // Double-tap to lock one-shot modifiers (not used - using Callum style)
#define ONESHOT_TIMEOUT 3000          // 3 second one-shot timeout (not used - using Callum style)

// Flow system oneshot configuration (PR #16174)
// Enhanced oneshot with timer support for auto-timeout and hold detection
#define FLOW_ONESHOT_TERM 500           // Auto-release oneshot after 500ms
#define FLOW_ONESHOT_WAIT_TERM 500      // Hold >500ms = normal mod/layer, not oneshot
#define FLOW_LAYERS_ENABLE              // Enable oneshot layer support

// Caps Word configuration (works with custom OS_SHFT via custom double-tap detection)
// Note: DOUBLE_TAP_SHIFT_TURNS_ON_CAPS_WORD removed - doesn't work with custom OS_SHFT
// Double-tap detection is implemented in oneshot.c instead
#define CAPS_WORD_INVERT_ON_SHIFT            // Shift inverts case during caps word (for "PDFs", "iPhone")
#define CAPS_WORD_IDLE_TIMEOUT 5000          // 5 second timeout for caps word

// Layer lock configuration
#define LAYER_LOCK_IDLE_TIMEOUT 60000  // 60 second timeout for layer lock
