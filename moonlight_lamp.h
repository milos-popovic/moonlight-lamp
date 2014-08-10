// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _moonlight_lamp_H_
#define _moonlight_lamp_H_
#include "Arduino.h"
//add your includes for the project moonlight_lamp here

#include "moonlight_types.h"
#include "moonlight_features.h"
#include "moonlight_level_tables.h"
#include "moonlight_coreography.h"

//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project moonlight_lamp here

int calc_steps(int from, int to, direction_t direction);
unsigned long calc_delay_us(int steps, int duration);

//Do not add code below this line
#endif /* _moonlight_lamp_H_ */
