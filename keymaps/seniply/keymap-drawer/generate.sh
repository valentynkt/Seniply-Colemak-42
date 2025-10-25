#!/bin/bash
# Generate keymap visualization for Seniply layout

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
KEYMAP_DIR="$(dirname "$SCRIPT_DIR")"
KEYMAP_FILE="$KEYMAP_DIR/keymap.c"
CONFIG_FILE="$SCRIPT_DIR/config.yaml"
LAYOUT_FILE="$SCRIPT_DIR/layout.yaml"
KEYMAP_JSON="$SCRIPT_DIR/keymap.json"
OUTPUT_SVG="$SCRIPT_DIR/seniply-layout.svg"
OUTPUT_PNG="$SCRIPT_DIR/seniply-layout.png"

echo "🎨 Generating Seniply keymap visualization..."
echo ""

# Check if keymap-drawer is installed
if ! command -v keymap &> /dev/null; then
    echo "❌ Error: keymap-drawer is not installed"
    echo "Install it with: pipx install keymap-drawer"
    exit 1
fi

# Step 1: Convert keymap.c to QMK JSON
echo "1️⃣  Converting keymap.c to JSON..."
qmk c2json -kb cleo/v2_02 -km seniply "$KEYMAP_FILE" > "$KEYMAP_JSON"
echo "   ✅ Generated keymap.json"

# Step 2: Parse QMK JSON to keymap-drawer YAML
echo "2️⃣  Parsing JSON to layout YAML..."
keymap -c "$CONFIG_FILE" parse -q "$KEYMAP_JSON" > "$LAYOUT_FILE"
echo "   ✅ Generated layout.yaml"

# Step 3: Draw SVG
echo "3️⃣  Drawing SVG..."
keymap -c "$CONFIG_FILE" draw "$LAYOUT_FILE" > "$OUTPUT_SVG"
echo "   ✅ Generated seniply-layout.svg"

# Step 4: Convert to PNG (if cairosvg is available)
if command -v cairosvg &> /dev/null; then
    echo "4️⃣  Converting to PNG..."
    cairosvg "$OUTPUT_SVG" -o "$OUTPUT_PNG" --dpi 150
    echo "   ✅ Generated seniply-layout.png"
else
    echo "4️⃣  Skipping PNG generation (cairosvg not installed)"
    echo "   Install with: pipx install cairosvg"
fi

echo ""
echo "✨ Done! Your keymap visualizations are ready:"
echo "   📄 YAML: $LAYOUT_FILE"
echo "   🖼️  SVG:  $OUTPUT_SVG"
if [ -f "$OUTPUT_PNG" ]; then
    echo "   🖼️  PNG:  $OUTPUT_PNG"
fi
echo ""
echo "💡 Tip: Open the SVG file in your browser to view it!"
