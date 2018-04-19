#ifndef __PINOUTS_H__
#define __PINOUTS_H__

#define LOOP_LED 7
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
#define NSS_CONV 17      // NSS for SPI, CONV for LTC5587 - White
#define MISO_SDO 18      // MISO for SPI, SDO for LTC5587 - Yellow
#define SCK_SCK 19        // SCK on both SPI and LTC5587 - Green
#define SERVO_PIN 9

//TEMP
#define TEMP_PIN 14
#define TURB_STRAIGHT_PIN 15
#define TURB_90_PIN 16

/* TOF */
#define SPEAKER_PIN 20
#define MIC_PIN 23

#endif
