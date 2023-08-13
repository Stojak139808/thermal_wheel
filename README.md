Heat wheel thing

## Building/Flashing

In order to buld and flash this program, you will need:

```
avr-gcc
avr-objcopy
avrdude
avrlibc
```

Package names are dependent on your distribution so I won't bother listing them
here.

To build the firmware:

```
make
```

To flash:
```
make flash
```

> You might need to change the device name in the makefile, the default name is
> `/dev/ttyACM0`
