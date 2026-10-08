#include <Arduino.h>
#include <OneButton.h>

#ifndef LED1_PIN
#define LED1_PIN 2 // Built-in Blue LED trên DOIT ESP32 DevKit V1
#endif

#ifndef LED2_PIN
#define LED2_PIN 5 // LED ngoai gan tren test board
#endif

#ifndef BTN_PIN
#define BTN_PIN 4  // Nut nhan ngoai gan tren test board
#endif

// Khoi tao OneButton: chan BTN_PIN, active LOW = true (INPUT_PULLUP)
OneButton button(BTN_PIN, true);

// Bien xac dinh LED dang duoc dieu khien (1: LED1 builtin, 2: LED2 ngoai)
int selectedLED = 1;

// Trang thai cua tung LED
bool led1State = false;
bool led1Blinking = false;
unsigned long led1PrevMillis = 0;

bool led2State = false;
bool led2Blinking = false;
unsigned long led2PrevMillis = 0;

const unsigned long blinkInterval = 200; // Nhap nhay chu ky 200ms

// Double click: Chuyen che do dieu khien giua hai LED (LED1 <-> LED2)
void handleDoubleClick() {
    selectedLED = (selectedLED == 1) ? 2 : 1;
    Serial.print("[CHUYEN CHE DO] Dang chon dieu khien LED: ");
    Serial.println(selectedLED == 1 ? "LED 1 (Built-in GPIO 2)" : "LED 2 (Ngoai GPIO 5)");
}

// Single click: Bat / Tat LED dang duoc chon
void handleClick() {
    if (selectedLED == 1) {
        if (led1Blinking) {
            led1Blinking = false;
            led1State = false;
        } else {
            led1State = !led1State;
        }
        digitalWrite(LED1_PIN, led1State ? HIGH : LOW);
        Serial.print("[DIEU KHIEN LED 1] Trang thai: ");
        Serial.println(led1State ? "ON" : "OFF");
    } else {
        if (led2Blinking) {
            led2Blinking = false;
            led2State = false;
        } else {
            led2State = !led2State;
        }
        digitalWrite(LED2_PIN, led2State ? HIGH : LOW);
        Serial.print("[DIEU KHIEN LED 2] Trang thai: ");
        Serial.println(led2State ? "ON" : "OFF");
    }
}

// Long press (>1s): Kich hoat che do nhap nhay 200ms cho LED dang duoc chon
void handleLongPress() {
    if (selectedLED == 1) {
        led1Blinking = !led1Blinking;
        if (!led1Blinking) {
            led1State = false;
            digitalWrite(LED1_PIN, LOW);
        }
        Serial.print("[GIU NUT] LED 1 nhap nhay 200ms: ");
        Serial.println(led1Blinking ? "BAT" : "TAT");
    } else {
        led2Blinking = !led2Blinking;
        if (!led2Blinking) {
            led2State = false;
            digitalWrite(LED2_PIN, LOW);
        }
        Serial.print("[GIU NUT] LED 2 nhap nhay 200ms: ");
        Serial.println(led2Blinking ? "BAT" : "TAT");
    }
}

void setup() {
    Serial.begin(115200);

    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);

    // Thiet lap thoi gian nhan giu (long press) la 1000ms
    button.setPressTicks(1000);

    // Gan su kien cho nut nhan
    button.attachClick(handleClick);
    button.attachDoubleClick(handleDoubleClick);
    button.attachLongPressStart(handleLongPress);

    Serial.println("=================================================");
    Serial.println("ESP32 DevKit V1 - OneButton Dual LED Controller");
    Serial.println("LED 1: Built-in LED (GPIO 2)");
    Serial.println("LED 2: External LED (GPIO 5)");
    Serial.println("Button: External PushButton (GPIO 4)");
    Serial.println("-------------------------------------------------");
    Serial.println("Thao tac:");
    Serial.println("1. Double Click : Chuyen doi dieu khien giua LED1 va LED2");
    Serial.println("2. Single Click : Bat/Tat LED dang chon");
    Serial.println("3. Long Press   : Nhap nhay LED dang chon (200ms)");
    Serial.println("=================================================");
}

void loop() {
    // Luon goi tick() de thu vien giai ma trang thai phim
    button.tick();

    unsigned long currentMillis = millis();

    // Xu ly nhay LED 1 (non-blocking)
    if (led1Blinking) {
        if (currentMillis - led1PrevMillis >= blinkInterval) {
            led1PrevMillis = currentMillis;
            led1State = !led1State;
            digitalWrite(LED1_PIN, led1State ? HIGH : LOW);
        }
    }

    // Xu ly nhay LED 2 (non-blocking)
    if (led2Blinking) {
        if (currentMillis - led2PrevMillis >= blinkInterval) {
            led2PrevMillis = currentMillis;
            led2State = !led2State;
            digitalWrite(LED2_PIN, led2State ? HIGH : LOW);
        }
    }
}
