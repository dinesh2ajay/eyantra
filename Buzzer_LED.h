/*
*
* Team Id: 		eYRCPlus-PS1#1354
* Author List: 	Nivetha C
* Filename:		Buzzer_LED.h
* Theme: 		Puzzle Solver 1
* Functions: 	buzzerled(), right_led_on(), left_led_on(), led_off(), buzzer_on(), buzzer_off()
*
*/

class buzzer_led
{
	public:
	buzzer_led();
	void left_led_on();
	void right_led_on();
	void led_off();
	void buzzer_on();
	void buzzer_off();
};

/*
*
* Function Name: 	buzzer_led
* Logic: 			When instance of buzzer_led class is created, this is called and this configures the ports as output.
*
*/

buzzer_led::buzzer_led()
{
	DDRL |= (1<<6); // Left LED is connected to PL6 --> Pin 18 on expansion slot. PL6 is set as output
	DDRG |= (1<<1); // Right LED is connected to PG1 --> Pin 31 on expansion slot PG1 is set as output
	cli(); //Clears the global interrupts
	DDRC = DDRC | 0x08;		//Setting PORTC 3 as output
	PORTC = PORTC & 0xF7;	//Setting PORTC 3 logic low to turnoff buzzer
	sei(); //Enables the global interrupts
}

/*
*
* Function Name: 	right_led_on
* Logic: 			Function used to turn ON right LED connected to PG1.
* Example call:		buzzer_led.right_led_on() with buzzer_led being instance of buzzer_led class.
*
*/

void buzzer_led::right_led_on(void)
{
	PORTG |= (1<<1);
	PORTL &= ~(1<<6);
}

/*
*
* Function Name: 	left_led_on
* Logic: 			Function used to turn ON left LED connected to PL6.
* Example call:		buzzer_led.left_led_on() with buzzer_led being instance of buzzer_led class.
*
*/

void buzzer_led::left_led_on(void)
{
	PORTL |= (1<<6);
	PORTG &= ~(1<<1);
}

/*
*
* Function Name: 	led_off
* Logic: 			Function used to turn OFF both right and left LED's.
* Example call:		buzzer_led.led_off() with buzzer_led being instance of buzzer_led class.
*
*/

void buzzer_led::led_off(void)
{
	PORTG &= ~(1<<1);
	PORTL &= ~(1<<6);
}

/*
*
* Function Name: 	buzzer_on
* Logic: 			Function used to turn ON buzzer.
* Example call:		buzzer_led.buzzer_on() with buzzer_led being instance of buzzer_led class.
*
*/

void buzzer_led::buzzer_on (void)
{
	unsigned char port_restore = 0;
	port_restore = PINC;
	port_restore = port_restore | 0x08;
	PORTC = port_restore;
}

/*
*
* Function Name: 	buzzer_off
* Logic: 			Function used to turn OFF buzzer.
* Example call:		buzzer_led.buzzer_off() with buzzer_led being instance of buzzer_led class.
*
*/

void buzzer_led::buzzer_off (void)
{
	unsigned char port_restore = 0;
	port_restore = PINC;
	port_restore = port_restore & 0xF7;
	PORTC = port_restore;
}
