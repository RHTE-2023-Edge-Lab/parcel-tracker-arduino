# Parcel Tracker - Arduino Firmware

## BOM

* [ESP8266 Wemos D1 Mini](https://www.ebay.fr/itm/144652168812)
* [Kit RFID RC522](https://www.ebay.fr/itm/133514459432)
* Breadboard
* Wires

## Wiring

Wire the RC522 to the Wemos D1 Mini like so:

* RST -> D3
* SDA -> D8
* MOSI -> D7
* MISO -> D6
* SCK -> D5

## Dependencies

* [MFRC522](https://github.com/miguelbalboa/rfid)
* [PubSubClient](https://github.com/knolleary/PubSubClient)

## Setup

Install :

* VSCode
* [PlatformIO IDE](https://docs.platformio.org/en/latest/integration/ide/vscode.html) plugin

* Fix permissions on /dev/ttyUSB0

```sh
sudo usermod -a -G dialout $(id -un)
```

Create a new project :

* Board Name: **Wemos D1 R2 and mini**
* Framework: **Arduino**

In PIO Home, add the library **MFRC522 by Miguel André Balboa** (search for **header:MFRC522.h**).

Add the library **PubSubClient by Nick O'Leary**.

Monitor messages on the chosen topic:

```sh
sudo dnf install mosquitto
mosquitto_sub -h test.mosquitto.org -t "/test/nmasse@redhat.com/label-reader" -u wildcard -v
```
