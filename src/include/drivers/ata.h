#ifndef ATA_H
#define ATA_H
#include <stdbool.h>
#include <sys/system.h>
//For checking and connecting to ATA drives
#define VENDOR_ID 0x8086
#define DEVICE_ID 0x7010


#define DRIVE_SELECT_IO_PORT 0x1F6
#define COMMAND_IO_PORT 0x1F7
#define STATUS_PORT 0x1F7
#define SECTOR_COUNT_PORT 0x1F2
#define LBA_LO_PORT 0x1F3
#define LBA_MID_PORT 0x1F4
#define LBA_HI_PORT 0x1F5

#define IDENTIFY_COMMAND 0xEC
#define PRIMARY_DRIVE_IDENTIFY_COMMAND 0xA0
#define SECONDARY_DRIVE_IDENTIFY_COMMAND 0xB0

#define STATUS_BUSY 0x80
#define STATUS_READY 0x40
#define STATUS_DRQ 0x08
#define STATUS_DF 0x20
#define STATUS_ERR 0x01

#define NO_DRIVE 0xFF

typedef struct ata_drive
{
    bool is_drive_connected;
    int sector_count;
} ata_drive_t;

ata_drive_t* null_drive = {0};

bool is_drive_connected();
ata_drive_t* initialize_ata_driver();
#endif