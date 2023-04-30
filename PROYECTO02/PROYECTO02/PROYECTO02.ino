/*
 * Universidad del Valle de Guatemala
 * Electrónica Digital 2
 * Carlos Mauricio Molina López (#21253)
 * LABORATORIO 07 - TFT SPI
 * 22/03/23
 */
/////////////////////////////////////////////////////////////// Librerias
#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include <SPI.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "bitmaps.h"
#include "font.h"
#include "lcd_registers.h"

// UART 
//#include <HardwareSerial.h>

// El SPI es el 0
// MOSI va a PA_5
// MISO va a PA_4
// SCK va a PA_2

#define LCD_RST PD_0
#define LCD_DC PD_1
#define LCD_CS PA_3

// Definición de LEDs RGB
#define LED_R RED_LED
#define LED_G GREEN_LED
#define LED_B BLUE_LED
//***************************************************************************************************************************************
// Variables
//***************************************************************************************************************************************
int juego = 0;
int buttonState1 = 0;
int buttonState2 = 0;
//int posx_inicial=166;
//int x = 10;
char dato = '5';
char direccion = 'D';

char datito = '9';

int limitx1 = 50;
int limitx2 = 50;

int positionfinal = 0;

// Variables de Sondeo
//int posx_actual = 166; 
int posx_actual = 106;
int posy_actual = 177;

// Sondeo de Fantasmita
int posx_actual2 = 166;
int posy_actual2 = 175;

// Mapeo tracks donde pueden pasar los personajes horizontales
int walls_horizontales[21][4]{
  {65,158,2,6},
  {178,266,2,6},
  {65,266,33,37},
  {66,119,56,60},
  {128,152,56,60},
  {176,200,56,60},
  {224,265,56,60},
  {128,195,80,84},
  {60,128,104,108},
  {205,270,105,109},
  {128,200,125,129},
  {67,152,152,156},
  {176,164,149,153},
  {66,80,173,177}, 
  {104,224,173,177},
  {248,264,173,177},
  {64,104,198,202},
  {130,152,198,202},
  {226,200,198,202},
  {224,264,198,202},
  {64,265,221,225}    
};

// Mapeo tracks donde pueden pasar los personajes verticales
int walls_verticales[24][4]{
  {2,6,2,73},
  {2,6,151,190},
  {2,6,119,238},
  {20,24,282,214},
  {42,46,3,214},
  {66,70,34,73},
  {66,70,83,166},
  {66,70,174,214},
  {92,96,2,49},
  {92,96,58,97},
  {92,96,151,190},
  {92,96,199,237},
  {114,118,3,49},
  {114,118,58,97},
  {114,118,151,190},
  {114,118,199,237},
  {138,142,34,73},
  {138,142,84,166},
  {138,142,176,214},
  {162,166,3,214},
  {202,206,2,73},
  {200,204,151,190},
  {186,200,174,214},
  {202,206,199,237}
};



//***************************************************************************************************************************************
// Prototipo de Funciones 
//***************************************************************************************************************************************
void LCD_Init(void);
void LCD_CMD(uint8_t cmd);
void LCD_DATA(uint8_t data);
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_Clear(unsigned int c);
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);  // USE
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);  // USE
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);   // USE
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);  // USE
void LCD_Print(String text, int x, int y, int fontSize, int color, int background);

void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);

void posibilidad_de_movimiento(void);
void JUEGO(void);
void refresh_botones(void);
void refresh_botones2(void);
void movimiento_pacmanh(int posx_inicial, int posx_final, int posy, char horizontal_orientation);
void movimiento_pacmanv(int posy_inicial, int posy_final, int positionx, char vertical_orientation);
void movimiento_fantasmah(int posx_inicial2, int posx_final2, int posy2, char horizontal_orientation2);
void movimiento_fantasmav(int posy_inicial2, int posy_final2, int positionx2, char vertical_orientation2);
void posible_movimiento_horizontal(int posx_actual, int posy_actual, char orientacion);

