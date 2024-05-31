//******* LIBRERIAS
#include <Wire.h>			// libreria de comunicacion por I2C
#include <LCD.h>			// libreria para funciones de LCD
#include <LiquidCrystal_I2C.h>		// libreria para LCD por I2C
LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7);
LiquidCrystal_I2C lcd2 (0x26, 2, 1, 0, 4, 5, 6, 7);

//******* PINES Y CONECCIONES
const int pulsador_ = 4;
const int pulsador_2 = 5;
const int bomba = 11;
const int bomba2 = 12;
volatile double waterFlow;
volatile double waterFlow2;
boolean state = false;
boolean state2 = false;
//int pulsador;
//int pulsador2;

String puesto = "";
double cantidad = 0.00;
double gasolina = 0.00;
double gasolina2 = 0.00;
double datos = 0;
bool estado1 = false;
bool estado2 = false;

int var = 0;
int var2 = 0;

void SerializeObject()
{
    String json;
    StaticJsonDocument<300> doc;
    doc["bomba"] = 1;
    doc["cantidad"] = 10;
    doc["tipo"] = true;

    serializeJson(doc, json);
    Serial.println(json);
}

void DeserializeObject()
{
    //String json = "{\"bomba\":2,\"cantidad\":10}";
    //String json = "{\"bomba\":2,\"cantidad\":10,\"tipo\":\"regular\"}";
    //String json = datosRecibidos;
    String json = Serial.readString();

    StaticJsonDocument<300> doc;
    DeserializationError error = deserializeJson(doc, json);
    if (error) { return; }

    int bomba = doc["bomba"];
    float cantidad = doc["cantidad"];
    String tipo = doc["tipo"];

    Serial.println(bomba);
    Serial.println(cantidad);
    Serial.println(tipo);
}

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
  digitalWrite(bomba, HIGH);
  digitalWrite(bomba2, HIGH);
}

void loop() {

  String datosRecibidos = Serial.readString();
  datos = datosRecibidos.toDouble();
  // ------------------ descomposicion de datos
  if(datos >= 2000.00){
    puesto = "BOMBA 2";
    datos = datos - 2000.00;
    gasolina2 = datos;
    estado2 = true;
    lcd2.clear();
    lcd2.setCursor(0, 0);
    lcd2.print(puesto);
    lcd2.setCursor(0, 1);
    lcd2.print("Lista para dispensar");
    if(gasolina2 <= 0){
      var2 = 2;
    }
    else if(gasolina2 > 0){
      var2 = 1;
    }
  }
  else if(datos >= 1000.00){
    puesto = "BOMBA 1";
    datos = datos - 1000.00;
    gasolina = datos;
    estado1 = true;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(puesto);
    lcd.setCursor(0, 1);
    lcd.print("Lista para dispensar");
    if(gasolina <= 0){
      var = 2;
    }
    else if(gasolina > 0){
      var = 1;
    }
  }

  // ------------------ BOMBA 1
  if (digitalRead(pulsador_) == 1 && estado1 == true) {
    digitalWrite(bomba, 0);
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
  /*if (waterFlow >= gasolina && state == true) {
    digitalWrite(bomba, 1);
    state = false;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(puesto);
    lcd.setCursor(0, 1);
    lcd.print("Carga completa");

    //Serial.println("termine, bomba1");
    double codigo = 1000 + gasolina;
    Serial.println(codigo);
    gasolina = 0;
    puesto = "";
    datos= 0;
  }*/
  switch (var) {
    case 1:
    delay(2000);
      if (waterFlow >= gasolina && state == true) {
      digitalWrite(bomba, 1);
      state = false;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(puesto);
      lcd.setCursor(0, 1);
      lcd.print("Carga completa");

      //Serial.println("termine, bomba1");
      double codigo = 1000 + gasolina;
      Serial.println(codigo);
      gasolina = 0;
      puesto = "";
      datos= 0;
      }
      else if (digitalRead(pulsador_) == 1 && state == true) {
      digitalWrite(bomba, 1);
      state = false;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(puesto);
      lcd.setCursor(0, 1);
      lcd.print("Carga completa");
      double codigo = 1000 + waterFlow;
      Serial.println(codigo);
      gasolina = 0;
      puesto = "";
      datos= 0;
      }
      break;
    case 2:
      delay(2000);
      if (digitalRead(pulsador_) == 1 && state == true) {
        digitalWrite(bomba, 1);
        state = false;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(puesto);
        lcd.setCursor(0, 1);
        lcd.print("Terminando");

        //Serial.println("termine, bomba1");
        double codigo = 1000 + waterFlow;
        Serial.println(codigo);
        gasolina = 0;
        puesto = "";
        datos= 0;
        var = 0;
      }
      break;
  }

  // ------------------ BOMBA 2
  if (digitalRead(pulsador_2) == 1 && estado2 == true) {
    digitalWrite(bomba2, 0);
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
  /*if (waterFlow2 >= gasolina2 && state2 == true) {
    digitalWrite(bomba2, 1);
    state2 = false;
    lcd2.clear();
    lcd2.setCursor(0, 0);
    lcd2.print(puesto);
    lcd2.setCursor(0, 1);
    lcd2.print("Carga completa");

    //Serial.println("termine, bomba 2");
    double codigo = 2000.00 + gasolina2;
    Serial.println(codigo);
    gasolina2 = 0;
    puesto = "";
    datos= 0;
  }*/

  switch (var2) {
    case 1:
      delay(2000);
        if (waterFlow2 >= gasolina2 && state2 == true) {
          digitalWrite(bomba2, 1);
          state2 = false;
          lcd2.clear();
          lcd2.setCursor(0, 0);
          lcd2.print(puesto);
          lcd2.setCursor(0, 1);
          lcd2.print("Carga completa");

          //Serial.println("termine, bomba 2");
          double codigo = 2000.00 + gasolina2;
          Serial.println(codigo);
          gasolina2 = 0;
          puesto = "";
          datos= 0;
        }
      else if (digitalRead(pulsador_2) == 1 && state2 == true) {
      digitalWrite(bomba2, 1);
      state2 = false;
      lcd2.clear();
      lcd2.setCursor(0, 0);
      lcd2.print(puesto);
      lcd2.setCursor(0, 1);
      lcd2.print("Carga completa");

      //Serial.println("termine, bomba 2");
      double codigo = 2000.00 + waterFlow2;
      Serial.println(codigo);
      gasolina2 = 0;
      puesto = "";
      datos= 0;
      }
      break;
    case 2:
    delay(2000);
      if (digitalRead(pulsador_2) == 1 && state2 == true) {
        digitalWrite(bomba2, 1);
        state2 = false;
        lcd2.clear();
        lcd2.setCursor(0, 0);
        lcd2.print(puesto);
        lcd2.setCursor(0, 1);
        lcd2.print("Terminando");

        //Serial.println("termine, bomba1");
        double codigo = 2000 + waterFlow2;
        Serial.println(codigo);
        gasolina2 = 0;
        puesto = "";
        datos= 0;
        var2 = 0;
      }
      break;
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