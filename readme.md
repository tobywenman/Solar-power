# Pi Pico code to implement MPPT charger.
This repository holds the code to allow the implementation of the circuit found [here.](https://github.com/tobywenman/mppt-charger)

The main algorithm is found in `src/main.cpp`, the PIO code to allow the interface with the potentiometer is in `pot.pio`, and the cMake file for building is the default name.

## Build
The first step requires downloading the Pi Pico C++ sdk. The process is described on its own [GitHub page](https://github.com/raspberrypi/pico-sdk) however for convenince I have listed a short version below. I utilised Ubuntu for this project however other OS should be supported. 

1. Download C++ sdk:
```
git clone https://github.com/raspberrypi/pico-sdk.git
```
2. Install prerequisites:
```
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib
```
3. Point cMake to the sdk:
```
export PICO_SDK_PATH=<absolute path to the sdk>
```
4. Create build folder and makefile:
```
mkdir build
cd build
cmake ../src
```
5. Build project:
```
make
```

The .uf2 file can then be copied to the Pi Pico to upload and run the code.

## Monitoring
The main way the Pico outputs is via serial data on the USB. This can be monitored by using Putty or a similar terminal. This implemenation only outputs the current, voltage and power measurements. 