//Llamar bitmaps de la memoria RAM
extern uint8_t Escenario[];
extern uint8_t Pacmanlados[];
extern uint8_t Pacmanarriba[];
extern uint8_t Pacmanabajo[];
extern uint8_t Pacmanmuerte[];
extern uint8_t Fantasmalados[];
extern uint8_t Fantasmaarriba[];
extern uint8_t Fantasmaabajo[];
extern uint8_t Fantasmaasustado[];
extern uint8_t Fantasmaasustadoaca[];
extern uint8_t Start[];

//***************************************************************************************************************************************
// Inicialización
//***************************************************************************************************************************************
void setup() {
  
  //LED RGB como Output
  pinMode(LED_R, OUTPUT);   
  pinMode(LED_B, OUTPUT);   
  pinMode(LED_G, OUTPUT);   
  
  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
  SPI.setModule(0);
  Serial.begin(9600);

  //Serial.println("Inicializamos Pantalla TFT");
  LCD_Init();
  LCD_Clear(0x00);
  
  delay(5000);

  // Mostramos el Menú Principal
  LCD_Bitmap(59,0,224,240,Start);

  Serial2.begin(9600);
  Serial3.begin(9600);
  
}
//***************************************************************************************************************************************
// Loop Infinito
//***************************************************************************************************************************************
void loop() {
  // Botones
  refresh_botones();
  //Si la letra recibida del uart es "l" se ejecuta el juego.
  if (dato == '1'){
    LCD_Bitmap(60, 0, 225, 240, Escenario);                             //Se dibuja el mapa
    LCD_Bitmap(posx_actual, posy_actual, 15, 14, pacmanpelota);         //Se dibuja pacman
    LCD_Bitmap(posx_actual2, posy_actual2, 16, 16, Fantasmaasustado);   //Se dibuja el fantasma
    juego = 1;          //Se cambia la variable para que se ejecute el funcionamiento del juego
    dato = '0';        
    Serial2.write(49);
    JUEGO();
  }
  
}

//***************************************************************************************************************************************
// FUNCIONES
//***************************************************************************************************************************************


//Función para ejecutar el juego.
void JUEGO(void){
  //Si se presiona el boton la variable juego cambia a "1" para que los mecanismos del juego entren en función
  while (juego == 1){
    
    //Se chequea si se ha presionado algún botón.
    refresh_botones();
    refresh_botones2();

    //Realizamos la acción que obliga a hacer el botón

    /////////////////////////////////////////////////////////////// PRIMER JUGADOR
    // Pacman se mueve hacia la derecha al presionar el boton que manda "l"
    if (dato == '1'){
      //Esta función envía el pixel final horizontalmente en el que pacman puede moverse dependiendo del track en el que esté
      posible_movimiento_horizontal(posx_actual, posy_actual,'D');
      //Esta función maneja el movimiento de pacman en el eje horizontal
      movimiento_pacmanh(posx_actual,285,posy_actual,'D');
      dato = '0';
      //225
    }

    // Pacman se mueve hacia la izquierda al presionar el boton que manda "2"
    else if (dato == '2'){
      //Esta función envía el pixel final horizontalmente en el que pacman puede moverse dependiendo del track en el que esté
      posible_movimiento_horizontal(posx_actual, posy_actual,'D');
      movimiento_pacmanh(posx_actual,60,posy_actual,'I');
      dato = '0';
      //106
    }
  
    // Pacman se mueve hacia la abajo al presionar el boton que manda "3"
    else if (dato == '3'){
      //Esta función envía el pixel final horizontalmente en el que pacman puede moverse dependiendo del track en el que esté
      posible_movimiento_horizontal(posx_actual, posy_actual,'D');
      movimiento_pacmanv(posy_actual,223,posx_actual,'A');
      dato = '0';
    }
    
    // Pacman se mueve hacia la arriba al presionar el boton que manda "4"    
    else if (dato == '4'){
      posible_movimiento_horizontal(posx_actual, posy_actual,'D');
      movimiento_pacmanv(posy_actual,4,posx_actual,'U');
      dato = '0';
    }

    // Si ninguna letrá es presionada, pacman se que da inmovil
    else if ( (dato != '1') && (dato != '2') && (dato != '3') && (dato != '4') ){
      ;
    }

    /////////////////////////////////////////////////////////////// SEGUNDO JUGADOR
    // Fantasma se mueve hacia la derecha al presionar el boton que manda "l"
    if (datito == '1'){
      movimiento_fantasmah(posx_actual2,285,posy_actual2,'D');
      datito = '0';
    }

    // Fantasma se mueve hacia la izquierda al presionar el boton que manda "2"
    else if (datito == '2'){
      movimiento_fantasmah(posx_actual2,60,posy_actual2,'I');
      datito = '0';
    }
  
    // Fantasma se mueve hacia la abajo al presionar el boton que manda "3"
    else if (datito == '3'){
      movimiento_fantasmav(posy_actual2,223,posx_actual2,'A');
      datito = '0';
    }
    
    // Fantasma se mueve hacia la arriba al presionar el boton que manda "4"   
    else if (datito == '4'){
      movimiento_fantasmav(posy_actual2,4,posx_actual2,'U');
      datito = '0';
    }

    // El Fantasma no se mueve
    else if ( (datito != '1') && (datito != '2') && (datito != '3') && (datito != '4') ){
      ;
    }
    
  }
}

