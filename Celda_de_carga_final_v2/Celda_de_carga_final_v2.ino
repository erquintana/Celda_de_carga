/*%%%%%%%%%%%%%%%%%%   Programa de Registro de Celda de Carga Versión 2.0    %%%%%%%%%%%%%%%%%%

*El siguiente código fue desarrollado con la finalidad de obtener datos experimetales
del comportamiento de los motores manufacturados por el Grupo de Ingeniería Aeroespacial
de la Universidad de Costa Rica.

*Version 1.0:
  -por Esteban Jiménez Sánchez
  -B13519
  -28/AGO/2017

*Version 2.0
  -por Esteban Rodríguez Quintana
  -B66076
  -DIC/2018

*Se utilizaron librerías de ejemplo de H711, LiquidCrystal, SD, SPI, RTClib.

*Lista de Componentes:
  -
  -
  -
  -
  -
  -

*Conexiones:
  -LCD RS pin to digital pinS 12
  -LCD Enable pin to digital pin 11
  -LCD D4 pin to digital pin 5
  -LCD D5 pin to digital pin 4
  -LCD D6 pin to digital pin 3
  -LCD D7 pin to digital pin 2
  -LCD R/W pin to ground
  -LCD VSS pin to ground
  -LCD VCC pin to 5V
  -10K resistor:
  -ends to +5V and ground
  -wiper to LCD VO pin (pin 3)

*/

//%%%%%%%%%%%%%%%%% LIBRERÍAS %%%%%%%%%%%%%%%%%
#include <HX711.h>    //librería sensor de carga
#include <LiquidCrystal.h>      //librería LCD
#include <SPI.h>      //librería Datalogging
#include <SD.h>     //librería Datalogging
#include <RTClib.h>     //librería reloj tiempo real
#include <Wire.h>     //librería reloj tiempo real
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

char filename[] = "LOGGER00.cvs";     //nombre del archivo donde se graba
#define boton_amarillo 12
#define boton_rojo 11
#define LOG_INTERVAL 10      //tiempo en milisegundos para intervalo de medición
#define factor_de_calibracion 12500.00
//LOGGER
#define chipSelect 10      //declaración pin SD
File logfile;     //decalración de objeto para grabar en archivo

//RTC
RTC_PCF8523 rtc;      //declaracion del objeto RTC

//LCD
const int rs = 6, en = 7, d4 = 5, d5 = 4, d6 = 3, d7 = 2;     //declaracion de pines para LCD
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);      //declaración objeto LCD

//SENSOR DE CARGA
const int DOUT = 9, CLK = 8;      //declaración salida de datos y reloj
HX711 sensor(DOUT,CLK);     //declaración del objeto para sensor de carga

//%%%%%%%%%%%%%%%%% VOID SETUP %%%%%%%%%%%%%%%%%
void setup() {
  Serial.begin(38400);      //inicialización de comunicación serial
  lcd.begin(16,2);      //inicialización de comunucación con LCD
  Wire.begin();       //inicialización de comunucación con RTC

  pinMode(chipSelect,OUTPUT);
  pinMode(boton_amarillo, INPUT);     //activación de pin para boton amarillo
  pinMode(boton_rojo, INPUT);     //activación de pin para boton rojo

  sensor.set_scale(factor_de_calibracion);     //defie factor de calibración en sensor de carga
  sensor.tare();      //setea escala a cero
  long zero_factor = sensor.read_average();     //factor cero para sensor de carga

  revisar_nombre(filename);     //revisa si filename ya existe

  logfile = SD.open(filename  , FILE_WRITE);      //variable usada para escritura en filename

//IMRESIÓN DE ENCABEZADO PARA SERIAL Y ARCHIVO .cvs:
  logfile.println("Universidad de Costa Rica");
  logfile.println("Grupo de Ingeniería Aeroespacial");
  logfile.println("");
  logfile.println("Experimentación en Cohetería");
  logfile.println("Tomando datos en:  ");
  logfile.println(filename);
  logfile.println("");

//ECHO A SERIAL:
  Serial.println("Universidad de Costa Rica");
  Serial.println("Grupo de Ingeniería Aeroespacial");
  Serial.println("");
  Serial.println("Experimentación en Cohetería");
  Serial.println("Tomando datos en:  ");
  Serial.println(filename);
  Serial.println("");

//IMPRESIONES PARA LCD:
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GIA - UCR");
  delay(3000);
  lcd.clear();
  lcd.print("Experimentacion");
  lcd.setCursor(0,1);
  lcd.print("en coheteria");
  delay(2000);
  lcd.clear();
  lcd.print("Inicializando SD");
  delay(2000);
//REVISION SI SD ESTÁ PRESENTE EN EL DATALOGGER:
  if(!SD.begin(chipSelect)){
    logfile.println("SD no inicializada correctamente");
    Serial.println("SD no inicializada correctamente");
    lcd.setCursor(0,0);
    lcd.print("SD no disponible");
    while(1);
  };

  logfile.println("SD inicializada corectamente");
  Serial.println("SD inicializada corectamente");

  logfile.println("SD inicializada corectamente");
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.print("SD inicializada");
  lcd.setCursor(0,1);
  lcd.print("correctamente");
  delay(2000);
  lcd.clear();
  lcd.print("Presione:");
  delay(2000);
  lcd.clear();
  lcd.print("Amarillo->iniciar");
  lcd.setCursor(0,1);
  lcd.print("Rojo->detiene");

  logfile.println("|\tMillis\t|\tDate\t|\tgramos\t|");
  Serial.println("|\tMillis\t|\tDate\t|\tgramos\t|");


}      //final de void setup

//%%%%%%%%%%%%%%%%% VOID LOOP %%%%%%%%%%%%%%%%%
void loop() {

  int iniciar = digitalRead(boton_amarillo);
  int detener = digitalRead(boton_rojo);

  lcd.clear();
  lcd.print("Tomando datos en:");
  lcd.setCursor(1,0);
  lcd.print(filename);

  if (logfile) {




    }

    else {
      lcd.clear();
      lcd.home();
      lcd.print("Eror abriedo")
      lcd.setCursor(1,0);
      lcd.print(filename);

      Serial.print("Error abriendo ");
      Serial.println(filename);
    }
}     //final de void loop


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% METODOS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//REVISION DE EXISTENCIA DEL ARCHIVO FILENAME PARA CREAR NUEVO EN CASO DE QUE YA EXISTA
//nombre_archivo debe ser cadena donde 6 y 7 son digitos cero
    char revisar_nombre(char nombre_archivo[]){
      while( SD.exists(nombre_archivo)){
        for(uint8_t i = 0; i<100; i++){
          nombre_archivo[6] = i/10 + '0';
          nombre_archivo[7] = i%10 + '0';
          break;
        };
      };
      return nombre_archivo;
    };

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
