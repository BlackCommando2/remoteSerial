Stream *remoteSerial;
char remoteSingleData;
bool isDataDigit = false, _initData = false;
String dataString = "";
bool baseX = false, baseY = false, baseR = false;
bool pressed = false;
JSONVar baseData;
JSONVar stringHandleType;
JSONVar jsonHandleType;
int stringHandlerIndex = 1;
int jsonHandlerIndex = 1;
int baseSingleData = 0;
int maxHandlers = 50;
bool defautlCheck = false;
String allKeys[50];
void (*serialDataRecieve[25])(String);
void (*baseDataRecieve[25])(JSONVar);

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
            int typeIndex = jsonHandleType[dataString];
            // Serial.println("jsonIndex: "+(String)typeIndex);
            if (typeIndex != 0)
            {
                baseDataRecieve[typeIndex](baseData);
            }
            else
            {
                typeIndex = stringHandleType[dataString]; 
                // Serial.println("StringIndex: "+(String)typeIndex);
                // typeIndex = typeIndex == -1? 0 : typeIndex;
                if(typeIndex != 0)
                {
                    // int typeIndex = stringHandleType[dataString];
                    // typeIndex = typeIndex == -1? 0 : typeIndex;
                    serialDataRecieve[typeIndex](dataString);
                }
                else
                {
                    typeIndex = typeIndex == -1? 0 : typeIndex;
                    serialDataRecieve[typeIndex](dataString);
                }
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
    // Serial.println("json: "+(String)(int)jsonHandleType[type] + " "+type);
    // Serial.println("string: "+(String)(int)stringHandleType[type] + " "+type);
    if((int)jsonHandleType[type]!=0 && (int)stringHandleType[type]!=0)
    {
        Serial.println("Error: Can't have two same type: " + type);
    }
    else
    {
        stringHandleType[type] = stringHandlerIndex;
        Serial.println("HandlerIndex: " + (String)stringHandlerIndex);
        Serial.println("Type: " + type);
        serialDataRecieve[stringHandlerIndex++] = f;
    }
}

void setSerialReciever(void (*f)(String))
{
    int defaultIndex = 0;
    stringHandleType["string"] = defaultIndex;
    Serial.println("HandlerIndex: " + (String)defaultIndex);
    // Serial.println("Type: " + type);
    Serial.println("Type: string");
    serialDataRecieve[defaultIndex] = f;
}

void setSerialReciever(void (*f)(JSONVar))
{
    int defaultIndex = 0;
    stringHandleType["string"] = defaultIndex;
    Serial.println("HandlerIndex: " + (String)defaultIndex);
    // Serial.println("Type: " + type);
    Serial.println("Type: string");
    baseDataRecieve[defaultIndex] = f;
}

void setSerialReciever(void (*f)(JSONVar), String type)
{
    // Serial.println("SERialjson: "+(String)(int)jsonHandleType[type] + " "+type);
    // Serial.println("SERialstring: "+(String)(int)stringHandleType[type] + " "+type);
    if((int)jsonHandleType[type]!=0 && (int)stringHandleType[type]!=0)
    {
        Serial.println("Error: Can't have two same type: " + type);
    }
    else
    {
        jsonHandleType[type] = jsonHandlerIndex;
        Serial.println("BaseHandlerIndex: " + (String)jsonHandlerIndex);
        Serial.println("Type: " + type);
        baseDataRecieve[jsonHandlerIndex++] = f;
    }
}