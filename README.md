## ConveyorX

### Overview

**ConveyorX** is a real-time embedded system developed on the **STM32F401VE** microcontroller. It monitors and measures the **speed** and **count** of objects on a conveyor belt using **Timer Input Capture**, **GPIO polling**, and **LCD display**. This system is built with a clear focus on real-time responsiveness and low-level embedded logic, and is fully compatible with both **Proteus simulation** and STM32 hardware boards.

It demonstrates critical embedded engineering concepts including:

- Timer-based measurement (TIM2 input capture)
- Poll-based object detection (IR)
- PWM signal control for motors (optional)
- LCD interfacing with real-time updates
- Interrupt handling for system safety
- Structured modular C programming and custom driver layering

![ConveyorX System Overview](https://github.com/user-attachments/assets/984412f8-db41-4e62-95c0-4b706ce76adc)

---

### Features

- High-precision **speed measurement** using TIM2 Input Capture
- Real-time **object counting** with IR sensor polling logic
- Integrated **emergency stop** and **reset** functions via EXTI
- Visual display on **16x2 character LCD**
- Structured modular drivers for RCC, GPIO, LCD, EXTI, ObjectDetection, and TIM2
- Fully tested in **Proteus simulation** and deployable on STM32 development boards

---

### Functional Summary

#### Inputs:
- **IR Sensor (PA2):** Detects object when it blocks the IR beam (active LOW logic)
- **Emergency Button (PC2):** Halts system updates and shows alert message
- **Reset Button (PC6):** Resets emergency flag and LCD display

#### Outputs:
- **LCD Display:**
  - **Line 0:** Shows object **Speed** in mm/s
  - **Line 1:** Shows object **Count** (number of objects passed)

---

### Functional Breakdown

#### ✅ Object Detection (IR Polling Logic)

- A simple polling routine is used to monitor the IR sensor connected to **PA2**
- When a **falling edge** is detected (from HIGH ➜ LOW), the object count is incremented
- The signal is **debounced** with a small delay to avoid false triggering

```c
void check_objects_count(uint32 *counter) {
    static uint8 last_pin_state = HIGH;
    uint8 current_pin_state = GPIO_ReadPin(ir_port, ir_pin);

    if (last_pin_state == HIGH && current_pin_state == LOW) {
        (*counter)++;
    }
    last_pin_state = current_pin_state;
}
````

#### ✅ Speed Measurement (TIM2 Input Capture)

* Configured on **PA0 (TIM2\_CH1)** to capture timestamps on **rising edges**
* Captures two successive time values (capture1, capture2) and calculates time difference
* Speed is calculated as:

  $$
  \text{Speed (mm/s)} = \frac{\text{Distance per object}}{\text{Time between two objects (µs)}}
  $$
* A filtering check is applied to discard invalid captures (e.g., noise or zero period)

#### ✅ LCD Display (HD44780 Driver, 4-bit Mode)

* Connected via GPIOB, the LCD is initialized in 4-bit mode
* Regular updates show:

  * Line 0 ➜ "Speed: <value> mm/s"
  * Line 1 ➜ "Count: <value>"
* LCD updates are **guarded against emergency flag** to ensure safety
* Functions used:

  ```c
  LCD_SetCursor(row, col);
  LCD_WriteString("text");
  LCD_WriteInteger(value);
  ```

#### ✅ Emergency & Reset System (EXTI Interrupts)

* **EXTI2 (PC2)** triggers `Emergency_Stop_Handler()` and sets a global flag
* LCD is cleared and system stops updating when emergency is active
* **EXTI6 (PC6)** clears the flag and resets system via `Reset_Button_Handler()`
* Critical sections managed using `__disable_irq()` and `__enable_irq()` to prevent race conditions

---

### Project Structure

```
ConveyorX/
├── cmake/                                 # CMake build system setup
│   ├── ArmToolchain.cmake                 # Toolchain path and compiler settings
│   ├── Device.cmake                       # STM32F401VE configuration
│   ├── IncludeList.cmake                  # Header include paths
│   └── SourceList.cmake                   # Source file list
│
├── Proteus/
│   └── ConveyorX.pdsprj                   # Proteus simulation project file
│
├── EXTI/                                  # External interrupt driver (emergency & reset buttons)
├── Gpio/                                  # GPIO abstraction for digital I/O
├── ADC/                                   # ADC driver (reserved for future analog input use)
├── PWM/                                   # PWM generation driver (optional motor speed control)
├── Tim2/                                  # General-purpose TIM2 configuration and input capture
├── TimerDelay/                            # Crude delay implementation using loops
├── LCD/                                   # 16x2 character LCD driver in 4-bit mode
├── ObjectDetection/                       # IR sensor edge-detection polling logic
├── Rcc/                                   # RCC driver for enabling peripheral clocks
├── TIM2/                                  # TIM2 input capture driver for object speed measurement
├── Lib/                                   # Utility macros (bit manipulation, types, etc.)
├── include/                               # Central header file directory
├── src/
│   └── main.c                             # Main program logic and system orchestration
├── STM32-base/                            # Low-level startup and linker files
├── STM32-base-STM32Cube/                  # CMSIS and STM32 HAL headers
├── CMakeLists.txt                         # Root CMake build script
├── .gitignore                             # Git ignore rules
└── README.md                              # Project documentation

```

---

### How to Build & Run

1. **Toolchain Setup:**
   In `cmake/ArmToolchain.cmake`, define the path to your ARM toolchain:

   ```cmake
   set(ARM_TOOLCHAIN_DIR "<Your ARM GCC Path>")
   ```

2. **Build:**
   Use **CLion**, **STM32CubeIDE**, or any CMake-compatible IDE to compile.

3. **Run in Proteus:**

   * Open `ConveyorX.pdsprj` in **Proteus 8 Professional**
   * Load the generated `.hex` file into STM32 MCU
   * Simulate object passing by pulsing the IR sensor input (PA2)
   * Observe object count and speed displayed on LCD

---

### Team

<div>
  <table align="center">
    <tr>
      <td align="center">
        <a href="https://github.com/YassienTawfikk" target="_blank">
          <img src="https://avatars.githubusercontent.com/u/126521373?v=4" width="150px;" alt="Yassien Tawfik"/><br/>
          <sub><b>Yassien Tawfik</b></sub>
        </a>
      </td>
      <td align="center">
        <a href="https://github.com/malak-emad" target="_blank">
          <img src="https://avatars.githubusercontent.com/u/126415070?v=4" width="150px;" alt="Malak Emad"/><br/>
          <sub><b>Malak Emad</b></sub>
        </a>
      </td>
      <td align="center">
        <a href="https://github.com/Ayat-Tarek" target="_blank">
          <img src="https://avatars.githubusercontent.com/u/125220726?v=4" width="150px;" alt="Ayat Tarek"/><br/>
          <sub><b>Ayat Tarek</b></sub>
        </a>
      </td>
      <td align="center">
        <a href="https://github.com/RawanAhmed444" target="_blank">
          <img src="https://avatars.githubusercontent.com/u/94761201?v=4" width="150px;" alt="Rawan Shoaib"/><br/>
          <sub><b>Rawan Shoaib</b></sub>
        </a>
      </td>
      <td align="center">
        <a href="https://github.com/Biatris-003" target="_blank">
          <img src="https://avatars.githubusercontent.com/u/125445159?v=4" width="150px;" alt="Biatris Benjamin"/><br/>
          <sub><b>Biatris Benjamin</b></sub>
        </a>
      </td>      
    </tr>
  </table>
</div>
