#include "ee.h"
#include "ee_irq.h"
#include "sensors.h"
#include "algebra.h"

// Primary (XT, HS, EC) Oscillator without PLL
_FOSCSEL(FNOSC_PRI);
// OSC2 Pin Function: OSC2 is Clock Output - Primary Oscillator Mode: XT Crystal
_FOSC(OSCIOFNC_ON & POSCMD_XT);
// Watchdog Timer Enabled/disabled by user software
_FWDT(FWDTEN_OFF);
// Disable Code Protection
_FGS(GCP_OFF);

unsigned char radiationH = 0;
unsigned char radiationL = 0;
unsigned char hum_int = 0;
unsigned char hum_dec = 0;
unsigned char temp_int = 0;
unsigned char temp_dec = 0;
unsigned char wind_int = 0;
unsigned char wind_dec = 0;

 float hum_con=0;
 float hum_ant=35;

unsigned char top_sensor_reading_units_A=0;
unsigned char top_sensor_reading_decimals_A=0;
unsigned char middle_sensor_reading_units_A=0;
unsigned char middle_sensor_reading_decimals_A=0;
unsigned char bottom_sensor_reading_units_A=0;
unsigned char bottom_sensor_reading_decimals_A=0;

unsigned char top_sensor_reading_units_B=0;
unsigned char top_sensor_reading_decimals_B=0;
unsigned char middle_sensor_reading_units_B=0;
unsigned char middle_sensor_reading_decimals_B=0;
unsigned char bottom_sensor_reading_units_B=0;
unsigned char bottom_sensor_reading_decimals_B=0;

unsigned char top_sensor_reading_units_C=0;
unsigned char top_sensor_reading_decimals_C=0;
unsigned char middle_sensor_reading_units_C=0;
unsigned char middle_sensor_reading_decimals_C=0;
unsigned char bottom_sensor_reading_units_C=0;
unsigned char bottom_sensor_reading_decimals_C=0;

unsigned char top_sensor_reading_units_D=0;
unsigned char top_sensor_reading_decimals_D=0;
unsigned char middle_sensor_reading_units_D=0;
unsigned char middle_sensor_reading_decimals_D=0;
unsigned char bottom_sensor_reading_units_D=0;
unsigned char bottom_sensor_reading_decimals_D=0;

unsigned char status_relay=0; // is the relay activated?
unsigned char status_relay_confirmation=0;
unsigned char temperature = 0;
unsigned char pulse_cnt=0;
unsigned char battery_level = 0;


unsigned char sensor_signal=0;

unsigned char height_id=1;
volatile int comm_timeout=0;
volatile int error_comm_A;
volatile int error_comm_B;
volatile int error_comm_C;
volatile int error_comm_D;
volatile int error_comm_F;

#define top_sensor_id 1
#define middle_sensor_id 2
#define bottom_sensor_id 3
#define area_id 1

 unsigned char BufferIn[80];
 unsigned char BufferOut[80];

 unsigned char led=0;

 unsigned int System_mode = 1;  // 1:Manual 2:Intelligent 3:Timer


 void reset_buffers (void){

 	int a=0;

 	    for ( a=0;a<80;a++){
 	    	BufferIn[a]=0;
 	    	BufferOut[a]=0;
 	    }
 	    top_sensor_reading_units_A=0;
 	    top_sensor_reading_decimals_A=0;
 	    middle_sensor_reading_units_A=0;
 	    middle_sensor_reading_decimals_A=0;
 	    bottom_sensor_reading_units_A=0;
 	    bottom_sensor_reading_decimals_A=0;

 	   top_sensor_reading_units_B=0;
 	   top_sensor_reading_decimals_B=0;
 	   middle_sensor_reading_units_B=0;
 	   middle_sensor_reading_decimals_B=0;
 	   bottom_sensor_reading_units_B=0;
 	   bottom_sensor_reading_decimals_B=0;

 	   top_sensor_reading_units_C=0;
 	   top_sensor_reading_decimals_C=0;
 	   middle_sensor_reading_units_C=0;
 	   middle_sensor_reading_decimals_C=0;
 	   bottom_sensor_reading_units_C=0;
 	   bottom_sensor_reading_decimals_C=0;

 	  top_sensor_reading_units_D=0;
 	  top_sensor_reading_decimals_D=0;
 	  middle_sensor_reading_units_D=0;
 	  middle_sensor_reading_decimals_D=0;
      bottom_sensor_reading_units_D=0;
      bottom_sensor_reading_decimals_D=0;

 	    error_comm_B=0;
 	    sensor_signal=0;
 	    temperature=0;
 }
 char read_sensor_frame(unsigned char ch[80], unsigned char len){


 	unsigned char i;
 	comm_timeout=0;


 	for(i=0; ((i<len)&&(comm_timeout==0)); i++){

 		if(i>0 && !(ch[0]==0x7E)) // Check for instruction header
 		{
 			i=0;
 		}

 		SetRelAlarm(AlarmTimeOut, 16, 0); // 2.112S timeout
 		while ((!IFS0bits.U1RXIF) && (comm_timeout==0));
 		Serial_Receive(&ch[i]);
 		IFS0bits.U1RXIF = 0;


 		if(comm_timeout==0){

 			CancelAlarm(AlarmTimeOut);
 		}
 	}



  return comm_timeout;
 }
 TASK(Task_TimeOut){

 	comm_timeout=1;

 }

