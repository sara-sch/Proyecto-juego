
/* Librería para el uso de la pantalla ILI9341 en modo 8 bits
 * Basado en el código de martinayotte - https://www.stm32duino.com/viewtopic.php?t=637
 * Adaptación, migración y creación de nuevas funciones: Pablo Mazariegos y José Morales
 * Con ayuda de: José Guerra
 * IE3027: Electrónica Digital 2 - 2019
 */
//*********************************************
#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
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
#include <SPI.h>
#include <SD.h>


/* Librería para el uso de la pantalla ILI9341 en modo 8 bits
 * Basado en el código de martinayotte - https://www.stm32duino.com/viewtopic.php?t=637
 * Adaptación, migración y creación de nuevas funciones: Pablo Mazariegos y José Morales
 * Con ayuda de: José Guerra
 * IE3027: Electrónica Digital 2 - 2019
 */
//*********************************************
#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
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

#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};  
//*********************************************
// Functions Prototypes
//*********************************************
void LCD_Init(void);
void LCD_CMD(uint8_t cmd);
void LCD_DATA(uint8_t data);
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_Clear(unsigned int c);
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void LCD_Print(String text, int x, int y, int fontSize, int color, int background);

void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);
void Read_SD(void);



File myFile;
char contenidoSD;
uint16_t caracter;
unsigned char data2[9000]={};
int indice = 0;
uint8_t bandera = 0;
uint16_t temp = 0;


