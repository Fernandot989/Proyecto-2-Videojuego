
#include "TM4C123GH6PM.h"
//Notas a utilizar
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0


// Esto cambia la velocidad de la canción
int tempo = 105;

// Pin del buzzer
int buzzer = 2;

//Melodia de la intro
int melody[] = {

  NOTE_B4, 16, NOTE_B5, 16, NOTE_FS5, 16, NOTE_DS5, 16, //1
  NOTE_B5, 32, NOTE_FS5, -16, NOTE_DS5, 8, NOTE_C5, 16,
  NOTE_C6, 16, NOTE_G6, 16, NOTE_E6, 16, NOTE_C6, 32, NOTE_G6, -16, NOTE_E6, 8,

  NOTE_B4, 16,  NOTE_B5, 16,  NOTE_FS5, 16,   NOTE_DS5, 16,  NOTE_B5, 32,  //2
  NOTE_FS5, -16, NOTE_DS5, 8,  NOTE_DS5, 32, NOTE_E5, 32,  NOTE_F5, 32,
  NOTE_F5, 32,  NOTE_FS5, 32,  NOTE_G5, 32,  NOTE_G5, 32, NOTE_GS5, 32,  NOTE_A5, 16, NOTE_B5, 8
};

// numro de bits para cada nota
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// Duración de la melodia
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

char mensaje = '9';

void setup() {
  //Iniciación de comunicación serial
  Serial.begin(9600);
  delay(5000);
  //Iniciación comunicación serial con otra tiva
  Serial2.begin(9600);
  
}

void loop() {
//Melodia
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
  
    divider = melody[thisNote + 1];
    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      noteDuration = (wholenote) / abs(divider);
    }


    tone(buzzer, melody[thisNote], noteDuration * 0.9);

    delay(noteDuration);

    noTone(buzzer);
  }
  //Melodia de muerte no utilizada
  if(0){
    tone(2, NOTE_B3, 70); // nota B de la octava 3, frecuencia 247 Hz, duración 70 ms
    delay(150);

    tone(2, NOTE_FS3, 70); // nota F# de la octava 3, frecuencia 185 Hz, duración 70 ms
    delay(150);

    tone(2, NOTE_E3, 70); // nota E de la octava 3, frecuencia 165 Hz, duración 70 ms
    delay(150);

    tone(2, NOTE_DS3, 70); // nota D# de la octava 3, frecuencia 156 Hz, duración 70 ms
    delay(150);

    tone(2, NOTE_B2, 70); // nota B de la octava 2, frecuencia 123 Hz, duración 70 ms
    delay(150);

    tone(2, NOTE_FS2, 70); // nota F# de la octava 2, frecuencia 93 Hz, duración 70 ms
    delay(150);

    tone(2, NOTE_E2, 70); // nota E de la octava 2, frecuencia 82 Hz, duración 70 ms
    delay(150);

    tone(2, NOTE_DS2, 70); // nota D# de la octava 2, frecuencia 78 Hz, duración 70 ms
    delay(150);
  
    tone(2, NOTE_B1, 70); // nota B de la octava 1, frecuencia 62 Hz, duración 70 ms
    delay(150);

    tone(2, NOTE_FS1, 70); // nota F# de la octava 1, frecuencia 46 Hz, duración 70 ms
    delay(150);

    tone(2, NOTE_E1, 70); // nota E de la octava 1, frecuencia 41 Hz, duración 70 ms
    delay(150);

    tone(2, NOTE_DS1, 70); // nota D# de la octava 1, frecuencia 39 Hz, duración 70 ms
    delay(150);

    tone(2, NOTE_B0, 70); // nota B de la octava 0, frecuencia 31 Hz, duración 70 ms
    delay(150);
  }
}
