/*
*
* Team Id: 			eYRCPlus-PS1#1354
* Author List: 		e-Yantra Tutorial code with slight modifications by Dinesh M
* Filename: 		Encoder.h
* Theme: 			Puzzle Solver 1
* Functions:		left_encoder_pin_config(), right_encoder_pin_config(),left_position_encoder_interrupt_init(),
*					right_position_encoder_interrupt_init(), Encoder(), angle_rotate(unsigned int),
*					linear_distance_mm(unsigned int), forward_mm(unsigned int), back_mm(unsigned int), 
*					soft_right_degrees(unsigned int), soft_left_degrees(unsigned int), soft_right_1_degrees(unsigned int)
*					soft_left_1_degrees(unsigned int), soft_right_2_degrees(unsigned int), soft_left_2_degrees(unsigned int)
*
* Global Variables: ShaftcountLeft, ShaftcountRight, Degrees
*/

volatile unsigned long int ShaftcountLeft = 0; //to keep track of left position encoder
volatile unsigned long int ShaftcountRight = 0; //to keep track of right position encoder
volatile unsigned int Degrees; //to accept angle in degrees for turning


/*
*
* Function Name: 	left_encoder_pin_config
* Logic: 			Set's the data direction register and enables pull up for interrupt.
*
*/

void left_encoder_pin_config (void)
{
	DDRE  = DDRE & 0xEF;  //Set the direction of the PORTE 4 pin as input
	PORTE = PORTE | 0x10; //Enable internal pull-up for PORTE 4 pin
}

/*
*
* Function Name: 	right_encoder_pin_config
* Logic: 			Set's the data direction register and enables pull up for interrupt.
*
*/

void right_encoder_pin_config (void)
{
	DDRE  = DDRE & 0xDF;  //Set the direction of the PORTE 4 pin as input
	PORTE = PORTE | 0x20; //Enable internal pull-up for PORTE 4 pin
}

/*
*
* Function Name: 	left_position_encoder_interrupt_init
* Logic: 			Set's the type of triggering and enables the external interrupt 4
*
*/


void left_position_encoder_interrupt_init (void) //Interrupt 4 enable
{
	cli(); //Clears the global interrupt
	EICRB = EICRB | 0x02; // INT4 is set to trigger with falling edge
	EIMSK = EIMSK | 0x10; // Enable Interrupt INT4 for left position encoder
	sei();   // Enables the global interrupt
}

/*
*
* Function Name: 	right_position_encoder_interrupt_init
* Logic: 			Set's the type of triggering and enables the external interrupt 5
*
*/


void right_position_encoder_interrupt_init (void) //Interrupt 5 enable
{
	cli(); //Clears the global interrupt
	EICRB = EICRB | 0x08; // INT5 is set to trigger with falling edge
	EIMSK = EIMSK | 0x20; // Enable Interrupt INT5 for right position encoder
	sei();   // Enables the global interrupt
}

/*
*
* Function Name: 	Encoder_config
* Logic: 			Calls the pin configuration and interrupt initialization routines for the left and right encoder
*
*/

void Encoder_config()
{
	left_encoder_pin_config(); //left encoder pin config
	right_encoder_pin_config(); //right encoder pin config
	cli(); //Clears the global interrupt
	left_position_encoder_interrupt_init();
	right_position_encoder_interrupt_init();
	sei();
}
  
/*
*
* Function Name: 	ISR
* Input:			INT5_vect --> Function is automatically invoked when external interrupt occurs by Right encoder.
* Logic: 			Interrupt service routine for right position encoder. When interrupted, the tick count is incremented.
*
*/


ISR(INT5_vect)
{
	ShaftcountRight++;  //increment right shaft position node_count
}

/*
*
* Function Name: 	ISR
* Input:			INT4_vect --> Function is automatically invoked when external interrupt occurs by Left encoder.
* Logic: 			Interrupt service routine for left position encoder. When interrupted, the tick count is incremented.
*
*/


ISR(INT4_vect)
{
	ShaftcountLeft++;  //increment left shaft position node_count
}