int state;
int deco;
int decor;
char rawdata;
char data;
int butt1 = 0;           // boton jugador 1
int butt2 = 0;           // boton jugador 2  
int mov = 0;             // joystick jugador 1
int mov2 = 0;           // joystick jugador 1 
int caminox1 = 10;      //posición inicial en x
int caminoy1 = 10;      //posición inicial en y
int caminox2 = 253;
int caminoy2 = 160;
int tank1;
int tank2;
int estadoc1;
int estadoc2;
//*********************************************
// Inicialización
//*********************************************
void setup() {

  
  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
  Serial.begin(115200);   
  Serial2.begin(115200); 
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial.println("Inicio");
  LCD_Init();
  LCD_Clear(0x00);

  SPI.setModule(0);
  pinMode(PA_3, OUTPUT);      //Pin para tarjeta SD

  if (!SD.begin(PA_3)) {      
    Serial.println("initialization failed!");
    return;
  }
  
  deco = 0;
  state = 0;
  tank1 = 0;
  tank2 = 0;
  estadoc1 = 0;




  
}
//*********************************************
// Loop Infinito
//*********************************************
void loop() {

      if (Serial2.available()) {          // If anything comes in Serial
            rawdata = Serial2.read();      // Leer datos
            data = rawdata;
    
            
            if(data == 'a'){              //no1 
            butt1 = 0;
            Serial.println("no1");
            }
            else if(data == 'b'){         //yes1 
            butt1 = 1;
            Serial.println("yes1");
            }
            else if(data == 'c'){         //no2
            butt2 = 0;
            Serial.println("no2");
            }
            else if(data == 'd'){         //yes2 
            butt2 = 1;
            Serial.println("yes2");
            }
            else if(data == 'e'){         //right1 
            mov = 4;
            Serial.println("right1");
            }
            else if(data == 'f'){         //left1 
            mov = 3;
            Serial.println("left1");
            }
            else if(data == 'g'){         //down1
            mov = 2;
            Serial.println("down1");
            }
            else if(data == 'h'){         //up1 
            mov = 1;
            Serial.println("up1");
            }
            else if(data == 'i'){         //zero1 
            mov = 0;
            }
            else if(data == 'j'){         //right2 
            mov2 = 4;
            Serial.println("right2");
            }
            else if(data == 'k'){         //left2 
            mov2 = 3;
            Serial.println("left2");
            }
            else if(data == 'l'){         //down2
            mov2 = 2;
            Serial.println("down2");
            }
            else if(data == 'm'){         //up2 
            mov2 = 1;
            Serial.println("up2");
            }
            else if(data == 'n'){         //zero2 
            mov2 = 0;
            }
          }

      if(state == 0){                                   // Menú del juego

       if(decor == 0){                    
      
       FillRect(0, 0, 320, 240, 0x0000);
       myFile = SD.open("logo.txt");
       Read_SD();
       LCD_Bitmap(120,88,70,61,data2);                  // Colocar logo extraído de la SD
       String text2 = "TANKS";
       LCD_Print(text2, 115, 30, 2, 0xFF20, 0x0000);    // Colocar título del juego
       String text1 = "Press any button to start!";
       LCD_Print(text1, 55, 200, 1, 0x0000, 0xffff);    // Colocar instrucciones para comenzar el juego
       decor = 1;
       }
       
       if(butt1 == 1 || butt2 == 1){                    // Limpiar la pantalla y pasar al siguiente estado si se presiona uno de los botones
          state = 1;
          LCD_Clear(0x00);
          exit;
        }
        return;
        }

        if(state == 1){                                 // Juego
          

          if(deco == 0){
            
              myFile = SD.open("crate.txt");            // Colocar crates extraídas de la SD
              Read_SD();
              LCD_Bitmap(192,96,32,32,data2);
              LCD_Bitmap(64,32,32,32,data2);
              LCD_Bitmap(64,160,32,32,data2);
              
              LCD_Bitmap(caminox1, caminoy1, 38, 41, tankb_d);    // Colcar bitmaps iniciales de los tanques
              LCD_Bitmap(caminox2, caminoy2, 38, 41, tankb_u);
              deco = 1;
              return;
            }
          
                                                                   // Movimiento del primer tanque
          if(mov == 4){
            if(caminox1 <= 250){  
              caminox1= caminox1+1;
              LCD_Bitmap(caminox1, caminoy1, 38, 41, tankb_r);
            }
            else{
               LCD_Bitmap(caminox1, caminoy1, 38, 41, tankb_r);     // No moverse si se sale del borde
              }
        }
        else if(mov == 3){
          if(caminox1 >= 9){  //288
              caminox1= caminox1-1;
              LCD_Bitmap(caminox1, caminoy1, 38, 41, tankb_l);
          }
          else{
              LCD_Bitmap(caminox1, caminoy1, 38, 41, tankb_l);
            }
        }
    
          else if(mov == 1){
            if(caminoy1 >= 9){
              caminoy1= caminoy1-1;
              LCD_Bitmap(caminox1, caminoy1, 38, 41, tankb_u);
            }
            else{
              LCD_Bitmap(caminox1, caminoy1, 38, 41, tankb_u);
              }
        }
        else if(mov == 2){
          if(caminoy1 <= 200){
          caminoy1= caminoy1+1;
          LCD_Bitmap(caminox1, caminoy1, 38, 41, tankb_d);
          }
          else{
            LCD_Bitmap(caminox1, caminoy1, 38, 41, tankb_d);
            }
        }
    

                                                                        // Movimiento del segundo tanque
        if(mov2 == 4){
            if(caminox2 <= 250){  
              caminox2= caminox2+1;
              LCD_Bitmap(caminox2, caminoy2, 38, 41, tankb_r);
            }
            else{
               LCD_Bitmap(caminox2, caminoy2, 38, 41, tankb_r);
              }
        }
        else if(mov2 == 3){
          if(caminox2 >= 9){  //288
              caminox2= caminox2-1;
              LCD_Bitmap(caminox2, caminoy2, 38, 41, tankb_l);
          }
          else{
              LCD_Bitmap(caminox2, caminoy2, 38, 41, tankb_l);
            }
        }
    
          else if(mov2 == 1){
            if(caminoy2 >= 9){
              caminoy2= caminoy2-1;
              LCD_Bitmap(caminox2, caminoy2, 38, 41, tankb_u);
            }
            else{
              LCD_Bitmap(caminox2, caminoy2, 38, 41, tankb_u);
              }
        }
        else if(mov2 == 2){
          if(caminoy2 <= 200){
          caminoy2= caminoy2+1;
          LCD_Bitmap(caminox2, caminoy2, 38, 41, tankb_d);
          }
          else{
            LCD_Bitmap(caminox2, caminoy2, 38, 41, tankb_d);
            }
        }
     
    
    
    // Tank 1

    if(estadoc1 == 0){                                          // Estado para que el contador del primer tanque muestre un 1 si toca algún crate
    if(caminox1 >= 202 && caminox1 <= 214 &&
       caminoy1 >= 86 && caminoy1 <= 98){
      tank1 = 1;                                                
      estadoc1 = 1;                                             // Cambiar estado
      caminox1 = 10;                                            // Regresar a posición inicial
      caminoy1 = 10;
      return;
      }

    if(caminox1 >= 64 && caminox1 <= 96 &&
       caminoy1 >= 28 && caminoy1 <= 33){
      tank1 = 1;
      estadoc1 = 1;
      caminox1 = 10;
      caminoy1 = 10;
      return;
      }

    if(caminox1 >= 64 && caminox1 <= 96 &&
       caminoy1 >= 160 && caminoy1 <= 192){
      tank1 = 1;
      estadoc1 = 1;
      caminox1 = 10;
      caminoy1 = 10;
      return;
      }
      
    }

if(estadoc1 == 1){                                              // Estado para que el contador del primer tanque muestre un 2 si toca algún crate
    if(caminox1 >= 202 && caminox1 <= 214 &&
       caminoy1 >= 86 && caminoy1 <= 98){
      tank1 = 2;
      estadoc1 = 2;
      caminox1 = 10;
      caminoy1 = 10;
      return;
      }
    if(caminox1 >= 60 && caminox1 <= 65 &&
       caminoy1 >= 28 && caminoy1 <= 33){
      tank1 = 2;
      estadoc1 = 2;
      caminox1 = 10;
      caminoy1 = 10;
      return;
       }
    if(caminox1 >= 60 && caminox1 <= 65 &&
       caminoy1 >= 160 && caminoy1 <= 192){
      tank1 = 2;
      estadoc1 = 2;
      caminox1 = 10;
      caminoy1 = 10;
      return;
      }
      
    }

if(estadoc1 == 2){                                                    // Estado para que el contador del primer tanque muestre un 3 si toca algún crate
    if(caminox1 >= 202 && caminox1 <= 214 &&
       caminoy1 >= 86 && caminoy1 <= 98){
      tank1 = 3;
      estadoc1 = 2;
      caminox1 = 10;
      caminoy1 = 10;
      return;
      }

    if(caminox1 >= 60 && caminox1 <= 65 &&
       caminoy1 >= 28 && caminoy1 <= 33){
      tank1 = 3;
      estadoc1 = 2;
      caminox1 = 10;
      caminoy1 = 10;
      return;
      }

    if(caminox1 >= 60 && caminox1 <= 65 &&
       caminoy1 >= 160 && caminoy1 <= 192){
      tank1 = 3;
      estadoc1 = 2;
      caminox1 = 10;
      caminoy1 = 10;
      return;
      }
      
    }



    // Tank 2

      if(estadoc2 == 0){                                            // Estado para que el contador del segundo tanque muestre un 1 si toca algún crate
      if(caminox2 >= 202 && caminox2 <= 214 &&
       caminoy2 >= 86 && caminoy2 <= 98){
      tank2 = 1;
      estadoc2 = 1;
      caminox2 = 253;
      caminoy2 = 160;
      
      return;
      }

      if(caminox2 >= 60 && caminox2 <= 65 &&
       caminoy2 >= 28 && caminoy2 <= 33){
      tank2 = 1;
      estadoc2 = 2;
      caminox2 = 253;
      caminoy2 = 160;
      return;
      }


    if(caminox2 >= 64 && caminox2 <= 96 &&
       caminoy2 >= 160 && caminoy2 <= 192){
      tank2 = 1;
      estadoc2 = 1;
      caminox2 = 253;
      caminoy2 = 160;
      return;
      }
      
    }

if(estadoc2 == 1){                                            // Estado para que el contador del segundo tanque muestre un 2 si toca algún crate
    if(caminox2 >= 202 && caminox2 <= 214 &&
       caminoy2 >= 86 && caminoy2 <= 98){
      tank2 = 2;
      estadoc2 = 2;
      caminox2 = 253;
      caminoy2 = 160;
      return;
      }

      if(caminox2 >= 60 && caminox2 <= 65 &&
       caminoy2 >= 28 && caminoy2 <= 33){
      tank2 = 2;
      estadoc2 = 2;
      caminox2 = 253;
      caminoy2 = 160;
      return;
      }
   
       
    if(caminox2 >= 60 && caminox2 <= 65 &&
       caminoy2 >= 160 && caminoy2 <= 192){
      tank2 = 2;
      estadoc2 = 2;
      caminox2 = 253;
      caminoy2 = 160;
      return;
      }
      
    }

if(estadoc2 == 2){                                            // Estado para que el contador del segundo tanque muestre un 3 si toca algún crate
    if(caminox2 >= 202 && caminox2 <= 214 &&
       caminoy2 >= 86 && caminoy2 <= 98){
      tank2 = 3;
      estadoc2 = 2;
      caminox2 = 253;
      caminoy2 = 160;
      return;
      }

     if(caminox2 >= 60 && caminox2 <= 65 &&
       caminoy2 >= 28 && caminoy2 <= 33){
      tank2 = 3;
      estadoc2 = 2;
      caminox2 = 253;
      caminoy2 = 160;
      return;
      }


    if(caminox2 >= 60 && caminox2 <= 65 &&
       caminoy2 >= 160 && caminoy2 <= 192){
      tank2 = 3;
      estadoc2 = 2;
      caminox2 = 253;
      caminoy2 = 160;
      return;
      }
      
    }


                                                                 // Mostrar contadores en la LCD
    
    char buffer [sizeof(int)*8+1];                
    char *intStr = itoa(tank1, buffer, 10);
    String textt1 = String(intStr);
    LCD_Print(textt1, 80, 210, 2, 0x0000, 0xffff);
    Serial.print(textt1);

    char buffer2 [sizeof(int)*8+1];
    char *intStr2 = itoa(tank2, buffer2, 10);
    String textt2 = String(intStr2);
    LCD_Print(textt2, 220, 210, 2, 0x0000, 0xffff);
    Serial.print(textt2);
      
    
    }
  

}




