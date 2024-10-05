# Sigma Σ

Sigma is a control and monitoring program designed to run on Raspberry PI boards. The aims of Σ are: 
- Provide small 'control' display capability to present summary and key information, e.g.: temperature, ip address, network load, device status (network status, potential errors/warnings).
- Provide small 'keypad' to allow basic operations (two-button keypad under development), e.g.: new network device 'key-press' authorisation, device soft 'shutdown'.

Program is developed with an 'abstraction' in mind to make it suitable for the following applications:
- Node for IoT (Internet of Thinks) systems.
- Basic controls for 'headless' Raspberry Pi microcomputers.

Below is a picture of a prototype of the Raspberry Pi based device that controls two cooling fans via electrical relay (switch). Depending on temperature `Sigma` gives 'fan' on/off signals. For user's convenience device provides a small LCD display that allows on status check (no need for logging-in to app).

![Setup](./doc/PiAndSigma-255x255.jpeg "Setup: PI with display, temp. sensor and relay")

> **CAUTION:**
> 
> **This software controls devices that might need to be powerd directly from mains or perform kinetic actions (motors). It's *your* responsibility to ensure that setup is safe for you and others. If unsure always consult issues with profesional electrician.**

Currently, project supports following features: 
- Control display
- Temperature read-outs
- Relay control

## Logic

The purpose of the device is to perform given action upon occurrence of a certain conditions.

The conditions are defined as: 
### Upper limit: 
- **if** value exceeds `max` **then:** send signal `on`
- **if** value drops below `min` **then:** send signal `off`
### Low limit
- **if** value drops below `min` **then:** send signal `on`
- **if** value exceeds `max` **then:** send signal `off`

At this point only temperature sensors has been implemented and tested. Thus, the value can only be temperature. 'Logic' `Upper limit` is used to drive cooling (by swithing on fan), while logic `Low limit` is for heating applications.


## Requirements

1. Raspberry PI Computer, any of models: Zero / 3 / 4 / 5
2. MicroSD card with Raspberry Pi OS, or other Linux distribution supporting GPIO
3. Peripherals (if more, more fun): 
   * SPI LCD/OLED display such as Waveshare 0.96inch - 2.4inch, **NOTE:** tested only with [Waveshare 1.54" LCD](https://www.waveshare.com/wiki/1.54inch_LCD_Module)
   * 1wire DS18b20 temperature sensor
   * One or multiple chanel relay such as [WaveShare RPi Relay Board](https://www.waveshare.com/wiki/RPi_Relay_Board)


## Software Setup
Get, compile code and install: 
```bash
sudo git clone https://github.com/tools200ms/Sigma.git /usr/src/Sigma
sudo make compile install clean -C /usr/src/Sigma
```
### SystemD
If it's systemd distributions do the following:

```bash
# install service
sudo cp /usr/src/Sigma/dist/sigma.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable sigma
sudo systemctl start sigma

# check: 
service sigma status
```
### OpenRC
For OpenRC distros: 
```bash
# install service
sudo cp usr/src/Sigma/dist/sigma-openrc.sh /etc/init.d/sigma
sudo chmod +x /etc/init.d/sigma

sudo rc-update add sigma default
sudo rc-service sigma start

# check: 
service sigma status
```

## Futher features

### Physical contact access authentication
As sigma controls hardware elements such as key-pads and displays it can be used 
for access authentication. 
Like [Wi-Fi Protected Setup (WPS)](https://en.wikipedia.org/wiki/Wi-Fi_Protected_Setup) 
where Wi-fi access is granted by pressing a physical button instead of providing a 'classic' password. 
Sigma software can be used but to grand e.g. ssh access. 
Upon SSH login attempt to device, its screen can be used to display information about attempt, while keypad can be used to 
confirm or deny access.

Also, screen can be used to display pin to be used as a 
temporary password to make the security at tip-top.

## Software dependencies

Sigma uses [WiringPI](https://github.com/WiringPi/WiringPi) library for hardware calls.




