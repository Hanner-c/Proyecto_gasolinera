/*
          CONECCIONES
    1. el pulsador con pullup incluido, va al:    pin 3
    2. datos del sensor de flujo va al:           pin 2
    3. la bomba va al:                            pin 12
    4. dysplay datos al A4, reloj al A5

    pablo sugiere estas conecciones
*/
//******* LIBRERIAS
#include <Wire.h>			// libreria de comunicacion por I2C
#include <LCD.h>			// libreria para funciones de LCD
#include <LiquidCrystal_I2C.h>		// libreria para LCD por I2C
LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7);
LiquidCrystal_I2C lcd2 (0x26, 2, 1, 0, 4, 5, 6, 7);

//******* PINES Y CONECCIONES
const int pulsador_ = 3;
const int pulsador_2 = 4;
const int bomba    = 12;
volatile double waterFlow;
boolean state = false;
int pulsador;

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
  delay(600);
  lcd2.clear();
  lcd.clear();
  lcd.setCursor(0, 0);
  //lcd.print();
  lcd.print("waterFlow: ");
  lcd.print(waterFlow);
  lcd.print(" L");
  lcd2.setCursor(0, 0);
  //lcd.print();
  lcd2.print("waterFlow: ");
  lcd2.print(waterFlow);
  lcd2.print(" L");

  waterFlow = 0;
  pinMode(pulsador_, INPUT_PULLUP); //Resistencia de pullup interna
  attachInterrupt(0, pulse, RISING);  //DIGITAL Pin 2: Interrupt 0
  pinMode(bomba, OUTPUT);
}


void loop() {
  pulsador = digitalRead(pulsador_);
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
}
void pulse()
{
  waterFlow += 1.0 / 450.0;
}