////////////////////BOTONES primer jugador
//Esta función lee el uart del ESP32 y lo imprime en la consola de la computadora
void refresh_botones(void){
  if(Serial2.available()){
    dato = Serial2.read();
    Serial.print(dato);
  }
  ////////////////////PRUEBA BOTONES
  //Si es dato es "1" enciende la led roja
  if (dato == '1'){
    digitalWrite(LED_B, LOW);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_R, HIGH);   // Encendemos el LED Rojo
  }
  //Si es dato es "2" enciende la led azul
  else if (dato == '2'){
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_B, HIGH);
    digitalWrite(LED_G, LOW);
  }
  //Si es dato es "3" enciende la led verde
  else if (dato == '3'){
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_B, LOW);
    digitalWrite(LED_G, HIGH);
  }
  //Si es dato es "4" enciende la led blanca
  else if (dato == '4'){
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_B, HIGH);
    digitalWrite(LED_G, HIGH);
  }  
  
}
////////////////////BOTONES segundo jugador
void refresh_botones2(void){
  if(Serial3.available()){
    datito = Serial3.read();
    Serial.print(datito);
  }
  
}

////////////////////FANTASMA MOVEMENT (HORIZONTAL)
void movimiento_fantasmah(int posx_inicial, int posx_final, int posy, char horizontal_orientation){
  // MOVIMIENTO HACIA LA IZQUIERDA
  if (horizontal_orientation == 'I'){
    //El personaje se mueve constantemente media vez su posición sea menor a la del pixel final
        for(int x = posx_inicial; x>posx_final; x--){
          delay(28);
          //Movimiento hacia la izquierda
          posx_actual2 = (posx_actual2)-1;
          int anim = (x/11)%2;
          //Se dibuja el sprite el personaje
          LCD_Sprite(x,posy,16,16,Fantasmalados,3,anim,1,0);
          //Se hace la interrupción del boton
          if(Serial3.available()){
            break;
          }
          //Si llega a la posición final se queda con un frame del sprite sin moverse
          if(posx_actual2 == posx_final){
            LCD_Sprite(posx_final,posy,16,16,Fantasmalados,3,1,1,0);
          }
        }
      
    
  }

  // MOVIMIENTO HACIA LA DERECHA
  else if (horizontal_orientation == 'D'){   
        //El personaje se mueve constantemente media vez su posición sea menor a la del pixel final
        for(int x = posx_inicial; x<posx_final; x++){
          delay(26);
          //Movimiento hacia la derecha
          posx_actual2 = (posx_actual2)+1;
          int anim = (x/11)%2;
          //Se dibuja el sprite el personaje
          LCD_Sprite(x,posy,16,16,Fantasmalados,3,anim,0,0);
          V_line(x-1, posy, 12, 0x00);
          V_line(x-2, posy, 12, 0x00);
          //Se hace la interrupción del boton
          if(Serial3.available()){
            break;
          }
          //Si llega a la posición final se queda con un frame del sprite sin moverse
          if((posx_actual2) == posx_final){
            LCD_Sprite(posx_final,posy,16,16,Fantasmalados,3,1,0,0);
          } 
        }
      
    }
  
}

