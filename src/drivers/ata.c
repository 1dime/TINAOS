#ifndef ATA_C
#define ATA_C

pci_dev_t primary_ata_device;

ata_device_t primary_base = {.secondary = 0};
ata_device_t primary_replica = {.secondary = 1};
ata_device_t secondary_base = {.secondary = 0};
ata_device_t secondary_replica = {.secondary = 1};

//We kinda have to wait for io operations to be finished
void ata_io_wait(ata_device_t* device)
{
    //Just wait for a second
    inb(device->alt_status);
    inb(device->alt_status);
    inb(device->alt_status);
    inb(device->alt_status);
}


//We have to do a software reset
void ata_software_reset(ata_device_t* device)
{
    outb(device->control, ATA_CONTROL_SOFTWARE_RESET);
    ata_io_wait(device);
    outb(device->control, ATA_CONTROL_ZERO);
}


void ata_handler(bios_register_t* bios_reg)
{
    inb(primary_base.status);
    inb(primary_base.bmr_status);
    outb(primary_base.bmr_command, ATA_BMR_DMA_STOP_COMMAND);
}


void ata_open(vfs_node_t *node, uint32_t flags)
{
    return;
}

void ata_close(vfs_node_t *node)
{
    return;
}

uint32_t ata_read (vfs_node_t *node, uint32_t offset, uint32_t size, char *buffer)
{
    uint32_t start = offset / ATA_SECTOR_SIZE;
    uint32_t start_offset = offset % ATA_SECTOR_SIZE;
    uint32_t end = (offset + size - 1) / ATA_SECTOR_SIZE;
    uint32_t end_offset = (offset + size - 1) % ATA_SECTOR_SIZE;
    char *current_buffer = buffer;
    uint32_t counter = start;
    uint32_t read_size;
    uint32_t off, total = 0;
    while(counter <= end)
    {
        off = 0;
        read_size = ATA_SECTOR_SIZE;
        char *value = ata_read_sector((ata_device_t*)node->device, counter);

        if(counter == start)
        {
            off = start_offset;
            read_size = ATA_SECTOR_SIZE - off;
        }

        if(counter == end)
            read_size = end_offset  - off + 1;
        memcpy(current_buffer, value + off, read_size);
        current_buffer = current_buffer + read_size;
        total = total + read_size;
    }
    return total;
}

uint32_t ata_write(vfs_node_t *node, uint32_t offset, uint32_t size, char *buffer)
{
    uint32_t start = offset / ATA_SECTOR_SIZE;
    uint32_t start_offset = offset % ATA_SECTOR_SIZE;
    uint32_t end = (offset + size - 1) / ATA_SECTOR_SIZE;
    uint32_t end_offset = (offset + size - 1) % ATA_SECTOR_SIZE;

    char *current_bufffer = buffer;
    uint32_t counter = start;
    uint32_t write_size;
    uint32_t off, total = 0;

    while(counter <= end)
    {
        off = 0;
        write_size = ATA_SECTOR_SIZE;
        char *value = ata_read_sector((ata_device_t*) node->device, counter);
        if(counter == start)
        {
            off = start_offset;
            write_size = ATA_SECTOR_SIZE - off;
        }
        if(counter == end)
        {
            write_size = end_offset - off + 1;
        }
        memcpy(value + off, current_bufffer, write_size);
        ata_write_sector((ata_device_t*)node->device, counter, value);
        current_bufffer = current_bufffer + write_size;
        total = total + write_size;
        counter++;
    }

    return total;
}

void ata_write_sector(ata_device_t* device, uint32_t lba, char *buffer)
{
    memcpy(device->memory_buffer, buffer, ATA_SECTOR_SIZE);
    outb(device->bmr_command, 0);
    outl(device->bmr_prdt, (uint32_t) device->physical_prdt);
    outb(device->drive, 0xe0 | device->secondary << 4 | (lba & 0x0f000000) >> 24);
    outb(device->sector_count, 1);
    outb(device->lba_lo, lba & 0x000000ff);
    outb(device->lba_mid, (lba & 0x0000ff00) >> 8);
    outb(device->lba_high, (lba & 0x00ff0000) >> 16);
    outb(device->command, 0xCA);
    outb(device->bmr_command, 0x1);
    while(1)
    {
        int status = inb(device->bmr_status);
        int device_status = inb(device->status);
        if(!(status & 0x04))
        {
            continue;
        }
        if(!(device_status & 0x80))
        {
            break;
        }
    }
}

