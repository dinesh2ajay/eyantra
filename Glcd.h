/*
*
* Team Id: 			eYRCPlus-PS1#1354
* Author List: 		Dinesh M
* Filename: 		Glcd.h
* Theme: 			Puzzle Solver 1
* Functions:		Listed as function definitions inside class. Please refer the class.
* Global Variables: None
*
*/

#define E_DELAY 		 10
#define DATAPORT 		PORTJ
#define DATADDR 		DDRJ
#define DATAPIN 		PINJ
#define chip_selec_1 (1<<PL0)	//chip select for controller 1
#define chip_selec_2 (1<<PL1)	//chip select for controller 2
#define RS  		 (1<<PD5)	//register select
#define RW  		 (1<<PD6)	//read/write
#define EN  		 (1<<PD7)	//enable
#define RST  		 (1<<PA5)	//reset
class Glcd
{
	unsigned char coordinate_y,coordinate_x;
	void port_pin_config();
	void trigger_pulse();
	void glcd_on();
	void goto_col(unsigned int );
	void goto_row(unsigned int );
	unsigned char glcd_read(unsigned char);
	void goto_xy(unsigned int ,unsigned int );
	void glcd_write(unsigned char );
	void glcd_clrln(unsigned char );
	void glcd_off();
	void draw_point(unsigned int,unsigned int,unsigned char);
	void h_line(unsigned int ,unsigned int ,unsigned char,unsigned char);
	void v_line(unsigned int ,unsigned int ,signed int ,unsigned char);
	void zero();
	void one();
	void two();
	void three();
	void four();
	void five();
	void six();
	void seven();
	void eight();
	void nine();
	void ten();
	void D();
	void E();
	void P();
	void O();
	void S();
	void I();
	void T();
	void deposit();
	void Initialize_GLCD (void);
	public:
	void glcd_clear();
	void glcd_print(unsigned char);
	Glcd();
	
	
};

void Glcd::port_pin_config(void)
{
	DATAPORT 	= 0xFF;
	DATADDR  	= 0xFF;	//initialize as output
	PORTL &= 0b11111100;
	PORTD &= 0x1F;
	DDRL |= 0x03;
	DDRD |= 0xE0;
}

void Glcd::trigger_pulse()
{
	PORTD |= EN;	//EN high
	_delay_us(E_DELAY);
	PORTD &= ~EN;	//EN low
	_delay_us(E_DELAY);
}

void Glcd::glcd_on()
{
	PORTD &= ~RS;        			//RS low --> command
	PORTD &= ~RW;        			//RW low --> write
	
	DATAPORT = 0x3F;        			//ON command


	PORTL  |=  chip_selec_1;      //select chip 1
	PORTL  &= ~chip_selec_2;
	
	
	trigger_pulse();

	PORTL  &= ~chip_selec_1;      //select chip 2
	PORTL  |=  chip_selec_2;
	
	trigger_pulse();
}

void Glcd::goto_col(unsigned int x)
{
	unsigned char pattern;
	PORTD &= ~RS;					//RS low --> command
	PORTD &= ~RW;					//RW low --> write
	coordinate_y = x;
	if(x<64)             				//left section
	{
		PORTL |= chip_selec_1;		//deselect chip 1
		PORTL &= ~chip_selec_2;		//select chip 2

		pattern = x;      				//put column address on data port
	}
	else                 				//right section
	{
		PORTL &= ~chip_selec_1;	//deselct chip 1
		PORTL |= chip_selec_2;		//select chip 2

		pattern = x-64;   				//put column address on data port
	}
	pattern = (pattern | 0x40);  		//Command format
	DATAPORT = pattern;
	trigger_pulse();
}
//-------------------------------------------------------------------------------------------------------------
void Glcd::goto_row(unsigned int y)
{
	unsigned char pattern;
	PORTD &= ~RS;	 //RS low --> command
	PORTD &= ~RW;	 //RW low --> write
	pattern = (y | 0xB8); //put row address on data port set command
	coordinate_x = y;
	DATAPORT = pattern;
	trigger_pulse();
}

