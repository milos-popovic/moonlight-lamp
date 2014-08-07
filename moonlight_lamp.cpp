// Do not remove the include below
#include "moonlight_lamp.h"
#include "moonlight_features.h"
#include "moonlight_level_tables.h"
#include "moonlight_coreography.h"

const byte RESET_IDX = 255;

unsigned int current_position = 0;

unsigned int to_led_level(byte position) {
	unsigned int level = LEVEL_MAP[position];

	DEBUG_PRINT("## to_led_level ##\t");
	DEBUG_PRINT("Position: ");
	DEBUG_PRINT(position);
	DEBUG_PRINT("; Level: ");
	DEBUG_PRINTLN(level);

	return level;
}

void update_led_levels() {
	byte front_pos = current_position < HALF ? current_position : FULL_TURN - current_position;
	unsigned int front = to_led_level(front_pos);
	unsigned int back = to_led_level(HALF - front_pos);

	OCR1A = front;
	OCR1B = back;

	DEBUG_PRINT("## update_led levels ##\t");
	DEBUG_PRINT("OCR1A: ");
	DEBUG_PRINT(OCR1A);
	DEBUG_PRINT("; OCR1B: ");
	DEBUG_PRINTLN(OCR1B);
}

void sleep_stepper(boolean sleep) {
	digitalWrite(SLEEP, (sleep ? LOW : HIGH));
}

void step(direction_t direction) {
	sleep_stepper(false);
	digitalWrite(DIR, direction == CLOCKWISE ? HIGH : LOW);
	digitalWrite(STEP, HIGH);
	digitalWrite(STEP, LOW);
	current_position += direction;

	if (current_position == -1) {
		current_position = FULL_TURN - 1;
	} else if (current_position == FULL_TURN) {
		current_position = 0;
	}
}

void setup_stepper() {
	pinMode(STEP, OUTPUT);
	pinMode(DIR, OUTPUT);
	pinMode(SLEEP, OUTPUT);
	sleep_stepper(false);
	digitalWrite(DIR, LOW);
}

// prescale 8, Phase Correct, 8bit ~ 3.9 kHz
void setup_leds() {
	pinMode(FRONTLED, OUTPUT);
	pinMode(BACKLED, OUTPUT);
	// prescale 8, Phase Correct, 8bit ~ 3.9 kHz
	TCCR1A = _BV(WGM11) | _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1);
	TCCR1B = _BV(WGM12) | _BV(CS11);

	update_led_levels();
}

void setup_switch() {
	pinMode(SWITCH, INPUT_PULLUP);
}

#ifdef MEASURE_TMP

unsigned long trigger_tmp = 0;

void setup_tmp() {
	pinMode(TMP, INPUT);
}

float get_temp(byte read_times) {
	// converting from a 0 to 1024 digital range
	// to 0 to 5 volts (each 1 reading equals ~ 5 millivolts

	analogRead(TMP);// discard first reading in the loop
	float result = 0.0f;
	for (byte i = 0; i < read_times; i++) {
		result += (float) analogRead(TMP);
	}
	result /= (float) read_times;

	result *= 4.8828125f;
	result = (result - 480.0f) / 10.0f;
	return result;
}
#endif

void setup() {
	setup_leds();
	setup_stepper();
	setup_switch();

#ifdef MEASURE_TMP
	setup_tmp();
#endif

#if defined MEASURE_TMP || defined DEBUG
	Serial.begin(115200);
	Serial.println("Started");
#endif
}

int calc_steps(int from, int to, direction_t direction) {
	int steps = direction * (to - from);
	steps = steps >= 0 ? steps : FULL_TURN + steps;

	DEBUG_PRINT("## calc_steps ##\t");
	DEBUG_PRINT("From: ");
	DEBUG_PRINT(from);
	DEBUG_PRINT("; To: ");
	DEBUG_PRINT(to);
	DEBUG_PRINT("; Dir: ");
	DEBUG_PRINT(direction);
	DEBUG_PRINT("; Steps: ");
	DEBUG_PRINTLN(steps);

	return steps;
}

unsigned long calc_delay_us(int steps, int duration) {
	unsigned long duration_micros = (((unsigned long) duration) * 1000ul);
	unsigned long delay = max(150, steps == 0 ? duration_micros : duration_micros / steps);

	DEBUG_PRINT("## calc_delay_us ##\t");
	DEBUG_PRINT("Steps: ");
	DEBUG_PRINT(steps);
	DEBUG_PRINT("; Duration: ");
	DEBUG_PRINT(duration);
	DEBUG_PRINT("; Delay: ");
	DEBUG_PRINTLN(delay);

	return delay;
}

byte current_point_idx = 0;
ControlPoint current_point = POINTS[current_point_idx];

unsigned long current_micros = 0;
unsigned long trigger_micros = 0;
unsigned long elapsed_micros = 0;

unsigned long delay_micros = calc_delay_us(
		calc_steps(current_position, current_point.position, current_point.direction),
		current_point.duration);

boolean reset_done = false;

boolean at_postion() {
	return current_point.direction == WAIT ? true : current_point.position == current_position;
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
	int steps = calc_steps(current_position, current_point.position, current_point.direction);
	delay_micros = calc_delay_us(steps, current_point.duration);

	DEBUG_PRINT("## set_point ##\t");
	DEBUG_PRINT("ControlPoint(");
	DEBUG_PRINT(current_point.direction);
	DEBUG_PRINT(", ");
	DEBUG_PRINT(current_point.position);
	DEBUG_PRINT(", ");
	DEBUG_PRINT(current_point.duration);
	DEBUG_PRINTLN(")");
	DEBUG_PRINT("Trigger ");
	DEBUG_PRINT(trigger_micros);
	DEBUG_PRINT(" Delay ");
	DEBUG_PRINT(delay_micros);
	DEBUG_PRINT(" Steps ");
	DEBUG_PRINTLN(steps);
}

void next_point() {
	if (current_point_idx == RESET_IDX) {
		DEBUG_PRINTLN("Reset done!");
		reset_done = true;
		return;
	}
	current_point_idx = current_point_idx == POINTS_LEN - 1 ? 0 : current_point_idx + 1;
	set_point(POINTS[current_point_idx]);

	DEBUG_PRINT("## next_point ##\t");
	DEBUG_PRINT("Point ");
	DEBUG_PRINT(current_point_idx);
	DEBUG_PRINT(" on ");
	DEBUG_PRINTLN(current_micros);
}

void take_step() {
	trigger_micros = current_micros;
	step(current_point.direction);
	update_led_levels();
	if (at_postion()) {
		if (current_position % 8 == 0) {
			sleep_stepper(true);
		}
		next_point();
	}

	DEBUG_PRINT("## take_step ##\t");
	DEBUG_PRINT("Step; At position ");
	DEBUG_PRINTLN(current_position);
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
		DEBUG_PRINTLN("Switch pressed");

		current_point_idx = RESET_IDX;
		set_point(ControlPoint(CLOCKWISE, 0, 500));
	}

#ifdef MEASURE_TMP
	if (current_micros - trigger_tmp > 1000000) {
		trigger_tmp = current_micros;
		DEBUG_PRINT("Temperature: ");
		DEBUG_PRINTLN(get_temp(5));
	}
#endif
}
