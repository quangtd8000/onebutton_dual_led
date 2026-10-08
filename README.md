# ESP32 OneButton Dual LED Controller (Week 3 - PHY3640)

Dự án mở rộng điều khiển hai LED độc lập bằng một nút nhấn duy nhất sử dụng thư viện **OneButton** trên kit vi điều khiển **DOIT ESP32 DevKit V1** (thuộc học phần *Phát triển ứng dụng IoT - PHY3640*, Khoa Vật lý - ĐHKHTN, ĐHQGHN).

## 1. Yêu cầu chức năng
- **Double Click**: Chuyển đổi chế độ điều khiển giữa LED 1 (Built-in LED) và LED 2 (External LED).
- **Single Click**: Bật / Tắt LED đang được chọn. Nếu LED đang chọn đang ở trạng thái nhấp nháy, nhấn single click sẽ dừng nhấp nháy và tắt LED.
- **Long Press (Giữ nút > 1s)**: Kích hoạt chế độ nhấp nháy chu kỳ 200ms cho LED đang được chọn.
- **Thuật toán Non-blocking**: Toàn bộ thuật toán nhấp nháy được thực hiện bằng hàm `millis()`, đảm bảo không dùng `delay()` làm ảnh hưởng đến khả năng phản hồi của `button.tick()`.

## 2. Phần cứng và Sơ đồ nối dây
- **Board phát triển**: DOIT ESP32 DevKit V1 (30 chân, ESP32 Xtensa Dual-core).
- **LED 1 (Built-in)**: Chân **GPIO 2** tích hợp sẵn trên board (Blue LED, active HIGH).
- **LED 2 (External)**:
  - Anode (+) nối vào chân **GPIO 5** của ESP32 thông qua điện trở hạn dòng 220Ω hoặc 330Ω.
  - Cathode (-) nối vào chân **GND**.
- **Nút nhấn (External Push Button)**:
  - Một chân nối vào chân **GPIO 4** của ESP32.
  - Chân còn lại nối vào chân **GND**.
  - Sử dụng chế độ kéo lên nội (`INPUT_PULLUP`), active LOW.

## 3. Cấu hình PlatformIO (`platformio.ini`)
```ini
[env:esp32doit-devkit-v1]
platform = espressif32
board = esp32doit-devkit-v1
framework = arduino
monitor_speed = 115200
upload_speed = 921600
lib_deps =
    mathertel/OneButton @ ^2.6.1
build_flags =
    -D LED1_PIN=2
    -D LED2_PIN=5
    -D BTN_PIN=4
```

## 4. Hướng dẫn biên dịch và nạp code
1. Cài đặt VS Code và extension **PlatformIO IDE**.
2. Clone repository về máy tính:
   ```bash
   git clone https://github.com/quangtd8000/onebutton_dual_led.git
   ```
3. Mở thư mục dự án trong VS Code / PlatformIO.
4. Biên dịch mã nguồn:
   ```bash
   pio run
   ```
5. Kết nối ESP32 với máy tính qua cáp Micro-USB và nạp firmware:
   ```bash
   pio run -t upload
   ```
6. Mở Serial Monitor để quan sát log chuyển đổi chế độ và trạng thái LED:
   ```bash
   pio device monitor
   ```
