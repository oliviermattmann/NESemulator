

#ifndef NESEMULATOR_PPU2C02_H
#define NESEMULATOR_PPU2C02_H
#include <cstdint>


class PPU2C02 {

    PPU2C02();
    ~PPU2C02();

public:
    uint8_t readPPU(uint16_t address);
    void writePPU(uint16_t address, uint8_t data);

    //communication with cpu via main bus
    uint8_t readCPU(uint16_t address);
    void writeCPU(uint16_t address, uint8_t data);


private:

    /*
        PPUCTRL 	$2000 	VPHB SINN 	NMI enable (V), PPU master/slave (P), sprite height (H), background tile select (B), sprite tile select (S), increment mode (I), nametable select (NN)
        PPUMASK 	$2001 	BGRs bMmG 	color emphasis (BGR), sprite enable (s), background enable (b), sprite left column enable (M), background left column enable (m), greyscale (G)
        PPUSTATUS 	$2002 	VSO- ---- 	vblank (V), sprite 0 hit (S), sprite overflow (O); read resets write pair for $2005/$2006
        OAMADDR 	$2003 	aaaa aaaa 	OAM read/write address
        OAMDATA 	$2004 	dddd dddd 	OAM data read/write
        PPUSCROLL 	$2005 	xxxx xxxx 	fine scroll position (two writes: X scroll, Y scroll)
        PPUADDR 	$2006 	aaaa aaaa 	PPU read/write address (two writes: most significant byte, least significant byte)
        PPUDATA 	$2007 	dddd dddd 	PPU data read/write
     */
    uint8_t Control = 0x00;
    uint8_t Mask = 0x00;
    uint8_t Status = 0x00;
    uint8_t OAMAddress = 0x00;
    uint8_t OAMData = 0x00;
    uint8_t Scroll = 0x00;
    uint8_t PPUAddress = 0x00;
    uint8_t PPUData = 0x00;
    //MEMORY MAP
    /*
        $0000-$0FFF 	$1000 	Pattern table 0
        $1000-$1FFF 	$1000 	Pattern table 1
        $2000-$23FF 	$0400 	Nametable 0
        $2400-$27FF 	$0400 	Nametable 1
        $2800-$2BFF 	$0400 	Nametable 2
        $2C00-$2FFF 	$0400 	Nametable 3
        $3000-$3EFF 	$0F00 	Mirrors of $2000-$2EFF
        $3F00-$3F1F 	$0020 	Palette RAM indexes
        $3F20-$3FFF 	$00E0 	Mirrors of $3F00-$3F1F
     */
    uint8_t VRAM[16000];


};


#endif //NESEMULATOR_PPU2C02_H
