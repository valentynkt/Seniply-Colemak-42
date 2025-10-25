# Keymap-Drawer Setup Complete! 🎨

## ✅ What's Installed

1. **keymap-drawer** - Installed via `pipx`
2. **Configuration files** - Ready in `keymap-drawer/` directory
3. **Generate script** - `./generate.sh` to create visualizations

## 📋 Current Status

The setup is complete, but there's a known issue with `qmk c2json` not properly parsing keymaps with:
- Custom keycodes (OS_SHFT, MY_COPY, etc.)
- Complex layer-tap definitions
- Modified formatting

## 🔧 Workaround Options

### Option 1: Manual Layout YAML (Recommended for now)

Create `layout.yaml` manually by describing your layers. Here's a starter template:

```yaml
layout:
  qmk_keyboard: cleo/v2_02
  qmk_layout: LAYOUT_split_3x6_3

layers:
  BASE (Colemak-DH):
    - [LLOCK, Q, W, F, P, G,           J, L, U, Y, "'" , LLOCK]
    - [HYPER, A, R, S, T, D,           H, N, E, I, O, ";"]
    - [LLOCK, Z, X, C, V, B,           K, M, ",", ".", "/", "_"]
    - ["", "", Esc, "Space\n→NAV", "OS\nShift",   Tab, "Enter\n→SYM", Bksp, "", ""]

  NAV (Navigation + Vim):
    # ... continue for other layers
```

Then generate: `keymap -c config.yaml draw layout.yaml > seniply-layout.svg`

### Option 2: Use QMK Configurator

1. Go to https://config.qmk.fm/
2. Load your keyboard (cleo/v2_02)
3. Manually recreate your layout
4. Export JSON
5. Use that JSON with keymap-drawer

### Option 3: Fix Custom Keycodes

Replace custom keycodes in keymap.c temporarily:
- `OS_SHFT` → `OSM(MOD_LSFT)`
- `MY_COPY` → `LCTL(KC_C)`
- etc.

Then run `qmk c2json` again.

### Option 4: Use Alternative Visualizer

Try **keyboard-layout-editor.com** or **via/vial** for visual layouts.

## 🚀 Quick Commands Reference

```bash
# Navigate to directory
cd keyboards/cleo/v2_02/keymaps/seniply/keymap-drawer

# Try the auto-generate script (may need fixes)
./generate.sh

# Manual generation (if you have layout.yaml)
keymap -c config.yaml draw layout.yaml > seniply-layout.svg

# View the result
open seniply-layout.svg

# Convert to PNG (if cairosvg installed)
cairosvg seniply-layout.svg -o seniply-layout.png --dpi 150
```

## 📝 Files Created

```
keymap-drawer/
├── config.yaml          # Styling and keycode mappings
├── generate.sh          # Auto-generation script
├── README.md            # Full documentation
├── SETUP.md             # This file
└── (generated files)
    ├── keymap.json      # QMK JSON export
    ├── layout.yaml      # Parsed layout data
    ├── seniply-layout.svg   # SVG visualization
    └── seniply-layout.png   # PNG visualization (optional)
```

## 🎨 Configuration Highlights

Your `config.yaml` includes:
- **Layer-specific colors** - Each layer has its own color scheme
- **Custom keycode labels** - All your special keys (OS_*, MY_*, VIM_*) are defined
- **Dark mode support** - Auto-adapts to system theme
- **Vim-optimized labels** - Clear visualization of Vim keys ($, 0, ^)

## 🐛 Troubleshooting

### qmk c2json fails
This is expected with custom keycodes. Use manual layout.yaml or Option 3 above.

### keymap parse error
Check that all custom keycodes are defined in `config.yaml` under `qmk_keycode_map`.

### SVG looks wrong
Edit `config.yaml` to adjust:
- Key sizes (`key_w`, `key_h`)
- Colors (in `svg_style` section)
- Labels (in `qmk_keycode_map`)

## 💡 Next Steps

1. **Create manual layout.yaml** - See Option 1 above
2. **Or** wait for QMK to add better custom keycode support
3. **Or** temporarily simplify keymap.c to use only standard QMK keycodes

Once you have a working `layout.yaml`, you can:
```bash
# Generate beautiful visualizations
keymap -c config.yaml draw layout.yaml > my-layout.svg

# Share it!
open my-layout.svg
```

## 📚 Resources

- [keymap-drawer GitHub](https://github.com/caksoylar/keymap-drawer)
- [QMK Configurator](https://config.qmk.fm/)
- [Keyboard Layout Editor](http://www.keyboard-layout-editor.com/)

Your keymap-drawer setup is ready! While automatic generation needs some work, you can create beautiful manual visualizations using the config.yaml we've created. 🎉
