/********
Default E80 Lab 01 - BestASVCode - Sec 2 Team 1
Current Author: David Linn, Spencer Rosen, Max Maleno, Nick Koskelo
Christopher McElroy (cmcelroy@g.hmc.edu) '19 (contributed in 2017)
Previous Contributors:  Josephine Wong (jowong@hmc.edu) '18 (contributed in 2016)
                        Apoorva Sharma (asharma@hmc.edu) '17 (contributed in 2016)
*/
//How to orient the bot:
//If you pretend to be the robot, you should be able to read
//"Student Button" and "Student LED"

#define RFNAV 0

#include <Arduino.h>
#include <Wire.h>
#include <Pinouts.h>
#include <TimingOffsets.h>

#include <SensorGPS.h>
#include <SensorIMU.h>
#include <StateEstimator.h>
#include <Adafruit_GPS.h>
#include <ADCSampler.h>
#include <MotorDriver.h>
#include <Logger.h>
#include <Printer.h>
#include <PControl.h>
#define mySerial Serial1
#include <LED.h>  // A template of a data source library
#include <RF.h>

/////////////////////////* Global Variables *////////////////////////

MotorDriver motor_driver;
StateEstimator state_estimator;
PControl pcontrol;
SensorGPS gps;
Adafruit_GPS GPS(&mySerial);  // FIX THIS
ADCSampler adc;
SensorIMU imu;
Logger logger;
Printer printer;
//LED led;
RF rf;

// loop start recorder
int loopStartTime;
int currentTime;

// Waypoint setup
const int number_of_waypoints = 1;
const int waypoint_dimensions = 2;       // waypoints have two pieces of information, x then y.
double waypoints [] = { 0, 0 };   // listed as x0,y0,x1,y1, ... etc.

////////////////////////* Setup *////////////////////////////////

void setup() {
  
  logger.include(&imu);
  logger.include(&gps);
  logger.include(&state_estimator);
  logger.include(&motor_driver);
  logger.include(&adc);
  logger.include(&rf);
  logger.init();

  printer.init();
  imu.init();
  mySerial.begin(9600);
  gps.init(&GPS);
  motor_driver.init();
  //led.init();
  rf.init();

  pcontrol.init(number_of_waypoints, waypoint_dimensions, waypoints);
  
  const float origin_lat = 33.46254; //X on the beach
  const float origin_lon = -117.70555;
  //const float origin_lat = 33.46247; //Rocks
  //const float origin_lon = -117.70623;
  //const float origin_lat = 34.103810; //Scripps
  //const float origin_lon = -117.708313;
  state_estimator.init(origin_lat, origin_lon); 

  printer.printMessage("Starting main loop",10);
  loopStartTime = millis();
  printer.lastExecutionTime         = loopStartTime - LOOP_PERIOD + PRINTER_LOOP_OFFSET ;
  imu.lastExecutionTime             = loopStartTime - LOOP_PERIOD + IMU_LOOP_OFFSET;
  gps.lastExecutionTime             = loopStartTime - LOOP_PERIOD + GPS_LOOP_OFFSET;
  adc.lastExecutionTime             = loopStartTime - LOOP_PERIOD + ADC_LOOP_OFFSET;
  state_estimator.lastExecutionTime = loopStartTime - LOOP_PERIOD + STATE_ESTIMATOR_LOOP_OFFSET;
  pcontrol.lastExecutionTime        = loopStartTime - LOOP_PERIOD + P_CONTROL_LOOP_OFFSET;
//rf
  logger.lastExecutionTime          = loopStartTime - LOOP_PERIOD + LOGGER_LOOP_OFFSET;
  rf.lastExecutionTime              = loopStartTime - LOOP_PERIOD + RF_LOOP_OFFSET;
}



//////////////////////////////* Loop */////////////////////////

void loop() {
  currentTime=millis();
  
  if ( currentTime-printer.lastExecutionTime > LOOP_PERIOD ) {
    printer.lastExecutionTime = currentTime;
    printer.printValue(0,adc.printSample());
    printer.printValue(1,logger.printState());
    printer.printValue(2,gps.printState());   
    printer.printValue(3,state_estimator.printState());     
    printer.printValue(4,pcontrol.printWaypointUpdate());
    printer.printValue(5,pcontrol.printString());
    printer.printValue(6,motor_driver.printState());
    printer.printValue(7,imu.printRollPitchHeading());        
    printer.printValue(8,imu.printAccels());
    printer.printValue(9,rf.printPower()); //prints but doesn't log
    printer.printValue(10, pcontrol.printAverages());
    printer.printToSerial();  // To stop printing, just comment this line out
  }

  if ( currentTime-pcontrol.lastExecutionTime > LOOP_PERIOD ) {
    pcontrol.lastExecutionTime = currentTime;
    if (RFNAV) {
      pcontrol.rfNavigateLoop(rf);
    }
    else {
      pcontrol.calculateControl(&state_estimator.state);
    }
    motor_driver.driveForward(pcontrol.uL,pcontrol.uR);
  }

  if ( currentTime-adc.lastExecutionTime > LOOP_PERIOD ) {
    adc.lastExecutionTime = currentTime;
    adc.updateSample(); 
  }

  if ( currentTime-imu.lastExecutionTime > LOOP_PERIOD ) {
    imu.lastExecutionTime = currentTime;
    imu.read();     // blocking I2C calls
  }
  
  if (true){//(gps.loopTime(loopStartTime)) {
    gps.lastExecutionTime = currentTime;
    gps.read(&GPS); // blocking UART calls
  }

  if ( currentTime-state_estimator.lastExecutionTime > LOOP_PERIOD ) {
    state_estimator.lastExecutionTime = currentTime;
    state_estimator.updateState(&imu.state, &gps.state);
  }
  
  // use this as a template for new libraries!
  /*
  if (currentTime-led.lastExecutionTime > LOOP_PERIOD) {
    led.lastExecutionTime = currentTime;
    //do nothing for now
    
  }
  */

  if (currentTime - rf.lastExecutionTime > LOOP_PERIOD) {
    rf.lastExecutionTime = currentTime;
    rf.read();
  }

  if (currentTime- logger.lastExecutionTime > LOOP_PERIOD && logger.keepLogging) {
    logger.lastExecutionTime = currentTime;
    logger.log();
  }
}

