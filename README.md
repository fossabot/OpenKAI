<p align="center">
<img src="https://github.com/yankailab/OpenKAI/raw/master/data/OK.png" width=300px alt="OpenKAI">
</p>

## OpenKAI: A modern framework for unmanned vehicle and robot control
[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2FHotakaYagi%2FOpenKAI.svg?type=shield)](https://app.fossa.com/projects/git%2Bgithub.com%2FHotakaYagi%2FOpenKAI?ref=badge_shield)

OpenKAI (Kinetic AI) is an open-source framework that realize rapid development of unmanned vehicles and robots that incorporate AI, sensors, actuators, etc.. OpenKAI is light weight to run on embedded hardwares. A simple and unified multi-threading modular architecture written purely in C++ makes it highly customizable for versatile applications and easily expanded to support new devices.

We are keep adding supports for sensors and actuators available on market from hackable modules to industry solutions. The supported devices can be used easily as an abstracted object in OpenKAI. We welcome contributions from vendors to provide evaluation device, as well as coders from open-source community.

## Platforms
Tested but not limited to
* x86 PC (Ubuntu 20.04 LTS)
* NVIDIA Jetson series (JetPack/Ubuntu)
* Raspberry Pi (Raspberry Pi OS)

## Dependencies
* Mandatory: Pthread, Google glog, Eigen.
* Optional: OpenCV, Open3D, OpenCL, etc..

## Interfaces
* UART
* USB Serial
* CAN (incl. USB <-> CAN converter)
* RS-485 (USB <-> RS-485 converter)
* LAN/Wifi

## Protocols
* Mavlink 2
* Modbus
* TCP/UDP
* WebSocket
* JSON

## External controllers
* Pixhawk and its compatibles (Mavlink/UART)
* Oriental Motor (Modbus/RS-485)
* ZLAC 8015 (Modbus/RS-485)

and more are coming.

## Sensors
* Intel Realsense D400 series, L515, T265.
* LeddarTech LeddarVu (UART/USB)
* TOFsense
* Raspberry Camera module
* Hiphen Airphen
* Canon SLR
* Sony RX0 series
* Livox

and more are coming.

## Actuators
* Oriental Motor
* DRV8825 (RS485)

and more are coming.

## Displays
* Mutrics ARiC AR glasses (Coming soon)

## Showcase of Solutions

Updating...


## License
[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2FHotakaYagi%2FOpenKAI.svg?type=large)](https://app.fossa.com/projects/git%2Bgithub.com%2FHotakaYagi%2FOpenKAI?ref=badge_large)