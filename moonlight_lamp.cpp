// Do not remove the include below
#include "moonlight_lamp.h"

//#define DEBUG
#ifdef DEBUG
#	define DEBUG_PRINT(x) Serial.print(x)
#	define DEBUG_PRINTLN(x) Serial.println(x)
#else
#	define DEBUG_PRINT(x)
#	define DEBUG_PRINTLN(x)
#endif

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

const byte RESET_IDX = 255;

enum direction_t {
	COUNTER_CLOCKWISE = -1, CLOCKWISE = 1, WAIT = 0
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
		ControlPoint(WAIT, 0, 2000),
		ControlPoint(COUNTER_CLOCKWISE, HALF, 200),
		ControlPoint(WAIT, 0, 2000),
		ControlPoint(CLOCKWISE, QUARTER, 2000),
		ControlPoint(COUNTER_CLOCKWISE, 0, 1000),
		ControlPoint(WAIT, 0, 5000)
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
	digitalWrite(DIRPIN, direction == CLOCKWISE ? HIGH : LOW);
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
	setup_switch();

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

unsigned long calc_delay_us(int steps, int duration) {
	unsigned long duration_micros = (((unsigned long) duration) * 1000ul);
	unsigned long delay = max(150, steps == 0 ? duration_micros : duration_micros / steps);

//	Serial.print("Steps: ");
//	Serial.print(steps);
//	Serial.print("; Duration: ");
//	Serial.print(duration);
//	Serial.print("; Delay: ");
//	Serial.println(delay);

	return delay;
}

byte current_point_idx = 0;
ControlPoint current_point = POINTS[current_point_idx];

unsigned long current_micros = 0;
unsigned long trigger_micros = 0;
unsigned long elapsed_micros = 0;

unsigned long delay_micros = calc_delay_us(
		calc_steps(current_postion, current_point.position, current_point.direction),
		current_point.duration);

boolean reset_done = false;

boolean at_postion() {
	return current_point.direction == WAIT ? true : current_point.position == current_postion;
}

boolean delay_reached() {
	bool reached = elapsed_micros >= delay_micros;

#ifdef DEBUG
	if (reached) {
		DEBUG_PRINT("Delay ");
		DEBUG_PRINT(delay_micros);
		DEBUG_PRINT("us reached, delta ");
		DEBUG_PRINTLN(elapsed_micros - delay_micros);
	}
#endif
	return reached;
}

void set_point(ControlPoint point) {
	trigger_micros = current_micros;
	current_point = point;
	int steps = calc_steps(current_postion, current_point.position, current_point.direction);
	delay_micros = calc_delay_us(steps, current_point.duration);

//	Serial.print("ControlPoint(");
//	Serial.print(current_point.direction);
//	Serial.print(", ");
//	Serial.print(current_point.position);
//	Serial.print(", ");
//	Serial.print(current_point.duration);
//	Serial.println(")");
//	Serial.print("Trigger ");
//	Serial.print(trigger_micros);
//	Serial.print(" Delay ");
//	Serial.print(delay_micros);
//	Serial.print(" Steps ");
//	Serial.println(steps);
}

void next_point() {
	if (current_point_idx == RESET_IDX) {
//		Serial.println("Reset done!");
		reset_done = true;
		return;
	}
	current_point_idx = current_point_idx == POINTS_LEN - 1 ? 0 : current_point_idx + 1;
	set_point(POINTS[current_point_idx]);

//	Serial.print("Point ");
//	Serial.print(current_point_idx);
//	Serial.print(" on ");
//	Serial.println(current_micros);
}

void take_step() {
	trigger_micros = current_micros;
	step(current_point.direction);
	set_led_levels(current_postion);
	if (at_postion()) {
		next_point();
	}

//	Serial.print("Step; At position ");
//	Serial.println(current_postion);
}

boolean reset_switch_pressed() {
	return !((boolean) digitalRead(SWITCH));
}

void loop() {
	if (reset_done) {
		return;
	}

	current_micros = micros();
	elapsed_micros = current_micros - trigger_micros;

	if (delay_reached()) {
		if (at_postion()) {
			next_point();
		} else {
			take_step();
		}
	}

	if (reset_switch_pressed() and current_point_idx != RESET_IDX) {
//		Serial.println("Switch pressed");

		current_point_idx = RESET_IDX;
		set_point(ControlPoint(CLOCKWISE, 0, 500));
	}
}
