#include "RawNAND.h"

// TODO: Replace wait_us(int) to wait_ns 

RawNAND::RawNAND(PinName ceb0,PinName ceb1, PinName cle,
                 PinName ale, PinName web,
                 PinName reb, PinName wpb,
                 PinName rbb, 
                 PinName io1, PinName io2,
                 PinName io3, PinName io4,
                 PinName io5, PinName io6,
                 PinName io7, PinName io8)
        : _ceb0(ceb0,1),_ceb1(ceb1,1),_cle(cle,1),
          _ale(ale,1),_web(web,1),
          _reb(reb,1),_wpb(wpb,1),
          _rbb(rbb,PullUp),
          _io(io1,io2,io3,io4,io5,io6,io7,io8) {
          _io.mode(PullNone);
          _io.input();
              
}
void RawNAND::reset(uint8_t cs) {
    // wait ready
    while(_rbb==0){
        __NOP();    
    }
    // RESET COMMAND (0xff)
    if(cs==0) {
        _ceb0 = 0;
    } else if (cs==1) {
        _ceb1 = 1;
    }
    _cle = 1;
    _ale = 0;
    _web = 0;
    _io=0xff;
    _io.output();
    // wait setup time : max(tCS,tCLS,tALS,tDS,tWP)
    wait_us(tCS_US);
    _web = 1;
    // wait hold time : max(tCLH,tALH,tDH,tWH,tWB,tCH)
    wait_us(tWB_US);
    // check tRBB;
    while (_rbb==0){
        __NOP();
    }
    if(cs==0) {
        _ceb0 = 1;
    } else if (cs==1) {
        _ceb1 = 1;
    }
}
void RawNAND::idRead(uint8_t cs, uint8_t * readData) {
    // ID READ COMMAND (0x90)
    if(cs==0) {
        _ceb0 = 0;
    } else if (cs==1) {
        _ceb1 = 0;
    }
    _cle = 1;
    _ale = 0;
    _web = 0;
    _io=0x90;
    _io.output();
    // wait setup time : max(tCS,tCLS,tALS,tDS,tWP)
    wait_us(tCS_US);
    _web = 1;
    // wait hold time : max(tCLH,tALH,tDH,tWH)
    wait_us(tWH_US);
    
    // IO READ ADDRESS  (0x00)
    _cle=0;
    _ale=1;
    _web=0;
    _io=0x00;
    // wait setup time : max(tCLS,tALS,tDS,tWP)
    wait_us(tCLS_US);
    _web=1;
    // wait hold time : max(tCLH,tALH,tDH,tWH)
    wait_us(tWH_US);

    // ALE low and IO input
    _ale=0;
    _io.input();
    // ALE low to tREB low : tAR
    wait_us(tAR_US);

    // IO READ read data
    for (int l=0;l<5;l++) {
        _reb = 0;
        // wait max(tREA,tRP)
        wait_us(tREA_US);
        *(readData+l)=_io;
        _reb = 1;
        // wait tREH
        wait_us(tREH_US);
    }
    // wait io hiz
    if(cs==0) {
        _ceb0 = 1;
    } else if (cs==1) {
        _ceb1 = 1;
    }
    wait_us(tCHZ_US);
}
uint8_t RawNAND::statusRead(uint8_t cs) {
    uint8_t status;
    // wait ready
    while(_rbb==0){
        __NOP();    
    }

    // STATUS READ COMMAND (0x70)
    if(cs==0) {
        _ceb0 = 0;
    } else if (cs==1) {
        _ceb1 = 0;
    }
    _cle = 1;
    _ale = 0;
    _web = 0;
    _io=0x70;
    _io.output();
    // wait setup time : max(tCS,tCLS,tALS,tDS,tWP)
    wait_us(tCS_US);
    _web = 1;
    // wait hold time : max(tCLH,tDH,tWH)
    wait_us(tWH_US);
    _cle = 0;
    _io.input();
    // wait max(tWHR-tWH,tCLR-tWH)
    wait_us(tWHR_US-tWH_US);
    _reb = 0;
    // wait max(tREA,tRP)
    wait_us(tREA_US);
    status = _io;
    _reb = 1;
    // wait tREH
    wait_us(tREH_US);
    if(cs==0) {
        _ceb0 = 1;
    } else if (cs==1) {
        _ceb1 = 1;
    }
    wait_us(tCHZ_US);
    // wait io hiz
    return status;
}