unsigned char Glcd::glcd_read(unsigned char column)
{
	unsigned char read_data = 0; //Read data here
	DATADDR = 0x00;     //Input
	
	PORTD |= RW;	//Read
	PORTD |= RS;	//Data

	if(column<64)
	{
		PORTL |= chip_selec_1;		//Enable chip_selec_1
		PORTL &= ~chip_selec_2;		//Disable chip_selec_2
	}
	else
	{
		PORTL |= chip_selec_2;		//Enable chip_selec_2
		PORTL &= ~chip_selec_1;	//Disable chip_selec_1
	}
	_delay_us(1);        		//tasu
	PORTD |= EN;			//Latch RAM data into ouput register
	_delay_us(1);        		//twl + tf
	
	//Dummy read
	PORTD &= ~EN;			//Low Enable
	_delay_us(20);       		//tf + twl + chineese error
	
	PORTD |= EN;			//latch data from output register to data bus
	_delay_us(1);        		//tr + td(twh)
	
	read_data = DATAPIN;    	//Input data
	PORTD &= ~EN;			//Low Enable to remove data from the bus
	_delay_us(1);        		//tdhr
	DATADDR = 0xFF;     		//Output again
	return read_data;
}

void Glcd::goto_xy(unsigned int x,unsigned int y)
{
	goto_col(x);	//column
	goto_row(y);	//row
}
//--------------------------------------------------------------------------------------------------------------
void Glcd::glcd_write(unsigned char b)
{
	PORTD |= RS;	//RS high --> data
	PORTD &= ~RW;	//RW low --> write
	DATAPORT = b;    	//put data on data port
	_delay_us(1);
	trigger_pulse();
}
//--------------------------------------------------------------------------------------------------------------
void Glcd::glcd_clrln(unsigned char line)
{
	int i;
	goto_xy(0,line);      //At start of line of left side
	goto_xy(64,line);     //At start of line of right side

	PORTL |= chip_selec_1;

	for(i=0;i<64;i++)
	glcd_write(0);
}
//---------------------------------------------------------------------------------------------------------------

void Glcd::glcd_off()
{
	PORTD &= ~RS;					  //RS low --> command
	PORTD &= ~RW;					  //RW low --> write
	DATAPORT = 0x3E; 					  //OFF command

	PORTL  |=  chip_selec_1;        //select chip 1
	PORTL  &= ~chip_selec_2;
	trigger_pulse();

	PORTL  &= ~chip_selec_1;        //selcet chip 2
	PORTL  |=  chip_selec_2;
	
	trigger_pulse();
}

void Glcd::draw_point(unsigned int x,unsigned int y,unsigned char color)	//x = column,y = row
{
	unsigned char pattern = 0;
	goto_xy(x,(int)(y/8));  	//x is column y is row y/8 gives constant row value
	switch (color)
	{
		case 0:         //Blank spot
		pattern = ~(1<<(y%8)) & glcd_read(x);
		break;
		case 1:         //Dark spot
		pattern = (1<<(y%8)) | glcd_read(x);
		break;
	}
	goto_xy(x,(int)(y/8));
	glcd_write(pattern);
}
//------------------------------------------------------------------------------------------------------------------
void Glcd::h_line(unsigned int x,unsigned int y,unsigned char l,unsigned char c)
{
	unsigned int i;
	for(i=x; i<(l+x); i += 1)
	draw_point(i,y,c);
}
//------------------------------------------------------------------------------------------------------------------
void Glcd::v_line(unsigned int x,unsigned int y,signed int l,unsigned char c)
{
	unsigned int i;
	for(i=y; i<(y+l); i += 1)
	draw_point(x,i,c);
}
//------------------------------------------------------------------------------------------------------------------

void Glcd::Initialize_GLCD (void)
{
	cli();                    //Clears the global interrupts
	port_pin_config();	  //initialize ports
	sei();                    //Enables the global interrupts
}
void Glcd::zero()
{
	glcd_clear();
	for (unsigned char i=39;i<=48;i++){
		v_line(i,0,64,1);
		v_line(i+30,0,64,1);
	}
	for (unsigned char i=0;i<=8;i++){
		h_line(48,i,30,1);
		h_line(48,i+55,30,1);
	}
}

void Glcd::one()
{
	glcd_clear();
	for (unsigned char i=59;i<=68;i++){
		v_line(i,0,64,1);
		
	}
}
void Glcd::two()
{
	glcd_clear();
	for (unsigned char i=39;i<=48;i++){
		v_line(i,36,28,1);
		v_line(i+30,0,28,1);
	}
	for (unsigned char i=0;i<=8;i++){
		h_line(39,i,40,1);
		h_line(39,i+28,40,1);
		h_line(39,i+55,40,1);
	}

}

void Glcd::three()
{
	glcd_clear();
	for (unsigned char i=70;i<=78;i++){
		v_line(i,0,64,1);
	}
	for (unsigned char i=0;i<=8;i++){
		h_line(39,i,40,1);
		h_line(39,i+28,40,1);
		h_line(39,i+55,40,1);
	}

}

