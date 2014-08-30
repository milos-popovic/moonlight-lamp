/*
 * moonlight_coreography.h
 *
 *  Created on: Aug 7, 2014
 *      Author: dzambi
 */

#ifndef MOONLIGHT_COREOGRAPHY_H_
#define MOONLIGHT_COREOGRAPHY_H_

#include "moonlight_types.h"

#if LAMP == 0 // Test

ControlPoint POINTS[] = {
		ControlPoint(WAIT, 0, 3000ul),
		ControlPoint(CLOCKWISE, QUARTER, 2000ul),
		ControlPoint(WAIT, 0, 3000ul),
		ControlPoint(CLOCKWISE, HALF, 2000ul),
		ControlPoint(WAIT, 0, 3000ul),
		ControlPoint(CLOCKWISE, 3 * QUARTER, 2000ul),
		ControlPoint(WAIT, 0, 3000ul),
		ControlPoint(CLOCKWISE, 0, 2000ul),
		ControlPoint(WAIT, 0, 3000ul),

		ControlPoint(COUNTER_CLOCKWISE, 3 * QUARTER, 2000ul),
		ControlPoint(WAIT, 0, 3000ul),
		ControlPoint(COUNTER_CLOCKWISE, HALF, 2000ul),
		ControlPoint(WAIT, 0, 3000ul),
		ControlPoint(COUNTER_CLOCKWISE, QUARTER, 2000ul),
		ControlPoint(WAIT, 0, 3000ul),
		ControlPoint(COUNTER_CLOCKWISE, 0, 2000ul),
		ControlPoint(WAIT, 0, 3000ul)
};

#elif LAMP == 1 // Small

ControlPoint POINTS[] = {
	ControlPoint(WAIT, 0, 3000ul),
	ControlPoint(CLOCKWISE, HALF, 4000ul),
	ControlPoint(WAIT, 0, 48000ul),

	ControlPoint(COUNTER_CLOCKWISE, QUARTER, 2000ul),
	ControlPoint(WAIT, 0, 3000ul),
	ControlPoint(COUNTER_CLOCKWISE, 3 * QUARTER, 4000ul),
	ControlPoint(WAIT, 0, 3000ul),
	ControlPoint(CLOCKWISE, 0, 2000ul),
	ControlPoint(WAIT, 0, 10000ul),

	ControlPoint(CLOCKWISE, HALF, 4000ul),
	ControlPoint(WAIT, 0, 3000ul),
	ControlPoint(CLOCKWISE, 0, 2000ul),
	ControlPoint(WAIT, 0, 3000ul),
	ControlPoint(CLOCKWISE, HALF, 4000ul),
	ControlPoint(WAIT, 0, 3000ul),
	ControlPoint(CLOCKWISE, 0, 4000ul),
	ControlPoint(WAIT, 0, 7000ul)
};

#elif LAMP == 2 // Medium

ControlPoint POINTS[] = {
	ControlPoint(WAIT, 0, 3000ul),
	ControlPoint(CLOCKWISE, HALF, 4000ul),
	ControlPoint(WAIT, 0, 10000ul),
	ControlPoint(CLOCKWISE, 0, 4000ul),
	ControlPoint(WAIT, 0, 3000ul),
	ControlPoint(CLOCKWISE, HALF, 4000ul),
	ControlPoint(WAIT, 0, 3000ul),
	ControlPoint(COUNTER_CLOCKWISE, QUARTER, 2000ul),
	ControlPoint(WAIT, 0, 3000ul),
	ControlPoint(CLOCKWISE, HALF, 2000ul),
	ControlPoint(WAIT, 0, 34000ul),

	ControlPoint(CLOCKWISE, 0, 4000ul),
	ControlPoint(WAIT, 0, 3000ul),
	ControlPoint(CLOCKWISE, HALF, 4000ul),
	ControlPoint(WAIT, 0, 10000ul),

	ControlPoint(CLOCKWISE, 0, 4000ul),
	ControlPoint(WAIT, 0, 14000ul)
};

#elif LAMP == 3 // Large

ControlPoint POINTS[] = {
	ControlPoint(WAIT, 0, 3000ul),
	ControlPoint(CLOCKWISE, HALF, 4000ul),
	ControlPoint(WAIT, 0, 34000ul),

	ControlPoint(CLOCKWISE, 0, 4000ul),
	ControlPoint(WAIT, 0, 3000ul),
	ControlPoint(CLOCKWISE, HALF, 4000ul),
	ControlPoint(WAIT, 0, 27000ul),

	ControlPoint(CLOCKWISE, 0, 4000ul),
	ControlPoint(WAIT, 0, 10000ul),

	ControlPoint(CLOCKWISE, HALF, 4000ul),
	ControlPoint(WAIT, 0, 10000ul),

	ControlPoint(CLOCKWISE, 0, 4000ul)
};

#endif

const byte POINTS_LEN = (sizeof(POINTS) / sizeof(*POINTS));

#endif /* MOONLIGHT_COREOGRAPHY_H_ */
