#include <Ps3Controller.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

// Define motor driver pins
#define PWMA_PIN 15
#define AIN2_PIN 2
#define AIN1_PIN 4
#define BIN1_PIN 17
#define BIN2_PIN 5
#define PWMB_PIN 18


// Define laser control pin
#define LASER_PIN 14

// Define LDR sensor and buzzer pins
#define LDR_PIN 13
#define BUZZER_PIN 12

// OLED display setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Define PWM Parameters
const int motorFreq = 1000;
const int motorResolution = 8;
const int laserChannel = 5;
const int motorAChannel = 3;
const int motorBChannel = 4;

// Robot health system
int health = 10;
unsigned long lastDamageTime = 0;
unsigned long reviveTime = 0;
const int reviveDelay = 10000; // 10s
bool isReviving = false;

// Variables for laser
unsigned long lastShotTime = 0;
const int laserCooldown = 2000; // 2s cooldown
const int laserShotDuration = 2000; // 2s shot duration
bool laserFiring = false;

// Variables for standby toggle

// Variables for motor
int motorAPWM = 0;
int motorBPWM = 0;
bool motorDir = true;

// D-pad control state
bool dpadUp = false, dpadDown = false, dpadLeft = false, dpadRight = false;

// Function prototypes
void moveMotors(int mtrAspeed, int mtrBspeed, bool mtrdir);
void updateDisplay();
void handleLaser();
void handleRevive();
void handleLDR();


void setup() {
  
  // Setup Serial Monitor
  Serial.begin(115200);

  // PS3 controller setup
  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  Ps3.begin("a8:42:e3:bc:58:d2");

  // Pin setup
  pinMode(PWMA_PIN, OUTPUT);
  pinMode(PWMB_PIN, OUTPUT);
  pinMode(AIN1_PIN, OUTPUT);
  pinMode(AIN2_PIN, OUTPUT);
  pinMode(BIN1_PIN, OUTPUT);
  pinMode(BIN2_PIN, OUTPUT);
  pinMode(LASER_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  

  // PWM setup
  ledcSetup(motorAChannel, motorFreq, motorResolution);
  ledcSetup(motorBChannel, motorFreq, motorResolution);
  ledcSetup(laserChannel, motorFreq, motorResolution);

  ledcAttachPin(PWMA_PIN, motorAChannel);
  ledcAttachPin(PWMB_PIN, motorBChannel);
  ledcAttachPin(LASER_PIN, laserChannel);

  // Laser idle state
  ledcWrite(laserChannel, 20);

  // OLED setup
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  updateDisplay();

  // Print to Serial Monitor
  Serial.println("Ready.");
}

void onConnect() {
  // Print to Serial Monitor
  Serial.println("Connected.");
}

void loop() {
  
  if (!Ps3.isConnected()) return;

  handleLaser();
  handleRevive();
  handleLDR();
  updateDisplay();

}

void notify() {


  // D-pad controls
  dpadUp = Ps3.data.button.up;
  dpadDown = Ps3.data.button.down;
  dpadLeft = Ps3.data.button.left;
  dpadRight = Ps3.data.button.right;

  // Handle motor movement
  if (dpadUp) {
    motorDir = true;
    moveMotors(200, 200, motorDir);
  } else if (dpadDown) {
    motorDir = false;
    moveMotors(200, 200, motorDir);
  } else if (dpadLeft) {
    moveMotors(100, 200, motorDir);
  } else if (dpadRight) {
    moveMotors(200, 100, motorDir);
  } else {
    moveMotors(0, 0, motorDir);
  }

  // Handle laser shooting
  if (Ps3.data.button.circle && !laserFiring && (millis() - lastShotTime > laserCooldown)) {
    laserFiring = true;
    lastShotTime = millis();
  }
}

void moveMotors(int mtrAspeed, int mtrBspeed, bool mtrdir) {
  // Set motor directions
  if (mtrdir) {
    digitalWrite(AIN1_PIN, LOW);
    digitalWrite(AIN2_PIN, HIGH);
    digitalWrite(BIN1_PIN, LOW);
    digitalWrite(BIN2_PIN, HIGH);
  } else {
    digitalWrite(AIN1_PIN, HIGH);
    digitalWrite(AIN2_PIN, LOW);
    digitalWrite(BIN1_PIN, HIGH);
    digitalWrite(BIN2_PIN, LOW);
  }

  // Drive motors
  ledcWrite(motorAChannel, mtrAspeed);
  ledcWrite(motorBChannel, mtrBspeed);
}


void handleLaser() {
  if (laserFiring) {
    if (millis() - lastShotTime <= laserShotDuration) {
      ledcWrite(laserChannel, 255); // Full power
    } else {
      laserFiring = false;
      ledcWrite(laserChannel, 20); // Back to idle
    }
  }
}

void handleRevive() {
  if (!isReviving) return;

  if (millis() - reviveTime < reviveDelay) {
    int countdown = (reviveDelay - (millis() - reviveTime)) / 1000;
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(15, 15);
    display.print("Reviving:");
    display.setCursor(50, 40);
    display.print(countdown);
    display.display();
  } else {
    health = 10;
    isReviving = false;
  }
}

void handleLDR() {
  if (isReviving) return; // Ignore LDR if reviving

  int sensorValue = analogRead(LDR_PIN);
  if (sensorValue > 3500 && millis() - lastDamageTime > 500) { // Debounce for damage
    lastDamageTime = millis();
    health--;

    digitalWrite(BUZZER_PIN, HIGH); // Activate buzzer
    delay(200);                    // Short beep
    digitalWrite(BUZZER_PIN, LOW);

    if (health <= 0) {
      isReviving = true;
      reviveTime = millis();
    }
  }
}

void updateDisplay() {
  if (isReviving) return; // Don't update if reviving

  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(40, 0);
  if (health > 0) {
    display.print(":)");
  } else {
    display.print("X(");
  }

  display.setTextSize(2);
  display.setCursor(20, 40);
  display.print("HP: ");
  display.print(health);

  display.display();
}
