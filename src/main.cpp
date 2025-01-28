#include <Arduino.h>
#include <Servo.h>

const int mapPin = A0;  // Pin sensor MAP
const int servoPin = 9; // Pin servo
Servo servo;

const int noPressureValue = 72; // Nilai sensor tanpa tekanan
int mapData[5][2] = {
    {0, 20}, // Sensor 0 -> Servo 20%
    {1, 25}, // Sensor 1 -> Servo 25%
    {4, 30}, // Sensor 4 -> Servo 30%
    {8, 40}, // Sensor 8 -> Servo 40%
    {9, 50}  // Sensor 9 -> Servo 50%
};

int calculateActualValue(int pressure);
int interpolateServoPosition(int actualValue);

void setup()
{
  Serial.begin(9600);
  servo.attach(servoPin);
  servo.write(0); // Awal servo di posisi 0%
  Serial.println("System Ready");
}

void loop()
{
  int sensorValue = analogRead(mapPin);                      // Membaca nilai sensor MAP
  int pressure = map(sensorValue, 0, 1023, 0, 100);          // Mapping nilai sensor ke 0-100
  int actualValue = calculateActualValue(pressure);          // Hitung nilai aktual
  int servoPosition = interpolateServoPosition(actualValue); // Hitung bukaan servo

  // Konversi persen bukaan ke derajat servo (0-180 derajat)
  int servoAngle = map(servoPosition, 0, 100, 0, 180);

  servo.write(servoAngle);  // Kirim posisi ke servo
  Serial.print("Sensor: ");
  Serial.print(sensorValue);
  Serial.print(" | Pressure: ");
  Serial.print(pressure);
  Serial.print(" | Actual: ");
  Serial.print(actualValue);
  Serial.print(" | Servo Position: ");
  Serial.print(servoPosition);
  Serial.print("% | Servo Angle: ");
  Serial.print(servoAngle);
  Serial.println("°");

  delay(500); // Delay pembacaan setiap 100 ms
}

// Fungsi untuk menghitung nilai aktual berdasarkan tekanan
int calculateActualValue(int pressure)
{
  if (pressure < noPressureValue)
  {
    return noPressureValue - pressure;
  }
  else
  {
    return pressure - noPressureValue;
  }
}

// Fungsi untuk menghitung bukaan servo berdasarkan interpolasi linear
int interpolateServoPosition(int actualValue)
{
  for (int i = 0; i < 4; i++)
  {
    // Jika actualValue berada di antara dua nilai dalam tabel
    if (actualValue >= mapData[i][0] && actualValue <= mapData[i + 1][0])
    {
      // Interpolasi linear
      float range = mapData[i + 1][0] - mapData[i][0];
      float proportion = (float)(actualValue - mapData[i][0]) / range;
      int servoStart = mapData[i][1];
      int servoEnd = mapData[i + 1][1];
      return servoStart + proportion * (servoEnd - servoStart);
    }
  }

  // Jika actualValue di luar jangkauan tabel
  if (actualValue < mapData[0][0])
  {
    return mapData[0][1];
  }
  else
  {
    return mapData[4][1];
  }
}
