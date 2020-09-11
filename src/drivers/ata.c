#ifndef ATA_C
#define ATA_C
#include <drivers/ata.h>
#include <display/vga.h>

void add_drive(ata_drive* drive)
{
    //Add the drive to the drives list
    drives[drive_count] = drive;
    //Update drive count
    drive_count++;
}

ata_drive** get_drives()
{
    //Return the drives list
    return drives;
}

//Get a drive at index
ata_drive* get_drive(int index)
{
    //Return the drive at index
    return drives[index];
}

void poll_ata_drive(uint16_t io)
{
    //Loop 4 times
    for(int i  = 0; i < 4; i++)
    {
        //Read alt status 4 times
        inb(io + ATA_ALT_STATUS);
    }

retry_poll:;
    //Get ATA status
    uint8_t status = inb(io + ATA_STATUS);
    if(status & ATA_BSY) goto retry_poll;
retry_poll_again: status = inb(io + ATA_STATUS);
    if(status & ATA_ERR)
    {
        //Uh-oh! Something happened!
        krnl_panic("Device failure occurred while polling drive");
    }
    //If DRQ is not free, poll again
    if(!(status & ATA_DRQ)) goto retry_poll_again;
    //Just end the loop
    return;
}

void ata_read_once(uint8_t *buffer, uint32_t lba, ata_drive* device)
{
    uint8_t drive = device->drive_id;
    uint16_t io = device->io;
    switch(drive)
    {
        case (PRIMARY_BUS << 1 | MASTER_DRIVE):
            drive = MASTER_DRIVE;
            break;
        case (PRIMARY_BUS << 1 | SLAVE_DRIVE):
            drive = SLAVE_DRIVE;
            break;
        case (SECONDARY_BUS << 1 | MASTER_DRIVE):
            drive = MASTER_DRIVE;
            break;
        case (SECONDARY_BUS << 1 | SLAVE_DRIVE):
            drive = MASTER_DRIVE;
            break;
        default:
            write_error_string("[ATA] => ata_read_once: Unknown drive");
            return; //exit function
    }
    //Identify which command we can use
    uint8_t command = (drive==MASTER_DRIVE ? 0xE0 : 0xF0);
    //Identify the slave bit
    //uint8_t slave_bit = (drive == MASTER_DRIVE ? 0x00 : 0x01); //UNUSED
    //Write the command out
    outb(io + ATA_HD_SEL , (command | (uint8_t)((lba >> 24 & 0x0F))));
    outb(io + 1, 0x00);
    outb(io + ATA_SECTOR_COUNT_0, 1);
    outb(io + ATA_LBA_LOW, (uint8_t)((lba)));
    outb(io + ATA_LBA_MID, (uint8_t)((lba) >> 16));
    outb(io + ATA_COMMAND, ATA_COMMAND_READ_PIO);

    //Poll current io
    poll_ata_drive(io);
    //Loop for 256 times
    for(int i = 0; i < 256; i++)
    {
        //Read data from disk
        uint16_t data = inw(io + ATA_DATA);
        //Add data to the buffer
        *(uint16_t *)(buffer + i * 2) = data;
    }
    //Delay for 400ns
    for(int i = 0; i < 4; i++)
    {
        //Just a delay
        inb(io + ATA_ALT_STATUS);
    }
    //Return success -> void return type
    //return 1;
}

void ata_read(uint8_t *buffer, uint32_t lba, uint32_t sector_count, ata_drive* drive)
{
    //Loop through all sectors
    for(int sector = 0; sector < sector_count; sector_count++)
    {
        //Read data from sector to buffer
        ata_read_once(buffer, lba + sector, drive);
        //Update buffer by 512
        buffer += 512;
    }
}


void select_drive(uint8_t bus, uint8_t slot)
{
    //Check if we are on the primary bus
    if(bus == PRIMARY_BUS)
    {
        //Check if slot is master
        if(slot == MASTER_DRIVE)
            //Write 0xA0 for selection
            outb(PRIMARY_IO + ATA_HD_SEL, 0xA0);
        else
            //Write 0xB0 for selection
            outb(PRIMARY_IO + ATA_HD_SEL, 0xB0);
    }
    else
        //Check if slot is master
        if(slot == MASTER_DRIVE)
            //Write 0xA0 for secondary selection
            outb(SECONDARY_IO + ATA_HD_SEL, 0xA0);
        else
            //Write 0xB0 for secondary selection
            outb(SECONDARY_IO + ATA_HD_SEL, 0xB0);
}

