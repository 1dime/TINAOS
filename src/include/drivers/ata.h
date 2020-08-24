#ifndef ATA_H
#define ATA_H
#include <stdbool.h>
#include <sys/system.h>
//For checking and connecting to ATA drives
#define VENDOR_ID 0x8086
#define DEVICE_ID 0x7010

#define PRIMARY_DRIVE_IDENTIFIER 0xE0
#define SECONDARY_DRIVE_IDENTIFIER 0xF0

#define DRIVE_SELECT_IO_PORT 0x1F6
#define COMMAND_IO_PORT 0x1F7
#define STATUS_PORT 0x1F7

#define SECTOR_COUNT 0x1F2
#define LBA_LO_PORT 0x1F3
#define LBA_MID_PORT 0x1F4
#define LBA_HI_PORT 0x1F5

#define IDENTIFY_COMMAND 0xEC

#define NO_DRIVE 0xFF

bool is_drive_connected();
void initialize_ata_driver();
#endif