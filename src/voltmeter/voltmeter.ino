#include <PCD8544.h>

static PCD8544 lcd;

void setup() {
  // put your setup code here, to run once:
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  // PCD8544-compatible displays may have a different resolution...
  lcd.begin(84, 48);

}

void loop() {
  // put your main code here, to run repeatedly:

  int switch_DC = digitalRead(9);
  int switch_Serial = digitalRead(8);

  if (switch_DC) {

    // Read analog voltages
    float voltageA0 = analogRead(A0) * 5.0 / (1023.0*0.104) - 2.5/0.104;  // Convert to voltage (assuming 5V Arduino)
    float voltageA1 = analogRead(A1) * 5.0 / (1023.0*0.104) - 2.5/0.104;
    float voltageA2 = analogRead(A2) * 5.0 / (1023.0*0.104) - 2.5/0.104;
    float voltageA3 = analogRead(A3) * 5.0 / (1023.0*0.104) - 2.5/0.104;
  
  }

  // Write a piece of text on the first line...
  lcd.setCursor(0, 0);
  lcd.print(voltageA3);

  lcd.setCursor(0, 1);
  lcd.print(voltageA2);

  lcd.setCursor(0, 2);
  lcd.print(voltageA1);

  lcd.setCursor(0, 3);
  lcd.print(voltageA0);

}
