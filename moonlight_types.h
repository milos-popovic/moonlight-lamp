/*
 * moonlight_types.h
 *
 *  Created on: Aug 7, 2014
 *      Author: dzambi
 */

#ifndef MOONLIGHT_TYPES_H_
#define MOONLIGHT_TYPES_H_

// PINOUT
const byte SLEEP = 8;
const byte FRONTLED = 9;
const byte BACKLED = 10;
const byte SWITCH = 11;
const byte STEP = 12;
const byte DIR = 13;
const byte TMP = A0;

enum power_state_t {
	TURNED_ON = 1, TURNED_OFF = 0, TURNING_OFF = 2
};

enum direction_t {
	COUNTER_CLOCKWISE = -1, CLOCKWISE = 1, WAIT = 0
};

//const byte MICRO_STEPPING = 8;
//const float FULL_STEP = 7.5;
//const float MICRO_STEP = 0.9375f;	// STEP / MICRO_STEPPING;
//const int FULL_STEPS = 48;				// 360 / STEP
const unsigned int FULL_TURN = 384;		// 360 / MICRO_STEP
const byte HALF = 192; // FULL_TURN / 2
const byte QUARTER = 96; // FULL_TURN / 4

struct ControlPoint {
	direction_t direction;
	int position;
	unsigned long duration;

	ControlPoint(direction_t dir, int pos, unsigned long dur) {
		direction = dir;
		duration = dur;
		position = pos;
	}
	;
};

#endif /* MOONLIGHT_TYPES_H_ */
