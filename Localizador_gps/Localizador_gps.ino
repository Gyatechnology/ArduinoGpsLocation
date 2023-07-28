#include <ArduinoJson.h> // Library for create JSON objects
#include <SoftwareSerial.h>//incluimos SoftwareSerial 
#include <TinyGPS.h>//incluimos TinyGPS
#include <DHT.h>

#define DHTPIN 2 // Pin digital al que está conectado el sensor DHT11
#define DHTTYPE DHT11 // Tipo de sensor DHT



DHT dht(DHTPIN, DHTTYPE);
TinyGPS gps;//Declaramos el objeto gps
SoftwareSerial serialgps(4,3);//Declaramos el pin 4 Tx y 3 Rx
//Declaramos la variables para la obtención de datos
int year;
byte month, day, hour, minute, second, hundredths;
unsigned long chars;
unsigned short sentences, failed_checksum;


void setup()
{
Serial.begin(115200);//Iniciamos el puerto serie
serialgps.begin(9600);//Iniciamos el puerto serie del gps
dht.begin();
//Imprimimos:
Serial.println("");
Serial.println("GPS GY-GPS6MV2 Leantec");
Serial.println(" ---Buscando senal--- ");
Serial.println("");
}

void loop() {
  while(serialgps.available()){
  int c = serialgps.read(); 
  if(gps.encode(c)) {
  // Leer datos del sensor DHT11
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Hacer algo con los datos del sensor DHT11, por ejemplo, imprimir en el monitor serial
  Serial.print("Humedad: ");
  Serial.print(humidity);
  Serial.print("%\t");
  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.println("°C");

  delay(2000); // Esperar 2 segundos antes de tomar nuevas lecturas
    float latitude, longitude;
  gps.f_get_position(&latitude, &longitude);
  Serial.print("Latitud/Longitud: "); 
  Serial.print(latitude,5); 
  Serial.print(", "); 
  Serial.println(longitude,5);
  gps.crack_datetime(&year,&month,&day,&hour,&minute,&second,&hundredths);
  Serial.print("Fecha: "); Serial.print(day, DEC); Serial.print("/"); 
  Serial.print(month, DEC); Serial.print("/"); Serial.print(year);
  Serial.print(" Hora: "); Serial.print(hour, DEC); Serial.print(":"); 
  Serial.print(minute, DEC); Serial.print(":"); Serial.print(second, DEC); 
  Serial.print("."); Serial.println(hundredths, DEC);
  Serial.print("Altitud (metros): "); Serial.println(gps.f_altitude()); 
  Serial.print("Rumbo (grados): "); Serial.println(gps.f_course()); 
  Serial.print("Velocidad(kmph): "); Serial.println(gps.f_speed_kmph());
  Serial.print("Satelites: "); Serial.println(gps.satellites());
  Serial.println();
   gps.stats(&chars, &sentences, &failed_checksum);
 }
}