////////////////////PACMAN MOVEMENT (HORIZONTAL)
//Sigue el mismo concepto que el movimiento horizontal del fantasma
void movimiento_pacmanh(int posx_inicial, int posx_final, int posy, char horizontal_orientation){
  // MOVIMIENTO HACIA LA IZQUIERDA
  if (horizontal_orientation == 'I'){
      //El personaje se mueve constantemente media vez su posición sea menor a la del pixel final
        for(int x = posx_inicial; x>posx_final; x--){
          delay(28);
          //Movimiento hacia la izquierda
          posx_actual = posx_actual-1;
          int anim = (x/11)%2;
          LCD_Sprite(x,posy,15,13,Pacmanlados,3,anim,1,0);
          if(Serial2.available()){
            break;
          }

          if(posx_actual == posx_final){
            LCD_Sprite(posx_final,posy,15,13,Pacmanlados,3,1,1,0);
          }
        }
  
    
  }

  // MOVIMIENTO HACIA LA DERECHA
  else if (horizontal_orientation == 'D'){   
        for(int x = posx_inicial; x<posx_final; x++){
          delay(26);
          posx_actual = posx_actual+1;
          int anim = (x/11)%2;
          LCD_Sprite(x,posy,15,13,Pacmanlados,3,anim,0,0);
          V_line(x-1, posy, 12, 0x00);
          V_line(x-2, posy, 12, 0x00);
          if(Serial2.available()){
            break;
          }
          if(posx_actual == posx_final){
            LCD_Sprite(posx_final,posy,15,13,Pacmanlados,3,1,0,0);
          } 
        }
      
    
  }
}

////////////////////FANTASMA MOVEMENT (VERTICAL)
void movimiento_fantasmav(int posy_inicial, int posy_final, int posx, char vertical_orientation){

  // MOVIMIENTO HACIA ARRIBA
  if (vertical_orientation == 'U'){
        //El personaje se mueve constantemente media vez su posición sea menor a la del pixel final
        for(int y = posy_inicial; y>posy_final; y--){
          delay(28);
          //Movimiento hacia la arriba
          posy_actual2 = (posy_actual2)-1;
          int anim2 = (y/11)%2;
          //Se dibuja el sprite el personaje
          LCD_Sprite(posx,y,16,16,Fantasmaarriba,3,anim2,0,0);
      
          //Se hace la interrupción del boton
          if(Serial3.available()){
            break;
          }
          //Si llega a la posición final se queda con un frame del sprite sin moverse
          if(posy_actual2 == posy_final){
            LCD_Sprite(posx,posy_final,16,16,Fantasmaarriba,3,1,0,0);
          }
        }
      
  }

  // MOVIMIENTO HACIA ABAJO
  else if (vertical_orientation == 'A'){
        //El personaje se mueve constantemente media vez su posición sea menor a la del pixel final
        for(int y = posy_inicial; y<posy_final; y++){
          delay(26);
          //Movimiento hacia la abajo
          posy_actual2 = (posy_actual2)+1;
          int anim2 = (y/11)%2;
          //Se dibuja el sprite el personaje
          LCD_Sprite(posx,y,16,16,Fantasmaabajo,3,anim2,1,0);
      
          //Se hace la interrupción del boton
          if(Serial3.available()){
            break;
          }
          //Se dibuja el sprite el personaje
          if((posy_actual2) == posy_final){
            LCD_Sprite(posx,posy_final,16,16,Fantasmaabajo,3,1,1,0);
          }
        }
      
    
  }
}

