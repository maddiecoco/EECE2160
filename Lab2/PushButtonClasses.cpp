/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>

using namespace std;

const unsigned int LW_BRIDGE_BASE = 0xFF200000;  // Base offset 

// Length of memory-mapped IO window 
const unsigned int LW_BRIDGE_SPAN = 0x00DEC700; // Address map size

const unsigned int LEDR_OFFSET = 0xFF200000 - LW_BRIDGE_BASE; //Write the value in Hex here
const unsigned int SW_OFFSET   = 0xFF200040 - LW_BRIDGE_BASE; //Write the value in Hex here
const unsigned int KEY_OFFSET  = 0xFF200050 - LW_BRIDGE_BASE; //Write the value in Hex here

class DE1SoCfpga
{
    // Cyclone V FPGA device addresses

    public:
    char *pBase;
    int fd;
    
    DE1SoCfpga()
    {
       // Open /dev/mem to give access to physical addresses
	    fd = open( "/dev/mem", (O_RDWR | O_SYNC));
    	if (fd == -1)			//  check for errors in openning /dev/mem
	{
        cout << "ERROR: could not open /dev/mem..." << endl;
        exit(1);
	}
	
   // Get a mapping from physical addresses to virtual addresses
   char *virtual_base = (char *)mmap (NULL, LW_BRIDGE_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, LW_BRIDGE_BASE);
   if (virtual_base == MAP_FAILED)		// check for errors
   {
      cout << "ERROR: mmap() failed..." << endl;
      close (fd);		// close memory before exiting
      exit(1);        // Returns 1 to the operating system;
   }
    pBase = virtual_base; 
    }
    
    ~DE1SoCfpga()
    {
        
        if (munmap (pBase, LW_BRIDGE_SPAN) != 0)
	{
      cout << "ERROR: munmap() failed..." << endl;
      exit(1);
	}
    close (fd); 	// close memory
   
    }
    
    
    void RegisterWrite(unsigned int offset, int value)
    {
        * (volatile unsigned int *)(pBase + offset) = value;
    }
    
    int RegisterRead(unsigned int offset)
    {
        return *(volatile unsigned int *)(pBase + offset);
    }
    
};

class LEDControl
{ 
    
    public:
    char *pBase;
    int fd;
    DE1SoCfpga object1 ;
    
    LEDControl()
    {
       // Open /dev/mem to give access to physical addresses
	    fd = open( "/dev/mem", (O_RDWR | O_SYNC));
    	if (fd == -1)			//  check for errors in openning /dev/mem
	{
        cout << "ERROR: could not open /dev/mem..." << endl;
        exit(1);
	}
	
   // Get a mapping from physical addresses to virtual addresses
   char *virtual_base = (char *)mmap (NULL, LW_BRIDGE_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, LW_BRIDGE_BASE);
   if (virtual_base == MAP_FAILED)		// check for errors
   {
      cout << "ERROR: mmap() failed..." << endl;
      close (fd);		// close memory before exiting
      exit(1);        // Returns 1 to the operating system;
   }
    pBase = virtual_base; 
    }
    
    ~LEDControl()
    {
        
        if (munmap (pBase, LW_BRIDGE_SPAN) != 0)
	{
      cout << "ERROR: munmap() failed..." << endl;
      exit(1);
	}
    close (fd); 	// close memory
   
    }
    
    
     void Write1Led(int ledNum, int state) 
    {
        int ledval = object1.RegisterRead(LEDR_OFFSET);					
if (-1 < ledNum < 10) 
{ 
    if (state == 1) 
{					
    ledval = ledval | (1 << ledNum); 
}
    else if (state == 0) 
{
    ledval = ledval & ~(1 << ledNum);						
}

else 
{						
cout << "Input 0 or 1 for the state:" << endl; 
}

}
else 
{					
cout << "Input a number for the LED from 0 to 9." << endl; 


    }
    
    object1.RegisterWrite(LEDR_OFFSET, ledval);
    }
    
    void WriteAllLeds(int value) 
    {
        object1.RegisterWrite(LEDR_OFFSET, value);
    }
    
    void Read1Switch()
    {
    int choice;
    cout << "What switch do you want to read?" << endl;
    cin >> choice;
    
    int switchvalue = object1.RegisterRead(SW_OFFSET);
    switchvalue = (switchvalue) & (1 << choice);
    
    if (switchvalue != 0)
    {
        switchvalue = 1;
    }
    else
    {
       switchvalue = 0; 
    }
    
    cout << "Switch value: " << switchvalue << endl;
    }
    
    void ReadAllSwitches()
    {
        int switchval = object1.RegisterRead(SW_OFFSET);					
        cout << "Switch Value:  " << switchval << endl;
    }
    
    void PushButtonGet()
    {
        int keyval = object1.RegisterRead(KEY_OFFSET);
int a = keyval;
int val;

while(a == keyval)

{
int b = object1.RegisterRead(KEY_OFFSET);
a = b;

 if (a == 1)
{
val = object1.RegisterRead(LEDR_OFFSET);
cout << "Former led value: " << val;
val = val++;
WriteAllLeds(val);
}

 if (a == 2)
{
val = object1.RegisterRead(LEDR_OFFSET);
cout << "Former led value: " << val;
val = val--;
WriteAllLeds(val);
}

 if (a == 4)
{
val = object1.RegisterRead(LEDR_OFFSET);
cout << "Former led value: " << val;
val = (val >> 1);
WriteAllLeds(val);
}

 if (a == 8)
{
val = object1.RegisterRead(LEDR_OFFSET);
cout << "Former led value: " << val;
val = (val << 1);
WriteAllLeds(val);
}
}
    }
};

int main()
{
        DE1SoCfpga object1 ;
        LEDControl object2 ;
        int ledNum;
        int state;

        cout << "Enter an LED number from 0 to 9." << endl;
        cin >> ledNum;
        cout << "Enter a state of either 0 or 1." << endl;
        cin >> state;

        object2.Write1Led(ledNum, state);
        object2.ReadAllSwitches();

        int value = 0;
    	cout << endl;
        cout << "Enter an int value between 0 to 1023: " << endl;
        cin >> value;
        cout << "value to be written to LEDs = " << value << endl;
        object2.WriteAllLeds(value);
        int readLEDs = object1.RegisterRead(LEDR_OFFSET);
        cout << "value of LEDS read = " << readLEDs << endl;

        object2.PushButtonGet();
        
}


