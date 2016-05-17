/*
*
* Team Id: 			eYRCPlus-PS1#1354
* Author List: 		Dinesh M
* Filename: 		Pick_and_Deposit.h
* Theme: 			Puzzle Solver 1
* Functions:		turn_180_degrees(), goto_row(unsigned char,unsigned char)
*					goto_column(unsigned char,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char),
*					goto_column0(unsigned char,unsigned char), go_home_from_start(), go_bridge(),
*					deposit_and_buzz(unsigned char), sequence_operations(unsigned char,unsigned char,unsigned char);
*					pick_from_D1(unsigned char,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char),
*					place_in_D2(unsigned char,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char),
*					Pick_and_deposit()
* Global Variables: destination[2]
*
*/


#define row 0 //row
#define column 1 //column
unsigned char destination[2];
unsigned char count=0;
buzzer_led Buzzer_LED;

class Pick_and_deposit
{
	Glcd glcd;
	private:
		void turn_180_degrees();
		void goto_row(unsigned char,unsigned char);
		void goto_column(unsigned char,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char);
		void goto_column0(unsigned char,unsigned char);
		void go_home_from_start();
		void go_bridge();
		void deposit_and_buzz(unsigned char);
		void sequence_operations(unsigned char,unsigned char,unsigned char);
	public:
		void pick_from_D1(unsigned char,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char);
		void place_in_D2(unsigned char,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char);
		Pick_and_deposit();
};

/*
*
* Function Name: 	turn_180_degrees
* Input:			None
* Logic: 			In order to turn 180 degrees, first move back for 11 cm and take a soft left for 90 degrees
*					and soft left 2 for 90 degrees. delay is called to prevent electrical surges when there is sudden
*					change in motor's direction of rotation
*
*/

void Pick_and_deposit::turn_180_degrees()
{
	_delay_ms(70);
	back_mm(110);
	_delay_ms(80);
	soft_left_degrees(90);
	soft_left_2_degrees(90);
	_delay_ms(70);
}

/*
*
* Function Name: 	goto_row
* Input:			destination_row --> The row in which bot has to go in D1 or D2 to pick or place a number.
*					division --> specifies whether the robot is going to the destination row in division 1 or division 2.
* Logic: 			Refer the image before reading this. This function is common for both D1 and D2
*					Based on the Division ( 1 or 2 ), robot takes a set of motions to reach the row it has to go.
*					If division is 1 and robot has to go to row 1, then it takes a soft right and then a soft left.
*					If division is 2 and robot has to go to row 2, then it takes a soft left and soft right.
*					Similar manner, the motions are coded for row 0 and row 1 in D1 and row 0, row 1 and row 2 in D2
*
*/

void Pick_and_deposit::goto_row(unsigned char destination_row,unsigned char division)
{
	switch(destination_row)
	{
		case 0:
		{
			if(division==1)
			{
				destination[row]=destination_row;
				break;
			}
			if(division==2)
			{
				node_count_copy=node_count;
				soft_right_degrees(78);
				soft_left_1_degrees(88);
				if(node_count_copy!=node_count)
				node_count-=1;
				back_mm(20);
				destination[row]=destination_row;
				break;
				
			}
		}
		case 1:
		{
			if(division==1)
			{
				node_count_copy=node_count;
				soft_right_degrees(76);
				soft_left_1_degrees(88);
				if(node_count_copy!=node_count)
				node_count-=1;
				back_mm(20);
				destination[row]=destination_row;
				break;
				
			}
			if(division==2)
			{
				destination[row]=destination_row;
				break;
				
			}
			
		}
		
		case 2:
		{
			node_count_copy=node_count;
			soft_left_degrees(85);
			soft_right_1_degrees(76);
			if(node_count_copy!=node_count)
			node_count-=1;
			back_mm(20);
			destination[row]=destination_row;
			break;
		}
	}
}

/*
*
* Function Name: 	goto_column
* Input:			destination_column --> The column to which bot has to go in D1 or D2 to pick or place a number.
*					division --> specifies whether the robot is traversing in division 1 or division 2.
*					direction --> specifies whether the number has to be pick from right or left. 0 --> Right 1 --> Left
*					buzz --> specifies NOT to buzz, or buzz for 1 sec or continuous buzz. 0 --> No buzz, 1 --> buzz for 1s, 2 - continuous buzz
*					position --> specifies the position of the number in D2. Ranges from 0 to 23. For D1 it's not used. 
					number --> Specifies the actual number picked. sent to GLCD for printing. For D2, it's not used.
* Logic: 			Refer the image before reading this. This function is common for both D1 and D2
*					Since the robot has already reached the destination row, in order to reach destination column, it has to move straight.
*					so until destination column is reached the robot moves straight and also follows the line.
*					Once if it reaches destination column, it checks for Division 1 or 2.
*					If it's division 1 then the robot turns 180 degrees and picks a number based on value stored in "direction" variable
*					and glows repective LED. It also sends the number picked to glcd_print() to print it on GLCD.
*					If it's division 2, then sequence_operations is called to place the number.
*
*/

