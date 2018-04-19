/* DUMMY CLASS! USE AS A TEMPLATE FOR NEW LIBRARIES! */

// this includes everything from the header file
#include "RF.h"

// this allows you to use print calls
// (see IMU or ADC libraries for good examples)
#include "Printer.h"
extern Printer printer;

// constructor for class objects
RF::RF(void) : DataSource("rf","int") { //default initializes READING array and reading sum
}

void RF::init(void) {
  pinMode(NSS_CONV,OUTPUT);
  pinMode(MISO_SDO,INPUT);
  pinMode(SCK_SCK,OUTPUT);

  // this lets you print messages
  // the string gets displayed in the messages area
  // the number counts the number of times it is displayed at 10 Hz
  printer.printMessage("Initialized RF Reader at " + String(millis()), 10);
}

void RF::read(void) {
  readingSum = 0;

  digitalWrite(SCK_SCK, LOW);             // Start with clock low
  digitalWrite(NSS_CONV, HIGH);           // Bring CONV high
  delayMicroseconds(t1);                  // Wait a sec
  digitalWrite(NSS_CONV, LOW);            // Bring CONV low, acqusition done
  delayMicroseconds(t2);

  for (int i = 11; i>= 0; i--) {
    //Serial.println("***Start of For Loop***");

    delayMicroseconds(t5/2);
    digitalWrite(SCK_SCK, HIGH);
    delayMicroseconds(t6/2);
    READING[i] = digitalRead(MISO_SDO);
    delayMicroseconds(t6/2);
    digitalWrite(SCK_SCK, LOW);
    delayMicroseconds(t5/2);
    readingSum += READING[i] << i;
  }
}

String RF::printPower(void) {
  return "RF Power Recieved: " + String(readingSum);
}

size_t RF::writeDataBytes(unsigned char * buffer, size_t idx) {
  int * data_slot = (int *) &buffer[idx];
  data_slot[0] = readingSum;
  return idx + sizeof(int);
}
