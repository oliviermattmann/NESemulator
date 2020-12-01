//
// Created by colin on 01.12.2020.
//

#ifndef NESEMULATOR_PPU2C02_H
#define NESEMULATOR_PPU2C02_H

#include <cstdint>

/**
 *  Emulating the NES picture processing unit.
 *  Documentation from https://wiki.nesdev.com
 *  */
class PPU2C02 {

public: PPU2C02();

    ~PPU2C02();

    /* Registers */

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
        ST_SELECT  = 1 << 3,
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
        BACKGR_LEFT_COL_ENABLE  = 1 << 1,
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
        SPRITE_ZERO_HIT  = 1 << 6,
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

    /**
     * https://wiki.nesdev.com/w/index.php/PPU_registers#PPUSCROLL
     */
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

    /* Read and Write Operations */

    /**
     * Direct Access for reading register data.
     * @param addr requested register.
     * @return uint8_t data.
     */
    uint8_t readCPU(uint16_t addr) const;

    /**
    * Direct Access for writing register data.
    * Overwrites complete register.
    * @param addr requested register.
    * @param val data to write.
    */
    void writeCPU(uint16_t addr, uint8_t val);

    uint8_t readPPU(uint16_t address);

    void writePPU(uint16_t address, uint8_t data);


};


#endif //NESEMULATOR_PPU2C02_H