//*********************************************
// Función para inicializar LCD
//*********************************************
void LCD_Init(void) {
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RD, OUTPUT);
  for (uint8_t i = 0; i < 8; i++){
    pinMode(DPINS[i], OUTPUT);
  }
  //**************
  // Secuencia de Inicialización
  //**************
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_RST, HIGH);
  delay(5);
  digitalWrite(LCD_RST, LOW);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  delay(150);
  digitalWrite(LCD_CS, LOW);
  //**************
  LCD_CMD(0xE9);  // SETPANELRELATED
  LCD_DATA(0x20);
  //**************
  LCD_CMD(0x11); // Exit Sleep SLEEP OUT (SLPOUT)
  delay(100);
  //**************
  LCD_CMD(0xD1);    // (SETVCOM)
  LCD_DATA(0x00);
  LCD_DATA(0x71);
  LCD_DATA(0x19);
  //**************
  LCD_CMD(0xD0);   // (SETPOWER) 
  LCD_DATA(0x07);
  LCD_DATA(0x01);
  LCD_DATA(0x08);
  //**************
  LCD_CMD(0x36);  // (MEMORYACCESS)
  LCD_DATA(0x40|0x80|0x20|0x08); // LCD_DATA(0x19);
  //**************
  LCD_CMD(0x3A); // Set_pixel_format (PIXELFORMAT)
  LCD_DATA(0x05); // color setings, 05h - 16bit pixel, 11h - 3bit pixel
  //**************
  LCD_CMD(0xC1);    // (POWERCONTROL2)
  LCD_DATA(0x10);
  LCD_DATA(0x10);
  LCD_DATA(0x02);
  LCD_DATA(0x02);
  //**************
  LCD_CMD(0xC0); // Set Default Gamma (POWERCONTROL1)
  LCD_DATA(0x00);
  LCD_DATA(0x35);
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x02);
  //**************
  LCD_CMD(0xC5); // Set Frame Rate (VCOMCONTROL1)
  LCD_DATA(0x04); // 72Hz
  //**************
  LCD_CMD(0xD2); // Power Settings  (SETPWRNORMAL)
  LCD_DATA(0x01);
  LCD_DATA(0x44);
  //**************
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
  //**************
  LCD_CMD(0x2A); // Set_column_address 320px (CASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x3F);
  //**************
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
//*********************************************
// Función para enviar comandos a la LCD - parámetro (comando)
//*********************************************
void LCD_CMD(uint8_t cmd) {
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = cmd;
  digitalWrite(LCD_WR, HIGH);
}
//*********************************************
// Función para enviar datos a la LCD - parámetro (dato)
//*********************************************
void LCD_DATA(uint8_t data) {
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = data;
  digitalWrite(LCD_WR, HIGH);
}
//*********************************************
// Función para definir rango de direcciones de memoria con las cuales se trabajara (se define una ventana)
//*********************************************
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
//*********************************************
// Función para borrar la pantalla - parámetros (color)
//*********************************************
void LCD_Clear(unsigned int c){  
  unsigned int x, y;
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);   
  SetWindows(0, 0, 319, 239); // 479, 319);
  for (x = 0; x < 320; x++)
    for (y = 0; y < 240; y++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
    }
  digitalWrite(LCD_CS, HIGH);
} 
//*********************************************
// Función para dibujar una línea horizontal - parámetros ( coordenada x, cordenada y, longitud, color)
//********************************************* 
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
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
//*********************************************
// Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada y, longitud, color)
//********************************************* 
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i,j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
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
//*********************************************
// Función para dibujar un rectángulo - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//*********************************************
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  H_line(x  , y  , w, c);
  H_line(x  , y+h, w, c);
  V_line(x  , y  , h, c);
  V_line(x+w, y  , h, c);
}
//*********************************************
// Función para dibujar un rectángulo relleno - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//*********************************************
/*void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  unsigned int i;
  for (i = 0; i < h; i++) {
    H_line(x  , y  , w, c);
    H_line(x  , y+i, w, c);
  }
}
*/

