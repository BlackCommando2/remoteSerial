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
int maxHandlers = 50;
void (*serialDataRecieve[50])(String);
void (*baseDataRecieve[1])(JSONVar);

void registerSerial(Stream *serial)
{
    remoteSerial = serial;
}

void serialListener()
{
    if(remoteSerial->available())
    {
        remoteSingleData = (char)remoteSerial->read();
        if (remoteSingleData == '\n')
        {
            if (dataString == "base")
            {
                baseDataRecieve[0](baseData);
            }
            else if(dataString != "base")
            {
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
                dataString += remoteSingleData;
            }
        }
    }
}

void setSerialReciever(void (*f)(String), String type)
{
    serialHandleType[type] = serialHandlerIndex;
    Serial.println("HandlerIndex: " + (String)serialHandlerIndex);
    Serial.println("Type: " + type);
    serialDataRecieve[serialHandlerIndex++] = f;
}

void setBaseReciever(void (*f)(JSONVar), String type)
{
    Serial.println("BaseHandlerIndex: " + (String)0);
    Serial.println("Type: " + type);
    baseDataRecieve[0] = f;
}