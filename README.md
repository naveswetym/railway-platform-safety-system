🚆 Railway Platform Safety System

An Arduino-based smart safety system that monitors railway platform danger zones, counts passengers, and automatically triggers alerts + gate control when a person or object gets dangerously close to the track edge.

> Simulated using **Cirkit Designer IDE**

📌 Problem Statement

Railway platform accidents often occur due to passengers standing too close to the track edge. This system provides an automated, real-time safety mechanism to detect danger and alert both passengers and station staff instantly.

 ⚙️ How It Works

1. **Ultrasonic Sensor** continuously monitors the distance between the platform edge and nearby objects/persons
2. If an object comes within **20 cm** of the track edge:
   - 🔴 Red LED turns ON
   - 🔊 Buzzer sounds at 1kHz
   - 🚧 Servo gate closes automatically
   - LCD displays **"!! DANGER !!"**
3. When the zone is clear:
   - 🟢 Green LED turns ON
   - Gate opens
   - LCD displays **"SAFE"**
4. **IR Break-Beam Sensors** at entry and exit count the number of passengers on the platform in real time — displayed on the LCD

 🔧 Components Used

| Component | Purpose |
|---|---|
| Arduino UNO | Main microcontroller |
| HC-SR04 Ultrasonic Sensor | Track proximity detection |
| IR Proximity Sensor (x2) | Entry & exit passenger counting |
| SG90 Servo Motor | Automated safety gate |
| 16x2 I2C LCD Display | Status messages & people count |
| Red LED | Danger indicator |
| Green LED | Safe indicator |
| Buzzer | Audio alert |

 🗺️ Pin Configuration

| Pin | Component |
|---|---|
| 9 | Ultrasonic Trig |
| 10 | Ultrasonic Echo |
| 8 | Buzzer |
| 6 | Red LED |
| 7 | Green LED |
| 2 | Entry IR Sensor |
| 3 | Exit IR Sensor |
| 5 | Servo Motor |
| SDA/SCL | I2C LCD (address 0x27) |

💻 Simulation

This project was built and tested on **Cirkit Designer IDE** — an online circuit simulation platform.

📚 Libraries Required

- `Servo.h` — built into Arduino IDE
- `Wire.h` — built into Arduino IDE
- `LiquidCrystal_I2C` — install via Arduino Library Manager

 👤 Author

**Naveswety M**  
B.E. – Electronics and Communication Engineering  
Sathyabama Institute of Science and Technology, Chennai  

📄 License

This project is open source and free to use for educational purposes.
