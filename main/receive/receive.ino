#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(7, 8);

const uint64_t pipes[2] = {0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL};

void setup(void){
  Serial.begin(115200);
  Serial.println("RF24/examples/GettingStarted");

  radio.begin();
  radio.setAutoAck(false);

  radio.setRetries(15, 15);

  radio.openReadingPipe(1, pipes[0]);

  radio.startListening();
}

void loop(void){
  if (radio.available()){
    const char text[32] = "";
    bool done = false;
    struct dataStruct {
      float yall;
      float pitch;
      float roll;
    };
    
    dataStruct ypr;
    while (!done){
      done = radio.read(&ypr, sizeof(ypr));

      // Spew playload
      Serial.print("Got payload:  ");
      Serial.print(ypr.yall);
      Serial.print("  ");
      Serial.print(ypr.pitch);
      Serial.print("  ");
      Serial.print(ypr.roll);
      Serial.println("...");
    }
  }
}
