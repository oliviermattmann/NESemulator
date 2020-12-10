
#include "Bus.h"

PPU2C02::PPU2C02() {
    // Todo Init Code, Bus ect...
    // Set IRQ Ignore bit
    /*
     * https://wiki.nesdev.com/w/index.php/IRQ
     */
    // memory init
    for (uint8_t & text : this->VRAM) {
        text = 0x00;
    }
    for (uint8_t & text : this->SPR_RAM) {
        text = 0x00;
    }
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
    // After write increment address
    if(this->get_ppu_ctrl() & INCR_MODE) {
        this->ppu_addr += 1;
    } else {
        this->ppu_addr += 32;
    }
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


uint8_t PPU2C02::readPPU(uint8_t address) { //ToDo  distinguish color palettes here
    if (this->valid_read) { //if read twice
        this->valid_read = !this->valid_read;
        //combine buffered addresses to u16int and wrap around from $4000-$FFFF
        return this->VRAM[((address << 8) | (this->vram_buffer & 0xFF)) % 0x3FFF];
    } else {
        this->valid_read = !this->valid_read;
        //buffer address
        this->vram_buffer = address;
        return 0x0; //invalid
    }
}


void PPU2C02::writePPU(uint8_t address, uint8_t data) {
    this->VRAM[address % 0x3FFF] = data;
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
        case 0x4014:
            return this->get_oam_dma();
        default:
            return 0x0;
    }
}

void PPU2C02::writeCPU(uint16_t address, uint8_t data) {
    switch (address) {
        case 0x0000:
            this->set_ppu_ctrl(data);
            break;
        case 0x0001:
            this->set_ppu_mask(data);
            break;
        case 0x0002:
            this->set_ppu_stat(data);
            break;
        case 0x0003:
            this->set_oam_addr(data);
            break;
        case 0x0004:
            this->set_oam_data(data);
            break;
        case 0x0005:
            this->set_ppu_scro(data);
            break;
        case 0x0006:
            this->set_ppu_addr(data);
            break;
        case 0x0007:
            this->set_ppu_data(data);
            break;
        case 0x4014:
            this->set_oam_dma(data);
            break;
        default:
            break;
    }
}
void PPU2C02::clock() {
//does nothing for now
}
