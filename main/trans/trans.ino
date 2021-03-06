#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
RF24 radio(7, 8);

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

//SLC -> A5
// SDA -> A4
//INT -> D2
// AD0 low = 0x68 (default for SparkFun breakout and InvenSense evaluation board)
// AD0 high = 0x69

//need about 20s to calibrating and stable

MPU6050 mpu;
#define OUTPUT_READABLE_YAWPITCHROLL
#define INTERRUPT_PIN 2

const uint64_t pipes[2] = {0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL};

bool dmpReady = false;
uint8_t mpuIntStatus;
uint8_t devStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[1024];

Quaternion q;
VectorFloat gravity;
float ypr[3];

volatile bool mpuInterrupt = false;
void dmpDataReady()
{
  mpuInterrupt = true;
}

int led = 3;
int btn_mode = 4;
unsigned long TX_delay_calib = 0;
unsigned long led_delay_announce = 0;

void setup()
{
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  TWBR = 12;

#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif
  radio.begin();
  radio.setAutoAck(false);

  radio.setRetries(15, 15);
  radio.openWritingPipe(pipes[0]);

  radio.stopListening();

  // (115200 is required for Teapot Demo)
  Serial.begin(115200);

  Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();
  //  pinMode(INTERRUPT_PIN, INPUT);

  // verify connection
  Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

  // load and configure the DMP
  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  mpu.setXGyroOffset(-414);
  mpu.setYGyroOffset(91);
  mpu.setZGyroOffset(-23);
  mpu.setZAccelOffset(2032); // 1688 factory default for my test chip

  //      mpu.setXGyroOffset(290);
  //    mpu.setYGyroOffset(100);
  //    mpu.setZGyroOffset(-115);
  //    mpu.setZAccelOffset(1998);

  // make sure it worked (returns 0 if so)
  if (devStatus == 0)
  {
    Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);
    mpuInterrupt == false;
    Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
    attachInterrupt(0, dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;

    packetSize = mpu.dmpGetFIFOPacketSize();
  }
  else
  {
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
  }

  pinMode(led, OUTPUT);
  pinMode(btn_mode, INPUT_PULLUP);
}

void loop()
{

  if (!dmpReady)
    return;
  while (!mpuInterrupt && fifoCount < packetSize)
  {
  }

  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();

  fifoCount = mpu.getFIFOCount();
  if (fifoCount == 16384)
  {
    Serial.println(F("FIFO overflow!"));
  }
  else if (mpuIntStatus & 0x02)
  {
    while (fifoCount < packetSize)
      fifoCount = mpu.getFIFOCount();

    mpu.getFIFOBytes(fifoBuffer, packetSize);
    mpu.resetFIFO();
    fifoCount -= packetSize;

#ifdef OUTPUT_READABLE_YAWPITCHROLL
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    if (millis() - TX_delay_calib < 15000)
    {
      if (millis() - led_delay_announce > 300)
      {
        Serial.print("MPU lib at: ");
        Serial.print(millis());
        Serial.print(" With ypr: ");
        Serial.print(ypr[0] * 180 / M_PI);
        Serial.print("  ");
        Serial.print(ypr[2] * 180 / M_PI);
        Serial.print("  ");
        Serial.println(ypr[1] * 180 / M_PI);

        if (digitalRead(led) == LOW)
        {
          digitalWrite(led, HIGH);
        }
        else
        {
          digitalWrite(led, LOW);
        }
        led_delay_announce = millis();
      }
    }
    else
    {
      struct dataStruct
      {
        bool isChangeMode = false;
        float pitch = (ypr[2] * 180 / M_PI);
        float roll = (ypr[1] * 180 / M_PI);
      };
      dataStruct ypr;

      if (digitalRead(btn_mode) == 0)
        ypr.isChangeMode = true;
      else
        ypr.isChangeMode = false;

      digitalWrite(led, HIGH);
      radio.stopListening();
      bool ok = radio.write(&ypr, sizeof(ypr));
      if (ok)
      {
        Serial.print("ok with: ");
        Serial.print(ypr.isChangeMode);
        Serial.print("  ");
        Serial.print(ypr.pitch);
        Serial.print("  ");
        Serial.print(ypr.roll);
        Serial.println("...");
      }
      else
      {

        Serial.println("failed!");
      }
      radio.startListening();
    }

    mpu.resetFIFO();

#endif
  }
}
