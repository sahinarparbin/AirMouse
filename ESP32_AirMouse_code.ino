#include <BleMouse.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define LEFTBUTTON 19
#define RIGHTBUTTON 18
#define SPEED 10

Adafruit_MPU6050 mpu;
BleMouse bleMouse("ESP32_AirMouse", "SahinaTech", 100);

void setup() {
  Serial.begin(115200);

  pinMode(LEFTBUTTON, INPUT_PULLUP);
  pinMode(RIGHTBUTTON, INPUT_PULLUP);

  // Start BLE mouse
  bleMouse.begin();

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  // Wake up MPU6050
  mpu.enableSleep(false);
}

void loop() {
  if (bleMouse.isConnected()) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Debug: print gyro values
    Serial.print("Gyro Z: "); Serial.print(g.gyro.z);
    Serial.print(" | Gyro X: "); Serial.println(g.gyro.x);

    // Apply scaling and send movement
    int dx = (int)(g.gyro.z * -SPEED * 10); // X-axis of mouse = Z of gyro
    int dy = (int)(g.gyro.x * -SPEED * 10); // Y-axis of mouse = X of gyro

    // Only move if there's meaningful change
    if (abs(dx) > 1 || abs(dy) > 1) {
      bleMouse.move(dx, dy);
    }

    // Left click
    if (!digitalRead(LEFTBUTTON)) {
      Serial.println("Left click");
      bleMouse.click(MOUSE_LEFT);
      delay(300);
    }

    // Right click
    if (!digitalRead(RIGHTBUTTON)) {
      Serial.println("Right click");
      bleMouse.click(MOUSE_RIGHT);
      delay(300);
    }
  }

  delay(20); // loop smoothing
}