void Pick_and_deposit::goto_column(unsigned char destination_column,unsigned char division,
									unsigned char direction,unsigned char buzz,unsigned char position,unsigned char number)
{
	destination[column]=0;
	node_count_copy=node_count;
	count=0;
	while(destination_column!=destination[column]-1)
	{
		
		motor.forward();
		line_following();
		if(node_count==node_count_copy+1)
		{
			count+=1;
			destination[column]+=1;
			node_count_copy=node_count;
		}
		
	}
	
	if(division==1)
	{
		turn_180_degrees();
		glcd.glcd_print(number);
		if(direction==0) Buzzer_LED.right_led_on();
		if(direction==1) Buzzer_LED.left_led_on();
	}
	if(division==2)
	{
		sequence_operations(direction,position,buzz);
		motor.stop();
		_delay_ms(500);
		glcd.glcd_clear();
	}
	motor.stop();
	_delay_ms(70);
}

/*
*
* Function Name: 	goto_column0
* Input:			destination_row --> The row in which bot has to go in D1 or D2 to pick or place a number.
*					division --> specifies whether the robot is going to the destination row in division 1 or division 2
*					direction --> specifies whether number is picked right or left. 0 --> Right 1 --> Left
* Logic: 			Refer the image before reading this. This function is common for both D1 and D2
*					Count variable in the goto_column function specifies how far the robot has moved forward. So the same variable is
*					decremented until it becomes zero to reach at column 0 in D1 or D2. Once again goto_row is called to go back to the bridge
*					between D1 and D2.
*
*/

void Pick_and_deposit::goto_column0(unsigned char destination_row,unsigned char division)
{
	while(count!=0)
	{
		node_count_copy=node_count;
		_delay_ms(70);
		motor.forward();
		line_following();
		if(node_count!=node_count_copy) count-=1;
	}
	_delay_ms(70);
	goto_row(destination_row,division);
}

/*
*
* Function Name: 	go_home_from_start
* Input:			None
* Logic: 			Invoked when instance of pick and deposit class is created. Home position is assumed to be standing
*					at the bridge between D1 and D2. Initially robot is in START. So it moves forward until it counts 3 nodes(including start)
*					and then turns right and counts 2 nodes and then takes 180 degrees and faces D1 to pick a number.
*
*/

void Pick_and_deposit::go_home_from_start()
{
	node_count_copy=node_count;
	while(node_count!=node_count_copy+3)
	{
		motor.forward();
		line_following();
	}
	_delay_ms(70);
	
	soft_right_degrees(95);
	node_count_copy=node_count;
	while(node_count!=node_count_copy+2)
	{
		motor.forward();
		line_following();
	}
	_delay_ms(70);
	forward_mm(100);
	_delay_ms(70);
	
	soft_right_degrees(90);
	soft_right_2_degrees(85);
	
	node_count_copy=node_count;
	_delay_ms(70);
	while(node_count!=node_count_copy+1)
	{
		motor.forward();
		line_following();
	}
	motor.stop();
}

/*
*
* Function Name: 	go_bridge
* Input:			None.
* Logic: 			Until another node is counted move forward. After execution of this function robot will be in the 
*					bridge between D1 and D2 which is the home position.
*
*/

void Pick_and_deposit::go_bridge()
{
	node_count_copy=node_count;
	while(node_count!=node_count_copy+1)
	{
		motor.forward();
		line_following();
	}
}

/*
*
* Function Name: 	deposit_and_buzz
* Input:			buzz --> 0 --> no buzz, 1 --> buzz for 1s, 2 --> continuous buzzer
* Logic: 			In order to deposit any LED turned ON is made OFF and DEPOSIT is printed on GLCD.
*					Based on the value in buzz variable buzzer sound is made.
*
*/

void Pick_and_deposit::deposit_and_buzz(unsigned char buzz)
{
	Buzzer_LED.led_off();
	glcd.glcd_print(10); // Prints DEPOSIT on the GLCD
	switch(buzz)
	{
		case 0:
		break;
		case 1:
		Buzzer_LED.buzzer_on();
		_delay_ms(1000);
		Buzzer_LED.buzzer_off();
		glcd.glcd_clear();
		break;
		case 2:
		Buzzer_LED.buzzer_on();
		break;
	}
}

