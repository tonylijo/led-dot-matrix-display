//*********************************************UART EXAMPLE FOR AVR************USES POLLING FOR RX AND TX*************//
#include <avr/io.h>
#include <inttypes.h>




#define F_CPU 1000000

//ndif

//set desired baud rate

#define BAUDRATE 1200

//calculate UBRR value

#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)

//This function is used to initialize the USART
//at a given UBRR value
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

   UCSRC=(1<<USBS)|(3<<UCSZ0);


   //Enable The receiver and transmitter

   UCSRB=(1<<RXEN)|(1<<TXEN);


}


//This function is used to read the available data
//from USART. This function will wait untill data is
//available.
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


//This fuction writes the given "data" to
//the USART which then transmit it via TX line
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

int  main()
{
   
   char data;

  

   USARTInit();    //

   //Loop forever

   while(1)
   {
      //Read data
      data=USARTReadChar();

      /* Now send the same data but but surround it in
      square bracket. For example if user sent 'a' our
      system will echo back '[a]'.

      */

    //  USARTWriteChar('[');
      USARTWriteChar(data);
    //  USARTWriteChar(']');

   }
return 0;
}


