/*
 * ============================================================
 *   RAILWAY PLATFORM SAFETY SYSTEM
 * ============================================================
 * Author  : Naveswety M
 * College : Sathyabama Institute of Science and Technology
 * Simulation : Cirkit Designer IDE
 *
 * Description:
 *   An Arduino-based safety system for railway platforms that
 *   monitors the danger zone near the track using an ultrasonic
 *   sensor, counts passengers via IR break-beam sensors, and
 *   triggers visual + audio alerts with automatic gate control
 *   when a person/object gets too close to the track edge.
 *
 * Components Used:
 *   - Arduino UNO
 *   - HC-SR04 Ultrasonic Sensor (track proximity detection)
 *   - 2x IR Proximity Sensors (entry & exit passenger counting)
 *   - SG90 Servo Motor (automated safety gate)
 *   - 16x2 I2C LCD Display (status messages)
 *   - Red & Green LEDs (danger / safe indicator)
 *   - Buzzer (audio alert)
 *
 * Pin Configuration:
 *   Pin 9  → Ultrasonic Trig
 *   Pin 10 → Ultrasonic Echo
 *   Pin 8  → Buzzer
 *   Pin 6  → Red LED
 *   Pin 7  → Green LED
 *   Pin 2  → Entry IR Sensor
 *   Pin 3  → Exit IR Sensor
 *   Pin 5  → Servo Motor
 *   SDA/SCL → I2C LCD (0x27)
 * ============================================================
 */

#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ── Pin Definitions ──────────────────────────────────────────
#define trigPin   9    // Ultrasonic sensor trigger
#define echoPin   10   // Ultrasonic sensor echo
#define buzzer    8    // Buzzer for audio alert
#define redLED    6    // Red LED — danger indicator
#define greenLED  7    // Green LED — safe indicator
#define entryIR   2    // IR sensor at platform entry
#define exitIR    3    // IR sensor at platform exit

// ── Constants ────────────────────────────────────────────────
#define DANGER_DISTANCE_CM  20   // Trigger alert if object within 20 cm
#define GATE_CLOSED_ANGLE   90   // Servo angle when gate is closed
#define GATE_OPEN_ANGLE     0    // Servo angle when gate is open

// ── Object Declarations ──────────────────────────────────────
Servo gate;
LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD at I2C address 0x27, 16 cols x 2 rows

// ── Global Variables ─────────────────────────────────────────
int people    = 0;        // Current passenger count on platform
int prevEntry = HIGH;     // Previous state of entry IR sensor
int prevExit  = HIGH;     // Previous state of exit IR sensor

// ============================================================
void setup() {
  // Ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Output devices
  pinMode(redLED,   OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(buzzer,   OUTPUT);

  // IR break-beam sensors with internal pull-up resistors
  // Sensor reads LOW when beam is broken (person detected)
  pinMode(entryIR, INPUT_PULLUP);
  pinMode(exitIR,  INPUT_PULLUP);

  // Servo motor for gate control
  gate.attach(5);
  gate.write(GATE_OPEN_ANGLE);  // Start with gate open

  // LCD initialization
  lcd.init();
  lcd.backlight();

  Serial.begin(9600);
  Serial.println("Railway Platform Safety System Initialized.");
}

// ============================================================
void loop() {

  // ── STEP 1: Read IR Sensors for Passenger Counting ─────────
  int e = digitalRead(entryIR);
  int x = digitalRead(exitIR);

  // Increment count when entry beam is broken (HIGH → LOW transition)
  if (prevEntry == HIGH && e == LOW) {
    people++;
    delay(300);  // Debounce delay to avoid double counting
  }

  // Decrement count when exit beam is broken (ensure count never goes negative)
  if (prevExit == HIGH && x == LOW && people > 0) {
    people--;
    delay(300);
  }

  // Save current states for next loop comparison
  prevEntry = e;
  prevExit  = x;

  // ── STEP 2: Measure Distance with Ultrasonic Sensor ────────
  // Send a 10µs HIGH pulse to trigger ultrasonic burst
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure echo pulse duration and convert to distance (cm)
  // Formula: distance = (duration × speed of sound) / 2
  //          speed of sound ≈ 0.034 cm/µs
  long duration = pulseIn(echoPin, HIGH);
  int distance  = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // ── STEP 3: Evaluate Safety & Respond ──────────────────────
  if (distance < DANGER_DISTANCE_CM) {

    // DANGER: Object/person too close to track edge
    digitalWrite(redLED,   HIGH);
    digitalWrite(greenLED, LOW);
    tone(buzzer, 1000);           // Sound alarm at 1kHz
    gate.write(GATE_CLOSED_ANGLE); // Close safety gate

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("!! DANGER !!");

  } else {

    // SAFE: Platform clear
    digitalWrite(redLED,   LOW);
    digitalWrite(greenLED, HIGH);
    noTone(buzzer);               // Silence alarm
    gate.write(GATE_OPEN_ANGLE);  // Open gate

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   SAFE   ");
  }

  // ── STEP 4: Display Passenger Count on LCD ─────────────────
  lcd.setCursor(0, 1);
  lcd.print("People: ");
  lcd.print(people);
  lcd.print("   ");  // Clear leftover characters on LCD row

  delay(200);  // Short loop delay for stable sensor readings
}
