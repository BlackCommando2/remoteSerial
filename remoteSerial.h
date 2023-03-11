Stream *remoteSerial;
char remoteSingleData;
bool isDataDigit = false, _initData = false;
String dataString = "";
bool baseX = false, baseY = false, baseR = false;
bool pressed = false;
JSONVar baseData;
JSONVar serialHandleType;
int serialHandlerIndex = 0;
int baseSingleData = 0;

// void (*dataRecieve[100])(String);
void (*serialDataRecieve[100])(String);
void (*baseDataRecieve[1])(JSONVar);

void registerSerial(Stream *serial)
{
    remoteSerial = serial;
    // remoteSerial->begin(921600, SERIAL_8N1, 16, 17);
    // remoteSerial->setTimeout(1);
}

void serialListener()
{
    if(remoteSerial->available())
    {
        remoteSingleData = (char)remoteSerial->read();
        // Serial.println(remoteSingleData);
        if (remoteSingleData == '\n')
        {
            // Serial.println("GOOOOOOOOOO");
            if (dataString == "base")
            {
                baseDataRecieve[0](baseData);
                // Serial.println(JSON.stringify(baseData));
                // Serial.println("base");
            }
            else if(dataString != "base")
            {
                Serial.println("LOL: "+dataString);
                int typeIndex = serialHandleType[dataString];
                serialDataRecieve[typeIndex](dataString);
            }
            isDataDigit = false;
            baseX = false;
            baseY = false;
            baseR = false;
            dataString="";
            _initData = false;
        }
        else
        {
            if (!_initData && remoteSingleData!='-')
            {
                isDataDigit = isDigit(remoteSingleData);
                _initData = true;
                // Serial.println("not init"+(String)isDataDigit);
            }
            else if(remoteSingleData == '-')
            {
                isDataDigit = true;
                _initData = true;
            }

            if (isDataDigit)
            {
                if (remoteSingleData == ',')
                {
                    baseSingleData = dataString.toInt();
                    if (!baseX)
                    {
                        if (!pressed)
                        {
                            baseData["fx"] = baseSingleData;
                        }
                        // Serial.println("fx: "+String(baseData["fx"]));
                        baseX = true;
                        dataString = "";
                    }

                    else if (!baseY)
                    {
                        if (!pressed)
                        {
                            baseData["fy"] = baseSingleData;
                        }
                        baseData["type"] = "values";
                        baseY = true;
                        dataString = "";
                    }
                    else if (!baseR)
                    {
                        baseData["fr"] = baseSingleData / 4;
                        // posR = baseData["fr"];
                        // Serial.println("posR: " + String(posR));
                        baseData["type"] = "drive";
                        baseR = true;
                        isDataDigit = true; // redundant
                        dataString = "base";
                    }
                }
                else
                {
                    dataString += remoteSingleData;
                }
            }
            else
            {
                // Serial.println("f:"+remoteSingleData);
                dataString += remoteSingleData;
            }
        }
    }
}

void setSerialReciever(void (*f)(String), String type)
{
    serialHandleType[type] = serialHandlerIndex;
    Serial.println("HandlerIndex: " + serialHandlerIndex);
    Serial.println("Type: " + type);
    serialDataRecieve[serialHandlerIndex++] = f;
}

void setBaseReciever(void (*f)(JSONVar), String type)
{
    serialHandleType[type] = 0;
    Serial.println("HandlerIndex: " + 0);
    Serial.println("Type: " + type);
    baseDataRecieve[0] = f;
}