#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(7, 8);

const uint64_t pipes[2] = {0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL};

void setup(void)
{
  Serial.begin(115200);
  Serial.println("RF24/examples/GettingStarted");

  radio.begin();
  radio.setAutoAck(false);

  radio.setRetries(15, 15);

  radio.openReadingPipe(1, pipes[0]);

  radio.startListening();
}

void loop(void)
{
  // if there is data ready
  if (radio.available())
  {
    // Dump the payloads until we've gotten everything
    const char text[32] = "";
    bool done = false;
    struct dataStruct {
      float yall;
      float pitch;
      float roll;
    };
    dataStruct ypr;
    while (!done)
    {
      // Fetch the payload, and see if this was the last one.
      done = radio.read(&ypr, sizeof(ypr));

      // Spew it
      Serial.print("Got payload:  ");
      Serial.print(ypr.yall);
      Serial.print("  ");
      Serial.print(ypr.pitch);
      Serial.print("  ");
      Serial.print(ypr.roll);
      Serial.println("...");

      delay(20);
    }
  }
}
