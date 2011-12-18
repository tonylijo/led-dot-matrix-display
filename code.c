

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include<avr/interrupt.h>
unsigned char datarx;

#define BAUDRATE 1200

//calculate UBRR value

#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)



#define number_of_74hc595s 1 //How many of the shift registers are there daisey chained?
#define SPORT PORTB 	//shift register port
#define SRCLK_Pin PB1  //serial data clock to 74ls595
#define RCLK_Pin PB2 	//STORAGE register clock  PIN 12 OF SHIFT REGISTER
#define SER_Pin PB3	//serial data pin



void USARTInit()
{

   //Set Baud rate

   UBRRL = UBRRVAL;
   UBRRH = (UBRRVAL>>8);

   /*Set Frame Format


   >> Asynchronous mode
   >> No Parity
   >> 1 StopBit

   >> char size 8

   */

   UCSRC=(1<<URSEL)|(1<<USBS)|(3<<UCSZ0);


   //Enable The receiver and transmitter

   UCSRB=(1<<RXEN)|(1<<TXEN)|(1<<RXCIE);

//enable global interrupts 

sei();
}





ISR(USART_RXC_vect)

{

	//defien temp value for storing received byte

	uint8_t Temp;

	//Store data to temp

	datarx= UDR;

//	Temp++;//increment

	//send received data back

	// no need to wait for empty send buffer
 while(!(UCSRA & (1<<UDRE)))
   {
      //Do nothing
   }



	UDR=datarx;

}



char USARTReadChar()
{
   //Wait untill a data is available

   while(!(UCSRA & (1<<RXC)))
   {
      //Do nothing
   }

   //Now USART has got data from host
   //and is available is buffer

   return UDR;
}
void USARTWriteChar(char data)
{
   //Wait untill the transmitter is ready

   while(!(UCSRA & (1<<UDRE)))
   {
      //Do nothing
   }

   //Now write the data to USART buffer

   UDR=data;
}
int main(){
	DDRC=0xff;
	USARTInit();	
	DDRB = 0xFF;
	PORTB = 0x00;
char a,b;
int i;
while(1)
	{
	//a=USARTReadChar();	
//USARTWriteChar(a);

		 				// Counter used for displaying a number in binary via the shift register
		
		// PB1 = SERCLK  PB2 = RCLK  PB3 = SER
//	switch (datarx)
//		{
//		case 1:
		while(datarx=='A')
			{
				for(i=0;i<100;i++)
				{
				display(0b11111,0b10001,0b10001,0b11111,0b10001,0b10001,0b10001);
				}				
			}	
		while(datarx=='B')
			{	
				for(i=0;i<100;i++)
				{
				display(0b11110,0b10001,0b10001,0b11110,0b10001,0b10001,0b11110);
				}
			}
		while(datarx=='C')
			{
				for(i=0;i<100;i++)
				{
				display(0b11111,0b10000,0b10000,0b10000,0b10000,0b10000,0b11111);
                		}
		         }
//			break;
//		default:
//			break;
//		}



//shift(0b00001);
	}
	return 0;
}

void row(uint8_t row_no)
{
switch(row_no)
{
	case 1:
		PORTC=0b0000000;
		break;
	case 2:
		PORTC=0b0000001;
		break;
	case 3:
		PORTC=0b0000010;
		break;
	case 4:
		PORTC=0b0000011;
		break;
	case 5:
		PORTC=0b0000100;
		break;
	case 6:
		PORTC=0b0000101;
		break;
	case 7:
		PORTC=0b0000110;
		break;
	default:
		PORTC=0b1111111;
		break;
}
}


void display(uint8_t a,uint8_t b, uint8_t c,uint8_t d, uint8_t e,uint8_t f,uint8_t g)
{
row(1);
shift(a);
_delay_ms(2);
shift(0x00);

row(2);
shift(b);
_delay_ms(2);
shift(0x00);

row(3);
shift(c);

_delay_ms(2);
shift(0x00);

row(4);
shift(d);
_delay_ms(2);
shift(0x00);

row(5);
shift(e);
_delay_ms(2);
shift(0x00);

row(6);
shift(f);
_delay_ms(2);
shift(0x00);

row(7);
shift(g);
_delay_ms(2);
shift(0x00);
}

void shift( uint8_t data)

{
	SPORT &= ~(1 << RCLK_Pin); 				// Set the register-clock pin low


//unsigned long data=~data1;
int i;
	for ( i = 0; i < (8 * number_of_74hc595s); i++){	// Now we are entering the loop to shift out 8+ 
SPORT &= ~(1 << SRCLK_Pin);                     // Set the serial-clock pin low

//_delay_ms(100);
                SPORT &= ~(((data&(0x01<<i))>>i) << SER_Pin );  // Go through each bit of data and output it
//_delay_ms(100);
                SPORT |= (1 << SRCLK_Pin);                      // Set the serial-clock pin high
//_delay_ms(100);
                SPORT |= (((data&(0x01<<i))>>i) << SER_Pin );   // Set the datapin low again    
//_delay_ms(100);
        }




	SPORT |= (1 << RCLK_Pin);				// Set the register-clock pin high to update the output of the shift-register
}
