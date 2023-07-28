#include <ArduinoJson.h>     //incluimos la libreria que crea objetos JSON
#include <TinyGPS.h>         //incluimos TinyGPS
#include <SoftwareSerial.h>  //incluimos SoftwareSerial
#include <Adafruit_Sensor.h>
#include <DHT.h>  //incluimos DHT
#include <DHT_U.h>


#define DHTPIN 2       // Pin digital al que está conectado el sensor DHT11
#define DHTTYPE DHT11  // Tipo de sensor DHT


DHT_Unified dht(DHTPIN, DHTTYPE);
TinyGPS gps;                     //Declaramos el objeto gps
SoftwareSerial serialgps(4, 3);  //Declaramos el pin 4 Tx y 3 Rx


void setup() {
  Serial.begin(115200);
  serialgps.begin(9600);  //Iniciamos el puerto serie del gps
  dht.begin();
  sensor_t sensor;
}


void loop() {

  while (serialgps.available()) {
    int c = serialgps.read();
    if (gps.encode(c)) {

      DynamicJsonDocument doc(1024);
      JsonArray array = doc.to<JsonArray>();

      // Get temperature event and print its value.
      sensors_event_t event;
      dht.temperature().getEvent(&event);
      if (isnan(event.temperature)) {
        Serial.println(F("Error reading temperature!"));
      } else {
        JsonObject temperature = doc.createNestedObject();

        temperature["name"] = "temperature";
        temperature["value"] = event.temperature;
        temperature["measurement"] = "celcius";

        // array.add(temperature);`
        // Serial.println(F("°C"));
      }

      // Get humidity event and print its value.
      dht.humidity().getEvent(&event);
      if (isnan(event.relative_humidity)) {
        Serial.println(F("Error reading humidity!"));
      } else {
        JsonObject humidity = doc.createNestedObject();

        humidity["name"] = "humidity";
        humidity["value"] = event.relative_humidity;
        humidity["measurement"] = "percent";

        // array.add(humidity);
        // Serial.print(event.relative_humidity);
      }

      // Leer los datos del gps
      float latitude;
      float longitude;

      gps.f_get_position(&latitude, &longitude);

      JsonObject gps = doc.createNestedObject();

      // Asignar los valores al objeto JSON
      gps["name"] = "location";
      gps["latitude"] = latitude;
      gps["longitude"] = longitude;
      gps["measurement"] = "grades";

      // Serializar el objeto JSON en una cadena
      serializeJson(array, Serial);

      // Enviar la cadena JSON a través del puerto serie
      Serial.println();

      delay(10000);  // Esperar 2 segundos antes de enviar la siguiente actualización
    }
  }
}