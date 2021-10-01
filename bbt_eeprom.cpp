#include "bbt_eeprom.h"

bbt_eeprom::bbt_eeprom(uint16_t offsetReservedBlockAddress=2048-40){
    m_offsetReservedBlockAddress = offsetReservedBlockAddress;
    m_translateTableMaxIndex=0;
    m_tempSlot=0;
}

uint8_t bbt_eeprom::getStatus_EEP(){
    return m_eeprom.get(BBT_EEPROM_STATUS_ADDR);
}

void bbt_eeprom::setStatus_EEP(uint8_t status){
    m_eeprom.put(BBT_EEPROM_STATUS_ADDR,status);
}

void bbt_eeprom::clearAllSlot_EEP(){
    for (int l=0;l<BBT_EEPROM_SLOT_LENGTH;l++){
        setSlot_EEP(l,0,0,0);
    }
}

void bbt_eeprom::setSlot_EEP(uint8_t slot, uint8_t broken, uint8_t used,uint16_t blockAddress){
    m_eeprom.put(BBT_EEPROM_SLOT_BASE_ADDR+slot*BBT_EEPROM_SLOT_SIZE,   (blockAddress&0xFF) );
    m_eeprom.put(BBT_EEPROM_SLOT_BASE_ADDR+slot*BBT_EEPROM_SLOT_SIZE+1, ((broken&1)<<7) | ((used&1)<<6) | ((blockAddress>>8)&0x3F) );   
}

void bbt_eeprom::getSlot_EEP(uint8_t slot, uint8_t * broken, uint8_t * used, uint16_t * blockAddress){
    uint8_t  high_byte;
    uint8_t  low_byte;
    low_byte  = m_eeprom.get(BBT_EEPROM_SLOT_BASE_ADDR+slot*BBT_EEPROM_SLOT_SIZE);
    high_byte = m_eeprom.get(BBT_EEPROM_SLOT_BASE_ADDR+slot*BBT_EEPROM_SLOT_SIZE+1);
    *broken = high_byte >> 7;
    *used   = ( high_byte >> 6 ) & 1;
    *blockAddress = ( ( high_byte & 0x3F ) << 8 ) | low_byte;
}

uint16_t bbt_eeprom::slotToReservedBlockAddress(uint8_t slot){
    return m_offsetReservedBlockAddress + slot;
}

uint8_t  bbt_eeprom::reservedBlockAddressToSlot(uint16_t reservedBlockAddress){
    return reservedBlockAddress - m_offsetReservedBlockAddress;
}

// 0-19: free slot number
// 255: fail
uint8_t bbt_eeprom::getFreeSlot_EEP(){
    uint8_t  broken,used;
    uint16_t block_addr;
    for (int s=0;s<BBT_EEPROM_SLOT_LENGTH;s++){
        getSlot_EEP(s,&broken,&used,&block_addr);
        if( broken==0 && used==0 ) {
            return s;
        }
    }
    return 255;
}

void bbt_eeprom::updateTranslateTable(){
    m_translateTableMaxIndex = 0;
    uint8_t  broken,used;
    uint16_t block_addr;    
    for (int s=0;s<BBT_EEPROM_SLOT_LENGTH;s++){
        getSlot_EEP(s,&broken,&used,&block_addr);
        if (broken==0 && used==1) {
            m_translateTableFrom[m_translateTableMaxIndex]=block_addr;
            m_translateTableTo[m_translateTableMaxIndex]=slotToReservedBlockAddress(s);
            m_translateTableMaxIndex++;
        }
    }    
}

uint16_t bbt_eeprom::getTranslatedBlockAddress(uint16_t blockAddress){
    for (int i=0;i<m_translateTableMaxIndex;i++){
        if(m_translateTableFrom[i]==blockAddress){
            return m_translateTableTo[i];
        }
    }
    return blockAddress;
}

void     bbt_eeprom::setTempSlot_EEP(uint8_t tempSlot){
    m_eeprom.put(BBT_EEPROM_TEMPSLOT_ADDR,tempSlot);
}

uint8_t  bbt_eeprom::getTempSlot_EEP(){
    return m_eeprom.get(BBT_EEPROM_TEMPSLOT_ADDR);
}

void     bbt_eeprom::incrTempSlot_EEP(){
    setTempSlot_EEP(getTempSlot_EEP()+1);
}

void     bbt_eeprom::updateTempSlot(){
    m_tempSlot = getTempSlot_EEP();
}

void     bbt_eeprom::setTempSlot(uint8_t tempSlot){
        m_tempSlot = tempSlot;
}

uint8_t  bbt_eeprom::getTempSlot(){
        return m_tempSlot;
}

bool     bbt_eeprom::isReservedBlockAddress(uint16_t blockAddress){
    if(m_offsetReservedBlockAddress>=blockAddress){
        return true;
    }
    return false;
}

uint8_t  bbt_eeprom::get(uint16_t address){
    return m_eeprom.get(address);
}

void     bbt_eeprom::put(uint16_t address,uint8_t data){
    m_eeprom.put(address,data);
}
