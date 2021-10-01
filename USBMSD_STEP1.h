#ifndef USBMSD_STEP1_H
#define USBMSD_STEP1_H


#include "mbed.h"
#define USBMSD_STEP1_SECTORS  4016
#define USBMSD_STEP1_SECTOR_SIZE 512
#define USBMSD_STEP1_DISK_SIZE    USBMSD_STEP1_SECTOR_SIZE*USBMSD_STEP1_SECTORS
#include <stdlib.h>
#include "USBMSD.h"
#include "RawNAND.h"
#include "bbt_eeprom.h"

class USBMSD_STEP1: public USBMSD
{   public:
        USBMSD_STEP1(PinName ceb0, PinName ceb1, PinName cle,
                     PinName ale, PinName web,
                     PinName reb, PinName wpb,
                     PinName rbb, 
                     PinName io1, PinName io2,
                     PinName io3, PinName io4,
                     PinName io5, PinName io6,
                     PinName io7, PinName io8,
                     PinName ledRd,PinName ledWr,PinName swInit);

        virtual int disk_read(uint8_t* data, uint64_t block, uint8_t count);
        virtual int disk_write(const uint8_t* data, uint64_t block, uint8_t count);
        virtual int disk_initialize();
        virtual uint64_t disk_sectors();
        virtual uint64_t disk_size();
        virtual int disk_status();


        uint8_t get_cs_from_block(uint16_t block);
        uint16_t get_chip_block_from_block(uint16_t block);
        

        DigitalOut * m_ledRead;
        DigitalOut * m_ledWrite;
        DigitalIn  * m_swInit;
//        DigitalIn  * m_isp;


    private:
        RawNAND    * m_rawNand;
        bbt_eeprom * m_bbt;
        uint8_t      m_buffer[512];
};

#endif
