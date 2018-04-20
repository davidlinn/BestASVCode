//DON'T USE A0 (14) or A1 (15)- they are driven by op-amps on the E79 motor board attached to E79 temp and pressure, which we're not using
//Be careful about using A6/20 and A9/23- they might be used for speaker/mic
//DON'T USE SDA (A4/18) and SCL (A5/19)- they are used by the GPS for I2C
// pins A10-A14 are _NOT_ 5V tolerarant!  All the others are.



#ifndef __PINOUTS_H__
#define __PINOUTS_H__

//#define LOOP_LED 7
#define USER_SWITCH 2

#define MOTOR_L_FORWARD 6		// M4: 8Y is blue, 7Y is yellow
#define MOTOR_L_REVERSE 23
#define MOTOR_R_FORWARD 4		// M2: 4Y is black, 3Y is red
#define MOTOR_R_REVERSE 21      
#define MOTOR_V_FORWARD	5		// M3  pins 16, 17
#define MOTOR_V_REVERSE	22
#define MOTOR_X_FORWARD 3		// M2 pins 20,22
#define MOTOR_X_REVERSE 20

//RF+SERVO
#define NSS_CONV 9      // NSS for SPI, CONV for LTC5587 - White - D9
#define MISO_SDO 8      // MISO for SPI, SDO for LTC5587 - Yellow - D8
#define SCK_SCK 7        // SCK on both SPI and LTC5587 - Green - D7

//TEMP
#define TEMP_PIN 28 //A17
#define TURB_STRAIGHT_PIN 27 //A16
#define TURB_90_PIN 26 //A15
#define TURB2_PIN 17   //A3 - Blue

/* TOF */
//#define SPEAKER_PIN 20 //A6
//#define MIC_PIN 23 //A9

#endif
