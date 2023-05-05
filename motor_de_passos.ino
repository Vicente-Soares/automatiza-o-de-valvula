#include <SoftwareSerial.h>
#include <AccelStepper.h>
#include <Arduino.h>
#include <TM1637Display.h>
 
#define CLK 8
#define DIO 9 
#define key1 10
#define key2 13 
#define key3 12 
#define key4 11 

TM1637Display display(CLK, DIO);
#define TEST_DELAY 2000
const int pinoRX = 5; 
const int pinoTX = 3; 
const int DIR = 4;
const int STEP = 7;
int x;
int dadoBluetooth = 0;
const int INTERRUPCAO_SENSOR = 0;
const int PINO_SENSOR = 2;
#define InterfaceMotor 1
unsigned long contador = 0;
const float FATOR_CALIBRACAO = 4.5;
float fluxo = 0;
float volume = 0;
float volume_total = 0;
unsigned long tempo_antes = 0;
SoftwareSerial bluetooth(pinoRX, pinoTX);
AccelStepper Stepper(InterfaceMotor, STEP, DIR);

void setup() {
 pinMode(key1, INPUT_PULLUP);
 pinMode(key2, INPUT_PULLUP);
 pinMode(key3, INPUT_PULLUP);
 pinMode(key4, INPUT_PULLUP);
 display.setBrightness(0x0f); 
 Serial.begin(9600);
 bluetooth.begin(9600);
 pinMode(PINO_SENSOR, INPUT_PULLUP);
 Stepper.setSpeed(200);

}

void loop() {
  int key1S = digitalRead(key1);
  int key2S = digitalRead(key2);
  int key3S = digitalRead(key3);
  int key4S = digitalRead(key4);
  if((millis() - tempo_antes) > 1000){

   
    detachInterrupt(INTERRUPCAO_SENSOR);

   
    fluxo = ((1000.0 / (millis() - tempo_antes)) * contador) / FATOR_CALIBRACAO;

  
    Serial.print("Fluxo de: ");
    Serial.print(fluxo);
    Serial.println(" L/min");
   display.showNumberDec(fluxo);

   
    volume = fluxo / 60;

   
    volume_total += volume;

   
    Serial.print("Volume: ");
    Serial.print(volume_total);
    Serial.println(" L");
    Serial.println();
 
    contador = 0;


    tempo_antes = millis();


    attachInterrupt(INTERRUPCAO_SENSOR, contador_pulso, FALLING);
    
  }

  dadoBluetooth = Serial.read();
  dadoBluetooth= bluetooth.read();
    
    if(dadoBluetooth == '1' || !key1S){

   Stepper.moveTo(-700);
    Serial.print("100% aberto");
 }
  if(dadoBluetooth == '2'){

   Stepper.moveTo(-350);
   Serial.print("75% aberto");
 }
 if(dadoBluetooth == '3' || !key4S){

   Stepper.moveTo(0);
    Serial.print("50% aberto");
 }
 if(dadoBluetooth == '4'){

   Stepper.moveTo(350);
    Serial.print("25% aberto");
 }
 if(dadoBluetooth == '5'  || !key3S){

   Stepper.moveTo(700);
    Serial.print("0% aberto");
    
 }
 Stepper.run(); 
 }

void contador_pulso() {
  
  contador++;
  
}
 
