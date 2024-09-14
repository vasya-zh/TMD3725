#include <Wire.h>
#include <TMD3725.h>
#include <Arduino.h>

#define TMD3725ADDR 0x39

TMD3725 tmd3725;
optics_val colordata;

REDIRECT_STDOUT_TO(Serial);
int reginfo[35];
int colorarray[9];

void setup() {
  Serial.begin(9600);
  Serial.println("TCA9544A and TMD3725 example\n");
  Wire.begin();

  // check TMD3725 availability
  if (tmd3725.begin())
    Serial.println("tmd3725 connected");
  else
    Serial.println("tmd3725 not connected");

  tmd3725.init(reginfo);
}

void loop() {
  tmd3725.get_all_data(reginfo);
  colordata = tmd3725.get_calib_color(reginfo);
  tmd3725.print_color(colordata);  //HBB: deactivated Serial printout
  tmd3725.print_color_json(colordata, millis());
  delay(1000);                      // wait for a second
}
