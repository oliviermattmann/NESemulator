
#ifndef NESEMULATOR_PPU2C02_H
#define NESEMULATOR_PPU2C02_H
#include "SFML/Graphics.hpp"
#include "Screen.h"
#include <cstdint>
#include "Bus.h"
#include <functional>

class Bus;
/**
 *  Emulating the NES picture processing unit.
 *  Documentation from https://wiki.nesdev.com
 *  */
class PPU2C02 {


public:
    Bus *bus;
    PPU2C02(Bus *busRef, Screen &screenRef);
    ~PPU2C02();
    void setNMI(std::function<void(void)> nmi);

    uint8_t buffer = 0x00;
    uint16_t ppuAddress = 0x0000;
    uint16_t tempAddress = 0x0000;
    bool addrLatch = false;

    int16_t scanLine = -1;
    uint16_t cycle = 0;
    std::function<void(void)> nmiVblank;

    uint8_t nameTable[2][1024];
    uint8_t paletteTable[32];


    //Variables for the Screen
    uint8_t pixelData[8][8];
    bool frameDone = false;
    Screen &ppuScreen;
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



    uint8_t vram_buffer_r;
    uint8_t vram_buffer_d;
    uint8_t vram_buffer_w;

    int valid_read;
    int valid_write;

    /**
     * Resets Buffer values.
     */
    void reset_buff();

    uint8_t SPR_RAM[256];

    /**
     * Contains a color in rgb code.
     */
    struct color {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    /**
     * 56 colors. https://wiki.nesdev.com/w/images/5/59/Savtool-swatches.png
     */
    color pallets[64];

    /* Registers */
    /*
           PPUCTRL 	    $2000 	VPHB SINN 	NMI enable (V), PPU master/slave (P), sprite height (H), background tile select (B), sprite tile select (S), increment mode (I), nametable select (NN)
           PPUMASK 	    $2001 	BGRs bMmG 	color emphasis (BGR), sprite enable (s), background enable (b), sprite left column enable (M), background left column enable (m), greyscale (G)
           PPUSTATUS 	$2002 	VSO- ---- 	vblank (V), sprite 0 hit (S), sprite overflow (O); read resets write pair for $2005/$2006
           OAMADDR 	    $2003 	aaaa aaaa 	OAM read/write address
           OAMDATA 	    $2004 	dddd dddd 	OAM data read/write
           PPUSCROLL 	$2005 	xxxx xxxx 	fine scroll position (two writes: X scroll, Y scroll)
           PPUADDR 	    $2006 	aaaa aaaa 	PPU read/write address (two writes: most significant byte, least significant byte)
           PPUDATA 	    $2007 	dddd dddd 	PPU data read/write
        */
    uint8_t ppu_ctrl;   // Control Register
    uint8_t ppu_mask;   // Mask Register
    uint8_t ppu_stat;   // Status Register
    uint8_t ppu_scro;   // Fine Scroll Register
    uint8_t ppu_addr;   // Address Register
    uint8_t ppu_data;   // Data Register
    uint8_t oam_addr;   // Address Register, OAM
    uint8_t oam_data;   // Data Register, OAM
    uint8_t oam_dma;    // Direct Memory Access Register, OAM

    /* Register Operations */

    // Control Register

    /**
    *  To check or set a specific bit
    *  in PPU2C02 Control Register.
    *  0000 0000
    *  VPHB SINN
    */
    enum CTRL_MASK {
        NAMET_SELECT_RB = 1 << 0,
        NAMET_SELECT_LB = 1 << 1,
        INCR_MODE = 1 << 2,
        ST_SELECT = 1 << 3,
        BT_SELECT = 1 << 4,
        SPRITE_HEIGHT = 1 << 5,
        PPU_MS = 1 << 6,
        NMI_ENABLE = 1 << 7,
    };

    /**
     * Single bit access for control register.
     * @param b specified bit.
     * @param status set value.
     */
    void set_ppu_ctrl(CTRL_MASK b, bool status);

