/*
*
* Team Id: 		eYRCPlus-PS1#1354
* Author List: 	e-Yantra Tutorial code with slight modifications by Dinesh M
* Filename: 	White_line_sensor.h
* Theme: 		Puzzle Solver 1
* Functions:	adc_pin_config(), adc_init(), ADC_Conversion(unsigned char), White_line_sensor()
*
*/

#include <math.h> //included to support power function

class White_line_sensor
{
	private:
	void adc_pin_config (void); // Configures the respective GPIO as inputs
	void adc_init(); // Initializes the ADC by setting Vref
	public:
	White_line_sensor(); // Constructor to call pin_config() and init() functions
	unsigned char ADC_Conversion(unsigned char Ch); // Converts the analog voltage to digital value
	unsigned char Left,Center,Right; // Converted digital values of Left, Center and Right white lines.
	
};

/*
*
* Function Name: 	adc_pin_config
* Logic: 			Function used to configure ADC by setting data direction as input and make ports floating.
*
*/

void White_line_sensor::adc_pin_config (void)
{
	DDRF = 0x00; //set PORTF direction as input
	PORTF = 0x00; //set PORTF pins floating
	DDRK = 0x00; //set PORTK direction as input
	PORTK = 0x00; //set PORTK pins floating
}

/*
*
* Function Name: 	adc_init
* Logic: 			Function used to initialize ADC by setting Vref
*
*/

void White_line_sensor::adc_init()
{
	ADCSRA = 0x00;
	ADCSRB = 0x00;		//MUX5 = 0
	ADMUX = 0x20;		//Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
	ACSR = 0x80;
	ADCSRA = 0x86;		//ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0
}

/*
*
* Function Name: 	ADC_Conversion
* Input:			Channel --> Channel number : 1 - Right sensor, 2 - Center sensor, 3 - Left sensor
* Output:			ADC_value : 8 bit digital value ranging from 0 to 255
* Logic: 			Takes the sensor input, wait for conversion to complete and returns the converted digital value
* Example call:		Whitle_line_sensor.ADC_Conversion(1) --> returns a value from 0 to 255 depends on voltage read from Right sensor. Here
*					Whitle_line_sensor is an instance for White_line_sensor class
*
*/

unsigned char White_line_sensor::ADC_Conversion(unsigned char Channel)
{
	unsigned char ADC_value;
	if(Channel>7)
	{
		ADCSRB = 0x08;
	}
	Channel = Channel & 0x07;
	ADMUX= 0x20| Channel;
	ADCSRA = ADCSRA | 0x40;		//Set start conversion bit
	while((ADCSRA&0x10)==0);	//Wait for ADC conversion to complete
	ADC_value=ADCH;
	ADCSRA = ADCSRA|0x10; //clear ADIF (ADC Interrupt Flag) by writing 1 to it
	ADCSRB = 0x00;
	return ADC_value;
}

/*
*
* Function Name: 	White_line_sensor
* Logic: 			When instance of Whitle_line_sensor is created, this is called and this automatically configures and initializes ADC.
*
*/

White_line_sensor::White_line_sensor()
{
	cli(); //Clears the global interrupts
	adc_pin_config();
	adc_init();
	sei(); //Enables the global interrupts
	Left=0;
	Center=0;
	Right=0;

}
