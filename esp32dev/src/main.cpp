#include <Arduino.h>

#define RXD2 21
#define TXD2 19

#define NUM_MODULES 2

String module_id="2";

const unsigned long MODULE_TIMEOUT = 3000;

const float thresholdv = 4.0;
const float thresholdi = 2.0;
const float thresholdt = 45.0;
const float max_delta_v = 0.1;

unsigned long lastPacketTime[NUM_MODULES + 1];
bool moduleFailed[NUM_MODULES + 1] = {false};

float getMaxDelta(float v1, float v2, float v3) {
  float maxV = max(v1, max(v2, v3));
  float minV = min(v1, min(v2, v3));
  return maxV - minV;
}

void setup() {
  Serial.begin(115200);

  pinMode(TXD2, OUTPUT);
  digitalWrite(TXD2, HIGH);

  delay(100);

  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

  Serial.println("ESP32 DEVKIT");

  for (int i = 1; i <= NUM_MODULES; i++) {
    lastPacketTime[i] = millis();
  }

}

void loop() {
  while (Serial2.available() > 0) {
    String c = Serial2.readStringUntil('\n');
    c.trim();

    if (c.length() == 0) {
      continue;
    }

    int comma = 0;

    for (int i = 0; i < c.length(); i++) {
      if (c.charAt(i) == ',') {
        comma++;
      }
    }

    if (comma == 4) {
      int fc = c.indexOf(',');
      int sc = c.indexOf(',', fc + 1);
      int tc = c.indexOf(',', sc + 1);
      int foc = c.indexOf(',', tc + 1);

      int module = c.substring(0, fc).toInt();

      if (module < 1 || module > NUM_MODULES) {
        Serial.printf("ERROR: Invalid Module ID: %d\n", module);
        continue;
      }

      lastPacketTime[module] = millis();

      if (moduleFailed[module]) {
        Serial.printf("MODULE %d: COMMUNICATION RESTORED\n", module);
        moduleFailed[module] = false;
      }

      float v1 = c.substring(fc + 1, sc).toFloat();
      float v2 = c.substring(sc + 1, tc).toFloat();
      float v3 = c.substring(tc + 1, foc).toFloat();
      float current = c.substring(foc + 1).toFloat();

      Serial.printf(
        "Module %d - V1: %.2f, V2: %.2f, V3: %.2f, I: %.2f\n",
        module,
        v1,
        v2,
        v3,
        current
      );

      if (
        v1 <= thresholdv &&
        v2 <= thresholdv &&
        v3 <= thresholdv &&
        current <= thresholdi
      ) {
        if (getMaxDelta(v1, v2, v3) > max_delta_v) {
          Serial2.println(module_id+","+"0");
          Serial.printf(
            "MODULE %d : Cell Imbalance Detected\n",
            module
          );
        } else {
          Serial2.println(module_id+","+"1");
          Serial.printf(
            "MODULE %d STATUS: reciveng/sending\n",
            module
          );
        }
      } else {
        Serial2.println(module_id+","+"0");
        Serial.printf(
          "MODULE %d FAULT: Threshold Exceeded\n",
          module
        );
      }
    }

    else if (comma == 5) {
      int fc = c.indexOf(',');
      int sc = c.indexOf(',', fc + 1);
      int tc = c.indexOf(',', sc + 1);
      int foc = c.indexOf(',', tc + 1);
      int fic = c.indexOf(',', foc + 1);

      int module = c.substring(0, fc).toInt();

      if (module < 1 || module > NUM_MODULES) {
        Serial.printf("ERROR: Invalid Module ID: %d\n", module);
        continue;
      }

      lastPacketTime[module] = millis();

      if (moduleFailed[module]) {
        Serial.printf("MODULE %d: COMMUNICATION RESTORED\n", module);
        moduleFailed[module] = false;
      }

      float v1 = c.substring(fc + 1, sc).toFloat();
      float v2 = c.substring(sc + 1, tc).toFloat();
      float v3 = c.substring(tc + 1, foc).toFloat();
      float current = c.substring(foc + 1, fic).toFloat();
      float temperature = c.substring(fic + 1).toFloat();

      Serial.printf(
        "Module %d -> V1: %.2f, V2: %.2f, V3: %.2f, I: %.2f, T: %.2f C\n",
        module,
        v1,
        v2,
        v3,
        current,
        temperature
      );

      if (
        v1 <= thresholdv &&
        v2 <= thresholdv &&
        v3 <= thresholdv &&
        current <= thresholdi &&
        temperature < thresholdt
      ) {
        if (getMaxDelta(v1, v2, v3) > max_delta_v) {
          Serial2.println(module_id+","+"0");
          Serial.printf(
            "MODULE %d FAULT: Cell Imbalance Detected\n",
            module
          );
        } else {
          Serial2.println(module_id+","+"1");
          Serial.printf(
            "MODULE %d STATUS: OK\n",
            module
          );
        }
      } else {
        Serial2.println("0");
        Serial.printf(
          "MODULE %d FAULT: Threshold Exceeded\n",
          module
        );
      }
    }
  }

  for (int i = 1; i <= NUM_MODULES; i++) {
    if (i==module_id.toInt())
    {
      continue;
    }
    if (millis() - lastPacketTime[i] > MODULE_TIMEOUT) {
      if (!moduleFailed[i]) {
        Serial.println(
          "MODULE" +String(i+1)+" FAILED"
        );

        Serial2.println("Module"+module_id+"failed ,"+"0");

        moduleFailed[i] = true;
      }
    }
  }
}