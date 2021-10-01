#include "mbed.h"
#include "EEPROM.h"

#ifndef BBT_EEPROM_H
#define BBT_EEPROM_H

#define BBT_EEPROM_SLOT_LENGTH    40
#define BBT_EEPROM_SLOT_SIZE       2
#define BBT_EEPROM_STATUS_ADDR     0
#define BBT_EEPROM_TEMPSLOT_ADDR   1
#define BBT_EEPROM_SLOT_BASE_ADDR  2


class bbt_eeprom {
    public:
        bbt_eeprom(uint16_t offsetReservedBlockAddress);
        uint8_t getStatus_EEP();
        void    setStatus_EEP(uint8_t status);
        void clearAllSlot_EEP();
        void setSlot_EEP(uint8_t slot, uint8_t broken, uint8_t used,uint16_t blockAddress);
        void getSlot_EEP(uint8_t slot, uint8_t * broken, uint8_t * used, uint16_t * blockAddress);
        
        // Reserved Block Address <-> Slot translation
        uint16_t slotToReservedBlockAddress(uint8_t slot);
        uint8_t  reservedBlockAddressToSlot(uint16_t reservedBlockAddress);
        
        // get not broken not used slot
        uint8_t getFreeSlot_EEP();

        // build ram bbt table
        void updateTranslateTable();


        // get translated block address
        uint16_t getTranslatedBlockAddress(uint16_t blockAddress);

        void     setTempSlot_EEP(uint8_t tempSlot);
        uint8_t  getTempSlot_EEP();
        void     incrTempSlot_EEP();

        // build ram temp value
        void     updateTempSlot();

        void     setTempSlot(uint8_t tempSlot);
        uint8_t  getTempSlot();

        bool     isReservedBlockAddress(uint16_t blockAddress);
        
        uint8_t  get(uint16_t address);
        void     put(uint16_t address,uint8_t data);

    protected:
        EEPROM   m_eeprom;
        uint16_t m_offsetReservedBlockAddress;
        uint16_t m_translateTableFrom[BBT_EEPROM_SLOT_LENGTH];
        uint16_t m_translateTableTo[BBT_EEPROM_SLOT_LENGTH];
        int      m_translateTableMaxIndex;
        uint8_t  m_tempSlot;
};


#endif
