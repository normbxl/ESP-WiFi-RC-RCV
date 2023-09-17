# ESP-WiFi-RC-RCV
Firmware for a WiFi connectable ESP-12e dual DC motor controller and double servo output. 

The idea is to control two DC motors and two servos with an Android App to quickly build something remote controlled like a car or boat

## Hardware

This firmware was written for a custom designed PCB which includes:

- ESP-12e, Node MCU 1.0 compatible with automatic reset and programming logic
- CH340 USB-UART interface
- Toshiba TB6612F double H-bridge motor driver
- dual server connector
- ~~ADC input or battery monitor~~ (design fault, not working)

All parts can be sourced by the [seeedstudio](https://www.seeedstudio.com/opl.html) Open Parts Library (OPL).

## Firmware

The device first tries to connect to known WiFi network (hard-coded), when failing it will create a soft AP to which the controller should connect.

It will announce itself with the **MDNS** (Bonjour) protocol as "wifi-rc" and wait for incoming **UDP** packets on port **4210**.

For simplicity and flexibility a JSON protocol was chosen. Each received packet is acknowledge by a simple "ok" except for a `get_configuration` request, which returns the current configuration.

## JSON protocol

The device has basically four actuators, two motors and two servos. Usually GPIO pin can be configured as either output or input but since the GPIOs for the motor driver IC are hard wired they are fixed as outputs. 

The two GPIOs for the servo signal are free and can be configured as digital outputs or analogue (PWM) outputs.

### configure pins

```json
{
  "cmd": "set",
  "type": "config",
  "S1": "SERVO", /*other possible values:NOUSE, DIGIT, ANLOG */
  "S2": "SERVO" /* other possible values:NOUSE, DIGIT, ANLOG */
}
```

### get configuration

To receive the current configuration send the following payload.

```json
{
  "cmd": "get",
  "type": "config"
}
```

The returned JSON contains the port configuration and the current control values.

```json
{
    "S1": {
        "type": "SERVO",
        "value" : 123
    },
    "S2": {
        "type": "ANLOG",
        "value" : 123
    },
    "M1": {
        "type": "MOTOR",
        "value" : 123
    },
    "M2": {
        "type": "MOTOR",
        "value" : 123
    },
    "adc": 1023
}
```

All values are in the servo typical range of [-127, 127].

Ports that are configured as digital outputs will output **high** when the value is greater than 0.

Analogue output ports will use the **absolute** value, so -63 and +63 will both generate a PWM with 50% duty-cycle.

### setting actuator values

Accepted values are within the range of -127 to +127.

```json
{
    "cmd" : "set",
    "type": "control",
    "S1": -120,
    "S2": 127,
    "M1": 0,
    "M2": 30
}

```