/* Program the Timer1 peripheral to raise interrupts */
void T1_program(void)
{
	T1CON = 0;		/* Stops the Timer1 and reset control register	*/
	TMR1  = 0;		/* Clear contents of the timer register	*/
	PR1   = 0xffff;		/* Load the Period register wit the value 0xffff	*/
	IPC0bits.T1IP = 5;	/* Set Timer1 priority to 1		*/
	IFS0bits.T1IF = 0;	/* Clear the Timer1 interrupt status flag	*/
	IEC0bits.T1IE = 1;	/* Enable Timer1 interrupts		*/
	T1CONbits.TON = 1;	/* Start Timer1 with pre-scaler settings at 1:1

				* and clock source set to the internal
				* instruction cycle			*/
	T1CONbits.TCKPS=1; // Pre-scaler 0:1  1 8 64 256
}

/* Clear the Timer1 interrupt status flag */
void T1_clear(void)
{
	IFS0bits.T1IF = 0;
}

/* This is an ISR Type 2 which is attached to the Timer 1 peripheral IRQ pin
 * The ISR simply calls CounterTick to implement the timing reference
 */
ISR2(_T1Interrupt)
{
	/* clear the interrupt source */
	T1_clear();

	/* count the interrupts, waking up expired alarms */
	CounterTick(myCounter);
}

TASK(TaskLCD)
{
	/*led++;
	EE_leds(led);*/
	switch(System_mode)
		{

		case 1:
			LCD_manual();
			break;

		case 2:
			LCD_intelligent();
			break;

		case 3:
			LCD_timer();
			break;
		}

}

void my_button(void)
{


	if (EE_button_get_S3() == 1){


		if (System_mode<3){
			System_mode++;
		}
		else{
			System_mode = 1;
		}
		EE_leds(0b00001111);

	}
	if ((EE_button_get_S6() == 1)&&(System_mode==1)){

		status_relay = 1;
		EE_leds(0b11110000);

		}
	if (EE_button_get_S5() == 1){

		status_relay = 0;
		EE_leds(0b10101010);

		}
	if (EE_button_get_S4() == 1){

		EE_leds(0b01010101);

		}



	switch(System_mode)
	{

		case 1:
			LCD_manual();
			break;

		case 2:
			LCD_intelligent();
			break;

		case 3:
			LCD_timer();
			break;
	}

	Delay(Delay_10mS_Cnt);
}

int main(void)
{
	/* Clock setup for 40MIPS */
		CLKDIVbits.DOZEN = 0;
		CLKDIVbits.PLLPRE = 0;
		CLKDIVbits.PLLPOST = 0;
		PLLFBDbits.PLLDIV = 78;
	/* Program Timer 1 to raise interrupts */
	T1_program();

	/* Init devices */
	EE_buttons_init(&my_button, 0xFF);

	/* Init leds */
	EE_leds_init();

	/* Init LCD */
	EE_lcd_init();
	EE_lcd_clear();
    LCD_manual();
	/* Init analog sensors */
	EE_analog_init();
	Serial_Init();

	/* Program cyclic alarms which will fire after an initial offset, and after that periodically */

			//Timeout=2.112sec    Offset=2.64sec   Period: 59.928sec
			 SetRelAlarm(AlarmA, 1, 454);   	// Sensor 01
			 SetRelAlarm(AlarmB, 21, 454);		// Sensor 02
			 SetRelAlarm(AlarmC, 41, 454);		// Sensor 03
			 SetRelAlarm(AlarmD, 61, 454);		// Weather
			 SetRelAlarm(AlarmE, 81, 454);		// Control algorithm
			 SetRelAlarm(AlarmF, 101, 454);		// Actuator
			 SetRelAlarm(AlarmSend, 121,454); 	// Monitor
			 SetRelAlarm(AlarmLCD, 141, 454);	// Update LCD

	 /* Forever loop: background activities (if any) should go here */
	for (;;);

	return 0;
}

