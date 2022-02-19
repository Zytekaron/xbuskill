# xbuskill

`xbuskill` is a command-line utility to allow you to run cleanup commands when
enabled and a specific USB device is unplugged. Designed to work on Linux.

## Setup

### Figure out which USB device you want to track

1. Plug in the USB
2. Run `udevadm monitor --environment --udev | grep ID`
3. Unplug the USB
4. Take note of one of the ID fields, such as `ID_SERIAL` or `ID_SERIAL_SHORT` (if there aren't any, jump to [Debugging udev](#debugging-udev))

### Create the udev rule

1. Put the following into `/etc/udev/rules.d/xbuskill.rules`
```sh
ACTION=="remove", \
SUBSYSTEM=="usb", \
ENV{DEVTYPE}=="usb_device", \
ENV{ID_SERIAL_SHORT}=="61252038755497360876", \
RUN+="/usr/bin/xbuskill execute"
```
2. Modify the second to last line, using the `ID_SERIAL` or `ID_SERIAL_SHORT` for
   your USB device (if you know what you're doing, you can use other fields too)
3. You may need to change `usb` to `block` on line 2 if it isn't working
4. Run `sudo udevadm control --reload`

### Configure xbuskill

1. In this directory, run `sudo make build install clean`
2. Create `/etc/xbuskill/config`, entering one command per line which you would like to run when `xbuskill` is executed. Example:
```sh
# Comments and blank lines are supported in this
# file. Commands will be run with root priviliges.
/home/z/.killscript # Bash scripts are more useful
```
`/home/z/.killscript` (make sure to `chmod +x /home/z/.killscript`)
```sh

echo "buskill invoked at $(date +%s)" > /etc/xbuskill/log

shutdown now # Shut down the machine to prevent any file access
```

### Debugging udev

Debugging `xbuskill` and udev rules can be tricky. Run `sudo su` and then `xbuskill execute` to ensure all your commands work *before* looking for issues in the udev configuration. Also, keep in mind that udev restricts certain behavior, such as `mount` and `umount`, during rule execution. See the [Arch Wiki: udev](https://wiki.archlinux.org/title/Udev).

If you're having trouble with the udev rules, try running `udevadm monitor --environment --udev` and then unplugging your USB. Read through the entire output, and look for a section of text (separated by a blank line) where all of your rules apply, such as the following. You may need to remove or change some rules to get it to work, but make sure you keep `ACTION=="remove"` and at least one rule to identify a specific device.
```
ACTION=remove
...
SUBSYSTEM=usb
...
ID_SERIAL_SHORT=61252038755497360876
```

If you're still stuck, you're welcome to [contact me](https://zytekaron.com/contact) and I can try to help.

## Usage

Commands:
```sh
xbuskill enable  # enable xbuskill (create /tmp/xbuskill)
xbuskill disable # enable xbuskill (remove /tmp/xbuskill)
xbuskill status  # check if xbuskill is active (check for /tmp/xbuskill)
xbuskill list    # list all the commands that are run when xbuskill executes
xbuskill execute # automatically called by udev when the usb event runs
```

Best practices:
1. Run `sudo xbuskill enable` to enable the executor. You can plug in the USB before or after this, it doesn't matter.
2. Run `sudo xbuskill disable` when you're in a safe location before unplugging your USB.
3. Use an extension cord and keep the USB device close to your person to prevent it from being taken with the device.
   To protect a laptop or pc at home, keep the usb securely locked in place (or maybe just invest in a laptop lock).
