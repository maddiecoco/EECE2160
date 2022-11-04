#include "DE1SoCfpga.h"
#ifndef SEVENSEGMENT_H 
#define SEVENSEGMENT_H

using namespace std;

class SevenSegment: public DE1SoCfpga
{
private:
    unsigned int reg0_hexValue;
    unsigned int reg1_hexValue;

public:
       
    SevenSegment();
    ~SevenSegment();
    void Hex_ClearAll();
    void Hex_ClearSpecific(int index);
    void Hex_WriteSpecific(int index, int value);
    void Hex_WriteNumber(int number);
    

//turn off all the displays ~SevenSegment() { //Hex_ClearAll();
};

#endif