/*
*
* Function Name: 	angle_rotate
* Input:			Degrees --> Angle in degrees by which the robot should turn.
* Logic: 			Initially Required shaft count is found by dividing the degrees by the resolution of the encoder. 
*					Then its converted into an nearest integer. Now this variable (ReqdShaftcountInt) is compared with
*					ShaftcountRight and ShaftcountLeft (incremented in ISR). Once if it becomes equals to or greater than
*					that value, the control comes out of while loop.
*
*/

void angle_rotate(unsigned int Degrees)
{
	float ReqdShaftcount = 0;
	unsigned long int ReqdShaftcountInt = 0;
	ReqdShaftcount = (float) Degrees/ 4.090; // division by resolution to get shaft count
	ReqdShaftcountInt = (unsigned int) ReqdShaftcount;
	ShaftcountRight = 0;
	ShaftcountLeft = 0;
	while (1)
	{
		if((ShaftcountRight >= ReqdShaftcountInt) | (ShaftcountLeft >= ReqdShaftcountInt))
		break;
	}
	motor.stop(); //Stop robot
}

/*
*
* Function Name: 	linear_distance_mm
* Input:			DistanceInMM --> distance in millimeters by which the robot has to move straight.
* Logic: 			Initially Required shaft count is found by dividing the degrees by the resolution of the encoder.
*					Then its converted into an nearest integer. Now this variable (ReqdShaftcountInt) is compared with
*					ShaftcountRight and ShaftcountLeft (incremented in ISR). Once if it becomes equals to or greater than
*					that value, the control comes out of while loop.
*
*/


void linear_distance_mm(unsigned int DistanceInMM)
{
	float ReqdShaftcount = 0;
	unsigned long int ReqdShaftcountInt = 0;
	ReqdShaftcount = DistanceInMM / 5.338; // division by resolution to get shaft node_count
	ReqdShaftcountInt = (unsigned long int) ReqdShaftcount;
	ShaftcountRight = 0;
	while(1)
	{
		if(ShaftcountRight > ReqdShaftcountInt)
		{
			break;
		}
	}
	motor.stop(); //Stop robot
}
void forward_mm(unsigned int DistanceInMM)
{
	motor.forward();
	linear_distance_mm(DistanceInMM);
}
void back_mm(unsigned int DistanceInMM)
{
	motor.back();
	linear_distance_mm(DistanceInMM);
}
void soft_left_degrees(unsigned int Degrees)
{
	// 176 pulses for 360 degrees rotation 2.045 degrees per count
	motor.soft_left(); //Turn soft left
	Degrees=Degrees*2;
	angle_rotate(Degrees);
}
void soft_right_degrees(unsigned int Degrees)
{
	// 176 pulses for 360 degrees rotation 2.045 degrees per node_count
	motor.soft_right();  //Turn soft right
	Degrees=Degrees*2;
	angle_rotate(Degrees);
}

void soft_left_1_degrees(unsigned int Degrees)
{
	node_count_copy=node_count;
	while(1)
	{
		motor.forward();
		line_following();
		if(node_count!=node_count_copy)
		{
			motor.soft_left(); //Turn soft left
			Degrees=Degrees*2;
			angle_rotate(Degrees);
			break;
		}
	}
	
}
void soft_right_1_degrees(unsigned int Degrees)
{
	// 176 pulses for 360 degrees rotation 2.045 degrees per node_count
	node_count_copy=node_count;
	while(1)
	{
		motor.forward();
		line_following();
		if(node_count!=node_count_copy)
		{
			motor.soft_right(); //Turn soft left
			Degrees=Degrees*2;
			angle_rotate(Degrees);
			break;
		}
	}
}

void soft_left_2_degrees(unsigned int Degrees)
{
	// 176 pulses for 360 degrees rotation 2.045 degrees per node_count
	motor.soft_left_2(); //Turn reverse soft left
	Degrees=Degrees*2;
	angle_rotate(Degrees);
}
void soft_right_2_degrees(unsigned int Degrees)
{
	// 176 pulses for 360 degrees rotation 2.045 degrees per node_count
	motor.soft_right_2();  //Turn reverse soft right
	Degrees=Degrees*2;
	angle_rotate(Degrees);
}

