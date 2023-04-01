#include <Arduino_JSON.h>
#include <EspNow.h>
#include <remoteSerial.h>
Peer baseDirection;
JSONVar serialBase;

void setup()
{
  Serial.begin(115200);
  Serial2.begin(921600, SERIAL_8N1, 16, 17);
  registerSerial(&Serial2);
  setId("TenZZ");
  baseDirection.init("WardL");
  Serial2.setTimeout(1);
  baseDirection.setOnRecieve(defaultHandler);
  setSerialReciever(cross, "cross");
  setSerialReciever(distanceModeOn, "dmOn");
  setSerialReciever(distanceModeOff, "dmOff");
  setBaseReciever(sendBase, "base");
  
}

void loop()
{
  serialListener();
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
