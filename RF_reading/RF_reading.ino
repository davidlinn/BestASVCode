// Max Maleno - 4/10/18, most recently edited 4/13/18
// mmaleno@hmc.edu
// E80 Test RF Power meter interface

unsigned int NSS_CONV = 12;      // NSS for SPI, CONV for LTC5587
unsigned int MISO_SDO = 8;      // MISO for SPI, SDO for LTC5587
unsigned int SCK_SCK = 7;        // SCK on both SPI and LTC5587

unsigned int READING[12];        // int array to store readings from LTC5587
                                 // this array will have the bits backwards
                                 // (MSB last, LSB first)

// Page 17 https://www.alliedcomponentworks.com/support/open/LTC5587.pdf
unsigned int t1 = 4;             // ADC reading
unsigned int t2 = 1;             // ADC reading
unsigned int t3 = 1;             // ADC reading
unsigned int t5 = 1;             // ADC reading
unsigned int t6 = 1;             // ADC reading
unsigned int tslow = 5;             // ADC reading

void setup() {
  Serial.begin(9600);
  pinMode(NSS_CONV,OUTPUT);
  pinMode(MISO_SDO,INPUT);
  pinMode(SCK_SCK,OUTPUT);
}

void loop() {
  //Serial.println("***********Start of Calculation***********");

  digitalWrite(SCK_SCK, LOW);             // Start with clock low
  digitalWrite(NSS_CONV, HIGH);           // Bring CONV high
  delayMicroseconds(t1);                  // Wait a sec
  digitalWrite(NSS_CONV, LOW);            // Bring CONV low, acqusition done
  delayMicroseconds(t2);
  
  //delayMicroseconds(t3);                  
  //READING[11] = digitalRead(MISO_SDO);
  //delayMicroseconds(t2);                
  //Serial.print("B11: ");
  //Serial.println(READING[11]);

  // 0603

  for (int i = 11; i>= 0; i--) {
    //Serial.println("***Start of For Loop***");

    delayMicroseconds(t5/2);
    digitalWrite(SCK_SCK, HIGH);
    delayMicroseconds(t6/2);
    READING[i] = digitalRead(MISO_SDO);
    delayMicroseconds(t6/2);
    digitalWrite(SCK_SCK, LOW);
    delayMicroseconds(t5/2);
   }

  // once this is working, this can be inplemented into
  // the above code

  unsigned int readingSum = 0;
  
  for (int i = 11; i>-1; i--) {
    if (READING[i] == 1) {
      readingSum += pow(2,i)+0.5;
    }
  }

  Serial.print("readingSum = ");
  Serial.println(readingSum);

  delay(1000);
  

}
