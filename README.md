# esp32_lasertagtank
This project brings the **classic AZ Tank Game** to real life using physical robots powered by **ESP32**. Each robot is equipped with:
- Class IIIa laser with PWM control
- LM393 photoresistor modules to detect laser hits
- Bluetooth control via PS3 controller
- OLED screen to display in-game status
- Dual N20 motors for movement (via TB6612FNG driver)

> The goal: Outshoot your opponent by navigating a maze, bouncing lasers off mirrors, and scoring precision hits!

---

## 🧠 How This Project Works

This real-life AZ Tank Game is a laser-tag-inspired robot battle system, where each tank is controlled and interacts with the environment through the following components:

- 🎮 **Bluetooth Controller Input**  
  Each robot connects wirelessly to a PS3 controller via Bluetooth. Players can control the tank’s actions in real time.

- 🔫 **Laser Firing System**  
  The robot features a Class IIIa 5mW laser that operates in two modes:  
  - **Idle Mode (2.3V):** Laser stays dim to indicate readiness.  
  - **Fire Mode (3.2V):** Laser brightens when shooting, controlled via PWM and a MOSFET.

- 👁️ **Laser Hit Detection**  
  Two LM393 photoresistor modules monitor light intensity. When a strong laser hit is detected, the robot registers it as a "hit."

- 📺 **OLED Display Feedback**  
  A 0.96” OLED screen shows real-time status such as score updates and game-over messages like “You lose.”

- 🧱 **Game Arena and Map**  
  The game is played in a 70cm x 30cm maze with reflective mirrors, enabling players to bounce lasers off walls for tactical shots.

---

⚠️ **Note:**  
This codebase is primarily for **testing basic hardware functions** of the robot (laser control, hit detection, OLED display, controller input).  
You're welcome to contribute — feel free to fork the repo and suggest improvements or add new gameplay features!

---

## 🎥 Demo
https://youtu.be/T_IwSI9VMSY
