#include "DE1SoCfpga.h"
#include <iostream>

using namespace std;

const unsigned int bitArray[16] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111, 119, 124, 57, 94, 121, 113};
const unsigned int mask[6] = {281474976710528, 281474976678143, 281474968387583, 281472846004223, 280929515864063, 141836999983103};

const unsigned int LW_BRIDGE_BASE = 0xFF200000;  // Base offset

// Length of memory-mapped IO window
const unsigned int LW_BRIDGE_SPAN = 0x00DEC700; // Address map size

const unsigned int HEX3_HEX0_OFFSET= 0xFF200020 - LW_BRIDGE_BASE;//;const unsigned int HEX5_HEX4_OFFSET= //;
const unsigned int HEX5_HEX4_OFFSET= 0xFF200030 - LW_BRIDGE_BASE;//;

     DE1SoCfpga::DE1SoCfpga()
     {
       // Open /dev/mem to give access to physical addresses
   fd = open( "/dev/mem", (O_RDWR | O_SYNC));
    if (fd == -1) //  check for errors in openning /dev/mem
{
        cout << "ERROR: could not open /dev/mem..." << endl;
        exit(1);
}

   // Get a mapping from physical addresses to virtual addresses
   char *virtual_base = (char *)mmap (NULL, LW_BRIDGE_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, LW_BRIDGE_BASE);
   if (virtual_base == MAP_FAILED) // check for errors
   {
      cout << "ERROR: mmap() failed..." << endl;
      close (fd); // close memory before exiting
      exit(1);        // Returns 1 to the operating system;
   }
    pBase = virtual_base;
    }

    DE1SoCfpga::~DE1SoCfpga()
    {
       
        if (munmap (pBase, LW_BRIDGE_SPAN) != 0)
{
      cout << "ERROR: munmap() failed..." << endl;
      exit(1);
}
    close (fd); // close memory
   
    }
    
    void DE1SoCfpga::RegisterWrite(unsigned int offset, int value)
    {
    cout << "in REg WR\n";
        * (volatile unsigned int *)(pBase + offset) = value;
    }
   
    int DE1SoCfpga::RegisterRead(unsigned int offset)
    {
        return *(volatile unsigned int *)(pBase + offset);
    }
