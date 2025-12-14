#ifndef BOOTLOADER_API_H
#define BOOTLOADER_API_H

#include <stdint.h>

typedef struct BootloaderSharedAPI
{
    void (*Blink)(uint32_t dlyticks);
    void (*TurnOn)(void);
    void (*TurnOff)(void);
} BootloaderSharedAPI;

#define API_ADDRESS  ((uint32_t)0x08018000)

#endif // BOOTLOADER_API_H
