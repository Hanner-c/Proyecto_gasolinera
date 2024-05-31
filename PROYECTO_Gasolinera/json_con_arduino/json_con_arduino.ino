#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
//String datosRecibidos = "";

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

void Dbomba(String datosRecibidos)
{
    String json = datosRecibidos;
    StaticJsonDocument<300> doc;
    DeserializationError error = deserializeJson(doc, datosRecibidos);
    if (error) { return; }

    String b = doc["bomba"];

    Serial.println(b);
   
}
void setup()
{
    Serial.begin(115200);

    //Serial.println("===== Object Example =====");
    //Serial.println("-- Serialize --");
    //SerializeObject();
    //Serial.println();
    Serial.println("-- Deserialize --");
    //DeserializeObject();

}

void loop()
{
  //Serial.println("===== Object Example =====");
  //Serial.println("-- Serialize --");
  //SerializeObject();
  //Serial.println();
  //Serial.println("-- Deserialize --");
  //DeserializeObject();

  //Serial.println(datosRecibidos);
  String datosRecibidos = Serial.readString();
  Serial.println(datosRecibidos);
  Dbomba(datosRecibidos);

  /*while (Serial.available() > 0) {
    char caracter = Serial.read();
    
    datosRecibidos += caracter;

    Serial.print(caracter);
  }

  if (datosRecibidos.endsWith("}")) {
    Serial.println("Mensaje completo: ");
    Serial.println(datosRecibidos);
    DeserializeObject();  
    datosRecibidos = "";
  }*/

  delay(1000);
}