TASK(TaskA)
{
	EE_leds(0b11111111);
	Delay(Delay_2mS_Cnt);

	led=0;


    //----------------------- Erasing buffers
     reset_buffers();
    //----------------------------------------//


    //------------- Transmit a message to the sensor node 1 -----------------------
     			BufferOut[0]=0x7E;//API mode start delimeter
     			BufferOut[1]=0x00;//Data length MSB
     			BufferOut[2]=0x12;//Data length LSB
     			BufferOut[3]=0x10;//API ID TX Request 64 bit address
     			BufferOut[4]=0x00;//Frame ID, if equals to 0x00 disable response frame
     			BufferOut[5]=0x00;//Destination Address MSB
     			BufferOut[6]=0x13;//Destination Address
     			BufferOut[7]=0xA2;//Destination Address
     			BufferOut[8]=0x00;//Destination Address
     			BufferOut[9]=0x40;//Destination Address
     			BufferOut[10]=0xA6;//Destination Address
     			BufferOut[11]=0xDA;//Destination Address
     			BufferOut[12]=0xAD;//Destination Address LSB --- Rudo's xbee address 0x50
     			BufferOut[13]=0xFF;//RESERVED
     			BufferOut[14]=0xFE;//RESERVED
     			BufferOut[15]=0x00;//Max number of hops, if set to zero max number of hops
     			BufferOut[16]=0x00;//disable ACK, disable route discovery
     			BufferOut[17]='S';//Data 1
     			BufferOut[18]='E';//Data 2
     			BufferOut[19]='N';//Data 3
     			BufferOut[20]=0x01;//Sensor ID 1-19
     			BufferOut[21]=0x00;//Checksum
     			Serial_Send_Frame(BufferOut,22);

     			Delay(Delay_1mS_Cnt);
     			error_comm_A = read_sensor_frame(BufferIn,24);


     			if ((!error_comm_A) && (BufferIn[3]==0x90))//&& (BufferIn[11]==0x50))
     				{
     				top_sensor_reading_units_A = BufferIn[16];
     				top_sensor_reading_decimals_A = BufferIn[17];

     				middle_sensor_reading_units_A = BufferIn[18];
     				middle_sensor_reading_decimals_A = BufferIn[19];

     				bottom_sensor_reading_units_A = BufferIn[20];
     				bottom_sensor_reading_decimals_A = BufferIn[21];

     				temperature = BufferIn[22];  // MSB-> Temperature sign
     				battery_level = BufferIn[23]; // 0-255


     			}



     			EE_leds(0b00000001);


}

TASK(TaskB)
{

	//------------- Transmit a message to the sensor node 4 -----------------------
						BufferOut[0]=0x7E;//API mode start delimeter
						BufferOut[1]=0x00;//Data length MSB
						BufferOut[2]=0x12;//Data length LSB
						BufferOut[3]=0x10;//API ID TX Request 64 bit address
						BufferOut[4]=0x00;//Frame ID, if equals to 0x00 disable response frame
						BufferOut[5]=0x00;//Destination Address MSB
						BufferOut[6]=0x13;//Destination Address
						BufferOut[7]=0xA2;//Destination Address
						BufferOut[8]=0x00;//Destination Address
						BufferOut[9]=0x40;//Destination Address
						BufferOut[10]=0xA6;//Destination Address
						BufferOut[11]=0xDA;//Destination Address
						BufferOut[12]=0x55;//Destination Address LSB --- Rudo's xbee address 0x50
						BufferOut[13]=0xFF;//RESERVED
						BufferOut[14]=0xFE;//RESERVED
						BufferOut[15]=0x00;//Max number of hops, if set to zero max number of hops
						BufferOut[16]=0x00;//disable ACK, disable route discovery
						BufferOut[17]='S';//Data 1
						BufferOut[18]='E';//Data 2
						BufferOut[19]='N';//Data 3
						BufferOut[20]=0x02;//Sensor ID 1-19
						BufferOut[21]=0x00;//Checksum
						Serial_Send_Frame(BufferOut,22);

						Delay(Delay_1mS_Cnt);
						error_comm_B = read_sensor_frame(BufferIn,24);


						if ((!error_comm_B) && (BufferIn[3]==0x90))//&& (BufferIn[11]==0x50))
							{
							top_sensor_reading_units_B = BufferIn[16];
							top_sensor_reading_decimals_B = BufferIn[17];

							middle_sensor_reading_units_B = BufferIn[18];
							middle_sensor_reading_decimals_B = BufferIn[19];

							bottom_sensor_reading_units_B = BufferIn[20];
							bottom_sensor_reading_decimals_B = BufferIn[21];

							temperature = BufferIn[22];  // MSB-> Temperature sign
							battery_level = BufferIn[23]; // 0-255


						}

			EE_leds(0b00000010);

}

