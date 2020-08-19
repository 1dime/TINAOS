#include <stdint.h>
#include <interrupts/interrupts.h>
#ifndef PCI_H
#define PCI_H

typedef union pci_dev
{
    uint32_t bits;
    struct
    {
        uint32_t always_zero : 2;
        uint32_t field_num : 6;
        uint32_t function_num : 3;
        uint32_t device_num : 5;
        uint32_t bus_num : 8;
        uint32_t reserved: 7;
        uint32_t enable: 1;
    };
} pci_dev_t;

#define PCI_ADDRESS 0xCF8
#define PCI_DATA 0xCFC

//Basic PCI information
#define VENDOR_ID 0x00
#define DEVICE_ID 0x02
#define PCI_COMMAND 0x04
#define STATUS 0x06
#define REVISION_ID 0x08
#define PROG_IF 0c09
#define SUBCLASS 0x0a
#define CLASS 0x0b
#define CACHE_LINE_SIZE 0x0c
#define LATENCY_TIMER 0x0d
#define HEADER_TYPE 0x0e
#define BIST 0x0f
#define BAR0 0x10
#define BAR1 0x14
#define BAR2 0x18
#define BAR3 0x1C
#define BAR4 0x20
#define BAR5 0x24
#define INTERRUPT_LINE 0x3C
#define SECONDARY_BUS 0x09

//Type of PCI device
#define HEADER_TYPE_DEVICE 0
#define HEADER_TYPE_BRIDGE 1
#define HEADER_TYPE_CARDBUS 2
#define TYPE_BRIDGE 0x0604
#define TYPE_SATA 0x0106
#define NONE 0xFFFF

#define DEVICE_PER_BUS 32
#define FUNC_PER_DEV 32

//For reading from PCI
uint32_t pci_read(pci_dev_t device, uint32_t field);
//For writing data to PCI
void pci_write(pci_dev_t device, uint32_t field, uint32_t value);
//For getting device type
uint32_t get_device_type(pci_dev_t device);
//For getting the secondary bus
uint32_t get_secondary_bus(pci_dev_t device);
//Reach PCI end
uint32_t is_pci_end(pci_dev_t device);
//Scan PCI function
pci_dev_t scan_device_function(uint16_t vendor, uint16_t device_id, uint32_t bus, uint32_t device, uint32_t function, int device_type);
//Scan the device
pci_dev_t scan_device(uint16_t vendor, uint16_t device_id, uint32_t bus, uint32_t device, int device_type);
//Scan the bus
pci_dev_t scan_bus(uint16_t vendor_id, uint16_t device_id, uint32_t bus, int device_type);
//Get the pci device
pci_dev_t get_pci_device(uint16_t vendor_id, uint16_t device_id, int device_type);
//Initialize our PCI driver
void init_pci_driver();
#endif