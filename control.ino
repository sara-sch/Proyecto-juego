  #define joyX 12     // pin analogico joystick jugador 1 
  #define joyY 13     // pin analogico joystick jugador 1 
  #define joyX2 39     // pin analogico joystick jugador 2    
  #define joyY2 34    // pin analogico joystick jugador 2
  int xValue;
  int yValue;
  int xValue2;
  int yValue2;
  int butt;            // boton jugador 1 
  char butt_s;       // char para boton jugador 1 
  int butt2;           // boton jugador 2 
  char butt2_s;      // char para boton jugador 2
  int flag = 0;
  char movement;
  char movement2;


 
void setup() {
  Serial.begin(115200);
  pinMode(joyX, INPUT);
  pinMode(joyY, INPUT);
  pinMode(joyX2, INPUT);
  pinMode(joyY2, INPUT);
  pinMode(23, INPUT_PULLUP);    // pin botón jugador 1 
  pinMode(22, INPUT_PULLUP);    // pin botón jugador 2 

  }
 
void loop() {

  
  xValue = analogRead(joyX);                  // Leer datos del joystick y el botón
  yValue = analogRead(joyY);
  butt = digitalRead(23);
  xValue2 = analogRead(joyX2);
  yValue2 = analogRead(joyY2);
  butt2 = digitalRead(22);

  if(xValue > 2700){                          // Mandar char para indicar posición en la que el joystick 1 se mueve
    movement = 'e'; //right1
    }
  else if(xValue < 600){
    movement = 'f'; //left1
    }
  else if(yValue > 2700){
    movement = 'g'; //down1
    }
  else if(yValue < 600){
    movement = 'h'; //up
    }
  else{
    movement = 'i'; //zero1
    }

if(xValue2 > 2400){                          // Mandar char para indicar posición en la que el joystick 2 se mueve
    movement2 = 'j'; //right2
    }
  else if(xValue2 < 800){
    movement2 = 'k'; //left2
    }
  else if(yValue2 > 2200){
    movement2 = 'l'; //down2
    }
  else if(yValue2 < 1500){
    movement2 = 'm'; //up2
    }
  else{
    movement2 = 'n'; //zero2
    }
    
 if(flag==4){                               // Incremento de bandera de 0 a 4
            flag = 0;
    }
   else{
     flag++;
    }


 if (butt== 1){                             // Mandar char para indicar si el botón 1 está presionado o no
  butt_s = 'a';  //no1
  }

 if (butt== 0){
  butt_s = 'b';  // yes1
  } 

 if (butt2== 1){                             // Mandar char para indicar si el botón 2 está presionado o no
  butt2_s = 'c'; //no2
  }

 if (butt2== 0){
  butt2_s = 'd'; //yes2
  } 


        
 if (flag == 1){                              // Mandar dato del joystick o botón acorde a la bandera 
   Serial.println(butt_s);
   delay(100);
   return;
   }

 if (flag == 2){
   Serial.println(movement);
   delay(100);
   return;
   }

 if (flag == 3){
   Serial.println(butt2_s);
   delay(100);
   return;
   }

 if (flag == 4){
   Serial.println(movement2);
   delay(100);
   return;
   }

 

}
