#include <Arduino.h>

#define RXD1 20
#define TXD1 21
#define NUM_MODULES 2

int m_id=0;

 int MODULE_ID = 1;

unsigned long lastPacketTime[NUM_MODULES + 1];
unsigned long currenttime=0;
unsigned long moduletime=0;

float v1 = 3.5;
float v2 = 3.55;
float v3 = 3.65;
float i  = 1.4;
float t  = 70.0;

unsigned long lasttime = 0;
unsigned long lasttimet = 0;

void setup() {
  Serial.begin(115200);
  delay(2000);

  pinMode(TXD1, OUTPUT);
  digitalWrite(TXD1, HIGH);

  pinMode(RXD1, INPUT_PULLUP);

  delay(100);

  Serial1.begin(115200, SERIAL_8N1, RXD1, TXD1);

  Serial.println("ESP32-C3");
  for (int i = 1; i <= NUM_MODULES; i++) {
  lastPacketTime[i] = millis();
}
}

void loop() {
  unsigned long currenttime = millis();

  if (currenttime - lasttime >= 500) {
    lasttime = currenttime;

    String d =String(MODULE_ID) + "," + String(v1, 2) + "," + String(v2, 2) + "," + String(v3, 2) + "," + String(i, 2);

    Serial.println(d);

    Serial1.println(d);
  }

  if (currenttime - lasttimet >= 5000) {
    lasttimet = currenttime;

    String d = String(MODULE_ID) + "," + String(v1, 2) + "," + String(v2, 2) + "," + String(v3, 2) + "," + String(i, 2) + "," + String(t, 2);

    Serial.println("data sent: " + d);

    Serial1.println(d);
  }

  while (Serial1.available() > 0) {
    currenttime=millis();
    String c = Serial1.readStringUntil('\n');
    c.trim();

    if (c.length() > 0) {
      Serial.println("message recieved: " + c);
      int fc= c.indexOf(',');
      int id=c.substring(0,fc).toInt();
      lastPacketTime[int(id)]=millis();


    }
  
  }

    for (int i = 1; i <= NUM_MODULES; i++) {
    if (i==MODULE_ID)
    {
      continue;
    }
    if (millis() - lastPacketTime[i] > 3000) {
      
        Serial.printf(
          "MODULE %d FAILED: COMMUNICATION LOST (>3000 ms)\n",
          i
        );

        Serial1.println("Module "+String(i)+"failed ,"+"0");
        Serial.println("Module "+String(i)+"failed ,"+"0");

        m_id= i;
}




    }
        if(t>60 && m_id==2)
    {
      Serial.println("temp exceeded");
    }

  }