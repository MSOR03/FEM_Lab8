#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

// Crear un objeto para el sensor
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

// -------------------------------
// Valores máximos y mínimos de cada eje (X, Y, Z)
float Xmax = 37.73;
float Xmin = -23.18;

float Ymax = 31.55;
float Ymin = -35;

float Zmax = 15.31;
float Zmin = -54.39;
// -------------------------------

void setup() {
  Serial.begin(9600);
  Serial.println("X,Y,Z,Magnitud,Heading");

  if (!mag.begin()) {
    Serial.println("No se detectó el HMC5883L. Verifica las conexiones o el modelo.");
    while (1);
  }
}

void loop() {
  sensors_event_t event;
  mag.getEvent(&event);

  // Valores crudos del sensor
  float rawX = event.magnetic.x;
  float rawY = event.magnetic.y;
  float rawZ = event.magnetic.z;

  // -------------------------------
  // Calcular offset por eje
  float offsetX = (Xmax + Xmin) / 2.0;
  float offsetY = (Ymax + Ymin) / 2.0;
  float offsetZ = (Zmax + Zmin) / 2.0;

  // Calcular escala por eje
  float scaleX = (Xmax - Xmin) / 2.0;
  float scaleY = (Ymax - Ymin) / 2.0;
  float scaleZ = (Zmax - Zmin) / 2.0;

  // Escala promedio
  float avgScale = (scaleX + scaleY + scaleZ) / 3.0;

  // Corrección de offset y soft iron
  float x = (rawX - offsetX) * (avgScale / scaleX);
  float y = (rawY - offsetY) * (avgScale / scaleY);
  float z = (rawZ - offsetZ) * (avgScale / scaleZ);
  // -------------------------------

  // Magnitud total del campo corregido
  float B_total = sqrt(x * x + y * y + z * z);

  // Dirección (rumbo magnético)
  float heading = atan2(y, x) * 180.0 / PI;
  if (heading < 0) heading += 360.0;

  // Imprimir datos en formato CSV
  Serial.print(x); Serial.print(",");
  Serial.print(y); Serial.print(",");
  Serial.print(z); Serial.print(",");
  Serial.print(B_total); Serial.print(",");
  Serial.println(heading);

  delay(500);
}