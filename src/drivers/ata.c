#ifndef ATA_C
#define ATA_C
#include <drivers/ata.h>

bool is_drive_connected()
{
    return (((unsigned char) inb(PRIMARY_DRIVE_IDENTIFY_COMMAND)) != NO_DRIVE);
}

/*
Initialize this ATA driver
*/
ata_drive_t* initiliaze_ata_driver()
{
    ata_drive_t* drive = (ata_drive_t*) malloc(sizeof(ata_drive_t*));
    //Validate if drive is connected
    if(!is_drive_connected())
    {
        //Notify that no primary drive exists
        write_error_string("initialize_ata_driver: No primary drive available");
        //Set drive connected to false
        null_drive->is_drive_connected = false;
        //Return the null drive
        return null_drive;
    }
    //Write that a primary drive was found
    write_pass_string("initialize_ata_driver: Found a primary ATA drive");
    //Set that a drive is connected
    drive->is_drive_connected = is_drive_connected();
    //Return the drive command
    return drive;
}
#endif