TASK(TaskC)
{
   //////////////////////////////////////////////////////////////
  /* //------------- Transmit a message to the sensor node 3 -----------------------
				BufferOut[0]=0x7E;//API mode start delimeter
				BufferOut[1]=0x00;//Data length MSB
				BufferOut[2]=0x12;//Data length LSB
				BufferOut[3]=0x10;//API ID TX Request 64 bit address
				BufferOut[4]=0x00;//Frame ID, if equals to 0x00 disable response frame
				BufferOut[5]=0x00;//Destination Address MSB
				BufferOut[6]=0x13;//Destination Address
				BufferOut[7]=0xA2;//Destination Address
				BufferOut[8]=0x00;//Destination Address
				BufferOut[9]=0x40;//Destination Address
				BufferOut[10]=0xA6;//Destination Address
				BufferOut[11]=0xDA;//Destination Address
				BufferOut[12]=0xA6;//Destination Address LSB --- Rudo's xbee address 0xA6
				BufferOut[13]=0xFF;//RESERVED
				BufferOut[14]=0xFE;//RESERVED
				BufferOut[15]=0x00;//Max number of hops, if set to zero max number of hops
				BufferOut[16]=0x00;//disable ACK, disable route discovery
				BufferOut[17]='S';//Data 1
				BufferOut[18]='E';//Data 2
				BufferOut[19]='N';//Data 3
				BufferOut[20]=0x01;//Sensor ID 1-19
				BufferOut[21]=0x00;//Checksum
				Serial_Send_Frame(BufferOut,22);

				Delay(Delay_1mS_Cnt);
				error_comm_C = read_sensor_frame(BufferIn,24);


			if ((!error_comm_C) && (BufferIn[3]==0x90))//&& (BufferIn[11]==0x50))
		{

					top_sensor_reading_units_C = BufferIn[16];
					top_sensor_reading_decimals_C = BufferIn[17];

					middle_sensor_reading_units_C = BufferIn[18];
					middle_sensor_reading_decimals_C = BufferIn[19];

					bottom_sensor_reading_units_C = BufferIn[20];
					bottom_sensor_reading_decimals_C = BufferIn[21];

					temperature = BufferIn[22];  // MSB-> Temperature sign
					battery_level = BufferIn[23]; // 0-255

					//sensor_signal = BufferIn[18]; // not part of digimesh protocol

				}
*/

			EE_leds(0b00000100);

  /////////////////////////////////////////////////////////////

}

