#include <PCD8544.h>

static PCD8544 lcd;
const float VREF = 5.0;
const int ADC_MAX_VALUE = 1023;

float global_max_voltageA0 = 0;
float global_max_voltageA1 = 0;
float global_max_voltageA2 = 0;
float global_max_voltageA3 = 0;

void setup() {
  // serial startup
  Serial.begin(9600);

  // Pin declaration
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  // lcd startup
  lcd.begin(84, 48);

}

// Function that returns the real voltage value by reversing the transfer function
float reverse_TF(int port){
  return analogRead(port) * VREF / (ADC_MAX_VALUE*0.104) - 2.5/0.104;
}


// Funtion that returns the magnitud of the signal
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
    else if (voltage == max_voltage){
      return max_voltage;
    }
    else {
      max_voltage = voltage;
      voltage = abs(reverse_TF(port));
    }
  }
}

void loop() {
  // Input pins
  int switch_DC = digitalRead(9);
  int switch_Serial = digitalRead(8);

  // Read voltages
  float voltageA0 = reverse_TF(A0);
  float voltageA1 = reverse_TF(A1);
  float voltageA2 = reverse_TF(A2);
  float voltageA3 = reverse_TF(A3);

  // DC meter state
  if (switch_DC) {
    if (voltageA0 > 20 || voltageA0 < -20){
      digitalWrite(13, HIGH);
    }
    else {digitalWrite(13, LOW);}
    if (voltageA1 > 20 || voltageA1 < -20){
      digitalWrite(12, HIGH);
    }
    else {digitalWrite(12, LOW);}
    if (voltageA2 > 20 || voltageA2 < -20){
      digitalWrite(11, HIGH);
    }
    else {digitalWrite(11, LOW);}
    if (voltageA3 > 20 || voltageA3 < -20){
      digitalWrite(10, HIGH);
    }
    else {digitalWrite(10, LOW);}

    lcd.setCursor(0, 0);
    lcd.print("DC Voltages");
    lcd.setCursor(0, 1);
    lcd.print("-----------");
    
  }

  // AC meter state
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

    if (global_max_voltageA0 > 20 || global_max_voltageA0 < -20){
      digitalWrite(13, HIGH);
    }
    else {digitalWrite(13, LOW);}
    if (global_max_voltageA1 > 20 || global_max_voltageA1 < -20){
      digitalWrite(12, HIGH);
    }
    else {digitalWrite(12, LOW);}
    if (global_max_voltageA2 > 20 || global_max_voltageA2 < -20){
      digitalWrite(11, HIGH);
    }
    else {digitalWrite(11, LOW);}
    if (global_max_voltageA3 > 20 || global_max_voltageA3 < -20){
      digitalWrite(10, HIGH);
    }
    else {digitalWrite(10, LOW);}

    lcd.setCursor(0, 0);
    lcd.print("AC Voltages");
    lcd.setCursor(0, 1);
    lcd.print("-----------");
  }

  // Serial comunication
  if (!switch_Serial){
    Serial.print("V1: ");
    Serial.println(voltageA3);

    Serial.print("V2: ");
    Serial.println(voltageA2);

    Serial.print("V3: ");
    Serial.println(voltageA1);

    Serial.print("V4: ");
    Serial.println(voltageA0);

  }

  // Voltage prints on display
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