char *ata_read_sector(ata_device_t* device, uint32_t lba)
{
    char *buffer = malloc(ATA_SECTOR_SIZE);
    outb(device->bmr_command, 0);
    outl(device->bmr_prdt, (uint32_t)device->physical_prdt);
    outb(device->drive, 0xe0 | device->secondary << 4 | (lba & 0x0f000000) >> 24);
    outb(device->sector_count, 1);
    outb(device->lba_lo, lba & 0x000000ff);
    outb(device->lba_mid, (lba & 0x0000ff00) >> 8);
    outb(device->lba_high, (lba & 0x00ff0000) >> 16);
    outb(device->command, 0xC8);
    outb(device->bmr_command, 0x8 | 0x1);
    while(1)
    {
        int status = inb(device->bmr_status);
        int device_status = inb(device->status);
        if(!(status & 0x04))
        {
            continue;
        }

        if(!(device_status & 0x80))
        {
            break;
        }
    }
}

vfs_node_t* make_ata_device(ata_device_t* ata_device)
{
    vfs_node_t* vfs = calloc(sizeof(vfs_node_t), 1);
    strcpy(vfs->name, "ata device");
    vfs->name[strlen(vfs->name)] = ata_device->mountpoint[strlen(ata_device->mountpoint) - 1];
    write_string_to_terminal("Found ATA: ");
    write_string_to_terminal(vfs->name);
    vfs->device = ata_device;
    vfs->flags = VFS_BLOCKDEVICE;
    vfs->read = ata_read;
    vfs->write = ata_write;
    vfs->open = ata_open;
    vfs->close = ata_close;
    return vfs;
}

void ata_device_start(ata_device_t* device, int primary)
{
    device->ata_prdt = (void*) malloc_4k(sizeof(prdt));
    memset(device->ata_prdt, 0, sizeof(prdt));
    device->physical_prdt = virt_2_phys(pager_dir, device->ata_prdt);
    device->memory_buffer = (void*) malloc_4k(4096);
    memset(device->memory_buffer, 0, 4096);
    device->ata_prdt[0].buffer = (uint32_t) virt_2_phys(pager_dir, device->memory_buffer);
    device->ata_prdt[0].transfer_size - ATA_SECTOR_SIZE;
    device->ata_prdt[0].end = ATA_MARK_END;

    uint16_t base_address = primary ? (0x1F0) : (0x170);
    uint16_t alt_status = primary ? (0x3F6) : (0x376);

    device->data = base_address;
    device->error = base_address - 1;
    device->sector_count = base_address + 2;
    device->lba_lo = base_address + 3;
    device->lba_mid = base_address + 4;
    device->lba_high = base_address + 5;
    device->drive = base_address + 6;
    device->command = base_address + 7;
    device->alt_status = alt_status;

    device->bar4 = pci_read(primary_ata_device, BAR4);
    if(device->bar4 & 0x1)
    {
        device->bar4 = device->bar4 & 0xfffffffc;
    }

    device->bmr_command = device->bar4;
    device->bmr_status = device->bar4 + 2;
    device->bmr_prdt = device->bar4 + 4;

    memset(device->mountpoint, 0, 32);
    strcpy(device->mountpoint, "/devices/hd");
    device->mountpoint[strlen(device->mountpoint)] = 'a' + (((!primary) << 1) | device->secondary);
}

void detect_ata_device(ata_device_t* device, int primary)
{
    ata_device_start(device, primary);

    ata_software_reset(device);
    ata_io_wait(device);
    outb(device->drive, (0xA + device->secondary) << 4);
    outb(device->sector_count, 0);
    outb(device->lba_lo, 0);
    outb(device->lba_mid, 0);
    outb(device->lba_high, 0);
    outb(device->command, ATA_IDENTIFY_COMMAND);
    if(!inb(device->status))
    {
        write_error_string("Attempted to find an ATA device but failed");
        return;
    }

    uint8_t drq = 0, err = 0;

    while(!drq && !err)
    {
        drq = inb(device->status) & ATA_STATUS_DRQ;
        err = inb(device->status) & ATA_STATUS_ERR;
    }

    if(err)
    {
        write_error_string("A problem rose when polling an ATA drive");
        return;
    }

    for(int i = 0; i < 256; i++)
    {
        ins(device->data);
    }

    uint32_t command_reg = pci_read(primary_ata_device, PCI_COMMAND);
    if(!(command_reg & (1 << 2)))
    {
        command_reg |= (1 << 2);
        pci_write(primary_ata_device, PCI_COMMAND, command_reg);
    }
    //Mount the file system
    mount_fs(device->mountpoint, make_ata_device(device));
}

void configure_ata()
{
    primary_ata_device = get_pci_device(ATA_VENDOR_ID, ATA_DEVICE_ID, -1);

    detect_ata_device(&primary_base, 1);
    detect_ata_device(&primary_replica, 1);
    detect_ata_device(&secondary_base, 0);
    detect_ata_device(&secondary_replica, 0);
}

#endif