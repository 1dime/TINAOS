#include <drivers/pci.h>
#include <display/vga.h>
#ifndef PCI_C
#define PCI_C

uint32_t pci_size_map[100];
pci_dev_t zero = {0};

//For reading from PCI
uint32_t pci_read(pci_dev_t device, uint32_t field)
{
    //Get the necessary 6 bits from the field
    device.field_num = (field & 0xFC) >> 2;
    //Enable the device
    device.enable = 1;
    //Send the goods to the PCI
    outb(PCI_ADDRESS, device.bits);

    //I kinda need the size of this field
    uint32_t field_size = pci_size_map[field];
    //Check if the field size is 1
    if(field_size == 1)
    {
        //Get the 3rd byte since we are in little endian
        return (uint8_t) inb(PCI_DATA + (field & 3));
    }
    else if(field_size == 2)
    {
        //Get the second byte and return it
        return (uint8_t) inb(PCI_DATA + (field & 2));
    }
    else if(field_size == 4)
    {
        //Just get all 4 bytes
        return (uint8_t) inb(PCI_DATA);
    }
    else
    {
        //Return default 0xFFFF
        return 0xFFFF;
    }
    
}

//For writing data to PCI
void pci_write(pci_dev_t device, uint32_t field, uint32_t value)
{
    //Set the field number of the device
    device.field_num = (field & 0xFC) >> 2;
    //Enable the device
    device.enable = 1;
    //Notify the device of where we want to write data
    outb(PCI_ADDRESS, device.bits);
    //And send the data
    outb(PCI_DATA, value);
}

//For getting device type
uint32_t get_device_type(pci_dev_t device)
{
    //Since we don't want to write files to the mouse,
    // we need to know what the device type is
    //Just read the pci class and subclass
    return pci_read(device, CLASS) << 8 | pci_read(device, SUBCLASS);
}

//For getting the secondary bus
uint32_t get_secondary_bus(pci_dev_t device)
{
    //Get the secondary bus from a PCI bridge
    return pci_read(device, SECONDARY_BUS);
}

//For validating if PCI is endpoint
uint32_t is_pci_end(pci_dev_t device)
{
    //By default, endpoint PCI header would be 0
    return !pci_read(device, HEADER_TYPE);
}

//Scan PCI function
pci_dev_t scan_device_function(uint16_t vendor, uint16_t device_id, uint32_t bus, uint32_t device, uint32_t function, int device_type)
{
    //Get the primary bus
    pci_dev_t pci_device = {0};
    //Set the bus number to num
    pci_device.bus_num = bus;
    //Set the device number
    pci_device.device_num = device;
    //Set the function number of the device
    pci_device.function_num = function;
    //Recursive importance. If the device is a pci bridge, get the bus it is hooked to
    if(get_device_type(pci_device) == TYPE_BRIDGE)
    {
        //Scan the bus with the given vendor id and other data
        scan_bus(vendor, device_id, get_secondary_bus(pci_device), device_type);
    }

    //Check if device type is a match
    if(device_type == -1 || device_type == get_device_type(pci_device))
    {
        //Get the device id
        uint32_t dev_id = pci_read(pci_device, DEVICE_ID);
        //Get the vendor id
        uint32_t vend_id = pci_read(pci_device, VENDOR_ID);
        //Check if both device and vendor id match
        if(dev_id == device_id || vend_id == vendor)
        {
            //Return the device
            return pci_device;
        }
    }
    //Return the primary bus
    return zero;
}

//Scan the device
pci_dev_t scan_device(uint16_t vendor, uint16_t device_id, uint32_t bus, uint32_t device, int device_type)
{
    //Get the primary device
    pci_dev_t primary_device = {0};
    //Set the bus number
    primary_device.bus_num = bus;
    //Set the device number
    primary_device.device_num = device;

    //Check if reading the PCI returns NONE
    if(pci_read(primary_device, VENDOR_ID) == NONE)
    {
        //Return zero
        return zero;
    }

    //Scan the 'function' of the PCI device
    pci_dev_t function = scan_device_function(vendor, device_id, bus, device, 0, device_type);
    //Validate bits in function
    if(function.bits)
        //Return the function
        return function;
    //Check if the primary device is an endpoint PCI
    if(is_pci_end(primary_device))
        //Return zero device
        return zero;
    //Loop through the functions of the PCI device
    for(int function_idx = 1; function_idx < FUNC_PER_DEV; function_idx++)
    {
        //Check if reading the device does not return none
        if(pci_read(primary_device, VENDOR_ID) != NONE)
        {
            //Set the function
            function = scan_device_function(vendor, device_id, bus, device, function_idx, device_type);
            //Validate the bits
            if(function.bits)
            {
                //Return the function
                return function;
            }
        }
    }
}

