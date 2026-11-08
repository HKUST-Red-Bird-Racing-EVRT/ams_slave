/**
 * @file BoardConfig.h
 * @author Oscar Chan, Red Bird Racing (oscarckh0818@gmail.com)
 * @date 2026-09-04
 * @version 2.0.1
 * @brief Board-specific pin definitions for the AMS slave firmware.
 *
 * @note These macros map the firmware signals to the ATmega328P pins used on the PCB.
 * Keep this file synchronized with the schematic and PCB.
 */

#ifndef BoardConfig_H
#define BoardConfig_H

#define CS PIN_PB2

#define STACK_READ PIN_PD1
#define STACK_VALUE A6

#define ALERT PIN_PD2
#define RECEIVE_INTERRUPT PIN_PD3
#define INTERRUPT PIN_PD4

// Jumper JP
#define JP1 PIN_PD5
#define JP2 PIN_PD6
#define JP3 PIN_PD7
#define JP4 PIN_PB0

// Temperature Sensor NTC
#define NTC1 A7
#define NTC2 PIN_PC0
#define NTC3 PIN_PC1
#define NTC4 PIN_PC2
#define NTC5 PIN_PC3

#endif // BoardConfig_H