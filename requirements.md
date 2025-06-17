# 📦 Project Requirements

This project includes both software and hardware components for a laser-tag-style robot game powered by ESP32.

---

## 🔩 Hardware List

| Component             | Notes                                 |
|----------------------|----------------------------------------|
| ESP32 Devkit V1       | Main controller board (Wi-Fi + BT)     |
| TB6612FNG Driver      | Dual DC motor driver for N20 motors    |
| 2x N20 Gear Motors    | Mobility                                |
| SSD1306 OLED (0.96")  | 128x64 I2C screen for game status       |
| LM393 LDR Module      | Laser hit detection                     |
| IRF520 MOSFET Module  | Controls laser via PWM                 |
| Class IIIa 5mW Laser  | Fireable laser weapon                   |
| Passive Buzzer + 220Ω | Audio feedback on hit                  |
| Mini Buck Converter   | Steps down 7.4V battery to 5V          |
| 7.4V Li-ion Battery   | Main power source                      |
| SPDT Switch           | On/off power control                   |

---

## 💻 Software

### Libraries (Arduino/PlatformIO):

Install via Library Manager or add to `platformio.ini`:
```ini
Adafruit_SSD1306
Adafruit_GFX
Ps3Controller
