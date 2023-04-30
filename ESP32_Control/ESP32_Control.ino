// Universidad del Valle de Guatemala
// Simulación y Fabricación de PCB's
// Carlos Molina
// 14/04/2023

/**************************** Configuration ************************************/

// UART
#include <Arduino.h>
#include <HardwareSerial.h>

#include <esp32-hal-uart.h>


/****************************** Variables ***********************************/
int cont = 0;
int const UP = 34;
int const DOWN = 12;
int const RIGHT = 27; 
int const LEFT = 32;

int buttonStateUP = 0;  
int buttonStateDOWN = 0;
int buttonStateLEFT = 0;  
int buttonStateRIGHT = 0;


//Definición de pins para RX y TX
#define RX_PIN 3
#define TX_PIN 1
/************************ Prototipo de Funcion *******************************/

/******************************* Main ****************************************/
void setup(){
  //Definición pines de salida y entrada
  pinMode(34, INPUT);
  pinMode(12, INPUT);
  pinMode(27, INPUT);
  pinMode(32, INPUT);

  pinMode(2, OUTPUT);

  pinMode(1, OUTPUT);

  delay(5000);
}

void loop(){
  //Leer los pines de los botones
  buttonStateUP = digitalRead(UP);
  buttonStateDOWN = digitalRead(DOWN);
  buttonStateRIGHT = digitalRead(RIGHT);
  buttonStateLEFT = digitalRead(LEFT);

  //Si el boton "Arriba" es presionado
  if (buttonStateUP == HIGH){
    while(buttonStateUP == HIGH){
      buttonStateUP = digitalRead(UP);
    }
    //Se inicia comunicación
    Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
    //Se manda dato
    Serial2.write(49);
    delay(50);
    //Se termina comunicación
    Serial2.end();
  }
  //Si el boton "Abajo" es presionado
  if (buttonStateDOWN == HIGH){
    while(buttonStateDOWN == HIGH){
      buttonStateDOWN = digitalRead(DOWN);
    }
    //Se inicia comunicación
    Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
    //Se manda dato
    Serial2.write(50);
    delay(50);
    //Se termina comunicación
    Serial2.end();
  }

  //Si el boton "Izquierda" es presionado
  if (buttonStateLEFT == HIGH){
    while(buttonStateLEFT == HIGH){
      buttonStateLEFT = digitalRead(LEFT);
    }
    //Se inicia comunicación
    Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
    //Se manda dato
    Serial2.write(51);
    delay(50);
    //Se termina comunicación
    Serial2.end();
  }

  //Si el boton "Derecha" es presionado
  if (buttonStateRIGHT == HIGH){
    while(buttonStateRIGHT == HIGH){
      buttonStateRIGHT = digitalRead(RIGHT);
    }
    //Se inicia comunicación
    Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
    //Se manda dato
    Serial2.write(52);
    delay(50);
    //Se termina comunicación
    Serial2.end();
  }
  Serial2.flush();
}
