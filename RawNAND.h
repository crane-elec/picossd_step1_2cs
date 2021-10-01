#ifndef RAWNAND_H
#define RAWNAND_H

#include "mbed.h"

// KIOXIA 1Gbit SLC NAND, 3.3V, x8, 24nm, TSOP
// TC58NVG0S3HTA00 datasheet 2019-10-01C 

//
// Timing parameter
//
// CLE Setup Time
#define tCLS_US  0.012

// CLE Hold Time
#define tCLH_US   0.005

// CEB Setup Time
#define tCS_US    0.020

// CEB Hold Time
#define tCH_US    0.005

// Write Pulse Width
#define tWP_US    0.012

// ALE Setup Time
#define tALS_US   0.012

// ALE Hold Time
#define tALH_US   0.005

// DATA Setup Time
#define tDS_US    0.012

// DATA Hold Time
#define tDH_US    0.005

// Write Cycle Time
#define tWC_US    0.025

// WEB High Hold Time
#define tWH_US    0.010

// WPB High to WEB Low
#define tWW_US    0.100

// Ready to REB Falling Edge
#define tRR_US    0.020

// Ready to WEB Falling Edge
#define tRW_US    0.020

// Read Pulse Width
#define tRP_US    0.012

// Read Cycle Time
#define tRC_US    0.025

// REB Access Time
#define tREA_US   0.020

// CEB Access Time
#define tCEA_US   0.025

// CLE Low to REB Low
#define tCLR_US   0.010

// ALE Low to REB Low
#define tAR_US    0.010

// REB High to Output Hold Time
#define tRHOH_US  0.025

// REB Low to Output Hold Time
#define tRLOH_US  0.005

// REB High to Output High Impedance
#define tRHZ_US   0.060

// CEB High to Output High Impedance
#define tCHZ_US   0.020

// CEB High to ALE or CLE Don't care
#define tCSD_US   0.000

// REB High Hold Time
#define tREH_US   0.010

// Output-High-Impedance-to-REB Falling Edge
#define tIR_US    0.000

// REB High to WEB Low
#define tRHW_US   0.030

// WEB High to CEB Low
#define tWHC_US   0.030

// WEB High to REB Low
#define tWHR_US   0.060

// WEB High to Busy
#define tWB_US    0.100

// Device Reset Time(Ready)
#define tRST_RDY_US       5.0

// Device Reset Time(Read)
#define tRST_READ_US      5.0

// Device Reset Time(Program)
#define tRST_PROGRAM_US  10.0

// Devise Reset Time(Erase)
#define tRST_ERASE_US   500.0

// Programming Time
#define tPROG_US        700.0

// Data Cache Busy Time in Write Cache( following 15h ) 
#define tDCBAYW2_US     700.0

// Number of Partial Program Cycles in the same page
#define NCYCLES_SAME_PAGE 4

// Block Erasing Time
#define tBERASE_US     5000.0

// Memory Cell Array to Staring Address ( tR )
#define MC2SA_US         25.0

// Data Cache Busy in Read Cache ( following 31h and 3Fh )
#define tDCBSYR1_US      25.0

// Data Cache Busy in Page Copy ( following 3Ah )
#define tDCBSYR2_US      30.0



class RawNAND
{
    public:
        RawNAND(PinName ceb0, PinName ceb1,
                PinName cle , PinName ale ,
                PinName web , PinName reb ,
                PinName wpb , PinName rbb , 
                PinName io1 , PinName io2 ,
                PinName io3 , PinName io4 ,
                PinName io5 , PinName io6 ,
                PinName io7 , PinName io8 );
        void reset(uint8_t cs);
        void idRead(uint8_t cs, uint8_t * readData);
        uint8_t statusRead(uint8_t cs);
        void setWriteProtect(uint8_t writeProtect);
        void pageRead(uint8_t cs, uint8_t * readData,uint16_t blockAddress,uint8_t pageAddress,uint16_t columnAddress,uint16_t beats);
        // return status
        uint8_t pageProgram(uint8_t cs, const uint8_t * writeData,uint16_t blockAddress,uint8_t pageAddress,uint16_t columnAddress,uint16_t beats);
        // return status
        uint8_t erase(uint8_t cs, uint16_t blockAddress);
        

    private:
        DigitalOut _ceb0;
        DigitalOut _ceb1;
        DigitalOut _cle;
        DigitalOut _ale;
        DigitalOut _web;
        DigitalOut _reb;
        DigitalOut _wpb;
        DigitalIn  _rbb;
        BusInOut   _io;

};

#endif
