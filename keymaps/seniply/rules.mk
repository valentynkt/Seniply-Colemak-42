SERIAL_DRIVER = usart           # Keep existing serial driver
RGB_MATRIX_ENABLE = no          # No RGB support per user request
MOUSEKEY_ENABLE = yes           # Mouse key support
CAPS_WORD_ENABLE = no           # Disabled - using Shift lock instead
LAYER_LOCK_ENABLE = yes         # Lock NAV/SYM layers with LLOCK
CONSOLE_ENABLE = no             # Disable console for size optimization
VIA_ENABLE = no                 # VIA not needed
VIAL_ENABLE = no                # Vial not needed

# Include custom oneshot implementation (Callum style)
SRC += oneshot.c

# Note: Using Callum-style one-shots (no timers, queue until used)
# Advantages: No timeout, stackable modifiers, layer-aware behavior
