# MobiOS GRUB Theme Background Image

## Requirements
- The background image should be a PNG file named `background.png`
- Recommended resolution: 1920x1080 pixels (or appropriate for your display)
- Place the file in this directory (`/boot/grub/themes/mobios/`)

## How to Add a Background Image
1. Create or obtain an image that represents MobiOS (logo, branding, etc.)
2. Convert the image to PNG format if necessary
3. Resize the image to an appropriate resolution for your display
4. Rename the image to `background.png`
5. Place the image in this directory
6. The GRUB configuration will automatically use this image as the background

## Temporary Background
If you don't have a custom image ready, you can use a solid color background by modifying the theme.txt file:
- Open `theme.txt`
- Replace `desktop-image: "background.png"` with `desktop-color: "#000080"` (navy blue)
- You can use any hex color code to match your preferred color scheme