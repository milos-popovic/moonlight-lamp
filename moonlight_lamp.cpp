// Do not remove the include below
#include "moonlight_lamp.h"

const byte FRONTLED = 3;
const byte BACKLED = 11;
const byte SWITCH = 7;

//const byte MICRO_STEPPING = 8;
//const float FULL_STEP = 7.5;
//const float MICRO_STEP = 0.9375f;	// STEP / MICRO_STEPPING;
//const int FULL_STEPS = 48;				// 360 / STEP
const int FULL_TURN = 384;		// 360 / MICRO_STEP
const int HALF = 192; // FULL_TURN / 2
const int QUARTER = 96; // FULL_TURN / 4

const byte STEPPIN = 4;
const byte DIRPIN = 2;

enum direction_t {
	COUNTER_CLOCKWISE = -1, CLOCKWISE = 1
};

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

ControlPoint POINTS[] = {
		ControlPoint(CLOCKWISE, 3 * QUARTER, 3000),
		ControlPoint(CLOCKWISE, 3 * QUARTER, 2000),
		ControlPoint(COUNTER_CLOCKWISE, HALF, 200),
		ControlPoint(COUNTER_CLOCKWISE, HALF, 2000),
		ControlPoint(CLOCKWISE, QUARTER, 2000),
		ControlPoint(COUNTER_CLOCKWISE, 0, 1000),
		ControlPoint(COUNTER_CLOCKWISE, 0, 5000)
};

const byte POINTS_LEN = (sizeof(POINTS) / sizeof(*POINTS));

int current_postion = 0;

void set_led_levels(byte level) {
	OCR2A = level;
	OCR2B = HALF - level;

//	Serial.print("OCR2A: ");
//	Serial.print(OCR2A);
//	Serial.print("; OCR2B: ");
//	Serial.println(OCR2B);
}

void step(direction_t direction) {
	digitalWrite(DIRPIN, direction == CLOCKWISE ? LOW : HIGH);
	digitalWrite(STEPPIN, HIGH);
	digitalWrite(STEPPIN, LOW);
	current_postion += direction;
	if (current_postion == -1) {
		current_postion = FULL_TURN - 1;
	} else if (current_postion == FULL_TURN) {
		current_postion = 0;
	}
}

void setup_stepper() {
	pinMode(STEPPIN, OUTPUT);
	pinMode(DIRPIN, OUTPUT);
	digitalWrite(DIRPIN, LOW);
}

byte position_to_level(int position) {
	byte level = position <= HALF - 1 ? position : FULL_TURN - position;

//	Serial.print("Position: ");
//	Serial.print(position);
//	Serial.print("; Level: ");
//	Serial.println(level);

	return level;
}

// prescale 8, Phase Correct, 8bit ~ 3.9 kHz
void setup_leds() {
	pinMode(FRONTLED, OUTPUT);
	pinMode(BACKLED, OUTPUT);
	// prescale 8, Phase Correct, 8bit ~ 3.9 kHz
	TCCR2A = _BV(WGM20) | _BV(COM2A1) | _BV(COM2B1);
	TCCR2B = _BV(CS21);

	set_led_levels(position_to_level(current_postion));
}

void setup_switch() {
	pinMode(SWITCH, INPUT_PULLUP);
}

void setup() {
	setup_leds();
	setup_stepper();
//	setup_switch();

	Serial.begin(115200);
	Serial.println("Started");
}

int calc_steps(int from, int to, direction_t direction) {
	int steps = direction * (to - from);
	steps = steps >= 0 ? steps : FULL_TURN + steps;

//	Serial.print("From: ");
//	Serial.print(from);
//	Serial.print("; To: ");
//	Serial.print(to);
//	Serial.print("; Dir: ");
//	Serial.print(direction);
//	Serial.print("; Steps: ");
//	Serial.println(steps);

	return steps;
}

int calc_delay_us(int steps, int duration) {
	if (steps == 0) {
		return 0;
	}
	int delay = max(150, (((long ) duration) * 1000) / steps);

//	Serial.print("Steps: ");
//	Serial.print(steps);
//	Serial.print("; Duration: ");
//	Serial.print(duration);
//	Serial.print("; Delay: ");
//	Serial.println(delay);

	return delay;
}

void step_to_point(ControlPoint point) {
	int steps = calc_steps(current_postion, point.position, point.direction);
	if (steps == 0) {
		delay(point.duration);
		return;
	}
	int step_delay = calc_delay_us(steps, point.duration);
	for (int i = 0; i < steps; i++) {
		step(point.direction);
		set_led_levels(position_to_level(current_postion));
		delayMicroseconds(step_delay);
	}
}

void loop() {
	for (byte i = 0; i < POINTS_LEN; i++) {
//		Serial.println("Point: ");
//		Serial.println(i);

		step_to_point(POINTS[i]);
	}
//	boolean switch_state = !((boolean)digitalRead(SWITCH));
//
//	if (switch_state) {
//		Serial.println(switch_state);
//		delay(500);
//	}
}
