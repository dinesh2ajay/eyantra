/*
*
* Team Id: 			eYRCPlus-PS1#1354
* Author List: 		Dinesh M
* Filename: 		Serial_communication.h
* Theme: 			Puzzle Solver 1
* Functions:		ISR, Serial_config
* Global Variables: serial_data[], serial_data_count
*
*/

unsigned char serial_data[255]; // Serial data that has been received by the robot.
unsigned char serial_data_count=0; // Stores the number of data in serial_data array

/*
*
* Function Name: 	ISR
* Input:			USART2_RX_vect
* Logic: 			During Serial Communication, when a data is received, it puts the incoming data into an array
*					called serial_data and also counts the number of incoming data. This serial_data_count will be 
*					used later for running a for loop.
*
*/

ISR(USART2_RX_vect)
{
	serial_data[serial_data_count]=UDR2;  // Move the contents from UDR2 to the serial_data array
	serial_data_count+=1; // Increment the count value by 1.
}

/*
*
* Function Name: 	Serial_config
* Logic: 			Configures the registers for Serial commmunication in USART2 of ATmega2560 microcontroller.
*					This is done by enabling the Receive enable bits. Baud rate register is also configured and
*					finally global interrupt is enabled.
*
*/

void Serial_config()
{
	
	cli();
	UCSR2B = (1<<RXEN2) | (1<<RXCIE2); 
	UCSR2C = (1<<UCSZ21) | (UCSZ20); // Sets the character size as 8 in a frame. (Number of data bits)
	UBRR2L = 0x5F; // Load baud rate register with 0x5F for XTAL of 14745600 Hz and 9600 baud rate.
	sei(); 
}