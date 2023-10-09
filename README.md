# A home sensor

A small and home-made environmental sensor, which will eventually
provide a fairly complete set of data about an environment, including,
for example, CO2, and other data measures. It is wifi-enabled, and 
can therefore also display alerts.

* A temperature and humidity sensor (HDC1080)
* A small OLED display (SH1106)
* A Raspberry Pi Pico microcontroller, with wireless

The aim is to have a more complete sensor, including air
quality, and one which will report via MQTT to a local Home Assistant 
or similar. 

The code is written using the Pico SDK, C/C++, and cmake.

The following files are omitted -- these are read by cmake and 
packaged into the complete application. 

* `/certs/client.key`
* `/certs/mqtt-password.txt`

## Build process

```bash
mkdir build
cd build
cmake -DWIFI_SSID=<<ssid>> -DWIFI_PASSWORD=<<password>> ..
make
```

