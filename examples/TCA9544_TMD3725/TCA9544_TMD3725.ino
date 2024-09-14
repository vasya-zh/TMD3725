#include <Wire.h>
#include <tca9544a.h>
#include <i2cdetect.h>
#include <TMD3725.h>
#include <Arduino.h>
#include <stdio.h>




#define TCA9544ADDR 0x70
#define TMD3725ADDR 0x39

TCA9544A TCA;
TMD3725 tmd3725;
optics_val colordata;

REDIRECT_STDOUT_TO(Serial);
int reginfo[35];
int colorarray[9];

void setup() {
  Serial.begin(9600);
  delay(3000);
  Serial.println("TCA9544A and TMD3725 example\n");
  Wire.begin();

  // check TCA9544A availability
  if (TCA.begin()) {
    Serial.println("TCA9544A connected");
    TCA.clearChannels();
    TCA.setChannel(0);

    // check TMD3725 availability
    if (tmd3725.begin())
      Serial.println("tmd3725 connected");
    else
      Serial.println("tmd3725 not connected");
  } else Serial.println("TCA9544A error");

  tmd3725.init(reginfo);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second
  tmd3725.get_all_data(reginfo);
  colordata = tmd3725.get_calib_color(reginfo);
  tmd3725.print_color(colordata);  //HBB: deactivated Serial printout
  tmd3725.print_color_json(colordata, millis());
}
