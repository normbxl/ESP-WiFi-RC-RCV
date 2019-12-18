#pragma once
typedef enum BoardConnector_en {
	S1 = 0, S2, M1, M2, MAX_NUM_PORTS
} BoardConnector_t;

typedef enum PortType_en {
	NOT_USED = 0,
	DIGITAL,
	SERVO,
	ANALOG,
	NUM_PORT_TYPES, 
	// IMPORTANT: H_BRIDGE must be the last entry!
	H_BRIDGE
} PortType_t;

typedef struct Port_s {
	PortType_t type;
	int value;
	uint8_t pinOut;
	uint8_t pinExtra;
} Port_t;
