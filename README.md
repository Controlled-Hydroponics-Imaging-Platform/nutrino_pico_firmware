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

## Pico_connect captive connect interface

## User Interface Usage

This project uses [pico captive connect](https://github.com/mohas95/pico_captive_connect) inteface library for connecting to wifi and sending data through mqtt protocol.

#### usage:

1. **First boot**  
   - Device starts in **AP mode**.  
   - Connect your phone/PC to Wi-Fi SSID:  
     - **SSID:** `PicoSetup`  
     - **Password:** `pico1234`  
   - Open any browser and go to 192.168.4.1  
   - Enter Wi-Fi credentials.

2. **STA mode**  
   - After reboot, Pico will connect to your Wi-Fi router.  
   - Look in your router’s DHCP table for the assigned IP.

3. **Re-provision**  
   - You can go back to **AP mode** by accessing the configuration page at the assigned IP
   - Press reset if Wi-Fi fails, it falls back to AP mode.  
   - Update credentials via captive portal.