TASK(TaskD)
{
	 //////////////////////////////////////////////////////////////

	//------------- Transmit a message to the climate node -----------------------
					BufferOut[0]=0x7E;//API mode start delimeter
					BufferOut[1]=0x00;//Data length MSB
					BufferOut[2]=0x0F;//Data length LSB
					BufferOut[3]=0x10;//API ID TX Request 64 bit address
					BufferOut[4]=0x00;//Frame ID, if equals to 0x00 disable response frame
					BufferOut[5]=0x00;//Destination Address MSB
					BufferOut[6]=0x13;//Destination Address
					BufferOut[7]=0xA2;//Destination Address
					BufferOut[8]=0x00;//Destination Address
					BufferOut[9]=0x40;//Destination Address
					BufferOut[10]=0xA8;//Destination Address
					BufferOut[11]=0x50;//Destination Address
					BufferOut[12]=0xC1;//Destination Address LSB --- Rudo's xbee address 0x50
					BufferOut[13]=0xFF;//RESERVED
					BufferOut[14]=0xFE;//RESERVED
					BufferOut[15]=0x00;//Max number of hops, if set to zero max number of hops
					BufferOut[16]=0x00;//disable ACK, disable route discovery
					BufferOut[17]='C';//Data 1
					BufferOut[18]=0x00;//Checksum
					Serial_Send_Frame(BufferOut,19);

					Delay(Delay_1mS_Cnt);
					error_comm_D = read_sensor_frame(BufferIn,24);


					if ((!error_comm_D) && (BufferIn[3]==0x90))//&& (BufferIn[11]==0x50))
						{
						radiationH = BufferIn[16];
						radiationL = BufferIn[17];

						hum_int = BufferIn[18];
						hum_dec = BufferIn[19];

						temp_int = BufferIn[20];
						temp_dec = BufferIn[21];

						wind_int = BufferIn[22];  // MSB-> Temperature sign
						wind_dec = BufferIn[23]; // 0-255

					}


					EE_leds(0b00001000);



	  /////////////////////////////////////////////////////////////



}
TASK(TaskE)
{

	char hum;
	static int counter = 0;


	//////////////////////////////////////////////////////
	if(!error_comm_A && !error_comm_B){
	#define alpha 0.1

	hum_con=((top_sensor_reading_units_A*alpha)+(1-alpha)*hum_ant);
	hum_ant=hum_con;
	hum_con=((middle_sensor_reading_units_A*alpha)+(1-alpha)*hum_ant);
	hum_ant=hum_con;
	hum_con=((bottom_sensor_reading_units_A*alpha)+(1-alpha)*hum_ant);
	hum_ant=hum_con;
	hum_con=((top_sensor_reading_units_B*alpha)+(1-alpha)*hum_ant);
	hum_ant=hum_con;
	hum_con=((middle_sensor_reading_units_B*alpha)+(1-alpha)*hum_ant);
	hum_ant=hum_con;
	hum_con=((bottom_sensor_reading_units_B*alpha)+(1-alpha)*hum_ant);
	hum_ant=hum_con;
	}
	/////////////////////////////////////////////////////



	if (System_mode == 2)
	{

		hum = hum_con;


		//Top sensor's average
		//hum = (top_sensor_reading_units_A + top_sensor_reading_units_B + top_sensor_reading_units_C + top_sensor_reading_units_D)/4;

		// All sensors average
		//hum = ((((top_sensor_reading_units_A + middle_sensor_reading_units_A + bottom_sensor_reading_units_A)/3) + ((top_sensor_reading_units_B + middle_sensor_reading_units_B + bottom_sensor_reading_units_B)/3) + ((top_sensor_reading_units_C + middle_sensor_reading_units_C + bottom_sensor_reading_units_C)/3) + ((top_sensor_reading_units_D + middle_sensor_reading_units_D + bottom_sensor_reading_units_D)/3))/4);

		if(!error_comm_A && !error_comm_B && hum< 32)
		{
			counter++;
			if (counter > 4)
			{
				status_relay=1;
				counter = 0;
			}
		}
		if(!error_comm_A && !error_comm_B && hum>=35)
		{
			counter++;
			if (counter > 4)
			{
				status_relay=0;
				counter=0;
			}
		}


	}

	EE_leds(0b00010000);
}

