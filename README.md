# xbuskill

`xbuskill` is a command-line utility to allow you to run cleanup commands when
enabled and a specific USB device is unplugged. Designed to work on Linux.

## Setup

### Figure out which USB device you want to track

1. Plug in the USB
2. Run `udevadm monitor --environment --udev | grep ID_SERIAL`
3. Unplug the USB
4. Note the `ID_SERIAL` or `ID_SERIAL_SHORT` to use in the `udev` rule

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
3. Run `sudo udevadm control --reload`

### Configure xbuskill

1. In this directory, run `sudo make build install clean`
2. Create `/etc/xbuskill/config`, entering one command per line which you would like to run when `xbuskill` is executed. Example:
```sh
# Comments and blank lines are supported in this
# file. Commands will be run with root priviliges.
sed -i "$ a\buskill invoked at $(date +%s)" /etc/xbuskill/log # pipes don't work
veracrypt -d # Dismount all veracrypt volumes right away
shutdown now # Shut down the machine to prevent any file access
```

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
1. Plug in the USB device you want to use
2. Run `sudo xbuskill enable` to enable the executor
3. When you're safe, run `sudo xbuskill disable` before unplugging your USB
4. Use an extension cord and keep the USB close to yourself to prevent it from being taken with the device. For protecting a laptop or pc at home, keep the usb securely locked in place (or just buy a laptop lock).
