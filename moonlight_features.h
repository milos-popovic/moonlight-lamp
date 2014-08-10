/*
 * moonlight_features.h
 *
 *  Created on: Aug 7, 2014
 *      Author: dzambi
 */

#ifndef MOONLIGHT_FEATURES_H_
#define MOONLIGHT_FEATURES_H_

// Feature Consts
#define NON_LINEAR

#define LAMP 0 // Test
//#define LAMP 1 // SMALL
//#define LAMP 2 // MEDIUM
//#define LAMP 3 // LARGE

//#define DEBUG
//#define MEASURE_TMP

#ifdef DEBUG
#	define DEBUG_PRINT(x) Serial.print(x)
#	define DEBUG_PRINTLN(x) Serial.println(x)
#else
#	define DEBUG_PRINT(x)
#	define DEBUG_PRINTLN(x)
#endif

#endif /* MOONLIGHT_FEATURES_H_ */s
