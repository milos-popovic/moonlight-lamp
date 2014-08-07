/*
 * moonlight_types.h
 *
 *  Created on: Aug 7, 2014
 *      Author: dzambi
 */

#ifndef MOONLIGHT_TYPES_H_
#define MOONLIGHT_TYPES_H_

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
	int duration;

	ControlPoint(direction_t dir, int pos, int dur) {
		direction = dir;
		duration = dur;
		position = pos;
	}
	;
};

#endif /* MOONLIGHT_TYPES_H_ */
