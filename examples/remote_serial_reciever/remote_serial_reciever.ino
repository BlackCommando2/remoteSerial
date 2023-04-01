#include <Arduino_JSON.h>
#include <EspNow.h>
#include <remoteSerial.h>
Peer baseDirection;
JSONVar serialBase;

void setup()
{
  Serial.begin(115200);
  Serial2.begin(921600, SERIAL_8N1, 16, 17);
  registerSerial(&Serial2); // register which Serial to Listen at
  setId("TenZZ");
  baseDirection.init("WardL");
  Serial2.setTimeout(1);
  baseDirection.setOnRecieve(defaultHandler); // set function to send remote analog Value to Base
  setSerialReciever(cross, "cross"); // set funciton for particular string recieved
  setSerialReciever(distanceModeOn, "dmOn");
  setSerialReciever(distanceModeOff, "dmOff");
  setBaseReciever(sendBase, "base");
}

void loop()
{
  serialListener(); //to start serial listener
}

void cross(String msg)
{
  Serial.println("LOL");
}

void sendBase(JSONVar msg)
{
  baseDirection.send(msg);
}

void distanceModeOn(String msg)
{
  serialBase["type"]="dmOn";
  Serial.println(JSON.stringify(serialBase));
  baseDirection.send(serialBase);
}

void distanceModeOff(String msg)
{
  serialBase["type"]="dmOff";
  Serial.println(JSON.stringify(serialBase));
  baseDirection.send(serialBase);
}

void defaultHandler(JSONVar msg)
{
  Serial.println("Default: " + JSON.stringify(msg));
}