#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>
#include "DE1SoCfpga.h"
#include "SevenSegment.h"

using namespace std;

const unsigned int bitArray[16] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111, 119, 124, 57, 94, 121, 113};
const unsigned int mask[6] = {281474976710528, 281474976678143, 281474968387583, 281472846004223, 280929515864063, 141836999983103};

const unsigned int LW_BRIDGE_BASE = 0xFF200000;  // Base offset

// Length of memory-mapped IO window
const unsigned int LW_BRIDGE_SPAN = 0x00DEC700; // Address map size

const unsigned int HEX3_HEX0_OFFSET= 0xFF200020 - LW_BRIDGE_BASE;//;const unsigned int HEX5_HEX4_OFFSET= //;
const unsigned int HEX5_HEX4_OFFSET= 0xFF200030 - LW_BRIDGE_BASE;//;

SevenSegment::SevenSegment()
    {
      reg0_hexValue=0;
      reg1_hexValue=0;
DE1SoCfpga::RegisterWrite(HEX3_HEX0_OFFSET, reg0_hexValue);
DE1SoCfpga::RegisterWrite(HEX5_HEX4_OFFSET, reg1_hexValue);

    }

//turn off all the displays
SevenSegment::~SevenSegment()
{
SevenSegment::Hex_ClearAll();
}

void SevenSegment::Hex_ClearAll()
{
   int val = 0;
   DE1SoCfpga::RegisterWrite(HEX3_HEX0_OFFSET, val);
   DE1SoCfpga::RegisterWrite(HEX5_HEX4_OFFSET, val);
}

//clears  (turns  off)  a  specified  7-segment display specified by indexwhere the index (0 to 5) represents one of the six displays.
void SevenSegment::Hex_ClearSpecific(int index)
{
    reg0_hexValue = DE1SoCfpga::RegisterRead(HEX3_HEX0_OFFSET);
    reg1_hexValue = DE1SoCfpga::RegisterRead(HEX5_HEX4_OFFSET);
   
   if ((index >=0)&&(index <= 3))
   {
       reg0_hexValue = (mask[index] & reg0_hexValue);
       DE1SoCfpga::RegisterWrite(HEX3_HEX0_OFFSET, reg0_hexValue);
   }
   if ((index >=4)&&(index<=5))
   {
       reg1_hexValue = (mask[index-4] & reg1_hexValue);
       DE1SoCfpga::RegisterWrite(HEX5_HEX4_OFFSET, reg1_hexValue);
   }
}
   
    //writes the digit or character value(from Figure 2 above) to the specified 7-segment display specified by indexwhere the index (0 to 5) represents one of the six displays.
void SevenSegment::Hex_WriteSpecific(int index, int value)
    {
    //RegisterWrite(HEX3_HEX0_OFFSET, bitArray[value]);
     SevenSegment::Hex_ClearSpecific(index);
     value = value & 0xf;
     int original;
    if ((index >=0)&&(index <= 3))
   {
   //int current;
       //current= object1.RegisterRead(HEX3_HEX0_OFFSET);
       original = DE1SoCfpga::RegisterRead(HEX3_HEX0_OFFSET);
       reg0_hexValue = (original | (bitArray[value] << (index * 8)));
       DE1SoCfpga::RegisterWrite(HEX3_HEX0_OFFSET, reg0_hexValue);
   }
   if ((index >=4)&&(index<=5))
   {


       original = DE1SoCfpga::RegisterRead(HEX5_HEX4_OFFSET);
       reg1_hexValue  = (original | (bitArray[value] << ((index-4) * 8)));
       DE1SoCfpga::RegisterWrite(HEX5_HEX4_OFFSET, reg1_hexValue);
   }


    }
   
    //writes a positive or negative number tothe 7-segment displays.
    void SevenSegment::Hex_WriteNumber(int number)
    {
	int x = 0;
	int counter = 1;
        for(int loop = 0; loop <6; loop++)
{
	x = (((0x00000F << (loop * 4)) & number) / counter);
	counter = counter * 16;
	SevenSegment::Hex_WriteSpecific(loop, x);
}

    }

