#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile int ledStates[4][6];
volatile int time = 0;
volatile int debug = 1;
volatile int segundo = 0;
volatile int minuto = 0;
volatile int hora = 0;

void init(){
  DDRB = 0b00001111;
  DDRD = 0b11111100;
  PORTD = 0x00;
  PORTB = 0x00;
  for(int i=0; i<4 ;i++){
    for(int j=0; j<6; j++){
      ledStates[i][j] = 0;
    }
  }
}

void initTimer1(){
  //TCCR1A = (1 << WGM10);
  TCCR1B = 0b00000101;
  TIMSK1 = 0b00000001;
  sei();
  TCNT1H = 0b11111110;
  TCNT1L = 0b11111011;
}

void escreveSegundo(){
  int decHora = hora/10;
  int uniHora = hora%10;
  
  ledStates[3][0] = (decHora % 2 == 1);
  decHora /= 2;
  ledStates[2][0] = (decHora % 2 == 1);
  decHora /= 2;
  
  ledStates[3][1] = (uniHora % 2 == 1);
  uniHora /= 2;
  ledStates[2][1] = (uniHora % 2 == 1);
  uniHora /= 2;
  ledStates[1][1] = (uniHora % 2 == 1);
  uniHora /= 2;
  ledStates[0][1] = (uniHora % 2 == 1);
  uniHora /= 2;
  
  
  int decMin = minuto/10;
  int uniMin = minuto%10;
  
  ledStates[3][2] = (decMin % 2 == 1);
  decMin /= 2;
  ledStates[2][2] = (decMin % 2 == 1);
  decMin /= 2;
  ledStates[1][2] = (decMin % 2 == 1);
  decMin /= 2;
  
  ledStates[3][3] = (uniMin % 2 == 1);
  uniMin /= 2;
  ledStates[2][3] = (uniMin % 2 == 1);
  uniMin /= 2;
  ledStates[1][3] = (uniMin % 2 == 1);
  uniMin /= 2;
  ledStates[0][3] = (uniMin % 2 == 1);
  uniMin /= 2;
  
  
  int decSec = segundo/10;
  int uniSec = segundo%10;
  
  ledStates[3][4] = (decSec % 2 == 1);
  decSec /= 2;
  ledStates[2][4] = (decSec % 2 == 1);
  decSec /= 2;
  ledStates[1][4] = (decSec % 2 == 1);
  decSec /= 2;
  
  ledStates[3][5] = (uniSec % 2 == 1);
  uniSec /= 2;
  ledStates[2][5] = (uniSec % 2 == 1);
  uniSec /= 2;
  ledStates[1][5] = (uniSec % 2 == 1);
  uniSec /= 2;
  ledStates[0][5] = (uniSec % 2 == 1);
  uniSec /= 2;
}

int main(){
  init();
  initTimer1();
  while(1){
escreveSegundo();
    for(int i=0;i<4;i++){
      
      PORTB = 0x00;
      PORTB |= (1 << i);
      
      for(int j=0;j<6;j++){
        PORTD = 0x00;
        if(ledStates[i][j] == 1)
          PORTD |= (1 << (j+2));
        //_delay_ms(500);
      }
    }
  }

}


ISR(TIMER1_OVF_vect){
  TCNT1H = 0b11111110;
  TCNT1L = 0b11111011;
  if(time % 60 == 0){
    if(segundo%60 == 0){
      if(minuto % 60 == 0){
        hora = (hora + 1) % 24;
      }
      minuto = (minuto + 1) % 60;
    }
    segundo = (segundo + 1) % 60;
  }
  time = (time + 1) % 60;
}
