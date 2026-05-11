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

## MQTT topics

```c
#define MQTT_SENSOROUT_PUB_TOPIC "nutrino/sensor_out" //Sensor output
#define MQTT_STATUS_PUB_TOPIC "nutrino/status_out" //sensor status output (mainly for calibration status)
#define MQTT_CMD_SUB_TOPIC "nutrino/cmd" //Sensor command listener "calph" for calibrate ph sensor, "calec" for calibrate ec sensor
```