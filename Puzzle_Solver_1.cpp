/*
*
* Team Id: 		eYRCPlus-PS1#1354
* Author List: 	Dinesh M, Nivetha C
* Filename: 	Puzzle_Solver_1.cpp
* Theme: 		Puzzle Solver 1
* Functions:	Boot_switch_config(), main()
* Global Variables:	None
*
*/

#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "White_line_sensor.h"
#include "Motor.h" 
#include "Line_following.h"  
#include "Buzzer_LED.h"
#include "Encoder.h"
#include "Serial_communication.h"
#include "Glcd.h"
#include "Pick_and_Deposit.h"

/* 9 elements are required to pick a single number from D1 and place it in a position in D2. 
9 information are
D1_row, D1_column, pick_right/left, number_to_be_picked, position_to_be_placed, D2row, D2col, place_right/left, buzzer_or_not 
These Information are communicated from python script to the robot using Serial communication in an array. These values are passed
to reppective functions for the traversal of the robot 
*/

/*
*
* Function Name: 	Boot_switch_config
* Logic: 			Sets the data direction for PORTE 7th pin as Input and enables pull up.
*/

void Boot_switch_config(void)
{
	DDRE = DDRE & 0x7F;  //PORTE 7 pin set as input
	PORTE = PORTE | 0x80;
}

/*
*
* Function Name: 	main
* Logic: 			Configures the Boot switch and Serial communication. Waits for the boot key press infinitely.
*					Once boot key is pressed, instance for pick_and_deposit class is created and it passes the 
*					serially communicated data to D1 and D2 functions for traversal of robot.
*/



int main(void)
{
	Boot_switch_config();
	Serial_config();
	while(1)
	{
		if((PINE & 0x80) != 0x80) // If Switch is pressed
		{
			Pick_and_deposit pick_and_deposit;
			for (unsigned char i=0;i<serial_data_count;i+=9)
			{
				/*
				serial_data[i] --> D1_row
				serial_data[i+1] --> D1_column
				serial_data[i+2] --> Right / Left
				serial_data[i+3] --> Number to be picked
				serial_data[i+4] --> position to be placed
				serial_data[i+5] --> D2_row
				serial_data[i+6] --> D2_column
				serial_data[i+7] --> Right / Left
				serial_data[i+8] --> Buzzer or NOT
				*/
				pick_and_deposit.pick_from_D1(serial_data[i],serial_data[i+1],serial_data[i+2],0,0,serial_data[i+3]);
				pick_and_deposit.place_in_D2(serial_data[i+5],serial_data[i+6],serial_data[i+7],serial_data[i+8],serial_data[i+4],0);
				// Both the above functions receive these values as "destination_row,destination_column,dest_direction,buzz,position,number"
			}
				
		}
		
	}
	return 0;
}