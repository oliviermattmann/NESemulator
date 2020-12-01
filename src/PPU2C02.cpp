//
// Created by colin on 01.12.2020.
//

#include "PPU2C02.h"



PPU2C02::PPU2C02() {
    // Todo Init Code, Bus ect...
    // Set IRQ Ignore bit
    /*
     * https://wiki.nesdev.com/w/index.php/IRQ
     */
    // disable PPU2C02 NMI and rendering
    this->set_ppu_ctrl(NMI_ENABLE, false);
    this->set_ppu_mask(SPRITE_ENABLE, false);
    this->set_ppu_mask(BACKGROUND_ENABLE, false);
    this->set_ppu_mask(BACKGR_LEFT_COL_ENABLE, false);
    this->set_ppu_mask(SPRITE_LEFT_COL_ENABLE, false);
    // I think this is all the rendering? D:
}

PPU2C02::~PPU2C02() = default;

void PPU2C02::set_ppu_ctrl(CTRL_MASK b, bool status) {
    if (status) {
        this->ppu_ctrl |= b;
    } else {
        this->ppu_ctrl &= ~b;
    }
}

bool PPU2C02::get_ppu_ctrl(CTRL_MASK b) const {
    return (b & this->ppu_ctrl);
}

void PPU2C02::set_ppu_ctrl(uint8_t val) {
    this->ppu_ctrl = val;
}

uint8_t PPU2C02::get_ppu_ctrl() const {
    return this->ppu_ctrl;
}

// Mask Register

void PPU2C02::set_ppu_mask(MASK_MASK b, bool status) {
    if (status) {
        this->ppu_mask |= b;
    } else {
        this->ppu_mask &= ~b;
    }
}

bool PPU2C02::get_ppu_mask(MASK_MASK b) const {
    return (b & this->ppu_mask);
}

void PPU2C02::set_ppu_mask(uint8_t val) {
    this->ppu_mask = val;
}

uint8_t PPU2C02::get_ppu_mask() const {
    return this->ppu_mask;
}

// Status Register

void PPU2C02::set_ppu_stat(STAT_MASK b, bool status) {
    if (status) {
        this->ppu_stat |= b;
    } else {
        this->ppu_stat &= ~b;
    }
}

bool PPU2C02::get_ppu_stat(STAT_MASK b) const {
    return (b & this->ppu_stat);
}

void PPU2C02::set_ppu_stat(uint8_t val) {
    this->ppu_stat = val;
}

uint8_t PPU2C02::get_ppu_stat() const {
    return this->ppu_stat;
};

// oam address register

void PPU2C02::set_oam_addr(uint8_t val) {
    this->oam_addr = val;
}

uint8_t PPU2C02::get_oam_addr() const {
    return this->oam_addr;
}

// oam data register

void PPU2C02::set_oam_data(uint8_t val) {
    this->oam_data = val;
}

uint8_t PPU2C02::get_oam_data() const {
    return this->oam_data;
}

// ppu scroll register

void PPU2C02::set_ppu_scro(uint8_t val) {
    this->ppu_scro = val;
}

uint8_t PPU2C02::get_ppu_scro() const {
    return this->ppu_scro;
}

// ppu address register

void PPU2C02::set_ppu_addr(uint8_t val) {
    this->ppu_addr = val;
}

uint8_t PPU2C02::get_ppu_addr() const {
    return this->ppu_addr;
}

// ppu data register

void PPU2C02::set_ppu_data(uint8_t val) {
    this->ppu_data = val;
}

uint8_t PPU2C02::get_ppu_data() const {
    return this->ppu_data;

}

// oam dma register

void PPU2C02::set_oam_dma(uint8_t val) {
    this->ppu_addr = val;
}

uint8_t PPU2C02::get_oam_dma() const {
    return this->ppu_addr;
}

/* Read and Write Operations */

uint8_t PPU2C02::readPPU(uint16_t address) {
    return 0;
}

void PPU2C02::writePPU(uint16_t address, uint8_t data) {
    return;
}


uint8_t PPU2C02::readCPU(uint16_t address) const {
    switch (address & 0x0007) {
        case 0x0000:
            return this->get_ppu_ctrl();
        case 0x0001:
            return this->get_ppu_mask();
        case 0x0002:
            return this->get_ppu_stat();
        case 0x0003:
            return this->get_oam_addr();
        case 0x0004:
            return this->get_oam_data();
        case 0x0005:
            return this->get_ppu_scro();
        case 0x0006:
            return this->get_ppu_addr();
        case 0x0007:
            return this->get_ppu_data();
        case 0x4014: // Todo What about this?
            return this->get_oam_dma();
        default:
            return 0x0;
    }
}

void PPU2C02::writeCPU(uint16_t address, uint8_t data) {
    switch (address) {
        case 0x0000:
            this->set_ppu_ctrl(data);
        case 0x0001:
            this->set_ppu_mask(data);
        case 0x0002:
            this->set_ppu_stat(data);
        case 0x0003:
            this->set_oam_addr(data);
        case 0x0004:
            this->set_oam_data(data);
        case 0x0005:
            this->set_ppu_scro(data);
        case 0x0006:
            this->set_ppu_addr(data);
        case 0x0007:
            this->set_ppu_data(data);
        case 0x4014:  // Todo What about this?
            this->set_oam_dma(data);
        default:
            break;
    }
}