TASK(TaskF)
{

	//------------- Transmit a message to the actuator node  -----------------------

					BufferOut[0]=0x7E;//API mode start delimiter
					BufferOut[1]=0x00;//Data length MSB
					BufferOut[2]=0x12;//Data length LSB
					BufferOut[3]=0x10;//API ID TX Request 64 bit address
					BufferOut[4]=0x00;//Frame ID, if equals to 0x00 disable response frame
					BufferOut[5]=0x00;//Destination Address MSB
					BufferOut[6]=0x13;//Destination Address
					BufferOut[7]=0xA2;//Destination Address
					BufferOut[8]=0x00;//Destination Address
					BufferOut[9]=0x40;//Destination Address
					BufferOut[10]=0xA6;//Destination Address
					BufferOut[11]=0xDA;//Destination Address
					BufferOut[12]=0xBF;//Destination Address LSB --- Actuator's xbee address 0x50
					BufferOut[13]=0xFF;//RESERVED
					BufferOut[14]=0xFE;//RESERVED
					BufferOut[15]=0x00;//Max number of hops, if set to zero max number of hops
					BufferOut[16]=0x00;//disable ACK, disable route discovery
					BufferOut[17]='A';//Data 1
					BufferOut[18]='1';//Data 2
					BufferOut[19]=0x15;//Node ID 0x15 - 0x1D
					BufferOut[20]=status_relay;//Data 3
					BufferOut[21]=0x00;//Checksum
					Serial_Send_Frame(BufferOut,22);

					//Delay(Delay_300ms_Cnt);


				Delay(Delay_1mS_Cnt);
				error_comm_F = read_sensor_frame(BufferIn,22);


				if ((!error_comm_F) && (BufferIn[3]==0x90))//&& (BufferIn[11]==0x50))
					{
						status_relay_confirmation = BufferIn[16]; // Status Relay
						pulse_cnt = BufferIn[17]; //Water Flow Byte 1
						pulse_cnt = BufferIn[18]; //Water Flow Byte 2
									BufferIn[19]; //Pressure Byte 1
									BufferIn[20]; //Pressure Byte 2
									BufferIn[21]; //Battery level
								//sensor_signal = BufferIn[18]; // not part of digimesh protocol
					}

				EE_leds(0b00100000);


}
TASK(TaskSend)
{

	//------------- Transmit a message to the computer -----------------------

			BufferOut[0]=0x7E;//API mode start delimiter
			BufferOut[1]=0x00;//Data length MSB
			BufferOut[2]=0x34;//Data length LSB
			BufferOut[3]=0x10;//API ID TX Request 64 bit address
			BufferOut[4]=0x00;//Frame ID, if equals to 0x00 disable response frame
			BufferOut[5]=0x00;//Destination Address MSB
			BufferOut[6]=0x13;//Destination Address
			BufferOut[7]=0xA2;//Destination Address
			BufferOut[8]=0x00;//Destination Address
			BufferOut[9]=0x40;//Destination Address
			BufferOut[10]=0xA6;//Destination Address
			BufferOut[11]=0xDB;//Destination Address
			BufferOut[12]=0x0E;//Destination Address LSB
			BufferOut[13]=0xFF;//RESERVED
			BufferOut[14]=0xFE;//RESERVED
			BufferOut[15]=0x00;//Max number of hops, if set to zero max number of hops
			BufferOut[16]=0x00;//disable ACK, disable route discovery
	    	BufferOut[17]=0x01;//Data 1
			BufferOut[18]=0x01;//AREA ID
			BufferOut[19]=top_sensor_reading_units_A;//Data 9
			BufferOut[20]=top_sensor_reading_decimals_A;//Data 10
			BufferOut[21]=middle_sensor_reading_units_A;//Data 16
			BufferOut[22]=middle_sensor_reading_decimals_A;//Data 17
			BufferOut[23]=bottom_sensor_reading_units_A;//Data 23
			BufferOut[24]=bottom_sensor_reading_decimals_A;//Data 24
			BufferOut[25]=0x02;//AREA ID
			BufferOut[26]=top_sensor_reading_units_B;//Data 9
			BufferOut[27]=top_sensor_reading_decimals_B;//Data 10
			BufferOut[28]=middle_sensor_reading_units_B;//Data 16
			BufferOut[29]=middle_sensor_reading_decimals_B;//Data 17
			BufferOut[30]=bottom_sensor_reading_units_B;//Data 23
			BufferOut[31]=bottom_sensor_reading_decimals_B;//Data 24
			BufferOut[32]=0x03;//AREA ID
			BufferOut[33]=radiationH;
			BufferOut[34]=radiationL;
			BufferOut[35]=hum_int;
			BufferOut[36]=hum_dec;
			BufferOut[37]=temp_int;
			BufferOut[38]=temp_dec;
			BufferOut[39]=wind_int;
			BufferOut[40]=wind_dec;
			BufferOut[41]=0x04;//AREA ID
			BufferOut[42]=status_relay_confirmation; //Data 27
			BufferOut[43]=pulse_cnt; //Data 30
			BufferOut[44]=pulse_cnt;
			BufferOut[45]=0x05;//AREA ID
			BufferOut[46]=(unsigned int)hum_con;
			BufferOut[47]=((unsigned int)(hum_con*100)%100);
			BufferOut[48]=error_comm_A; //Data 30
			BufferOut[49]=error_comm_B; //Data 30
			BufferOut[50]=error_comm_C; //Data 30
		    BufferOut[51]=error_comm_D; //Data 30
		    BufferOut[52]=error_comm_F; //Data 30
			BufferOut[53]=0x04;//Data 33
			BufferOut[54]=0x0A;//Data 33
			BufferOut[55]=0x00;//Checksum
			Delay(Delay_1mS_Cnt);
			Serial_Send_Frame(BufferOut,56);//size of(BufferOut);
////////////////////////////////////////////////////////////////////////

		/*	//------------- Transmit a message to the GPRS node -----------------------

						BufferOut[0]=0x7E;//API mode start delimiter
						BufferOut[1]=0x00;//Data length MSB
						BufferOut[2]=0x3D;//Data length LSB
						BufferOut[3]=0x10;//API ID TX Request 64 bit address
						BufferOut[4]=0x00;//Frame ID, if equals to 0x00 disable response frame
						BufferOut[5]=0x00;//Destination Address MSB
						BufferOut[6]=0x13;//Destination Address
						BufferOut[7]=0xA2;//Destination Address
						BufferOut[8]=0x00;//Destination Address
						BufferOut[9]=0x40;//Destination Address
						BufferOut[10]=0xA6;//Destination Address
						BufferOut[11]=0xDA;//Destination Address
						BufferOut[12]=0x6B;//Destination Address LSB
						BufferOut[13]=0xFF;//RESERVED
						BufferOut[14]=0xFE;//RESERVED
						BufferOut[15]=0x00;//Max number of hops, if set to zero max number of hops
						BufferOut[16]=0x00;//disable ACK, disable route discovery
				    	BufferOut[17]=0x01;//Data 1
						BufferOut[18]=0x01;//AREA ID
						BufferOut[19]=top_sensor_id;//Data 6
						BufferOut[20]=top_sensor_reading_units_A;//Data 9
						BufferOut[21]=top_sensor_reading_decimals_A;//Data 10
						BufferOut[22]=middle_sensor_id;//Data 13
						BufferOut[23]=middle_sensor_reading_units_A;//Data 16
						BufferOut[24]=middle_sensor_reading_decimals_A;//Data 17
						BufferOut[25]=bottom_sensor_id;//Data 20
						BufferOut[26]=bottom_sensor_reading_units_A;//Data 23
						BufferOut[27]=bottom_sensor_reading_decimals_A;//Data 24
						BufferOut[28]=0x02;//AREA ID
						BufferOut[29]=top_sensor_id;//Data 6
						BufferOut[30]=top_sensor_reading_units_B;//Data 9
						BufferOut[31]=top_sensor_reading_decimals_B;//Data 10
						BufferOut[32]=middle_sensor_id;//Data 13
						BufferOut[33]=middle_sensor_reading_units_B;//Data 16
						BufferOut[34]=middle_sensor_reading_decimals_B;//Data 17
						BufferOut[35]=bottom_sensor_id;//Data 20
						BufferOut[36]=bottom_sensor_reading_units_B;//Data 23
						BufferOut[37]=bottom_sensor_reading_decimals_B;//Data 24
						BufferOut[38]=0x03;//AREA ID
						BufferOut[39]=top_sensor_id;//Data 6
						BufferOut[40]=top_sensor_reading_units_C;//Data 9
						BufferOut[41]=top_sensor_reading_decimals_C;//Data 10
						BufferOut[42]=middle_sensor_id;//Data 13
						BufferOut[43]=middle_sensor_reading_units_C;//Data 16
						BufferOut[44]=middle_sensor_reading_decimals_C;//Data 17
						BufferOut[45]=bottom_sensor_id;//Data 20
						BufferOut[46]=bottom_sensor_reading_units_C;//Data 23
						BufferOut[47]=bottom_sensor_reading_decimals_C;//Data 24
						BufferOut[48]=0x04;//AREA ID
						BufferOut[49]=top_sensor_id;//Data 6
						BufferOut[50]=top_sensor_reading_units_D;//Data 9
						BufferOut[51]=top_sensor_reading_decimals_D;//Data 10
						BufferOut[52]=middle_sensor_id;//Data 13
						BufferOut[53]=middle_sensor_reading_units_D;//Data 16
						BufferOut[54]=middle_sensor_reading_decimals_D;//Data 17
						BufferOut[55]=bottom_sensor_id;//Data 20
						BufferOut[56]=bottom_sensor_reading_units_D;//Data 23
						BufferOut[57]=bottom_sensor_reading_decimals_D;//Data 24
						BufferOut[58]=status_relay; //Data 27
						BufferOut[59]=error_comm_D; //Data 30
					    BufferOut[60]=temperature; //Data 30
					    BufferOut[61]=pulse_cnt;
						BufferOut[62]=0x04;//Data 33
						BufferOut[63]=0x0A;//Data 33
						BufferOut[64]=0x00;//Checksum
						Delay(Delay_1mS_Cnt);
						Serial_Send_Frame(BufferOut,65);//size of(BufferOut);
*/

		//read_sensor_frame(BufferIn,7);

			EE_leds(0b01000000);
}
void LCD_manual(void){

	char hum_units = (unsigned int)hum_con;
	char hum_decimals =((unsigned int)(hum_con*100)%100);

	EE_lcd_clear();
	EE_lcd_home();
	EE_lcd_putc('M');
	EE_lcd_putc('O');
	EE_lcd_putc('D');
	EE_lcd_putc('O');
	EE_lcd_putc(' ');
	EE_lcd_putc('M');
	EE_lcd_putc('A');
	EE_lcd_putc('N');
	EE_lcd_putc('U');
	EE_lcd_putc('A');
	EE_lcd_putc('L');
	EE_lcd_putc(' ');
	EE_lcd_putc(' ');
	EE_lcd_putc('R');
	EE_lcd_putc(':');
	EE_lcd_putc(status_relay + 48);

	EE_lcd_line2();

		EE_lcd_putc('H');
		EE_lcd_putc('U');
		EE_lcd_putc('M');
		EE_lcd_putc(':');
		EE_lcd_putc((hum_units/10)+48);
		EE_lcd_putc((hum_units%10)+48);
		EE_lcd_putc('.');
		EE_lcd_putc((hum_decimals/10)+48);
		EE_lcd_putc((hum_decimals%10)+48);
		EE_lcd_putc(' ');
		EE_lcd_putc(' ');
		EE_lcd_putc(error_comm_A + 48);
		EE_lcd_putc(error_comm_B + 48);
		EE_lcd_putc(error_comm_C + 48);
		EE_lcd_putc(error_comm_D + 48);
		EE_lcd_putc(error_comm_F + 48);
}
void LCD_timer(void){

	EE_lcd_clear();
	EE_lcd_home();
	EE_lcd_putc('M');
	EE_lcd_putc('O');
	EE_lcd_putc('D');
	EE_lcd_putc('O');
	EE_lcd_putc(' ');
	EE_lcd_putc('T');
	EE_lcd_putc('I');
	EE_lcd_putc('M');
	EE_lcd_putc('E');
	EE_lcd_putc('R');
	EE_lcd_putc(' ');


	EE_lcd_line2();

	EE_lcd_putc('E');
	EE_lcd_putc('N');
	EE_lcd_putc(' ');
	EE_lcd_putc('D');
	EE_lcd_putc('E');
	EE_lcd_putc('S');
	EE_lcd_putc('A');
	EE_lcd_putc('R');
	EE_lcd_putc('R');
	EE_lcd_putc('O');
	EE_lcd_putc('L');
	EE_lcd_putc('L');
	EE_lcd_putc('O');

}
void LCD_intelligent(void){

	char hum_units = (unsigned int)hum_con;
	char hum_decimals =((unsigned int)(hum_con*100)%100);

	EE_lcd_clear();
	EE_lcd_home();
	EE_lcd_putc('M');
	EE_lcd_putc('O');
	EE_lcd_putc('D');
	EE_lcd_putc('O');
	EE_lcd_putc(' ');
	EE_lcd_putc('I');
	EE_lcd_putc('N');
	EE_lcd_putc('T');
	EE_lcd_putc(' ');
	EE_lcd_putc(' ');
	EE_lcd_putc('R');
	EE_lcd_putc(':');
	EE_lcd_putc(status_relay + 48);


	EE_lcd_line2();

			EE_lcd_putc('H');
			EE_lcd_putc('U');
			EE_lcd_putc('M');
			EE_lcd_putc(':');
			EE_lcd_putc((hum_units/10)+48);
			EE_lcd_putc((hum_units%10)+48);
			EE_lcd_putc('.');
			EE_lcd_putc((hum_decimals/10)+48);
			EE_lcd_putc((hum_decimals%10)+48);
			EE_lcd_putc(' ');
			EE_lcd_putc(' ');
			EE_lcd_putc(error_comm_A + 48);
			EE_lcd_putc(error_comm_B + 48);
			EE_lcd_putc(error_comm_C + 48);
			EE_lcd_putc(error_comm_D + 48);
			EE_lcd_putc(error_comm_F + 48);

}


