SERIAL_DRIVER = usart           # Keep existing serial driver
RGB_MATRIX_ENABLE = no          # No RGB support per user request
MOUSEKEY_ENABLE = yes           # Mouse key support
CAPS_WORD_ENABLE = yes          # Enable caps word (double-tap shift)
LAYER_LOCK_ENABLE = yes         # Lock NAV/SYM layers with LLOCK
CONSOLE_ENABLE = no             # Disable console for size optimization
VIA_ENABLE = no                 # Disabled - using keymap.c only (was causing LLOCK/FUN_KEY bugs)
VIAL_ENABLE = no                # Disabled - using keymap.c only (was causing keycode remapping issues)
VIAL_INSECURE = no              # Not needed (Vial disabled)
VIALRGB_ENABLE = no             # No RGB control via Vial (RGB disabled)

# Include custom oneshot implementation (Callum style)
SRC += oneshot.c

# Note: Using Callum-style one-shots (no timers, queue until used)
# Advantages: No timeout, stackable modifiers, layer-aware behavior
