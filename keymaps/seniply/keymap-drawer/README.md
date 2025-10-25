# Keymap Drawer for Seniply Layout

This directory contains configuration and scripts for generating beautiful visualizations of your Seniply keyboard layout using [keymap-drawer](https://github.com/caksoylar/keymap-drawer).

## Quick Start

### Generate Layout Visualization

Run the generation script:

```bash
cd keyboards/cleo/v2_02/keymaps/seniply/keymap-drawer
./generate.sh
```

This will create:
- `layout.yaml` - Parsed layout data from your keymap.c
- `seniply-layout.svg` - Beautiful SVG visualization of all layers
- `seniply-layout.png` - PNG version (if cairosvg is installed)

### View the Results

```bash
# Open SVG in your default browser
open seniply-layout.svg

# Or view PNG
open seniply-layout.png
```

## Manual Generation

If you prefer to run the commands manually:

### Step 1: Parse keymap.c to YAML

```bash
keymap parse -c config.yaml -q ../keymap.c > layout.yaml
```

### Step 2: Generate SVG visualization

```bash
keymap draw layout.yaml > seniply-layout.svg
```

### Step 3: (Optional) Convert to PNG

```bash
# Install cairosvg if needed
pipx install cairosvg

# Convert SVG to PNG
cairosvg seniply-layout.svg -o seniply-layout.png --dpi 150
```

## Customization

### Edit config.yaml

The `config.yaml` file controls how your layout is visualized:

**Visual Settings:**
- `dark_mode: auto` - Use system theme (or set to `true`/`false`)
- `n_columns: 2` - Show 2 layers side-by-side
- `key_w`, `key_h` - Key dimensions
- Layer-specific colors in the `svg_style` section

**Key Labels:**
- Customize how keycodes are displayed in `qmk_keycode_map`
- Add new custom keycodes
- Change labels for existing keys

**Example - Change a key label:**
```yaml
qmk_keycode_map:
  MY_CUSTOM:
    t: "Custom\nLabel"
    type: tap
```

### Edit layout.yaml

After generating `layout.yaml`, you can manually edit it to:
- Adjust layer names
- Modify key positions
- Add annotations or notes
- Highlight specific keys

Then regenerate the SVG:
```bash
keymap draw layout.yaml > seniply-layout.svg
```

## Color Scheme

The configuration uses layer-specific colors:
- **BASE** - Light gray (#e8e8e8) - Colemak-DH layout
- **NAV** - Light blue (#bbdefb) - Navigation layer
- **SYM** - Light green (#c8e6c9) - Symbols layer
- **NUM** - Light orange (#ffe0b2) - Numbers layer
- **FUN** - Light pink (#f8bbd0) - Function keys

Special keys:
- **Layer Lock** - Yellow (#fff176)
- **Hyper** - Purple (#ce93d8)
- **One-shot mods** - Cyan (#80deea)

## Advanced Usage

### Generate Single Layer

```bash
# Only show BASE and NAV layers
keymap draw -l "BASE (Colemak-DH)" "NAV (Navigation + Vim)" layout.yaml > base-nav.svg
```

### Export Dark Mode

```bash
# Force dark mode
keymap draw --dark-mode layout.yaml > seniply-dark.svg
```

### High-Resolution PNG

```bash
# Generate PNG at 300 DPI for printing
cairosvg seniply-layout.svg -o seniply-hires.png --dpi 300
```

### Generate Individual Layer PNGs

```bash
# Extract each layer separately
for layer in "BASE" "NAV" "SYM" "NUM" "FUN"; do
  keymap draw -l "$layer" layout.yaml > "layer-$layer.svg"
  cairosvg "layer-$layer.svg" -o "layer-$layer.png" --dpi 150
done
```

## Troubleshooting

### "keymap: command not found"

Install keymap-drawer:
```bash
pipx install keymap-drawer
```

### PNG generation fails

Install cairosvg (optional, for PNG export):
```bash
pipx install cairosvg
```

### Parse errors

If parsing fails, check:
1. Your keymap.c syntax is valid
2. All custom keycodes are defined in config.yaml
3. Try running: `keymap parse -c config.yaml -q ../keymap.c` to see error details

## Resources

- [keymap-drawer Documentation](https://github.com/caksoylar/keymap-drawer)
- [Configuration Reference](https://github.com/caksoylar/keymap-drawer/blob/main/CONFIGURATION.md)
- [QMK Keycode Reference](https://docs.qmk.fm/keycodes)

## Tips

- **Use monospace fonts**: The SVG uses 'Fira Code' or similar for better code readability
- **Browser viewing**: SVGs look best in Chrome/Firefox/Safari
- **Documentation**: Include the generated SVG in your README or documentation
- **Version control**: Commit the config.yaml but maybe not the generated files
- **Regenerate often**: Run `./generate.sh` after any keymap changes

## Example Output

Your visualization will show:
- All 5 layers (BASE, NAV, SYM, NUM, FUN) in a clean grid
- Color-coded keys for easy identification
- Hold/tap indicators for layer switches
- Special symbols for modifiers and navigation
- Clear labels for Vim-specific keys ($ 0 ^)
- Highlighted one-shot modifiers

Enjoy visualizing your Seniply layout! 🎨✨
