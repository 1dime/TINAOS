#ifndef ATA_H
#define ATA_H
#include <stdbool.h>
#include <sys/system.h>
//For checking and connecting to ATA drives
#define VENDOR_ID 0x8086
#define DEVICE_ID 0x7010

//Necessary for sending and recieving data from ATA drives
#define IDENTIFY_PRIMARY_DRIVE_PORT 0xA0
#define IDENTIFY_SECONDARY_DRIVE_PORT 0xB0

//Always returned if bus has no ATA drives
#define ILLEGAL_STATUS 0xFF

//For checking if a primary drive is connected
bool is_primary_ata_drive_connected();

//For checking if a secondary drive is connected
bool is_secondary_ata_drive_connected();
#endif