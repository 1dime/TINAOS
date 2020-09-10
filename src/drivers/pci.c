#include "../sys/system.c"
#include "../sys/memory.c"
#include <drivers/pci.h>
#include <stdint.h>
#include <stddef.h>

//All found pci devices
pci_device** devices;
//Device list size
uint32_t device_count = 0;

void register_pci_device(pci_device* device)
{
    //Add the device at device count
    devices[device_count] = device;
    //Update device count
    device_count++;
    //End any loop
    return;
}

uint16_t pci_config_read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t address;
    uint32_t lbus = (uint32_t) bus;
    uint32_t lslot = (uint32_t) slot;
    uint32_t lfunc = (uint32_t) func;
    uint16_t temp = 0;

    //Create a configuration address
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
                (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
    //Write the address
    outl(0xCF8, address);
    //Read the response
    temp = (uint16_t)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xffff);
    //Return the temporary value
    return (temp);
}

uint16_t pci_check_vendor(uint16_t bus, uint16_t slot)
{
    uint16_t vendor = pci_config_read_word(bus, slot, 0, 0);
    //uint16_t device = pci_config_read_word(bus, slot, 0, 2); //UNUSED

    //probably some checks?

    return vendor;
}


uint16_t pci_get_vendor(uint8_t bus, uint8_t slot, uint8_t function)
{
    //uint16_t vendor = pci_config_read_word(bus, slot, function, 0);
    ////Check if a vendor exists
    //if(vendor != 0xFFFF)
    //{
    //    //Return the vendor
    //    return vendor;
    //} 
//
    ////Return the vendor
    //return 0xFFFF;

    return pci_config_read_word(bus, slot, function, 0);
}

uint16_t pci_check_device(uint16_t bus, uint16_t slot, uint16_t function)
{
    //uint16_t device = pci_config_read_word(bus, slot, function, 2);
    ////Check if a device exists at that bus and slot
    //if(device != 0xFFFF)
    //{
    //    //Return the device
    //    return device;
    //}
    ////Return nothing
    //return (0xFFFF);    

    return pci_config_read_word(bus, slot, function, 2);
}

uint16_t pci_get_device(uint16_t bus, uint16_t device, uint16_t function)
{
    //Return found device
    return pci_check_device(bus, device, function);
}



void initialize_pci_devices()
{
    //Set devices
    devices = (pci_device**) malloc(32 * sizeof(pci_device));
    //Loop through all 256 PCI buses
    for(uint32_t bus = 0; bus < 256; bus++)
    {
        //Create a pci device instance by allocating memory space
        pci_device* pcidevice = (pci_device*) malloc(sizeof(pci_device));
        //Loop through 32 slots
        for(uint32_t slot = 0; slot < 32; slot++)
        {
            //Loop through all 8 possible functions
            for(uint32_t function = 0; function < 8; function++)
            {
                //Get the vendor of the current pci device
                uint16_t vendor = pci_get_vendor(bus, slot, function);
                //Check if the current vendor is none
                if(vendor == 0xFFFF)
                {
                    //Go on
                    continue;
                }
                //Get the current device associated with the given bus, slot, and function
                uint16_t device = pci_get_device(bus, slot, function);
                //Check if device exists
                if(device == 0xFFFF)
                {
                    //Go on
                    continue;
                }
                //Set the vendor for pcidevice
                pcidevice->vendor = vendor;
                //Set the device for pcidevice
                pcidevice->device = device;
                //Set the driver for pcidevice
                pcidevice->driver = 0;
                //Register the pci device
                register_pci_device(pcidevice);
            }
        }
    }
    
}

pci_device** get_all_devices()
{
    //Return all of the pci devices
    return devices;
}