#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile int ledStates[4][6];//matrix representing states of leds on clock

//incremented vars on each interrupt event
volatile int segundo = 0;
volatile int minuto = 0;
volatile int hora = 0;

int* arrTimePointers[] = {&segundo, &minuto, &hora};


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

/*
  Function to setup the Timer/Counter1.
  we need a count in seconds but our clock is 16Mhz,
  if we use a prescaler of 1024 we have a count clock in 15635,
  then so must rely 15625 times to have a clock of 1 Hz and so count one second at a time
*/
void initTimer1(){
  TCCR1B |= (1 << CS12) | (1 << CS10);  //0b00000101 for configure a 1024 prescaler on counter
  TIMSK1 |= (1 << TOIE1);               //0b00000001 for enable a overflow interrupt
  sei(); // enable a global interrupt

  /*
    inicializing timer at 49910 to set overflow with counting at 15625
  */
  TCNT1H = 0xC2;
  TCNT1L = 0xF6;
}

void updateScreen(){
  int decimal; = hora/10;
  int unit; = hora%10;
  int i,j,k;
  for(k=0; i<6; i +=2){
    decimal = *(arrTimePointers[k/2])/10;
    unit = *(arrTimePointers[k/2])%10;
    for(i=3; i>=1; i--){
      ledStates[i][k] = (decimal % 2 == 1);
      decimal /= 2;
    }
    for (j=3; j>=0; j--){
      ledStates[j][k+1] = (unit % 2 == 1);
      unit /= 2;      
    }

  }
  
}

int main(){
  init();
  initTimer1();
  while(1){
  updateScreen();
    for(int i=0;i<4;i++){
      PORTB = 0x00;
      PORTB |= (1 << i);
      for(int j=0;j<6;j++){
        PORTD = 0x00;
        if(ledStates[i][j] == 1)
          PORTD |= (1 << (j+2));
      }
    }
  }

}

/*
  Interrupt Service Routine, used with a 16bit Timer/Counter for divide 16Mhz for a utilizable clock
*/
ISR(TIMER1_OVF_vect){
  //reset timer-------------
  TCNT1H = 0xC2;
  TCNT1L = 0xF6;
  //------------------------
  //Handler time.
  if(segundo%60 == 0){
    if(minuto % 60 == 0){
      hora = (hora + 1) % 24;
    }
    minuto = (minuto + 1) % 60;
  }
  segundo = (segundo + 1) % 60;
}
