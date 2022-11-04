#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>
#ifndef DE1SoCfpga_H
#define DE1SoCfpga_H


class DE1SoCfpga
{
    // Cyclone V FPGA device addresses
    public:
    char *pBase;
    int fd;
   
    DE1SoCfpga();
    ~DE1SoCfpga();
    void RegisterWrite(unsigned int offset, int value);
    int RegisterRead(unsigned int offset);
};   
   
    
    #endif