void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+w;
  y2 = y+h;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = w*h*2-1;
  unsigned int i, j;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c);
      
      //LCD_DATA(bitmap[k]);    
      k = k - 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}
//*********************************************
// Función para dibujar texto - parámetros ( texto, coordenada x, cordenada y, color, background) 
//*********************************************
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
//*********************************************
// Función para dibujar una imagen a partir de un arreglo de colores (Bitmap) Formato (Color 16bit R 5bits G 6bits B 5bits)
//*********************************************
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]){  
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
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
//*********************************************
// Función para dibujar una imagen sprite - los parámetros columns = número de imagenes en el sprite, index = cual desplegar, flip = darle vuelta
//*********************************************
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset){
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
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

//***************************************** SD
void Read_SD(void){
  bandera = 0;  
  memset(data2, 0, 9000);
  indice = 0;
  
  if (myFile){    
    while (myFile.available()) {
       contenidoSD = myFile.read();
       if (contenidoSD == 'f'){
          caracter = 15;
       }
       else if (contenidoSD == 'a'){
          caracter = 10;
       }
       else if (contenidoSD == 'b'){
          caracter = 11;
       }
       else if (contenidoSD == 'c'){
          caracter = 12;
       }
       else if (contenidoSD == 'd'){
          caracter = 13;
       }
       else if (contenidoSD == 'e'){
          caracter = 14;
       }
       else if (contenidoSD == '0'){
          caracter = 0;
       }
       else if (contenidoSD == '1'){
          caracter = 1;
       }
       else if (contenidoSD == '2'){
          caracter = 2;
       }
       else if (contenidoSD == '3'){
          caracter = 3;
       }
       else if (contenidoSD == '4'){
          caracter = 4;
       }
       else if (contenidoSD == '5'){
          caracter = 5;
       }
       else if (contenidoSD == '6'){
          caracter = 6;
       }
       else if (contenidoSD == '7'){
          caracter = 7;
       }
       else if (contenidoSD == '8'){
          caracter = 8;
       }
       else if (contenidoSD == '9'){
          caracter = 9;
       }
       if (bandera == 0){
        temp = (caracter*16);        
       }
       if (bandera == 1){
        temp = (temp + (caracter));
        data2[indice] = temp;
        indice++;
       }
       bandera = !bandera;
       
    }
    myFile.close();
  }
}
