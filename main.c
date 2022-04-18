/*
 * Stopwatch.c
 *
 * Created: 4/8/2022 5:37:32 PM
 * Author : Vostro
 */ 

#define F_CPU 16000000UL
#define u_delay 3
#define d_delay 1

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


static short cnt=0;
static short cnt1=0;
static short cnt2=0;

volatile uint8_t count_flag = 0;

//Digit Map
#define DA 0
#define DB 1
#define DC 2
#define DD 3
#define DE 4
#define DF 5
#define DG 6
#define DP 7		//point

//Digit Position
#define D1 4
#define D2 5
#define D3 6
#define D4 7

char digit[10]={
	((1<<DA) | (1<<DB) | (1<<DC) | (1<<DD) | (1<<DE) | (1<<DF)),						//0
	((1<<DB) | (1<<DC)),																//1
	((1<<DA) | (1<<DB) | (1<<DD) | (1<<DE) | (1<<DG)),									//2
	((1<<DA) | (1<<DB) | (1<<DC) | (1<<DD) | (1<<DG)),									//3
	((1<<DB) | (1<<DC) | (1<<DF) | (1<<DG)),											//4
	((1<<DA) | (1<<DC) | (1<<DD) | (1<<DF) | (1<<DG)),									//5
	((1<<DA) | (1<<DC) | (1<<DD) | (1<<DE) | (1<<DF) | (1<<DG)),						//6
	((1<<DA) | (1<<DB) | (1<<DC)),														//7
	((1<<DA) | (1<<DB) | (1<<DC) | (1<<DD) | (1<<DE) | (1<<DF) | (1<<DG)),				//8
	((1<<DA) | (1<<DB) | (1<<DC) | (1<<DD) | (1<<DF) | (1<<DG)),						//9
};


ISR(INT0_vect){
	count_flag = ~count_flag;
}

ISR(INT1_vect){
	cnt = 0;
	count_flag = 0;
	PORTD =( 1<< D4); // DIG 4
	PORTC = ~digit[0];
	//_delay_ms(10);
}

void counting(void){
	if (cnt<=9)
	{
		PORTD =( 1<<D4); // DIG 4
		PORTC = ~digit[cnt%10];
		_delay_ms(u_delay);
	}
	
	else if ((cnt>9) && (cnt<99))
	{
		PORTD =( 1<<D4); // DIG 4
		PORTC = ~digit[cnt%10];
		_delay_ms(u_delay);
		
		PORTD =( 1<<D3); // DIG 3
		PORTC = ~digit[cnt/10];	
		//PORTC = ~(digit[cnt/10] | (1<<DP));	
		_delay_ms(u_delay);
	}
			
	else if ((cnt>99) && (cnt<999))
	{
		PORTD =( 1<<D4); // DIG 4
		PORTC = ~digit[cnt%10];
		_delay_ms(u_delay);
				
		cnt1 = cnt/10;
			
		PORTD =( 1<<D3); // DIG 3
		PORTC = ~digit[cnt1%10];
		_delay_ms(u_delay);
			
		PORTD = (1<<D2); // DIG 2
		PORTC = ~(digit[cnt1/10] | (1<<DP));
		_delay_ms(d_delay);
	}
			
	else if (cnt>999)
	{
		PORTD = (1<<D4); // DIG 4
		PORTC = ~digit[cnt%10];
		_delay_ms(u_delay);
				
		cnt1 = cnt/10;
		
		PORTD = (1<<D3); // DIG 3
		PORTC = ~digit[cnt1%10];
		_delay_ms(u_delay);
				
		cnt2 = cnt1/10;
		
		PORTD = (1<<D2); // DIG 2
		PORTC = ~(digit[cnt2%10] | (1<<DP));
		_delay_ms(d_delay);
		
		PORTD = (1<<D1); // DIG 1
		PORTC = ~digit[cnt2/10];
		_delay_ms(d_delay);
	}

}


int main(void){
	DDRC = 0xFF;
	DDRD = 0xF0;
	
	
	MCUCR = 0x0F;
	GICR = 0xC0;
	sei();
		
	while (1){

		counting();
		if(count_flag) {
			cnt++;
			if (cnt>5999) cnt=0;
		}
	}
}