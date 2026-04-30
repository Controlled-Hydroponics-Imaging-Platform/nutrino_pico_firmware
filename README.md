# nutrino_pico_firmware

Firmware for the nutrino controller within our Controlled Hydroponic Imaging Platform (CHIP)

## Build and installation

```bash
git clone git@github.com:Controlled-Hydroponics-Imaging-Platform/nutrino_pico_firmware.git

cd nutrino_pico_firmware

git submodule update --init --recursive --remote

mkdir build && cd build

cmake ..

make
```

## Hardwaire and circuit information

Please see the assets folder for all documentation on circuit hardware