////////////////////PACMAN MOVEMENT (VERTICAL)
//Es el mismo concepto que el moviemiento vertical del fantasma
void movimiento_pacmanv(int posy_inicial, int posy_final, int posx, char vertical_orientation){

  // MOVIMIENTO HACIA ARRIBA
  if (vertical_orientation == 'U'){
        for(int y = posy_inicial; y>posy_final; y--){
          delay(28);
          posy_actual = posy_actual-1;
          int anim2 = (y/11)%2;
          LCD_Sprite(posx,y,15,14,Pacmanarriba,3,anim2,1,0);
      
      
          if(Serial2.available()){
            break;
          }

          if(posy_actual == posy_final){
            LCD_Sprite(posx,posy_final,15,14,Pacmanarriba,3,1,1,0);
          }
          
        }
        
  }

  

  // MOVIMIENTO HACIA ABAJO
  else if (vertical_orientation == 'A'){
        for(int y = posy_inicial; y<posy_final; y++){
          delay(26);
          posy_actual = posy_actual+1;
          int anim2 = (y/11)%2;
          LCD_Sprite(posx,y,15,14,Pacmanabajo,3,anim2,1,0);
      
      
          if(Serial2.available()){
            break;
          }

          if(posy_actual == posy_final){
            LCD_Sprite(posx,posy_final,15,14,Pacmanabajo,3,1,1,0);
          }
           break;
        }
      
    
  }
}

////////////////////MOVIMIENTO POSIBLE (HORIZONTAL)
//Esta función envia el pixel final de cada track
void posible_movimiento_horizontal(int posx_actual, int posy_actual, char orientacion){
  //Sondea para encontrar si el personaje se encuentra en un track
  for (int i = 0; i<=21; i++){
    //Si este se encuentra en los limites establecidos en los tracks se envia la posición final
    if(posx_actual >= walls_horizontales[i][0] && posx_actual <= walls_horizontales[i][1] && posy_actual >= walls_horizontales[i][2] && posy_actual <= walls_horizontales[i][3]){
      //Si va la derecha se envia el final del track en la parte derecha
      if(orientacion == 'D'){
        positionfinal = walls_horizontales [i][1];
        }
        //Si va la izquierda se envia el final del track en la parte izquierda
        else if(orientacion == 'I'){
          positionfinal = walls_horizontales [i][0];
        }
        break;
      }
    }
   Serial.println(positionfinal);
}

////////////////////MOVIMIENTO POSIBLE (VERTICAL)
//Esta función envia el pixel final de cada track
void posible_movimiento_vertical(int posx_actual, int posy_actual, char orientacion){
  //Sondea para encontrar si el personaje se encuentra en un track
  for (int i = 0; i<=24; i++){
    //Si este se encuentra en los limites establecidos en los tracks se envia la posición final
    if(posx_actual >= walls_verticales[i][0] && posx_actual <= walls_verticales[i][1] && posy_actual >= walls_verticales[i][2] && posy_actual <= walls_verticales[i][3]){
      //Si va abajo se envia el final del track en la parte de abajo
      if(orientacion == 'A'){
        positionfinal = walls_verticales [i][1];
        }
        //Si va arriba se envia el final del track en la parte de arriba
        else if(orientacion == 'U'){
          positionfinal = walls_verticales [i][0];
        }
        break;
      }
    }
   Serial.println(positionfinal);
}


