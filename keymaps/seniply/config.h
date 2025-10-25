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

// Note: Using custom Callum-style one-shot modifiers instead of QMK's OSM
// Callum one-shots never timeout - they queue until used or cancelled

// Layer lock configuration
#define LAYER_LOCK_IDLE_TIMEOUT 60000  // 60 second timeout for layer lock
