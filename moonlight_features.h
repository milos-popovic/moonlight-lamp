/*
 * moonlight_features.h
 *
 *  Created on: Aug 7, 2014
 *      Author: dzambi
 */

#ifndef MOONLIGHT_FEATURES_H_
#define MOONLIGHT_FEATURES_H_

// Feature Consts
//#define DEBUG
//#define MEASURE_TMP
//#define NON_LINEAR
#define LAMP 0 // Test
//#define LAMP 1 // SMALL
//#define LAMP 2 // MEDIUM
//#define LAMP 3 // LARGE

// PINOUT
const byte SLEEP = 8;
const byte FRONTLED = 9;
const byte BACKLED = 10;
const byte SWITCH = 11;
const byte STEP = 12;
const byte DIR = 13;
#ifdef MEASURE_TMP
const byte TMP = A0;
#endif

#ifdef DEBUG
#	define DEBUG_PRINT(x) Serial.print(x)
#	define DEBUG_PRINTLN(x) Serial.println(x)
#else
#	define DEBUG_PRINT(x)
#	define DEBUG_PRINTLN(x)
#endif

#endif /* MOONLIGHT_FEATURES_H_ */s