/*
*
* Function Name: 	sequence_operations
* Input:			direction --> specifies whether the number has to be pick from right or left. 0 --> Right 1 --> Left
*					position --> specifies the position of the number in D2. Ranges from 0 to 23. For D1 it's not used.
*					buzz --> specifies NOT to buzz, or buzz for 1 sec or continuous buzz. 0 --> No buzz, 1 --> buzz for 1s, 2 - continuous buzz
*					
* Logic: 			This function is called only for D2. 
*					If the robot has picked the number at RIGHT, and if position of number to be deposited is from 0 to 11, then
*					robot turns 180 degree and deposits the number and buzz if necessary.
*					If position is from 12 to 23, the robot will first deposit and then turn 180 degrees.
*
*					If the robot has picked the number at LEFT and if the position of number to be deposited is form 0 to 11, then
*					number is deposited and then turns 180 degrees. If position is from 12 to 23 then after turning 180 it will deposit.
*
*/

void Pick_and_deposit::sequence_operations(unsigned char direction,unsigned char position,unsigned char buzz)
{
	switch(direction)
	{
		case 0:  //Right
		if(position>=0 && position<=11) //first two row in D2
		{
			turn_180_degrees();
			deposit_and_buzz(buzz);
		}
		else if(position>=12 && position <=23)
		{
			motor.stop();
			deposit_and_buzz(buzz);
			if(buzz!=2)
			turn_180_degrees();
			else
			back_mm(15);
		}
		break;
		case 1: //Left
		if(position>=0 && position<=11) // last two row in D2
		{
			motor.stop();
			deposit_and_buzz(buzz);
			if(buzz!=2)
			turn_180_degrees();
			else
			back_mm(15);
			
			
		}
		else if(position>=12 && position <=23)
		{
			
			turn_180_degrees();
			deposit_and_buzz(buzz);
		}
		break;
	}
}

/*
*
* Function Name: 	pick_from_D1
* Input:			destination_row --> The row in which bot has to go in D1 or D2 to pick or place a number.
*					division --> specifies whether the robot is going to the destination row in division 1 or division 2
*					dest_direction --> specifies whether number is picked right or left. 0 --> Right 1 --> Left
*					position --> specifies the position of the number in D2. Ranges from 0 to 23. For D1 it's not used.
*					buzz --> specifies NOT to buzz, or buzz for 1 sec or continuous buzz. 0 --> No buzz, 1 --> buzz for 1s, 2 - continuous buzz
*					number --> Specifies the number to be picked.
* Logic: 			To pick a number from D1, first robot will decide which row it has to go by goto_row. Once if it reaches the row,
*					the robot will decide on which column it has to move. After this number will be picked and it moves back to the column 0
*
*/

void Pick_and_deposit::pick_from_D1(unsigned char dest_row,unsigned char dest_column,unsigned char dest_direction,
									unsigned char buzz,unsigned char position,unsigned char number)
{
	goto_row(dest_row,1);
	goto_column(dest_column,1,dest_direction,buzz,position,number);
	goto_column0(dest_row,1);
	motor.stop();
}

/*
*
* Function Name: 	place_in_D2
* Input:			destination_row --> The row in which bot has to go in D1 or D2 to pick or place a number.
*					division --> specifies whether the robot is going to the destination row in division 1 or division 2
*					dest_direction --> specifies whether number is picked right or left. 0 --> Right 1 --> Left
*					position --> specifies the position of the number in D2. Ranges from 0 to 23. For D1 it's not used.
*					buzz --> specifies NOT to buzz, or buzz for 1 sec or continuous buzz. 0 --> No buzz, 1 --> buzz for 1s, 2 - continuous buzz
*					number --> Specifies the number to be picked.
* Logic: 			Now the number from D1 has been picked. Now the robot comes to the bridge. Inorder to traverse in D2, first robot 
*					determines which row it has to go. After reaching the required row, it moves into the required column in forward direction
*					After reaching the destination and placing the number if needs continuous buzzer, the robot stops. If not it wil continue
*					moving into D1 by going back to column 0 in D2 and then to the bridge.
*
*/

void Pick_and_deposit::place_in_D2(unsigned char dest_row,unsigned char dest_column,unsigned char dest_direction,
									unsigned char buzz,unsigned char pos,unsigned char num)
{
	
	go_bridge();
	goto_row(dest_row,2);
	goto_column(dest_column,2,dest_direction,buzz,pos,num);
	if(buzz==2) motor.stop();
	else
	{
		
		goto_column0(dest_row,2);
		go_bridge();
		motor.stop();
	}
	
}

/*
*
* Function Name: 	Pick_and_deposit
* Input:			None
* Logic: 			When instance of the class is created this function is invoked. Since this happens only once when bot is
*					in START, go_home_from_start is called from here after configuring the Encoder and clearing the GLCD.
*
*/

Pick_and_deposit::Pick_and_deposit()
{
	Encoder_config();
	glcd.glcd_clear();
	go_home_from_start();
	
}


