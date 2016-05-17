/*
*
* Team Id: 			eYRCPlus-PS1#1354
* Author List: 		Dinesh M
* Filename: 		Line_following.h
* Theme: 			Puzzle Solver 1
* Functions:		line_following()
* Global Variables: error, setpoint, correction, Proportional_constant, node_count, node_count_copy, flag, average, sum, position
* Global objects:	motor,white_line_sensor
*
*/


Motor motor; // Instance for motor class is created to set the velocity of motors.
White_line_sensor white_line_sensor; // Instance for white_line_sensor is created to read the 3 sensor values.
int error=0,set_point=96,correction=0; 
unsigned char Proportional_constant=2; // Proportional_constant is found experimentally by observing the Robot's performance for different values.
unsigned char node_count=0,flag=0; // node_count to count the nodes in the arena while traversing
unsigned int average=0,sum=0,position=0;


/*
*
* Function Name: 	line_following
* Logic: 			     First, the function reads the white line sensor values. With that it computes the current position by taking the average
*					and dividing it by weighted sum of sensor values. Now, error in position is found by subtracting the set_point from the
*					current position. Now, the correction in velocity has to be made if error exists. Negative values of correction means that
*					bot has moved left, so the right motor velocity is decreased by the correction value. If correction value is positive
*					then it means bot has moved right of the line and left motor velocity is decreased by correction value.
*					By this proportional control, the bot will continue to follow the black line.
*					
*					In addition to following the line, this function also counts the nodes when all sensor values goes as black.
*
*/

void line_following(void)
{
	// Get the sensor data from Left, Center and Right white line sensors by specifying the analog Channel number
	
	white_line_sensor.Left = white_line_sensor.ADC_Conversion(3);
	white_line_sensor.Center = white_line_sensor.ADC_Conversion(2);
	white_line_sensor.Right = white_line_sensor.ADC_Conversion(1);
	sum=0;
	average=0;
	
	// average = (Right_sensor_value * 0 * 100) + (Center_sensor_value * 1 * 100) + (Left_sensor_value * 2 * 100)
	// sum = Right_sensor_value + Center_sensor_line + Left_sensor_value
	// position = average / sum
	
	for (unsigned char iteration=0;iteration<3;iteration++)
	{
		average+=white_line_sensor.ADC_Conversion(iteration+1)*iteration*100;
		sum+=white_line_sensor.ADC_Conversion(iteration+1);
		position=(average/sum);
	}
	
	// If robot is on the black line, position is measured as 96. So this will be set_point. If robot moves left or right, this position 
	// value will vary accordingly. The variation in position from the set_point is measured as error
	
	error=position-set_point;
	
	// Now correction in velocity has to be made. So, error is multiplied with Proportional constant to find the correction in velocity.
	correction=error*Proportional_constant;
	
	// A threshold has to be fixed or else correction values might go beyond limits. So if correction value is less than -127 its set as -127.
	// If its more than 127 its set as 127. So range of correction in velocity will be -127 to +127
	if(correction < -127 )	correction=-127;
	if(correction > 127 )	correction=127;
	
	/*
	      If correction value is negative, it means that robot has moved left of the black line. To bring back to the set position,
	robot has to make a right turn. So left motor velocity is maximum i.e 200 and right motor velocity is decreased by correction value.
	Ex: If correction is -40, then velocity will be set as velocity(200,160). Now robot takes right turn.
	*/
	if(correction <0)	motor.velocity(200,200+correction);
	
	/*
	     If correction is positive, it means robot has moved out of line to the right. So now left motor velocity is decreased, so as the bot
	will take left. For ex: correction = 30. then its velocity(170,200) 
	*/
	else motor.velocity(200-correction,200);
	
	/*
	Now the bot is able to follow the line. If all sensor reads black, then it's considered as node and count is incremented.
	Meanwhile,  once if node is counted, the same node shouldn't be counted once again. Inorder to avoid this, flag is SET once 
	if a node is counted.
	*/
	if(white_line_sensor.Left >40 && white_line_sensor.Right >40 && white_line_sensor.Center>40) 
	{
		if(flag==0)
		{
			node_count+=1;
			flag=1;
		}
	}
	
	// To continue counting the next node, the flag has to be RESET. For this, when at least one sensor read white, flag is made to RESET.
	// So if once again all sensors reads black, its counted as node and the process goes on.
	
	if (white_line_sensor.Left <40 || white_line_sensor.Right <40 || white_line_sensor.Center<40)
	{
		flag=0;
	}	
}	
