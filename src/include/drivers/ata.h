#ifndef ATA_H
#define ATA_H
#include <stdbool.h>
#include <sys/system.h>
//For checking and connecting to ATA drives
#define VENDOR_ID 0x8086
#define DEVICE_ID 0x7010

//Drive bus information
#define PRIMARY_BUS 0x00
#define SECONDARY_BUS 0x01

//Master / slave drive information
#define MASTER_DRIVE 0x00
#define SLAVE_DRIVE 0x01

//Primary / secondary input / output
#define PRIMARY_IO 0x1F0
#define SECONDARY_IO 0x170

//Hard drive select port
#define ATA_HD_SEL 0x06

//All the io base registers
#define ATA_DATA 0x00
#define ATA_ERROR 0x01
#define ATA_FEATURES 0x01
#define ATA_SECTOR_COUNT_0 0x02
#define ATA_SECTOR_COUNT_1 0x08
#define ATA_LBA_LOW 0x03
#define ATA_LBA_0   0x03
#define ATA_LBA_1   0x04
#define ATA_LBA_2 0x05
#define ATA_LBA_MID 0x04
#define ATA_LBA_3 0x09
#define ATA_LBA_4 0x0A
#define ATA_LBA_5 0x0B
#define ATA_LBA_HIGH 0x05
#define ATA_COMMAND 0x07
#define ATA_STATUS 0x07
#define ATA_IDENTIFY 0xEC

//Registers offset from control base
#define ATA_ALT_STATUS 0x0C
#define ATA_DEVICE_CONTROL 0x0C
#define ATA_DEVICE_ADDRESS 0x0D

//Status register
#define ATA_BSY 0x80
#define ATA_DRDY 0x40
#define ATA_DF 0x20
#define ATA_DSC 0x10
#define ATA_DRQ 0x08
#define ATA_CORR 0x04
#define ATA_IDX 0x02
#define ATA_ERR 0x01

//Error register
#define ADDRESS_MARKER_NOT_FOUND_ERROR 0x00
#define TRACK_ZERO_NOT FOUND_ERROR     0x01
#define ABORTED_COMMAND_ERROR          0x02
#define MEDIA_CHANGE_REQUEST_ERROR     0x03
#define IDENTITY_NOT_FOUND_ERROR       0x04
#define MEDIA_CHANGED_ERROR            0x05
#define UNCORRECTABLE_DATA_ERROR       0x06
#define BAD_BLOCK_DETECTED_ERROR       0x07

//The drive / head register
#define DRIVE_NUMBER_SELECT 0x04
#define DRIVE_LBA           0x06

#define NO_DRIVE 0xFFFF

#define ATA_WRITE_SECTORS_COMMAND 0x30
#define ATA_FLUSH_CACHE_COMMAND   0xE7

uint8_t* ata_buffer = 0;

//Device identifiers
#define ATA_MODEL 54

//Some device commands
#define ATA_COMMAND_READ_PIO 0x20

typedef struct
{
    char* drive_name;
    uint8_t drive_id;
    uint8_t lba_low;
    uint8_t lba_mid;
    uint8_t lba_high;
    uint16_t io;
    uint8_t (*read)(uint8_t* buffer, uint32_t lba, uint32_t sector_count, void* drive);
    uint8_t (*write)(uint8_t *buffer, uint32_t offset, uint32_t buffer_length, void* drive);
} ata_drive;

ata_drive **drives;
int drive_count;

//Add the selected drive to the drives list
void add_drive(ata_drive* drive);
//Get all of the given drives
ata_drive** get_drives();
//Get a drive at given index
ata_drive* get_drive(int index);
//Poll a drive at the given io
void poll_ata_drive(uint16_t io);
//Read data from drive once
void ata_read_once(uint8_t *buffer, uint32_t lba, ata_drive* device);
//Read all sector data from drive
void ata_read(uint8_t *buffer, uint32_t lba, uint32_t sector_count, ata_drive* drive);
//Select a drive at the given bus and slot
void select_drive(uint8_t bus, uint8_t slot);
//Get identifying information for drive at given bus and slot
uint8_t identify_drive(uint8_t bus, uint8_t drive);
//Get the drive's name
char* get_drive_name(uint8_t bus, uint8_t drive);
//Get the main drive (at 0)
ata_drive* get_main_drive();
//Initialize this ATA driver
void initialize_ata_driver();
#endif