//Scan the bus
pci_dev_t scan_bus(uint16_t vendor_id, uint16_t device_id, uint32_t bus, int device_type)
{
    //Loop through the devices per every bus
    for(int device_idx = 0; device_idx < DEVICE_PER_BUS; device_idx++)
    {
        //Scan the PCI device at the given index
        pci_dev_t device = scan_device(vendor_id, device_id, bus, device_idx, device_type);
        //Validate bits existing
        if(device.bits)
        {
            //Return the device
            return device;
        }
    }

    //Return zero, as no device was found in loop
    return zero;
}

//Get the pci device
pci_dev_t get_pci_device(uint16_t vendor_id, uint16_t device_id, int device_type)
{
    //Given the unique device id and vendor are given, try getting the device
    pci_dev_t device = scan_bus(vendor_id, device_id, 0, device_type);
    //Validate that bits exist
    if(device.bits)
    {
        //Return the device
        return device;
    }

    //We need to handle multiple device functions
    //So first, check if we have reached device endpoint
    if(is_pci_end(zero))
    {
        write_error_string("Failed to get PCI device: No PCI device found\n");
    }
    //Loop through pci device function
    for(int function_idx = 1; function_idx < FUNC_PER_DEV; function_idx++)
    {
        //Get the primary PCI device
        pci_dev_t primary_device = {0};
        //Set the function number to function_idx
        primary_device.function_num = function_idx;
        //Read the device's vendor id, check if it is none
        if(pci_read(primary_device, VENDOR_ID) == NONE)
            //Exit the loop
            break;
        //Set the device again by scanning the given function
        device = scan_bus(vendor_id, device_id, function_idx, device_type);
        //Validate bits
        if(device.bits)
        {
            //Return the device
            return device;
        }
    }

    write_pass_string("PCI device found\n");
    //Return zero
    return zero;
}

//Initialize our PCI driver
void init_pci_driver()
{
    //Just fill the size for each PCI size map field
    //Set vendor id to 2
    pci_size_map[VENDOR_ID] = 2;
    //Set device id to 2
    pci_size_map[DEVICE_ID] = 2;
    //Set PCI command to 2
    pci_size_map[PCI_COMMAND] = 2;
    //Set PCI status to 2
    pci_size_map[STATUS] = 2;
    //Set the PCI class to 1
    pci_size_map[CLASS] = 1;
    //Set the PCI subclass to 1
    pci_size_map[SUBCLASS] = 1;
    //Set the line size to 1
    pci_size_map[CACHE_LINE_SIZE] = 1;
    //Set the latency timer to 1
    pci_size_map[LATENCY_TIMER] = 1;
    //Set the header type to 1
    pci_size_map[HEADER_TYPE] = 1;
    //set PCI bist to 1
    pci_size_map[BIST] = 1;
    //Set PCI bar 0 to 4
    pci_size_map[BAR0] = 4;
    //Set PCI bar 1 to 4
    pci_size_map[BAR1] = 4;
    //Set PCI bar 2 to 4
    pci_size_map[BAR2] = 4;
    //Set PCI bar 3 to 4
    pci_size_map[BAR3] = 4;
    //Set PCI bar 4 to 4
    pci_size_map[BAR4] = 4;
    //Set PCI bar 5 to 4
    pci_size_map[BAR5] = 4;
    //Set interrupt line to 1
    pci_size_map[INTERRUPT_LINE] = 1;
    //Set the secondary bus to 1
    pci_size_map[SECONDARY_BUS] = 1;
}


#endif