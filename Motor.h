/*
*
* Team Id: 		eYRCPlus-PS1#1354
* Author List: 	Dinesh M,
* Filename: 	Motor.h
* Theme: 		Puzzle Solver 1
* Functions:	motion_pin_config(), timer5_init(), motion_set(unsigned char), velocity(unsigned char,unsigned char)
*				forward(), back(), soft_left(), soft_left_2(), soft_right(), soft_right_2(), stop()
* Global Variables:	None
*
*/

unsigned char node_count_copy=0;

class Motor
{
	public:
	Motor(); // Constructor to initialize ports
	private:
	void motion_pin_config (void);
	void timer5_init();
	void motion_set (unsigned char);
	public:
	void velocity (unsigned char, unsigned char);
	void forward (void); //both wheels forward
	void back (void); //both wheels backward
	void soft_left (void); //Left wheel stationary, Right wheel forward
	void soft_right (void); //Left wheel forward, Right wheel is stationary
	void soft_left_2 (void); //Left wheel backward, right wheel stationary
	void soft_right_2 (void); //Left wheel stationary, Right wheel backward
	void stop (void); //both wheels stop
};

/*
*
* Function Name: 	Motor
* Logic: 			When an instance of Motor class is created, this constructor will configure the motor pins, initializes
*					the timer and enables the interrupts.
* Example Call:		Motor motor; Here motor is the object created which automatically calls the constructor.
*
*/
Motor::Motor()
{
	cli(); //disable all interrupts
	motion_pin_config(); 
	timer5_init();
	sei(); //re-enable interrupts
}

/*
*
* Function Name: 	motion_pin_config
* Logic: 			Function to configure ports to enable robot's motion
*
*/

void Motor::motion_pin_config (void)
{
	DDRA = DDRA | 0x0F;
	PORTA = PORTA & 0xF0;
	DDRL = DDRL | 0x18;   //Setting PL3 and PL4 pins as output for PWM generation
	PORTL = PORTL | 0x18; //PL3 and PL4 pins are for velocity control using PWmotor.
}

// Timer 5 initialized in PWM mode for velocity control
// Prescalar:256
// PWM 8bit fast, TOP=0x00FF
// Timer Frequency:225.000Hz

/*
*
* Function Name: 	timer5_init
* Logic: 			Initializes the timer 5 of ATmega2560.
*
*/

void Motor::timer5_init()
{
	TCCR5B = 0x00;	//Stop
	TCNT5H = 0xFF;	//node_counter higher 8-bit value to which OCR5xH value is compared with
	TCNT5L = 0x01;	//node_counter lower 8-bit value to which OCR5xH value is compared with
	OCR5AH = 0x00;	//Output compare register high value for Left Motor
	OCR5AL = 0xFF;	//Output compare register low value for Left Motor
	OCR5BH = 0x00;	//Output compare register high value for Right Motor
	OCR5BL = 0xFF;	//Output compare register low value for Right Motor
	OCR5CH = 0x00;	//Output compare register high value for Motor C1
	OCR5CL = 0xFF;	//Output compare register low value for Motor C1
	TCCR5A = 0xA9;	/*{COM5A1=1, COM5A0=0; COM5B1=1, COM5B0=0; COM5C1=1 COM5C0=0}
 						For Overriding normal port functionality to OCRnA outputs.
				  		{WGM51=0, WGM50=1} Along With WGM52 in Tnode_count_copyR5B for Selecting FAST PWM 8-bit Mode*/
	
	TCCR5B = 0x0B;	//WGM12=1; CS12=0, CS11=1, CS10=1 (Prescaler=64)
}

/*
*
* Function Name: 	motion_set
* Input:			Direction
* Logic: 			Function used for setting motor's direction
* Example call:		motion_set(0x06) -> sets the motor to run in forward direction
*
*/

void Motor::motion_set (unsigned char Direction)
{
	unsigned char PortARestore = 0;

	Direction &= 0x0F; 			// removing upper nibble as it is not needed
	PortARestore = PORTA; 		// reading the PORTA's original status
	PortARestore &= 0xF0; 		// setting lower direction nibble to 0
	PortARestore |= Direction; 	// adding lower nibble for direction command and restoring the PORTA status
	PORTA = PortARestore; 		// setting the command to the port
}

/*
*
* Function Name: 	velocity
* Input:			left_motor, right_motor --> velocity valeus for left and right motor. Range is between 0 to 255
* Logic: 			Function used to control the velocity of motor
* Example call:		motor.velocity(255,255) with motor being instance of Motor class
*
*/

void Motor::velocity (unsigned char left_motor, unsigned char right_motor)
{
	OCR5AL = (unsigned char)left_motor;
	OCR5BL = (unsigned char)right_motor;
}

/*
*
* Function Name: 	forward
* Logic: 			Function used to set the motion of robot as forward. Both wheels run in forward direction.
*
*/
	
void Motor:: forward (void) //both wheels forward
{
	motion_set(0x06);
}

/*
*
* Function Name: 	back
* Logic: 			Function used to set the motion of robot as backwards. Both wheels run in backward direction.
*
*/

void Motor::back (void) //both wheels backward
{
	motion_set(0x09);
}

/*
*
* Function Name: 	soft_left
* Logic: 			Function used to set the motion of robot as soft left. Left motor is stopped and right motor is run forward
*
*/

void Motor::soft_left (void) //Left wheel stationary, Right wheel forward
{
	motion_set(0x04);
}

/*
*
* Function Name: 	soft_right
* Logic: 			Function used to set the motion of robot as soft right. Right motor is stopped and left motor is run forward.
*
*/

void Motor::soft_right (void) //Left wheel forward, Right wheel is stationary
{
	motion_set(0x02);
}

/*
*
* Function Name: 	soft_left_2
* Logic: 			Function used to set the motion of robot as soft left 2. Right motor is stopped and left motor runs backward.
*
*/

void Motor::soft_left_2 (void) //Left wheel backward, right wheel stationary
{
	motion_set(0x01);
}

/*
*
* Function Name: 	soft_right_2
* Logic: 			Function used to set the motion of robot as soft right 2. Left motor is stopped and right motor runs backward.
*
*/

void Motor::soft_right_2 (void)
{
	motion_set(0x08);
}

/*
*
* Function Name: 	stop
* Logic: 			Function used to stop both the motors.
*
*/

void Motor::stop (void)
{
	motion_set(0x00);
}

