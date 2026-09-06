#define BLYNK_TEMPLATE_ID "Template ID"
#define BLYNK_TEMPLATE_NAME "Medical Maintenance"
#define BLYNK_AUTH_TOKEN "Token No"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// ----------- WIFI -----------
char ssid[] = "Project";
char pass[] = "xxxxxxxx";

// ----------- PINS -----------
#define IN1 26
#define IN2 27
#define ENA 25

#define CURRENT_PIN 34
#define VIBRATION_PIN 14

#define BUZZER 13
#define RED_LED 32
#define GREEN_LED 33

#define DHTPIN 4
#define DHTTYPE DHT11

// ----------- OBJECTS -----------
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// ----------- LIMITS -----------
float CURRENT_LIMIT = 6.0;
float TEMP_LIMIT = 31.0;

// ----------- VARIABLES -----------
float current = 0;
float temperature = 0;
int vibrationState = 0;

bool fault = false;
bool faultLatched = false;
bool motorCommand = false;

String faultMessage = "";
String latchedFaultMessage = "";   // ✅ stores fault permanently

// ----------- CURRENT SENSOR -----------
float offsetVoltage = 0;
float sensitivity = 0.185;

// ----------- MOTOR -----------
void startMotor() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(ENA, HIGH);
}

void stopMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(ENA, LOW);
}

// ----------- CALIBRATION -----------
void calibrateCurrentSensor() {
  float sum = 0;
  for (int i = 0; i < 200; i++) {
    float v = analogRead(CURRENT_PIN) * (3.3 / 4095.0);
    sum += v;
    delay(5);
  }
  offsetVoltage = (sum / 200.0) / 0.647;
}

// ----------- CURRENT READ -----------
float readCurrent() {
  float sum = 0;
  for (int i = 0; i < 50; i++) {
    float v = analogRead(CURRENT_PIN) * (3.3 / 4095.0);
    sum += v;
  }

  float voltage = (sum / 50.0) / 0.647;
  return abs((voltage - offsetVoltage) / sensitivity);
}

// ----------- BLYNK BUTTON -----------
BLYNK_WRITE(V0) {
  int value = param.asInt();

  if (value == 1) {
    motorCommand = true;
    faultLatched = false;
    latchedFaultMessage = "";   // ✅ reset fault manually
  } else {
    motorCommand = false;
  }
}

// ----------- MAIN LOGIC -----------
void sendData() {

  // READ SENSORS
  current = readCurrent();

  float t = dht.readTemperature();
  if (!isnan(t)) temperature = t;

  vibrationState = digitalRead(VIBRATION_PIN);

  // ----------- FAULT DETECTION -----------
  fault = false;
  faultMessage = "";

  if (temperature > TEMP_LIMIT) {
    fault = true;
    faultMessage = "HIGH TEMP";
  }

  if (current > CURRENT_LIMIT) {
    fault = true;
    faultMessage = "OVER CURRENT";
  }

  if (vibrationState == HIGH) {
    fault = true;
    faultMessage = "VIBRATION";
  }

  // ✅ LATCH FAULT (only first time)
  if (fault && !faultLatched) {
    faultLatched = true;
    latchedFaultMessage = faultMessage;
  }

  // ----------- ACTION -----------
  if (faultLatched) {

    stopMotor();

    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);

    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
    delay(200);

    // 🔴 SHOW STORED FAULT (DOES NOT DISAPPEAR)
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FAULT:");
    lcd.setCursor(0, 1);
    lcd.print(latchedFaultMessage);

    // 🔴 VIBRATION LED (V3 stays ON)
    if (latchedFaultMessage == "VIBRATION") {
      Blynk.virtualWrite(V3, 1);
    } else {
      Blynk.virtualWrite(V3, 0);
    }

    Blynk.virtualWrite(V4, "FAULT: " + latchedFaultMessage);
  } 
  else {

    if (motorCommand) {
      startMotor();
      digitalWrite(GREEN_LED, HIGH);
    } else {
      stopMotor();
      digitalWrite(GREEN_LED, LOW);
    }

    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(temperature);
    lcd.print("C ");

    lcd.print("I:");
    lcd.print(current);

    lcd.setCursor(0, 1);
    if (!motorCommand) {
      lcd.print("Status: STOPPED ");
      Blynk.virtualWrite(V4, "STOPPED");
    } else {
      lcd.print("Status: RUNNING ");
      Blynk.virtualWrite(V4, "RUNNING");
    }

    // 🟢 NORMAL VIBRATION STATE
    if (vibrationState == HIGH) {
      Blynk.virtualWrite(V3, 1);
    } else {
      Blynk.virtualWrite(V3, 0);
    }
  }

  // SEND DATA
  Blynk.virtualWrite(V1, temperature);
  Blynk.virtualWrite(V2, current);
}

// ----------- SETUP -----------
void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  pinMode(VIBRATION_PIN, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  lcd.init();
  lcd.backlight();

  dht.begin();

  stopMotor();
  delay(2000);
  calibrateCurrentSensor();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(1000L, sendData);
}

// ----------- LOOP -----------
void loop() {
  Blynk.run();
  timer.run();
}