//***************************************************************************************************************************************
// Función para inicializar LCD
//***************************************************************************************************************************************
void LCD_Init(void) {
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_DC, OUTPUT);
  //****************************************
  // Secuencia de Inicialización
  //****************************************
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_DC, HIGH);
  digitalWrite(LCD_RST, HIGH);
  delay(5);
  digitalWrite(LCD_RST, LOW);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  delay(150);
  digitalWrite(LCD_CS, LOW);
  //****************************************
  LCD_CMD(0xE9);  // SETPANELRELATED
  LCD_DATA(0x20);
  //****************************************
  LCD_CMD(0x11); // Exit Sleep SLEEP OUT (SLPOUT)
  delay(100);
  //****************************************
  LCD_CMD(0xD1);    // (SETVCOM)
  LCD_DATA(0x00);
  LCD_DATA(0x71);
  LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0xD0);   // (SETPOWER) 
  LCD_DATA(0x07);
  LCD_DATA(0x01);
  LCD_DATA(0x08);
  //****************************************
  LCD_CMD(0x36);  // (MEMORYACCESS)
  LCD_DATA(0x40|0x80|0x20|0x08); // LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0x3A); // Set_pixel_format (PIXELFORMAT)
  LCD_DATA(0x05); // color setings, 05h - 16bit pixel, 11h - 3bit pixel
  //****************************************
  LCD_CMD(0xC1);    // (POWERCONTROL2)
  LCD_DATA(0x10);
  LCD_DATA(0x10);
  LCD_DATA(0x02);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC0); // Set Default Gamma (POWERCONTROL1)
  LCD_DATA(0x00);
  LCD_DATA(0x35);
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC5); // Set Frame Rate (VCOMCONTROL1)
  LCD_DATA(0x04); // 72Hz
  //****************************************
  LCD_CMD(0xD2); // Power Settings  (SETPWRNORMAL)
  LCD_DATA(0x01);
  LCD_DATA(0x44);
  //****************************************
  LCD_CMD(0xC8); //Set Gamma  (GAMMASET)
  LCD_DATA(0x04);
  LCD_DATA(0x67);
  LCD_DATA(0x35);
  LCD_DATA(0x04);
  LCD_DATA(0x08);
  LCD_DATA(0x06);
  LCD_DATA(0x24);
  LCD_DATA(0x01);
  LCD_DATA(0x37);
  LCD_DATA(0x40);
  LCD_DATA(0x03);
  LCD_DATA(0x10);
  LCD_DATA(0x08);
  LCD_DATA(0x80);
  LCD_DATA(0x00);
  //****************************************
  LCD_CMD(0x2A); // Set_column_address 320px (CASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x3F);
  //****************************************
  LCD_CMD(0x2B); // Set_page_address 480px (PASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0xE0);
