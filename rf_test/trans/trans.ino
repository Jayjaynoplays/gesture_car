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
  radio.openWritingPipe(pipes[0]);
  radio.printDetails();
}

void loop(void)
{
  radio.stopListening();

  const char text[] = "Hello Bitch";
  Serial.print("Now sending...");
  Serial.print(text);
  Serial.print("...");
  bool ok = radio.write(text, sizeof(text));

  if (ok)
    Serial.println("ok!");
  else
    Serial.println("failed!");

  radio.startListening();

  // Try again 1s later
  delay(1000);
}