    /**
    * Single bit access for control register.
    * @param b specified bit.
    * @return boolean value of bit.
    */
    bool get_ppu_ctrl(CTRL_MASK b) const;

    void set_ppu_ctrl(uint8_t val);

    uint8_t get_ppu_ctrl() const;


    // Mask Register

    /**
    *  To check or set a specific bit
    *  in PPU2C02 Mask Register.
    */
    enum MASK_MASK {
        GREYSCALE = 1 << 0,
        BACKGR_LEFT_COL_ENABLE = 1 << 1,
        SPRITE_LEFT_COL_ENABLE = 1 << 2,
        BACKGROUND_ENABLE = 1 << 3,
        SPRITE_ENABLE = 1 << 4,
        COLOR_EMPHASIS_R = 1 << 5,
        COLOR_EMPHASIS_G = 1 << 6,
        COLOR_EMPHASIS_B = 1 << 7
    };

    /**
     * Single bit access for mask register.
     * @param b specified bit.
     * @param status set value.
     */
    void set_ppu_mask(MASK_MASK b, bool status);

    /**
    * Single bit access for mask register.
    * @param b specified bit.
    * @return boolean value of bit.
    */
    bool get_ppu_mask(MASK_MASK b) const;

    void set_ppu_mask(uint8_t val);

    uint8_t get_ppu_mask() const;

    // Status Register

    /**
    *  To check or set a specific bit
    *  in PPU2C02 STATUS.
    */
    enum STAT_MASK {
        SPRITE_OVERFLOW = 1 << 5,
        SPRITE_ZERO_HIT = 1 << 6,
        VBLANK = 1 << 7,
    };

    /**
    * Single bit access for status register.
    * @param b specified bit.
    * @param status set value.
    */
    void set_ppu_stat(STAT_MASK b, bool status);

    /**
    * Single bit access for status register.
    * @param b specified bit.
    * @return boolean value of bit.
    */
    bool get_ppu_stat(STAT_MASK b) const;

    void set_ppu_stat(uint8_t val);

    uint8_t get_ppu_stat() const;

    // oam address register

    void set_oam_addr(uint8_t val);

    uint8_t get_oam_addr() const;

    // oam data register

    void set_oam_data(uint8_t val);

    uint8_t get_oam_data() const;

    // ppu scroll register

    void set_ppu_scro(uint8_t val);

    uint8_t get_ppu_scro() const;

    // ppu address register

    void set_ppu_addr(uint8_t val);

    uint8_t get_ppu_addr() const;

    // ppu data register

    void set_ppu_data(uint8_t val);

    uint8_t get_ppu_data() const;

    // oam dma register

    void set_oam_dma(uint8_t val);

    uint8_t get_oam_dma() const;

    // get/set register based on address (for CPU)
    uint8_t get_register(uint16_t address);

    void set_register(uint16_t address, uint8_t data);

    /* Read and Write Operations */

    /**
     * Direct Access for reading register data.
     * @param addr requested register.
     * @return uint8_t data.
     */
    uint8_t readCPU(uint16_t addr);

    /**
    * Direct Access for writing register data.
    * Overwrites complete register.
    * @param addr requested register.
    * @param val data to write.
    */
    void writeCPU(uint16_t addr, uint8_t val);

    uint8_t readPPU(uint16_t address);

    void writePPU(uint16_t address, uint8_t data);
/*
    void bindToBus(Bus *ourBus, Screen *screen1) {
        bus = ourBus;
    }
*/
    void clock();

    /**
     * The pattern table is an area of memory connected to the
     * PPU that defines the shapes of tiles that make up backgrounds
     * and sprites. Each tile in the pattern table is 16 bytes,
     * made of two planes. The first plane controls bit 0 of the color;
     * the second plane controls bit 1. Any pixel whose color is 0 is background/transparent
     * (represented by '.' in the following diagram)
     */
    void display_pixel(uint16_t p);

    void drawToScreen();

    void getPatternTile(uint16_t index);

    sf::Color getColor(int8_t index);
};
#endif //NESEMULATOR_PPU2C02_H