uint8_t identify_drive(uint8_t bus, uint8_t drive)
{
    //For drive stuff
    uint16_t io = 0;
    //Select the drive
    select_drive(bus, drive);
    //Check if bus is primary
    if(bus == PRIMARY_BUS)
        //Set io to primary drive
        io = PRIMARY_IO;
    else
        //Set io to secondary drive
        io = SECONDARY_IO;
    //Set sector count to 0
    outb(io + ATA_SECTOR_COUNT_0, 0);
    //Set LBA 0 to 0
    outb(io + ATA_LBA_LOW, 0);
    //Set LBA 1 to 0
    outb(io + ATA_LBA_MID, 0);
    //Set LBA 2 to 0
    outb(io + ATA_LBA_HIGH, 0);
    //Send the identify command
    outb(io + ATA_COMMAND, ATA_IDENTIFY);
    //Get the status
    uint8_t status = inb(io + ATA_STATUS);
    //Check if status != 0xFFFF
    if(status)
    {
        //Check if an error occurred
        if(status & ATA_ERR)
        {
            //Print that no drive was found on bus
            cprintf(VGA_COLOR_CYAN, "[ATA] => identify_drive: No drive found on %s bus", bus==PRIMARY_BUS?"primary":"secondary");
            return 0;
        }
        //Notify that we will be polling now
        write_current_operation("identify_drive", "Polling drive until BSY bit is free");
        //Loop until BSY bit is free
        while( (inb(io + ATA_STATUS)) & (ATA_BSY != 0) );
        //Read the status
read_status:    status = inb(io + ATA_STATUS);
        //We are now polling until DRQ bit is free
        write_current_operation("identify_drive", "Polling until DRQ bit is free");
        //Loop while DRQ is set, go to read_status
        while(!(status & ATA_DRQ)) goto read_status;
        //Print that the drive is online
        cprintf(VGA_COLOR_GREEN, "[ATA] => identify_drive: %s bus is online", bus==PRIMARY_BUS?"Primary":"Secondary");
        //Now we have to read the data
        for(int i = 0; i < 256; i++)
        {
            //Read into the ata buffer
            *(uint16_t *)(ata_buffer + (i * 2)) = inw(io + ATA_DATA);
        }
    }

    return 0;//?
}

char* get_drive_name(uint8_t bus, uint8_t drive)
{
    //Check if drive exists
    if(identify_drive(bus, drive))
    {
<<<<<<< HEAD
        //Initiate a drive
        //ata_drive *drive = (ata_drive*) malloc(sizeof(ata_drive*)); //UNUSED
        //Create a string holding drive data
        char* drive_data = (char*) malloc(40);
        //Loop for size of drive data
=======
        char* drive_buffer = (char*) malloc(40);
        //Loop and read buffer
>>>>>>> 1dae7cafe1a84976f910173930f4407564a06d90
        for(int idx = 0; idx < 40; idx += 2)
        {
            //Set current buffer index to future data from ata buffer
            drive_buffer[idx] = ata_buffer[ATA_MODEL + idx + 1];
            //Set future buffer data to data from ata buffer
            drive_buffer[idx + 1] = ata_buffer[ATA_MODEL + idx];
        }
<<<<<<< HEAD
        //Print that we found a drive called drive_data
        cprintf(VGA_COLOR_GREEN, "[ATA] => initialize_ata_driver: Found primary drive called %s", drive_data);
=======
        cprintf(red, drive_buffer);
        //Return drive buffer
        return drive_buffer;
    }else
    {
        //Return no drive
        return NO_DRIVE;
    }
}


ata_drive* get_main_drive()
{
    //Return the first drive
    return get_drives()[0];
}

