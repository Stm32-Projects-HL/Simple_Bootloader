# Simple Bootloader for STM32
This project demonstrates how to separate the bootloader from the application, manage flash memory layout, and jump from the bootloader to your main firmware.

## 📁 Repository Structure

```
Simple_Bootloader/
├── Application/              # Main user application code
├── simple_bootloader/        # Bootloader firmware source
├── shared_api/               # APIs & utilities shared between bootloader and app
├── .gitignore
└── (other config files)
```

## What Is a Bootloader?

A bootloader is a small program that runs immediately after reset and decides what firmware to run.  
It enables features such as:

✔️ Jumping to an application  
✔ Firmware updates without a debugger  
✔ Protection and validation of application code  
✔ Separation of the core firmware from update logic

This bootloader is designed for STM32 microcontrollers using the HAL library.

---

## 🚀 Features

✔ Configurable bootloader separate from application  
✔ Flash memory partitioning  
✔ Shared API between loader & application  
✔ Clean jump from bootloader to application  
✔ Sample application to show usage  

---
## 🛠️ Getting Started

### Prerequisites

Before building the project, install:

- **STM32CubeIDE** (or your preferred ARM toolchain)
- **STM32CubeProgrammer** (for flashing binaries)
- ST‑Link drivers

---

## 🧱 Flash Memory Layout

Typical memory layout using this bootloader:

```
0x0800_0000 ───────── Bootloader code
      ... (size defined by project)
0x0800_xxxx ───────── Application code
```

> Ensure that the application build settings place its vector table and reset handler at the correct offset so that jumping works properly.

---

## 🏗️ How It Works

1. **Bootloader resets** and executes from start of flash.
2. It checks for a valid application at the defined location.
3. If application is valid, it sets the stack pointer and **jumps to its reset handler**.
4. Application begins execution normally.

Key steps in the bootloader jump logic:

```c
uint32_t appStack = *(uint32_t*)(APP_START_ADDR);
uint32_t appEntry = *(uint32_t*)(APP_START_ADDR + 4);

__disable_irq();
__set_MSP(appStack);
((void (*)(void))appEntry)();
```

> You must update `APP_START_ADDR` based on your memory partition.

---

## 🔧 Linker Script Setup

To split flash between bootloader and application, adjust the **linker script (`.ld`)** or STM32CubeIDE **memory regions**.

Assume:

- STM32 MCU with 128KB Flash
- Bootloader uses **0x08000000 – 0x08003FFF** (16KB)
- Application starts at **0x08004000**

### ➤ Application Memory Configuration

```ld
/* Application Linker Script (simplified) */
FLASH (rx)  : ORIGIN = 0x08004000, LENGTH = 112K
RAM (rwx)   : ORIGIN = 0x20000000, LENGTH = 20K
```

### ➤ Vector Table Offset

In **`main.c`** of the Application:

```c
#define VECT_TAB_OFFSET  0x4000  // Application offset (16KB)
SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET;
```

---

## 🧱 STM32CubeIDE Setup Guide

### ➤ Create Two Projects:

- `simple_bootloader/`
- `Application/`

Each should target the **same MCU**.

### ➤ Configure Bootloader:

- Keep default Flash origin: `0x08000000`
- Flash size: ~16KB
- Enable SWD, GPIO for debugging if needed

### ➤ Configure Application:

- Change Flash origin to `0x08004000`
- Adjust linker script or CubeIDE memory settings

---

## 🔁 Bootloader Jump Flow

```
+--------------------------+
|   Start (Reset Handler)  |
+--------------------------+
             |
             v
+--------------------------+
| Check for valid app @ X |
|  (e.g., 0x08004000)      |
+--------------------------+
       | Yes          | No
       v              v
+----------------+  +-------------------+
| Set MSP        |  | Stay in bootloader|
| Set PC to app  |  | Blink LED / Wait  |
| Jump to App    |  | for update        |
+----------------+  +-------------------+
```

---

## 🔄 Flashing

Use **STM32CubeProgrammer**:

1. Flash the **bootloader** to start of flash.
2. Flash the **application** at the defined offset.
3. Reset the board — bootloader should run and jump to application.

---


