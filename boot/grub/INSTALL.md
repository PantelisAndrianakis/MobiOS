# MobiOS GRUB Bootloader Installation Guide

This guide provides instructions for installing and testing the MobiOS GRUB bootloader configuration.

## Prerequisites

- A working Linux installation with root access
- GRUB2 already installed on your system

## Installation Steps

1. **Copy the configuration files to your system's GRUB directory**

   ```bash
   # As root user or using sudo
   sudo cp -r boot/grub/grub.cfg /boot/grub/
   sudo cp -r boot/grub/themes /boot/grub/
   ```

2. **Update GRUB configuration to use the theme**

   Make sure the following line is in your `/etc/default/grub` file:

   ```
   GRUB_THEME="/boot/grub/themes/mobios/theme.txt"
   ```

   Then update GRUB:

   ```bash
   sudo update-grub
   # or on some distributions:
   sudo grub-mkconfig -o /boot/grub/grub.cfg
   ```

## Testing the Configuration

### Method 1: Reboot and Verify

The most direct way to test is to reboot your system:

```bash
sudo reboot
```

Upon restart, you should see:
- The "Welcome to MobiOS" message
- Menu entries for booting into MobiOS

### Method 2: Using GRUB Test Environment (for developers)

For testing without rebooting, you can use GRUB's built-in test environment:

```bash
sudo grub-emu
```

This opens a GRUB emulation window where you can test the menu navigation and appearance.

## Troubleshooting

### Theme Not Loading

If the theme doesn't load:

1. Verify the path to the theme file is correct
2. Ensure all theme files have proper permissions:
   ```bash
   sudo chmod -R 644 /boot/grub/themes/mobios/
   ```
3. Check GRUB logs:
   ```bash
   sudo cat /var/log/boot.log | grep -i grub
   ```

### Background Image Issues

If you've added a custom background image and it's not displaying:

1. Ensure the image is properly formatted (PNG format)
2. Verify the path is correct in the theme.txt file
3. Try using a different image with a standard resolution (e.g., 1920x1080)

## Customizing Your Installation

To add a custom background image:
1. Prepare a PNG image file
2. Place it at `/boot/grub/themes/mobios/background.png`
3. Modify the theme.txt file to use the image instead of the solid color background

## Next Steps

After successful installation, you can:
- Customize menu entries in grub.cfg
- Adjust theme settings in theme.txt
- Add your operating system's logo as the background image