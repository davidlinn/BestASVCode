/* DUMMY CLASS! USE AS A TEMPLATE FOR NEW LIBRARIES! */

// only define this class if it hasn't been before
#ifndef __RF_H__
#define __RF_H__

// include relevant libraries, always good to have these two
#include <Arduino.h>
#include "Pinouts.h"
#include "DataSource.h"

// controls how often and when in the loop this class's functions run


class RF : public DataSource {
public: // for functions outside code might call
  RF(void);

  void init(void);

  void read(void);

  String printPower(void);

  size_t writeDataBytes(unsigned char * buffer, size_t idx);

  unsigned int getPower(void);

  unsigned int t1 = 2;             // ADC reading
  unsigned int t2 = 1;             // ADC reading
  unsigned int t3 = 1;             // ADC reading
  unsigned int t5 = 1;             // ADC reading
  unsigned int t6 = 1;             // ADC reading
  unsigned int tslow = 5;             // ADC reading
  int lastExecutionTime = -1;

private: // for helper functions and hidden variables
  unsigned int READING[12];        // int array to store readings from LTC5587
                                 // this array will have the bits backwards
                                 // (MSB last, LSB first)
  unsigned int readingSum;
  
};

#endif