void RawNAND::setWriteProtect(uint8_t writeProtect){
    _wpb = writeProtect;
    // wait tWW
    wait_us(tWW_US);
}

void RawNAND::pageRead(uint8_t cs, uint8_t * readData,uint16_t blockAddress,uint8_t pageAddress,uint16_t columnAddress,uint16_t beats){
    // wait ready
    while(_rbb==0){
        __NOP();    
    }

    // 1ST READ COMMAND (0x00)
    if(cs==0) {
        _ceb0 = 0;
    } else if (cs==1) {
        _ceb1 = 0;
    }
    _cle = 1;
    _ale = 0;
    _web = 0;
    _io=0x00;
    _io.output();
    // wait setup time : max(tCS,tCLS,tALS,tDS,tWP)
    wait_us(tCS_US);

    _web = 1;
    // wait hold time : max(tCLH,tDH,tWH)
    wait_us(tWH_US);

    // 1st address column [7:0]
    _cle = 0;
    _ale = 1;
    _web = 0;
    _io  = columnAddress & 0xff;
    // wait setup time : max(tALS,tCLS,tDS,tWP)
    wait_us(tWP_US);

    _web = 1;
    // wait hold time : max(tCLH,tDH,tWH)
    wait_us(tWH_US);   

    // 2nd address column [11:8]
    _web = 0;
    _io  = (columnAddress>>8) & 0x0f;            
    // wait setup time : max(tALS,tCLS,tDS,tWP)
    wait_us(tWP_US);

    _web = 1;
    // wait hold time : max(tCLH,tDH,tWH)
    wait_us(tWH_US);
    
    // 3rd address  {blockAddress[1:0],pageAddress[5:0]}
    _web = 0;
    _io  = ((blockAddress<<6) | pageAddress) ;            
    // wait setup time : max(tALS,tDS,tWP)
    wait_us(tWP_US);

    _web = 1;
    // wait hold time : max(tALH,tDH,tWH)
    wait_us(tWH_US);
    
    // 4th address  blockAddress[9:2]
    _web = 0;
    _io  = (blockAddress>>2) ;   
    // wait setup time : max(tALS,tDS,tWP)
    wait_us(tWP_US);

    _web = 1;
    // wait hold time : max(tALH,tDH,tWH)
    wait_us(tWH_US);

    // 2ND READ COMMAND (0x30)
    _cle = 1;
    _ale = 0;
    _web = 0;
    _io  = 0x30;
    // wait setup time : max(tALS,tCLS,tDS,tWP)
    wait_us(tWP_US);

    _web = 1;
    // wait hold time : max(tALH,tDH,tWH,tWB)
    wait_us(tWB_US);

    _cle = 0;
    _io.input();

    // wait ready
    while(_rbb==0){
        __NOP();    
    }

    // wait tRR  ( RBB to REB Low )
    wait_us(tRR_US);

    // read sequence
    for (int b=0;b<beats;b++){
        _reb = 0;
        // wait max(tREA,tRP)
        wait_us(tREA_US);
         *(readData + b)= _io;
        _reb = 1;
        // wait tREH
        wait_us(tREH_US);
    }

    if(cs==0) {
        _ceb0 = 1;
    } else if (cs==1) {
        _ceb1 = 1;
    }
    // wait io hiz
    wait_us(tCHZ_US);

}


