/* Example program to show an ADC average program
*/

//#include <Arduino.h>
const uint16_t ADC_BITS = 16;  // use 16 to match word sizes
const uint16_t DAC_BITS = 16;  // use 16 to match word sizes (DAC board)
const float V_REF = 3.317;     // measured, unit #1

uint16_t adc_val = 0;
char msg[80];   // container for formatted strings
// intervals for various timed events
const uint32_t SENSORS_INTERVAL = 100;  // ms
const uint32_t PRINT_INTERVAL = 500;    // ms
//incrememting timer variables
// elapsedMicros since_sensors
uint32_t since_sensors;  // ms
uint32_t since_print;    // ms

void setup() {
  analogReadResolution(ADC_BITS);   // Set all to same bit depth  note: actual ADC resolution is 10 on T3.2
  analogWriteResolution(DAC_BITS);  // scale analogWrite() value 0 to 2^N-1, or 2^N for steady ON

  uint16_t t0 = millis();
  Serial.begin(115200);
  while (!Serial && (millis() - t0 < 5000)) {}  // include timeout if print console isn't opened
  sprintf(msg, "\n  %4lu ms, Console Serial port initialized.\n", millis() - t0);
  Serial.print(msg);
}

void loop() {
  // get next sensor readings set
  uint8_t ch = A0;
  uint8_t num_readings = 8;
  uint8_t sample_delay_us = 100;  // us

  // compute at set interval
  if (millis() > since_sensors + SENSORS_INTERVAL) {
    since_sensors = millis();
    adc_val = readAndAverage(ch, num_readings, sample_delay_us);  // get reading from selected ADC channel
  }

  // print at set interval
  if (millis() > since_print + PRINT_INTERVAL) {
    since_print = millis();
    sprintf(msg, "  ADC: %5u \n", adc_val);
    Serial.print(msg);
  }
}

/// read N times and average at interval t
uint16_t readAndAverage(uint8_t ch, uint8_t N, uint8_t t) {
  int32_t val;
  int32_t val_sum = 0;
  // average N readings
  for (uint8_t i = 0; i < N; i++) {
    val = analogRead(ch);
    val_sum += val;
    delayMicroseconds(t);
  }
  return (uint16_t)(val_sum / N);
}
