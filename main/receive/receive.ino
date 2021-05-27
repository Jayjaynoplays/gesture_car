#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(7, 8);

const uint64_t pipes[2] = {0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL};

int motoDrivers[] = {2, 3, 4, 5, 9, 10, 11, 12};
int led = 14;
unsigned long led_delay_blink = 0;

int fr_in3 = motoDrivers[1];
int fr_in4 = motoDrivers[0];

int br_in1 = motoDrivers[3];
int br_in2 = motoDrivers[2];

int fl_in1 = motoDrivers[4];
int fl_in2 = motoDrivers[5];

int bl_in3 = motoDrivers[6];
int bl_in4 = motoDrivers[7];

// yall: 9 -> 100
// pitch: -50 -> -20 : 10 -> 70, roll: -25 -> -5 : 15->30 with forwardPitch: 0->180, forwardRoll: 0->30
// roll: -60 -> -25 : 30 -> 60

void setup(void)
{
  Serial.begin(115200);
  Serial.println("RF24/examples/GettingStarted");

  for (int i = 0; i <= 7; i++)
  {
    pinMode(motoDrivers[i], OUTPUT);
  }
  pinMode(led, OUTPUT);

  radio.begin();
  radio.setAutoAck(false);

  radio.setRetries(15, 15);

  radio.openReadingPipe(1, pipes[0]);

  radio.startListening();
}

void pitchMove_adapt(float p, float r, String moveType)
{
  if (moveType == "forward")
  {
    if (r >= 15 && r <= -5)
    {
      if (r >= 15)
      {
        if (r <= 30)
        {
          forward(round(p * 3 * (-1) + r), round(p * 3 * (-1)), round(p * 3 * (-1) + r), round(p * 3 * (-1)));
        }
        else
        {
          forward(round(p * 3 * (-1) + 30), round(p * 3 * (-1)), round(p * 3 * (-1) + 30), round(p * 3 * (-1)));
        }
      }
      else
      {
        if (r >= -20)
        {
          forward(round(p * 3 * (-1)), round(p * 3 * (-1) + (r * (-1) + 10)), round(p * 3 * (-1)), round(p * 3 * (-1) + (r * (-1) + 10)));
        }
        else
        {
          forward(round(p * 3 * (-1)), round(p * 3 * (-1) + 30), round(p * 3), round(p * 3 * (-1) + 30));
        }
      }
    }
    else
    {
      forward(round(p * 3 * (-1)), round(p * 3 * (-1)), round(p * 3 * (-1)), round(p * 3 * (-1)));
    }
  }

  if (moveType == "back")
  {
    if (r >= 15 && r <= -5)
    {
      if (r >= 15)
      {
        if (r <= 30)
        {
          back(round(p * 3 + r), round(p * 3), round(p * 3 + r), round(p * 3));
        }
        else
        {
          back(round(p * 3 + 30), round(p * 3), round(p * 3 + 30), round(p * 3));
        }
      }
      else
      {
        if (r >= -20)
        {
          back(round(p * 3), round(p * 3 + (r * (-1) + 10)), round(p * 3), round(p * 3 + (r * (-1) + 10)));
        }
        else
        {
          back(round(p * 3), round(p * 3 + 30), round(p * 3), round(p * 3 + 30));
        }
      }
    }
    else
    {
      back(round(p * 3), round(p * 3), round(p * 3), round(p * 3));
    }
  }
}

void rollMove_adapt(float p, float r, String moveType)
{
  if (moveType == "go_right")
  {
    go_right(round(r * 2 * (-1) + 20), round(r * 2 * (-1) + 15), round(r * 2 * (-1)), round(r * 2 * (-1)));
    if (r < -60)
    {
      go_right(140, 135, 120, 120);
    }
  }

  if (moveType == "go_left")
  {
    go_left(round(r * 2 + 20), round(r * 2 + 20), round(r * 2), round(r * 2));
    if (r > 60)
    {
      go_left(140, 140, 120, 120);
    }
  }
}

void loop(void)
{
  if (radio.available())
  {
    if (millis() - led_delay_blink > 50)
      {
        if (digitalRead(led) == LOW)
        {
          digitalWrite(led, HIGH);
        }
        else
        {
          digitalWrite(led, LOW);
        }
        led_delay_blink = millis();
      }
    bool done = false;
    struct dataStruct
    {
      bool isChangeMode;
      float pitch;
      float roll;
    };

    dataStruct ypr;

    while (!done)
    {
      done = radio.read(&ypr, sizeof(ypr));

      float isChangeMode = ypr.isChangeMode;
      float p = ypr.pitch;
      float r = ypr.roll;

      bool isRol = false;

      if (r < -25 || r > 30)
      {
        isRol = true;
      }
      else
      {
        isRol = false;
      }

      if (isRol == false)
      {
        if (p <= -20)
        {
          pitchMove_adapt(p, r, "forward");
        }
        if (p >= 10)
        {
          pitchMove_adapt(p, r, "back");
        }
        if (p <= 10 && p >= -20)
        {
          stop_car();
        }
      }

      if (isRol == true)
      {
        if (r < -25)
        {
          rollMove_adapt(p, r, "go_right");
        }
        if (r > 30)
        {
          rollMove_adapt(p, r, "go_left");
        }
        if (r <= 30 && r >= -25)
        {
          stop_car();
        }
      }

      if (isChangeMode)
      {
        if (r < -25)
        {
          turn_CW(70, 70, 70, 70);
        }
        if (r > 30)
        {
          turn_CCW(70, 70, 70, 70);
        }
      }

      // Spew playload
      Serial.print("Got payload:  ");
      Serial.print(ypr.isChangeMode);
      Serial.print("  ");
      Serial.print(p);
      Serial.print("  ");
      Serial.print(r);
      Serial.println("...");
    }
  }
}
