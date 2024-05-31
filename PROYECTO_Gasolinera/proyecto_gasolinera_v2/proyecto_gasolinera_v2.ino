/*
          CONECCIONES
    1. el pulsador con pullup incluido, va al:    pin 3
    2. datos del sensor de flujo va al:           pin 2
    3. la bomba va al:                            pin 12
    4. dysplay datos al A4, reloj al A5

*/
//******* LIBRERIAS
#include <Wire.h>			// libreria de comunicacion por I2C
#include <LCD.h>			// libreria para funciones de LCD
#include <LiquidCrystal_I2C.h>		// libreria para LCD por I2C
LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7);
LiquidCrystal_I2C lcd2 (0x26, 2, 1, 0, 4, 5, 6, 7);

//******* PINES Y CONECCIONES
const int pulsador_ = 4;
const int pulsador_2 = 5;
const int bomba    = 12;
const int bomba2    = 11;
volatile double waterFlow;
volatile double waterFlow2;
boolean state = false;
boolean state2 = false;
int pulsador;
int pulsador2;
void setup() {
  //******** INICIO DE DATOS
  Serial.begin(9600);
  lcd.setBacklightPin(3,POSITIVE);

  lcd.setBacklight(HIGH);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("GASOLINERA");
  lcd2.setBacklightPin(3,POSITIVE);
  lcd2.setBacklight(HIGH);
  lcd2.begin(16, 2);
  lcd2.clear();
  lcd2.setCursor(1, 0);
  lcd2.print("GASOLINERA");
  delay(1500);
  lcd2.clear();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("BOMBA 2");
  lcd2.setCursor(0, 0);
  lcd2.print("BOMBA 1");

  waterFlow = 0;
  waterFlow2 = 0;

  pinMode(pulsador_, INPUT_PULLUP);
  pinMode(pulsador_2, INPUT_PULLUP);


  attachInterrupt(digitalPinToInterrupt(2), pulse, RISING);  //DIGITAL Pin 2: Interrupt 0
  attachInterrupt(digitalPinToInterrupt(3), pulse2, RISING);  //DIGITAL Pin n: Interrupt 0
  pinMode(bomba, OUTPUT);
  pinMode(bomba2, OUTPUT);
}


void loop() {
  pulsador = digitalRead(pulsador_);
  pulsador2 = digitalRead(pulsador_2);

  // ------------------ BOMBA 1
  if (pulsador == 1) {
    digitalWrite(bomba, 1);
    state = true;
    waterFlow = 0;
  }
  if (state == true) {
    lcd.setCursor(0, 0);
    lcd.print("waterFlow:");
    lcd.print(waterFlow);
    lcd.print("L");
  }
  if (waterFlow >= 1) {
    digitalWrite(bomba, 0);
    state = false;
  }

  // ------------------ BOMBA 2
  if (pulsador2 == 1) {
    digitalWrite(bomba2, 1);
    state2 = true;
    waterFlow2 = 0;
  }
  if (state2 == true) {
    lcd2.setCursor(0, 0);
    lcd2.print("waterFlow:");
    lcd2.print(waterFlow2);
    lcd2.print("L");
  }
  if (waterFlow2 >= 1) {
    digitalWrite(bomba2, 0);
    state2 = false;
  }
}
void pulse()
{
  waterFlow += 1.0 / 450.0;
}
void pulse2()
{
  waterFlow2 += 1.0 / 450.0;
}