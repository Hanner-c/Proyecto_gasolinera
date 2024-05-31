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

String puesto = "";
double cantidad = 0.00;
double gasolina = 0.00;
double gasolina2 = 0.00;
double datos = 0;
bool estado1 = false;
bool estado2 = false;

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
  lcd.print("BOMBA 1");
  lcd2.setCursor(0, 0);
  lcd2.print("BOMBA 2");

  waterFlow = 0;
  waterFlow2 = 0;

  pinMode(pulsador_, INPUT_PULLUP);
  pinMode(pulsador_2, INPUT_PULLUP);


  attachInterrupt(digitalPinToInterrupt(2), pulse, RISING);  //DIGITAL Pin 2: Interrupt 0
  attachInterrupt(digitalPinToInterrupt(3), pulse2, RISING);  //DIGITAL Pin n: Interrupt 0
  pinMode(bomba, OUTPUT);
  pinMode(bomba2, OUTPUT);

  Serial.println("-- Nueva comunicacion --");
}

void loop() {
  pulsador = digitalRead(pulsador_);
  pulsador2 = digitalRead(pulsador_2);
  String datosRecibidos = Serial.readString();
  datos = datosRecibidos.toDouble();
  // ------------------ descomposicion de datos
  if(datos >= 2000.00){
    puesto = "bomba 2";
    datos = datos - 2000.00;
    gasolina2 = datos;
    Serial.println(datosRecibidos);
    Serial.println(puesto);
    Serial.println(gasolina2);
    estado2 = true;
  }
  else if(datos >= 1000.00){
    puesto = "bomba 1";
    datos = datos - 1000.00;
    gasolina = datos;
    Serial.println(datosRecibidos);
    Serial.println(puesto);
    Serial.println(gasolina);
    estado1 = true;
  }

  // ------------------ BOMBA 1
  if (pulsador == 1 && estado1 == true) {
    digitalWrite(bomba, 1);
    state = true;
    waterFlow = 0.00;
    estado1 = false;
  }
  if (state == true) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(puesto);
    lcd.setCursor(0, 1);
    lcd.print(waterFlow);
    lcd.print("L");
  }
  if (waterFlow >= gasolina && state == true) {
    digitalWrite(bomba, 0);
    state = false;
    lcd.clear();
    Serial.println("termine, bomba1");
    gasolina = 0;
    puesto = "";
    datos= 0;
  }

  // ------------------ BOMBA 2
  if (pulsador2 == 1 && estado2 == true) {
    digitalWrite(bomba2, 1);
    state2 = true;
    waterFlow2 = 0.00;
    estado2 = false;
  }
  if (state2 == true) {
    lcd2.clear();
    lcd2.setCursor(0, 0);
    lcd2.print(puesto);
    lcd2.setCursor(0, 1);
    lcd2.print(waterFlow2);
    lcd2.print("L");
  }
  if (waterFlow2 >= gasolina2 && state2 == true) {
    digitalWrite(bomba2, 0);
    state2 = false;
    lcd2.clear();
    Serial.println("termine, bomba 2");
    gasolina2 = 0;
    puesto = "";
    datos= 0;
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