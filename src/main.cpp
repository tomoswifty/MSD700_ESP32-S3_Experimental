#include <Arduino.h>
#include <Wire.h>
#include <IM920s.h>
#include <IM920sDS4Client.h>
#include <JetsonAutoControl.h>
#include <Msd700.h>

#define USB_SERIAL Serial
#define IM920s_SERIAL Serial1

Msd700 msd700(35, 36, 10, 40, 39, 9);

IM920s im920s(IM920s_SERIAL);
IM920sDS4Client dualshock4;

JetsonAutoControl jetson;

bool autoControlEnable = false;

void setup()
{
  USB_SERIAL.begin(115200);
  USB_SERIAL.setTimeout(50);

  im920s.begin(19200, SERIAL_8N1, 4, 5);
  // im920s.changeBaudrate(7);
  im920s.disableCharacterMode();

  dualshock4.setTimeoutThreshold(100);
  dualshock4.setLeftHatDeadZone(32, 32);
  dualshock4.setRightHatDeadZone(32, 32);

  Wire.setPins(47, 21);
  Wire.begin();
}

void loop()
{
  String payload = im920s.receivePayload();
  dualshock4.parse(payload);
  // USB_SERIAL.println(payload);

  // Serial.print(msd700.getLeftMotorRevolutionSpeed());
  // Serial.print(",");
  // Serial.println(msd700.getLeftMotorRevolutionSpeed());

  String string = Serial.readStringUntil('\n');
  jetson.parse(string);

  // if (string.length() > 1)
  // {
  //   Serial1.print("TXDA ");
  //   Serial1.print("Data from jetson:");
  //   Serial1.print(jetson.getVelocityInDirectionOfTravel());
  //   Serial1.print(";");
  //   Serial1.print(jetson.getAngularVelocity());
  //   Serial1.println("");
  // }

  msd700.travelByInverseKinematics(jetson.getVelocityInDirectionOfTravel(), jetson.getAngularVelocity());
  // if (autoControlEnable)
  // {
  // }

  // if (dualshock4.getAnalogButtonValue(Ds4AnalogButtonEnum::L2) >= 240 & dualshock4.getAnalogButtonValue(Ds4AnalogButtonEnum::R2) >= 240)
  // {
  //   autoControlEnable = false;
  //   if (1)
  //   {
  //     msd700.travelByInverseKinematics(
  //         -Msd700::joystickToVelocityInDirectionOfTravel(dualshock4.getAnalogHatValue(Ds4AnalogHatEnum::LeftY)),
  //         -Msd700::joystickToAngularVelocity(dualshock4.getAnalogHatValue(Ds4AnalogHatEnum::LeftX)));
  //   }
  //   else
  //   {
  //     msd700.travelByDirectControl(
  //         msd700.joystickToRevolutionSpeed(dualshock4.getAnalogHatValue(Ds4AnalogHatEnum::LeftY)),
  //         msd700.joystickToRevolutionSpeed(dualshock4.getAnalogHatValue(Ds4AnalogHatEnum::RightY)));
  //   }
  // }
  // else if(!autoControlEnable)
  // {
  //   msd700.travelByDirectControl(0,0);
  // }

  // if(dualshock4.getButtonPress(Ds4ButtonEnum::Circle))
  // {
  //   autoControlEnable = true;
  //   msd700.travelByDirectControl(0,0);
  // }
}