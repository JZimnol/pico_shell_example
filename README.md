# Raspberry Pi Pico FreeRTOS Shell Example Application

This repository showcases an example usage of
[pico_freertos_shell](https://github.com/JZimnol/pico_freertos_shell) - a
FreeRTOS module that allows you to add an interactive shell with custom commands
to your application.

## Compiling the application

**NOTE:** This example assumes that this repository is in the same folder as
[FreeRTOS-Kernel](https://github.com/FreeRTOS/FreeRTOS-Kernel.git) and
[pico-sdk](https://github.com/raspberrypi/pico-sdk.git). If not, change the
`PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` variables in the `CMakeLists.txt`
file accordingly.

**NOTE:** This example targets the `pico_w` board but can also be built for the
`pico` board.

To compile the application, run the following commands:

```shell
# these commands may vary depending on the OS
mkdir build/
cd build
cmake .. && make -j
```

**NOTE:** please refer to the pico_freertos_shell's main
[CMakeLists.txt](https://github.com/JZimnol/pico_freertos_shell/blob/master/CMakeLists.txt)
file for a list of available compile time CMake options.

## Running the application

Flash the board using generated binary file. Open your serial terminal (e.g.
minicom, picocom, putty) and a shell prompt should appear. Type `help` or
`helptree` for a list of available commads. Try some example commands:
- `helloworld arg1 "arg with a space"`
- `helptree variable`
- `variable read`
- `variable set 123`
- `reboot 1000`