void Glcd::four()
{
	glcd_clear();
	for (unsigned char i=39;i<=48;i++){
		v_line(i,0,28,1);
		v_line(i+30,0,64,1);
	}
	for (unsigned char i=0;i<=8;i++){
		h_line(39,i+28,40,1);
		
	}

}


void Glcd::five()
{
	glcd_clear();
	for (unsigned char i=39;i<=48;i++){
		v_line(i,0,28,1);
		v_line(i+30,36,28,1);
	}
	for (unsigned char i=0;i<=8;i++){
		h_line(39,i,40,1);
		h_line(39,i+28,40,1);
		h_line(39,i+55,40,1);
	}

}

void Glcd::six()
{
	glcd_clear();
	for (unsigned char i=39;i<=48;i++){
		v_line(i,0,64,1);
		v_line(i+30,36,28,1);
	}
	for (unsigned char i=0;i<=8;i++){
		h_line(39,i,40,1);
		h_line(39,i+28,40,1);
		h_line(39,i+55,40,1);
	}

}

void Glcd::seven()
{
	glcd_clear();
	for (unsigned char i=70;i<=78;i++){
		v_line(i,0,64,1);
	}
	for (unsigned char i=0;i<=8;i++){
		h_line(39,i,40,1);
	}

}
void Glcd::eight()
{
	glcd_clear();
	for (unsigned char i=40;i<=48;i++){
		v_line(i,0,64,1);
		v_line(i+30,0,64,1);
		
	}
	for (unsigned char i=0;i<=8;i++){
		h_line(40,i,39,1);
		h_line(40,i+28,39,1);
		h_line(40,i+55,39,1);
		
	}
}

void Glcd::nine()
{
	glcd_clear();
	for (unsigned char i=39;i<=48;i++){
		v_line(i,0,28,1);
		v_line(i+30,0,64,1);
	}
	for (unsigned char i=0;i<=8;i++){
		h_line(40,i,39,1);
		h_line(40,i+28,39,1);
	}

}

void Glcd::D()
{
	glcd_clear();
	for (unsigned char i=3;i<=8;i++){
		v_line(i,20,24,1);
		v_line(i+9,20,30,1);
	}
	for (unsigned char i=20;i<=25;i++){
		h_line(0,i,12,1);
		h_line(0,i+24,12,1);
	}
}

void Glcd::E()
{
	for (unsigned char i=20;i<=25;i++){
		v_line(i,20,24,1);
	}
	for (unsigned char i=20;i<=25;i++){
		h_line(20,i,12,1);
		h_line(20,i+12,12,1);
		h_line(20,i+24,12,1);
	}

}
void Glcd::P()
{
	for (unsigned char i=36;i<=41;i++){
		v_line(i,20,30,1);
		v_line(i+12,20,18,1);
	}
	for (unsigned char i=20;i<=25;i++){
		h_line(36,i,12,1);
		h_line(36,i+12,12,1);
	}

}

void Glcd::O()
{
	for (unsigned char i=58;i<=63;i++){
		v_line(i,20,24,1);
		v_line(i+11,20,30,1);
	}
	for (unsigned char i=20;i<=25;i++){
		h_line(58,i,11,1);
		h_line(58,i+24,11,1);
	}

}

void Glcd::S()
{
	for (unsigned char i=78;i<=83;i++){
		v_line(i,20,12,1);
		v_line(i+10,32,12,1);
	}
	for (unsigned char i=20;i<=25;i++){
		h_line(78,i,16,1);
		h_line(78,i+12,10,1);
		h_line(78,i+24,16,1);
		
	}

}

void Glcd::I()
{
	for (unsigned char i=97;i<=102;i++){
		v_line(i,20,30,1);
	}
}

void Glcd::T()
{
	for (unsigned char i=113;i<=118;i++){
		v_line(i,20,30,1);
	}
	for (unsigned char i=20;i<=25;i++){
		h_line(106,i,22,1);
	}

}

void Glcd::deposit()
{
	D();
	E();
	P();
	O();
	S();
	I();
	T();
}

void Glcd::glcd_clear()
{
	int i;
	for(i=0;i<8;i++)
	glcd_clrln(i);
}

void Glcd::glcd_print(unsigned char dat)
{
	switch(dat)
	{
		case 1: { one(); break;}
		case 2: { two(); break;}
		case 3: { three(); break;}
		case 4: { four(); break;}
		case 5: { five(); break;}
		case 6: { six(); break;}
		case 7: { seven(); break;}
		case 8: { eight(); break;}
		case 9: { nine(); break;}
		case 0: { zero(); break;}
		case 10: { deposit(); break;}
		
	}
}

Glcd::Glcd()
{
	coordinate_y=0;
	coordinate_x=0;
	Initialize_GLCD();
	glcd_on();
}