//  LCD_DATA(0x8F);
  LCD_CMD(0x29); //display on 
  LCD_CMD(0x2C); //display on

  LCD_CMD(ILI9341_INVOFF); //Invert Off
  delay(120);
  LCD_CMD(ILI9341_SLPOUT);    //Exit Sleep
  delay(120);
  LCD_CMD(ILI9341_DISPON);    //Display on
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar comandos a la LCD - parámetro (comando)
//***************************************************************************************************************************************
void LCD_CMD(uint8_t cmd) {
  digitalWrite(LCD_DC, LOW);
  SPI.transfer(cmd);
}
//***************************************************************************************************************************************
// Función para enviar datos a la LCD - parámetro (dato)
//***************************************************************************************************************************************
void LCD_DATA(uint8_t data) {
  digitalWrite(LCD_DC, HIGH);
  SPI.transfer(data);
}
//***************************************************************************************************************************************
// Función para definir rango de direcciones de memoria con las cuales se trabajara (se define una ventana)
//***************************************************************************************************************************************
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  LCD_CMD(0x2a); // Set_column_address 4 parameters
  LCD_DATA(x1 >> 8);
  LCD_DATA(x1);   
  LCD_DATA(x2 >> 8);
  LCD_DATA(x2);   
  LCD_CMD(0x2b); // Set_page_address 4 parameters
  LCD_DATA(y1 >> 8);
  LCD_DATA(y1);   
  LCD_DATA(y2 >> 8);
  LCD_DATA(y2);   
  LCD_CMD(0x2c); // Write_memory_start
}
//***************************************************************************************************************************************
// Función para borrar la pantalla - parámetros (color)
//***************************************************************************************************************************************
void LCD_Clear(unsigned int c){  
  unsigned int x, y;
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_DC, HIGH);
  digitalWrite(LCD_CS, LOW);   
  SetWindows(0, 0, 319, 239); // 479, 319);
  for (x = 0; x < 320; x++)
    for (y = 0; y < 240; y++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
    }
  digitalWrite(LCD_CS, HIGH);
} 
//***************************************************************************************************************************************
// Función para dibujar una línea horizontal - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_DC, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  SetWindows(x, y, l, y);
  j = l;// * 2;
  for (i = 0; i < l; i++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i,j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_DC, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + y;
  SetWindows(x, y, x, l);
  j = l; //* 2;
  for (i = 1; i <= j; i++) {
    LCD_DATA(c >> 8); 
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);  
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  H_line(x  , y  , w, c);
  H_line(x  , y+h, w, c);
  V_line(x  , y  , h, c);
  V_line(x+w, y  , h, c);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo relleno - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  unsigned int i;
  for (i = 0; i < h; i++) {
    H_line(x  , y  , w, c);
    H_line(x  , y+i, w, c);
  }
}
//***************************************************************************************************************************************
// Función para dibujar texto - parámetros ( texto, coordenada x, cordenada y, color, background) 
//***************************************************************************************************************************************
void LCD_Print(String text, int x, int y, int fontSize, int color, int background) {
  int fontXSize ;
  int fontYSize ;
  
  if(fontSize == 1){
    fontXSize = fontXSizeSmal ;
    fontYSize = fontYSizeSmal ;
  }
  if(fontSize == 2){
    fontXSize = fontXSizeBig ;
    fontYSize = fontYSizeBig ;
  }
  
  char charInput ;
  int cLength = text.length();
  Serial.println(cLength,DEC);
  int charDec ;
  int c ;
  int charHex ;
  char char_array[cLength+1];
  text.toCharArray(char_array, cLength+1) ;
  for (int i = 0; i < cLength ; i++) {
    charInput = char_array[i];
    Serial.println(char_array[i]);
    charDec = int(charInput);
    digitalWrite(LCD_CS, LOW);
    SetWindows(x + (i * fontXSize), y, x + (i * fontXSize) + fontXSize - 1, y + fontYSize );
    long charHex1 ;
    for ( int n = 0 ; n < fontYSize ; n++ ) {
      if (fontSize == 1){
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontYSize) + n);
      }
      if (fontSize == 2){
        charHex1 = pgm_read_word_near(bigFont + ((charDec - 32) * fontYSize) + n);
      }
      for (int t = 1; t < fontXSize + 1 ; t++) {
        if (( charHex1 & (1 << (fontXSize - t))) > 0 ) {
          c = color ;
        } else {
          c = background ;
        }
        LCD_DATA(c >> 8);
        LCD_DATA(c);
      }
    }
    digitalWrite(LCD_CS, HIGH);
  }
}
//***************************************************************************************************************************************
// Función para dibujar una imagen a partir de un arreglo de colores (Bitmap) Formato (Color 16bit R 5bits G 6bits B 5bits)
//***************************************************************************************************************************************
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]){  
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_DC, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+width;
  y2 = y+height;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = 0;
  unsigned int i, j;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      //LCD_DATA(bitmap[k]);    
      k = k + 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una imagen sprite - los parámetros columns = número de imagenes en el sprite, index = cual desplegar, flip = darle vuelta
//***************************************************************************************************************************************
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset){
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_DC, HIGH);
  digitalWrite(LCD_CS, LOW); 

  unsigned int x2, y2;
  x2 =   x+width;
  y2=    y+height;
  SetWindows(x, y, x2-1, y2-1);
  int k = 0;
  int ancho = ((width*columns));
  if(flip){
  for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width -1 - offset)*2;
      k = k+width*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k - 2;
     } 
  }
  }else{
     for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width + 1 + offset)*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k + 2;
     } 
  }
    }
  digitalWrite(LCD_CS, HIGH);
}