void initiliaze_ata_driver()
{
    //Get primary drive's name
    char* primary_master_drive_name = get_drive_name(PRIMARY_BUS, MASTER_DRIVE);
    //Check if drive name is not no drive
    if(primary_master_drive_name != NO_DRIVE)
    {
        //Allocate some memory
        ata_drive* drive = (ata_drive*) malloc(sizeof(ata_drive));
        //Set the drive name
        drive->drive_name = primary_master_drive_name;
        //Set the drive id
        drive->drive_id = (PRIMARY_BUS << 1) | MASTER_DRIVE;
        //Get the base address
        uint16_t base_address = drive->drive_id ? (0x1F0) : (0x170);
        //Set the low address
        drive->lba_low = (base_address + 3);
        //Set the mid address
        drive->lba_mid = (base_address + 4);
        //Set the high address
        drive->lba_high = (base_address + 5);
        //Set drive io
        drive->io = PRIMARY_IO;
        //Set read
        drive->read = ata_read;
        //Add the drive to a list of drives
        add_drive(drive);
        //Print that a drive was found
        cprintf(green, "[ATA] => initialize_ata_driver: Found primary master drive with name: %s", primary_master_drive_name);
    }
    //Get primary drive's name
    char* primary_slave_drive_name = get_drive_name(PRIMARY_BUS, SLAVE_DRIVE);
    //Check if drive name is not no drive
    if(primary_slave_drive_name != NO_DRIVE)
    {
        //Allocate some memory
        ata_drive* drive = (ata_drive*) malloc(sizeof(ata_drive));
        //Set the drive name
        drive->drive_name = primary_slave_drive_name;
        //Set the drive id
        drive->drive_id = (PRIMARY_BUS << 1) | SLAVE_DRIVE;
        //Get the base address
        uint16_t base_address = drive->drive_id ? (0x1F0) : (0x170);
        //Set the low address
        drive->lba_low = (base_address + 3);
        //Set the mid address
        drive->lba_mid = (base_address + 4);
        //Set the high address
        drive->lba_high = (base_address + 5);
        //Set drive io
        drive->io = PRIMARY_IO;
        //Set read
        drive->read = ata_read;
        //Add the drive to a list of drives
        add_drive(drive);
        //Print that a drive was found
        cprintf(green, "[ATA] => initialize_ata_driver: Found primary slave drive with name: %s", primary_slave_drive_name);
    }

    //Get secondary drive's name
    char* secondary_master_drive_name = get_drive_name(SECONDARY_BUS, MASTER_DRIVE);
    //Check if it exists
    if(secondary_master_drive_name != NO_DRIVE)
    {
        //Allocate some memory
        ata_drive* drive = (ata_drive*) malloc(sizeof(ata_drive));
        //Set the drive name
        drive->drive_name = secondary_master_drive_name;
        //Set the drive id
        drive->drive_id = (SECONDARY_BUS << 1) | MASTER_DRIVE;
        //Get the base address
        uint16_t base_address = drive->drive_id ? (0x1F0) : (0x170);
        //Set the low address
        drive->lba_low = (base_address + 3);
        //Set the mid address
        drive->lba_mid = (base_address + 4);
        //Set the high address
        drive->lba_high = (base_address + 5);
        //Set drive io
        drive->io = SECONDARY_IO;
        //Set read
        drive->read = ata_read;
        //Add the drive to a list of drives
        add_drive(drive);
        //Print that a drive was found
        cprintf(green, "[ATA] => initialize_ata_driver: Found secondary master drive with name: %s", secondary_master_drive_name);
    }
    //Get secondary drive's name
    char* secondary_slave_drive_name = get_drive_name(SECONDARY_BUS, SLAVE_DRIVE);
    //Check if it exists
    if(secondary_slave_drive_name != NO_DRIVE)
    {
        //Allocate some memory
        ata_drive* drive = (ata_drive*) malloc(sizeof(ata_drive));
        //Set the drive name
        drive->drive_name = secondary_slave_drive_name;
        //Set the drive id
        drive->drive_id = (SECONDARY_BUS << 1) | SLAVE_DRIVE;
        //Get the base address
        uint16_t base_address = drive->drive_id ? (0x1F0) : (0x170);
        //Set the low address
        drive->lba_low = (base_address + 3);
        //Set the mid address
        drive->lba_mid = (base_address + 4);
        //Set the high address
        drive->lba_high = (base_address + 5);
        //Set drive io
        drive->io = SECONDARY_IO;
        //Set read
        drive->read = ata_read;
        //Add the drive to a list of drives
        add_drive(drive);
        //Print that a drive was found
        cprintf(green, "[ATA] => initialize_ata_driver: Found secondary slave drive with name: %s", secondary_slave_drive_name);
>>>>>>> 1dae7cafe1a84976f910173930f4407564a06d90
    }
}

#endif