#ifndef ATA_C
#define ATA_C
#include <drivers/ata.h>

/*
Check if a primary (or master) drive is connected or not
*/
bool is_primary_ata_drive_connected()
{
    //Return if a primary drive is connected or not
    return (((unsigned char) inb(IDENTIFY_PRIMARY_DRIVE_PORT)) != ILLEGAL_STATUS);
}

/*
Check if a secondary (or slave) drive is connected to the bus
*/
bool is_secondary_ata_drive_connected()
{
    //Return if a secondary drive is connected or not
    return (((unsigned char) inb(IDENTIFY_SECONDARY_DRIVE_PORT)) != ILLEGAL_STATUS);
}

/*
Initialize this ATA driver
*/
void initiliaze_ata_driver()
{
    //First, check if there is a primary drive
    if(is_primary_ata_drive_connected())
    {
        //Check if there is a secondary drive
        if(is_secondary_ata_drive_connected())
        {
            //We will use this drive for backups
            write_pass_string("initiliaze_ata_driver: Primary and secondary ATA drives exist, using secondary for backup\n");
        }
        else
        {
            //We will use a secondary partition for backup
            write_pass_string("initiliaze_ata_driver: Primary drive exists, but not secondary. Will use another partition for OS backup\n");
        }
    }
    else
    {
        //No drives are connected
        write_error_string("initiliaze_ata_driver: No ATA drives connected");
    }
}
#endif