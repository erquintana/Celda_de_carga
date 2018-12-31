/*
 * Programa de Registro de Celda de Carga
 * 
 * Esteban Jiménez Sánchez
 * B13519
 * 28/AGO/2017
 * Versión: 1.0
 * 
 * Programa basado en el programa de calibración de celda de carga de SparkFun Electronics
 * el programa de Datalogging Arduino Shield de Adafruit, y el codigo ejemplo en del Arduino
 * Projects Book
 * 
 * Celda de Carga
 * 8 ----> CLK
 * 9 ----> DOUT
 * 5V ---> VCC
 * GND --> GND
 * 
 * Pantalla LCD
 * LCD ----> Arduino
 * 4 ------> 12
 * 6 ------> 11
 * 11 -----> 5
 * 12 -----> 4
 * 13 -----> 3
 * 14 -----> 2
 */

#include <LiquidCrystal.h>    //Libreria de Pantalla
#include "HX711.h"            //Libreria de Celda de Carga

//Librerias de Lector SD
#include <SPI.h>
#include <SD.h>

//Librerias de RTC
#include <Wire.h>
#include "RTClib.h"

//Inicialización de RTC
RTC_PCF8523 rtc;

const int chipSelect = 10; //Puerto de Tarjeta SD

LiquidCrystal lcd(6, 7, 5, 4, 3, 2);

//Definición de Pines de Celda de Carga
#define DOUT  9
#define CLK  8

#define LOG_INTERVAL  1 //Intervalo de Medición
#define SYNC_INTERVAL 1*LOG_INTERVAL //Intervalo de Registro

uint32_t syncTime = 0;//Tiempo de ultima sincronización

HX711 Sensor(DOUT, CLK); //Definicion de objeto de celda de carga

//FACTOR DE CALIBRACION
float calibration_factor = 7050; //Factor de Calibración para la Celda de Carga

File logfile;


/*void error(char *str)
{
  
  lcd.clear();
  lcd.print(char);
  delay (2000);

  while(1);
}*/

void setup() {
  lcd.begin(16, 2);
  lcd.print("Inicializando");
  lcd.setCursor(0,1);
  lcd.print("el sistema");

  pinMode(10, OUTPUT);

  if (!SD.begin(chipSelect)) {
   // error("Fallo de Tarjeta");
  }
  char filename[] = "LOGGER00.TXT";
  logfile = SD.open(filename, FILE_WRITE);
  
  if (! logfile) {
    //error("No se creo archivo");
  }
  
  logfile.println("Experimentación en Cohetería");
  logfile.println("Lecturas de Celda de Carga");
  logfile.println("");
  logfile.println("Fecha de Programación: 28-AGO-2017");
  logfile.println(filename);
  
  

  Wire.begin(); 

  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  logfile.println("Milli-Segundos,Fecha,Hora,Peso (Lbs)");  

  Sensor.set_scale(calibration_factor);
  Sensor.tare();  //Reset the scale to 0
  
long zero_factor = Sensor.read_average();

lcd.clear();
lcd.setCursor(0,0);
lcd.print("Factor zero:");
lcd.setCursor(0,1);
lcd.print(zero_factor);
delay(3000);

lcd.clear();
lcd.setCursor(0,0);
lcd.print("Inicializacion");
lcd.setCursor(0,1);
lcd.print("Finalizada");
delay(1000);
lcd.clear();
//delay(1000);
}

void loop() {
  DateTime now=rtc.now();
 // delay((LOG_INTERVAL -1) - (millis() % LOG_INTERVAL));
  uint32_t m = millis();

  logfile.print(m);
  logfile.print(",");

  now = rtc.now();
  logfile.print(now.year(), DEC);
  logfile.print("/");
  logfile.print(now.month(), DEC);
  logfile.print("/");
  logfile.print(now.day(), DEC);
  logfile.print(",");
  logfile.print(now.hour(), DEC);
  logfile.print(":");
  logfile.print(now.minute(), DEC);
  logfile.print(":");
  logfile.print(now.second(), DEC);

//  logfile.print(",");
//  logfile.print(daysOfTheWeek[now.dayOfTheWeek()]);
  
  
  double medida;
  medida=Sensor.get_units();
  lcd.setCursor(0,0);
  lcd.print("Peso: ");
  lcd.print(medida);
  lcd.print(" lbs");
  logfile.print(",");    
  logfile.print(medida);
  delay(50);
  lcd.clear();
  logfile.println();

  //  if ((millis() - syncTime) < SYNC_INTERVAL) return;
  syncTime = millis();
  logfile.flush();

}
