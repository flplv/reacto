# Building and running MSP430 examples

1. You need to install the compiler:

```sh
        sudo apt-get install binutils-msp430 gcc-msp430 msp430-libc mspdebug
```

2. Change the target MCU on the mcp430.sconscript file, variable `mcu_flag` in `_SetupEnv` method of the class.

3. Connect the Launchpad board to your usb port, and run:

```sh
    scons -Q msp430_blink_deploy
```
