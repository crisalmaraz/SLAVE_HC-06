#include <SPI.h>
#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SH1106.h>
#include <OneWire.h>
#include <SoftwareSerial.h>

#include <ArduinoJson.h>

#define SSDT 7 //controlTemperatura
#define SSDV 8//Vacio

//#include <LiquidCrystal_I2C.h>

//Adafruit_SH1106 display(4);
SoftwareSerial BT(3, 2);

//LiquidCrystal_I2C lcd(0x27, 16, 2);


const char* json = "{\"TempTermocople\":150,\"TempDs18b20_1\":150,\"TempDs18b20_2\":150,\"TempBme280\":150,\"HumedadBme280\":100,\"HumedadResistiva\":100,\"PresionBme280\":99999.99,\"Altitud_m\":9999.99}";

const char* jsonControl = "{\"estado\":0,\"modo\":false,\"tempI\":0,\"tempS\":0,\"sc\":false,\"sv\":false}";
String lec;

char entrada;
int TempTermocople = 0, TempDs18b20_1 = 0, TempDs18b20_2 = 0, TempBme280 = 0, HumedadBme280 = 0,  HumedadResistiva = 0, PresionBme280 = 0, Altitud_m = 0;
int estado = 0;
bool modo = false;
int tempI = 0;
int tempS = 0;
bool sc = false;
bool sv = false;


void setup()   {
  pinMode(SSDT, OUTPUT);
  //pinMode(SSDB1, OUTPUT);
  //pinMode(SSDB2, OUTPUT);
  pinMode(SSDV, OUTPUT);

  Serial.begin(9600);

  //lcd.begin();
  //lcd.backlight();

  BT.begin(9600);
  Serial.print("test");
}


void loop() {
  if (Serial.available()) {
    const size_t capacity = JSON_OBJECT_SIZE(6) + 40;
    DynamicJsonDocument doc(capacity);
    DeserializationError error = deserializeJson(doc, Serial);

    if (error) {
      //Serial.print("error al deserealización: ");
      //Serial.println(error.c_str());
      return;
    } else {
      estado = doc["estado"];
      modo = doc["modo"];
      tempI = doc["tempI"];
      tempS = doc["tempS"];
      sc = doc["sc"];
      sv = doc["sv"];
    }

    //Serial.println(estado);
    //Serial.println(modo);
    //Serial.println(temp);
    //Serial.println(sc);
    //Serial.println(sv);
  }


  if (BT.available())  {
    const size_t capacity = JSON_OBJECT_SIZE(8) + 130;
    DynamicJsonDocument doc(capacity);
    DeserializationError error = deserializeJson(doc, BT);
    if (error) {
      //Serial.print("error al deserealización: ");
      //Serial.println(error.c_str());
      return;
    } else {
      TempTermocople = doc["TempTermocople"];
      TempDs18b20_1 = doc["TempDs18b20_1"];
      TempDs18b20_2 = doc["TempDs18b20_2"];
      TempBme280 = doc["TempBme280"];
      HumedadBme280 = doc["HumedadBme280"];
      HumedadResistiva = doc["HumedadResistiva"];
      PresionBme280 = doc["PresionBme280"];
      Altitud_m = doc["Altitud_m"];

      serializeJson(doc, Serial);
      Serial.println();


    }

  }

  if (estado) {
    if (sv) {
      digitalWrite(SSDV, HIGH);
    } else {
      digitalWrite(SSDV, LOW);
    }

    if (modo) {
      if (TempTermocople < tempI) {
        digitalWrite(SSDT, HIGH);
      }

      if (TempTermocople > tempS) {
        digitalWrite(SSDT, LOW);
      }
    } else {
      if (sc) {
        digitalWrite(SSDT, HIGH);
      } else {
        digitalWrite(SSDT, LOW);
      }
    }
    
 } else {
   digitalWrite(SSDT, LOW);
   digitalWrite(SSDV, LOW);
 }
}
