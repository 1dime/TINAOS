#include <stdint.h>
#ifndef ATA
#define ATA

typedef struct physical_region_descriptor_table
{
    uint32_t buffer;
    uint32_t transfer_size;
    uint32_t end;
} prdt;

typedef struct ata_device
{
    uint16_t data;
    uint16_t error;
    uint16_t sector_count;

    union
    {
        uint16_t sector_num;
        uint16_t lba_lo;
    };

    union
    {
        uint16_t cylinder_low;
        uint16_t lba_mid;
    };

    union
    {
        uint16_t cylinder_high;
        uint16_t lba_high;
    };

    union
    {
        uint16_t command;
        uint16_t status;
    };

    union
    {
        uint16_t control;
        uint16_t alt_status;
    };

    int secondary;
    uint32_t bar4;
    //bmr goods
    uint32_t bmr_command;
    uint32_t bmr_prdt;
    uint32_t bmr_status;

    //The physical region descriptor table of this device
    prdt* ata_prdt;
    uint8_t physical_prdt;

    uint8_t* memory_buffer;
    uint8_t* physical_memory_buffer;
} ata_device_t;

//Some ATA device information
#define ATA_VENDOR_ID 0x8086
#define ATA_DEVICE_ID 0x7010

//ATA control regulation
#define ATA_CONTROL_STOP_INTERRUPT 0x2
#define ATA_CONTROL_SOFTWARE_RESET 0x4
#define ATA_CONTROL_HIGH_ORDER_BYTE 0x80
#define ATA_CONTROL_ZERO 0x00

//Some command regulation
#define ATA_IDENTIFY_COMMAND 0xEC
#define ATA_DMA_READ_COMMAND 0xC8
#define ATA_READ_PIO_COMMAND 0x20

//Definitions for status regulation
#define ATA_STATUS_ERR 0x0
#define ATA_STATUS_DRQ 0x8
#define ATA_STATUS_SRV 0x10
#define ATA_STATUS_DF 0x20
#define ATA_STATUS_RDY 0x40
#define ATA_STATUS_BSY 0x80

//And these are for bus master commands
#define ATA_BMR_DMA_START_COMMAND 0x1
#define ATA_BMR_DMA_STOP_COMMAND 0x0
#define ATA_BMR_READ_COMMAND 0x8

//And the bus master's status
#define ATA_BMR_INT_STATUS 0x4
#define ATA_BMR_ERR_STATUS 0x2

//These bad boys are for the PRDT
#define ATA_SECTOR_SIZE 512
#define ATA_MARK_END 0x8000

//We kinda have to wait for io operations to be finished
void ata_io_wait(ata_device_t* device);

//We have to do a software reset
void ata_software_reset(ata_device_t* device);


#endif