# Raspberry pi pico drone

A micro drone is made using the Raspberry Pi Pico W. It is fitted with the MPU6050 IMU module and BMP280 barometric sensor. The drone will be controlled using TinyMPC and employ a Micro-ROS client on FreeRTOS.

🚧	***(Work in Progress)*** 

## Bill of Materials

The following components were used in this project:

| Part | Quantity |
| :-- | :-- |
| Raspberry Pi Pico W | 1 |
| MPU6050 module| 1 |
| BMP280 module| 1 |
| 1S 500mAh Lipo Battery| 1 |
| 1020 Coreless Motor | 4 |
| 66mm propeller (2 pairs of CW and 2 pairs of CCW) | 4  |
| XH2.54 2p connector pairs (for motors and battery) | 5 |
| si2302 N channel Mosfet| 4 |
| 1N4148 Diode | 5 |
| 100 ohm resistor | 3 |
| 300 ohm resistor | 1 |
| 1k ohm resistor | 1 |
| 10k ohm resistor | 4 |
| Female Pin headers | 40 |
| Push Button| 1 |
| LEDs | 3 |

*CW - Clockwise and CCW - Counter clockwise*

Some tools/parts used int he project are as follows:

| | Tool/Part |
| --| --|
|1| Soldering iron|
|2| 3D printer|
|3| Hot air gun|
|4| Crimping tool|
|4| Solder paste |


## Project Assembly and Wiring

The PCB was made with KiCAD and the project files are available in the [pcb directory](./hardware/pcb/).

<p align="center">
  <img title='Schematic' src=images/schematic.png>
  <img title='Pcb kicad' src=images/pcb_kicad.png width="400">
</p>

<p align="center">
  <img title='Pcb front' src=images/pcb_front.jpeg width="400">
  <img title='Pcb back' src=images/pcb_back.jpeg width="400">
</p>

The insides of one of the female headers had to be filed to enable the MPU6050 module be soldered and positioned horizontally to the pcb.

<p align="center">
  <img title='Header filed 1' src=images/header_filed_1.jpeg width="400">
  <img title='Header filed 2' src=images/header_filed_2.jpeg width="400">
</p>

3D printed frame courtesy of this [thingiverse project](https://www.thingiverse.com/thing:4799075).

<p align="center">
  <img title='Drone sensors' src=images/drone_sensors.jpeg width="400">
</p>

<p align="center">
  <img title='Full assembly front' src=images/full_assembly_front.jpeg width="400">
  <img title='Full assembly back' src=images/full_assembly_side.jpeg width="400">
</p>

## Software installation
FreeRTOS
MicroROS
TinyMPC

TODO:
- Credit Anish Natekar's work
- Add component links
- Note on LED designations

# Acknowledgement
- [PicoW_Copter](https://github.com/anish-natekar/PicoW_Copter)
- [TinyMPC](https://github.com/TinyMPC/TinyMPC)
