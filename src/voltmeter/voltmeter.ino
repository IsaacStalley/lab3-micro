#include <PCD8544.h>

static PCD8544 lcd;
const float VREF = 5.0; // Reference voltage of Arduino (in volts)
const int ADC_MAX_VALUE = 1023; // Maximum value of the ADC (10-bit ADC)

float global_max_voltageA0 = 0;
float global_max_voltageA1 = 0;
float global_max_voltageA2 = 0;
float global_max_voltageA3 = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  // PCD8544-compatible displays may have a different resolution...
  lcd.begin(84, 48);

}

float reverse_TF(int port){
  return analogRead(port) * VREF / (ADC_MAX_VALUE*0.104) - 2.5/0.104;
}


float get_max_voltage(int port){
  float max_voltage = abs(reverse_TF(port));
  float voltage = abs(reverse_TF(port));
  int counter = 0;

  while(1){
    if (voltage > max_voltage){
      while (1){
        max_voltage = voltage;
        voltage = abs(reverse_TF(port));
        if (voltage <= max_voltage){
          return max_voltage;
        }
      }
    }
    else {
      max_voltage = voltage;
      voltage = abs(reverse_TF(port));
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  int switch_DC = digitalRead(9);
  int switch_Serial = digitalRead(8);

  float voltageA0 = reverse_TF(A0);
  float voltageA1 = reverse_TF(A1);
  float voltageA2 = reverse_TF(A2);
  float voltageA3 = reverse_TF(A3);

  if (switch_DC) {
    lcd.setCursor(0, 0);
    lcd.print("DC Voltages");
    lcd.setCursor(0, 1);
    lcd.print("------------");
  
  }

  else {
    float max_voltage = get_max_voltage(A0);
    if (max_voltage > global_max_voltageA0 || global_max_voltageA0 - max_voltage > 1){
      global_max_voltageA0 = max_voltage;
    }

    voltageA0 = global_max_voltageA0 / sqrt(2);

    max_voltage = get_max_voltage(A1);
    if (max_voltage > global_max_voltageA1 || global_max_voltageA1 - max_voltage > 1){
      global_max_voltageA1 = max_voltage;
    }
    voltageA1 = global_max_voltageA1 / sqrt(2);
  
    max_voltage = get_max_voltage(A2);
    if (max_voltage > global_max_voltageA2 || global_max_voltageA2 - max_voltage > 1){
      global_max_voltageA2 = max_voltage;
    }
    voltageA2 = global_max_voltageA2 / sqrt(2);

    max_voltage = get_max_voltage(A3);
    if (max_voltage > global_max_voltageA3 || global_max_voltageA3 - max_voltage > 1){
      global_max_voltageA3 = max_voltage;
    }
    voltageA3 = global_max_voltageA3 / sqrt(2);

    lcd.setCursor(0, 0);
    lcd.print("AC Voltages");
    lcd.setCursor(0, 1);
    lcd.print("------------");
  }

  // Write a piece of text on the first line...
  lcd.setCursor(0, 2);
  lcd.print("V1: ");
  lcd.print(voltageA3, 2);

  lcd.setCursor(0, 3);
  lcd.print("V2: ");
  lcd.print(voltageA2, 2);

  lcd.setCursor(0, 4);
  lcd.print("V3: ");
  lcd.print(voltageA1, 2);

  lcd.setCursor(0, 5);
  lcd.print("V4: ");
  lcd.print(voltageA0, 2);
}