uint8_t RawNAND::erase(uint8_t cs, uint16_t blockAddress){
    // wait ready
    while(_rbb==0){
        __NOP();    
    }

    // 1ST ERASE COMMAND (0x60)
    if(cs==0) {
        _ceb0 = 0;
    } else if (cs==1) {
        _ceb1 = 0;
    }
    _cle = 1;
    _ale = 0;
    _web = 0;
    _io=0x60;
    _io.output();
    // wait setup time : max(tCS,tCLS,tALS,tDS,tWP)
    wait_us(tCS_US);

    _web = 1;
    // wait hold time : max(tCLH,tDH,tWH)
    wait_us(tWH_US);


    // 1st page address  {blockAddress[1:0],pageAddress[5:0]}
    _cle = 0;
    _ale = 1;
    _web = 0;
    _io  = (blockAddress<<6) ;            
    // wait setup time : max(tALS,tDS,tWP)
    wait_us(tWP_US);

    _web = 1;
    // wait hold time : max(tALH,tDH,tWH)
    wait_us(tWH_US);
    
    // 2nd page address  blockAddress[9:2]
    _web = 0;
    _io  = (blockAddress>>2) ;   
    // wait setup time : max(tALS,tDS,tWP)
    wait_us(tWP_US);

    _web = 1;
    // wait hold time : max(tALH,tDH,tWH)
    wait_us(tWH_US);

    // 2ND ERASE COMMAND (0xD0)
    _cle = 1;
    _ale = 0;
    _web = 0;
    _io  = 0xD0;
    // wait setup time : max(tALS,tCLS,tDS,tWP)
    wait_us(tWP_US);

    _web = 1;
    // wait hold time : max(tALH,tDH,tWH,tWB)
    wait_us(tWB_US);

    _cle = 0;
    _io.input();

    // wait ready
    while(_rbb==0){
        __NOP();    
    }

    return statusRead(cs);
}

uint8_t RawNAND::pageProgram(uint8_t cs, const uint8_t * writeData,uint16_t blockAddress,uint8_t pageAddress,uint16_t columnAddress,uint16_t beats){
    // wait ready
    while(_rbb==0){
        __NOP();    
    }

    // 1ST PROGRAM COMMAND (0x80)
    if(cs==0) {
        _ceb0 = 0;
    } else if (cs==1) {
        _ceb1 = 0;
    }
    _cle = 1;
    _ale = 0;
    _web = 0;
    _io  = 0x80;
    _io.output();
    // wait setup time : max(tCS,tCLS,tALS,tDS,tWP)
    wait_us(tCS_US);

    _web = 1;
    // wait hold time : max(tCLH,tDH,tWH)
    wait_us(tWH_US);

    // 1st address column [7:0]
    _cle = 0;
    _ale = 1;
    _web = 0;
    _io  = columnAddress & 0xff;
    // wait setup time : max(tALS,tCLS,tDS,tWP)
    wait_us(tWP_US);

    _web = 1;
    // wait hold time : max(tCLH,tDH,tWH)
    wait_us(tWH_US);   

    // 2nd address column [11:8]
    _web = 0;
    _io  = (columnAddress>>8) & 0x0f;            
    // wait setup time : max(tALS,tCLS,tDS,tWP)
    wait_us(tWP_US);

    _web = 1;
    // wait hold time : max(tCLH,tDH,tWH)
    wait_us(tWH_US);
    
    // 3rd address  {blockAddress[1:0],pageAddress[5:0]}
    _web = 0;
    _io  = ((blockAddress<<6) | pageAddress) ;            
    // wait setup time : max(tALS,tDS,tWP)
    wait_us(tWP_US);

    _web = 1;
    // wait hold time : max(tALH,tDH,tWH)
    wait_us(tWH_US);
    
    // 4th address  blockAddress[9:2]
    _web = 0;
    _io  = (blockAddress>>2) ;   
    // wait setup time : max(tALS,tDS,tWP)
    wait_us(tWP_US);

    _web = 1;
    // wait hold time : max(tALH,tDH,tWH)
    wait_us(tWH_US);

    // datain
    _ale = 0;
    for (int b=0;b<beats;b++) {
        _io = *(writeData+b);
        _web = 0;
        // setup
        // wait setup time : max(tALS,tCLS,tDS,tWP)
        wait_us(tWP_US);
        
        _web = 1;
        // hold
        // wait hold time : max(tALH,tDH,tWH)
        wait_us(tWH_US);        
    }

    // 2ND PROGRAM COMMAND (0x10)
    _cle = 1;
    _ale = 0;
    _io  = 0x10;
    _web = 0;
    // wait setup time : max(tALS,tCLS,tDS,tWP)
    wait_us(tWP_US);

    _web = 1;
    // wait hold time : max(tCLH,tDH,tWH,tWB)
    wait_us(tWB_US);
    
    _cle = 0;
    _io.input();
    
    // wait ready
    while(_rbb==0){
        __NOP();    
    }
    
    return statusRead(cs);
}