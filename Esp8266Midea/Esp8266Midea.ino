
#define IR_EMITER       5
#define FIVE_SECONDS    5000
#define MIDEA_COOL_HEAT

#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <MideaIR.h>

#define IR_EMITER 5

IRsend irsend(IR_EMITER);
MideaIR remote_control(&irsend);

void setup(){
  // Define IR PIN as Output
  pinMode(IR_EMITER, OUTPUT);

  // Turn Air Conditioner ON
  remote_control.turnON();
  delay(FIVE_SECONDS);

  // Set Temperature
  remote_control.setTemperature(18);
  remote_control.emit();

  // Set temperature and mode
  remote_control.setTemperature(23);
  remote_control.setMode(mode_heat);
  remote_control.emit();

  // Set temperature, mode and fan_speed
  remote_control.setTemperature(25);
  remote_control.setMode(mode_ventilate);
  remote_control.setSpeedFan(fan_speed_3);
  remote_control.emit();

  // Turn Air Conditioner OFF
  remote_control.turnOFF();
}

void loop(){}
