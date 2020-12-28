
#include <random>
#include <functional>
#include "PPU2C02.h"

PPU2C02::PPU2C02(Bus& mainBus, Screen& screen) :
        ppuScreen(screen),
        bus(mainBus)
{
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
    // Buffer
    this->reset_buff();
    // disable PPU2C02 NMI and rendering
    this->set_ppu_ctrl(NMI_ENABLE, false);
    this->set_ppu_mask(SPRITE_ENABLE, false);
    this->set_ppu_mask(BACKGROUND_ENABLE, false);
    this->set_ppu_mask(BACKGR_LEFT_COL_ENABLE, false);
    this->set_ppu_mask(SPRITE_LEFT_COL_ENABLE, false);

    //default color pallets
    color arr[] {
        {84, 84, 84},
        {0, 30, 116},
        {8, 16, 144},
        {48, 0, 136},
        {68, 0, 100},
        {92, 0, 48},
        {84, 4, 0},
        {60, 24, 0},
        {32, 42, 0},
        {8, 58, 0},
        {0, 64, 0},
        {0, 60, 0},
        {0, 50, 60},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {152, 150, 152},
        {8, 76, 196},
        {48, 50, 236},
        {92, 30, 228},
        {136,20, 176},
        {160, 20, 100},
        {152, 34, 32},
        {120, 60, 0},
        {84, 90, 0},
        {40, 114, 0},
        {8, 124, 0},
        {0, 118, 40},
        {0, 102, 120},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {236, 238, 236},
        {76, 154, 236},
        {120, 124, 236},
        {176, 98, 236},
        {228, 84, 236},
        {236, 88, 180},
        {236, 106, 100},
        {212, 136, 32},
        {160, 170, 0},
        {116, 196, 0},
        {76, 208, 32},
        {56, 204, 108},
        {56, 180, 204},
        {60, 60, 60},
        {0, 0, 0},
        {0, 0, 0},
        {236, 238, 236},
        {168, 204, 236},
        {188, 188, 236},
        {212, 178, 236},
        {236, 174, 236},
        {236, 174, 212},
        {236, 180, 176},
        {228, 196, 144},
        {204, 210, 120},
        {180, 222, 120},
        {168, 226, 144},
        {152, 226, 180},
        {160, 214, 228},
        {160, 162, 160},
        {0, 0, 0},
        {0, 0, 0}
    };

    for (int i = 0; i < 64; i++) {
        this->pallets[i] = arr[i];
    }
}

PPU2C02::~PPU2C02() = default;

void PPU2C02::setNMI(std::function<void()> nmi) {
    nmiVblank = nmi;
}

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

/*
uint8_t PPU2C02::readPPU(uint8_t address) {
 To  distinguish color palettes here
    if (this->valid_read) { //if read twice
        this->valid_read = 0;
        //combine buffered addresses to u16int and wrap around from $4000-$FFFF
        return this->VRAM[((address << 8) | (this->vram_buffer_r & 0xFF)) % 0x3FFF];
    } else {
        this->valid_read = 1;
        //buffer address
        this->vram_buffer_r = address;
        return 0x0; //invalid
    }*/

uint8_t PPU2C02::readPPU(uint16_t address) {
    address = address & 0x4000;
    if (address <= 0x1FFF) {
        return bus->cartridge.chrData[address];
    } else if (address <= 0x3EFF) {
        address = (address & 0x0400);
        if (address < 0x0200) {
            return name_table[address];
        } else {
            return attr_table[address & 0x0200];
        }
    } else if (address < 0x3F20) {
        address = address & 0x0020;
        if (address < 0x0010) {
            return image_palette[address];
        } else {
            return sprite_palette[address & 0x0010];
        }
    }
    return 0;
}


void PPU2C02::writePPU(uint16_t address, uint8_t data) {
   /* if (this->get_ppu_stat() & VBLANK) {
        return;
    } else if (this->valid_write) {
        this->valid_write = 0;
        this->VRAM[((address << 8) | (this->vram_buffer_w & 0xFF)) % 0x3FFF] = this->vram_buffer_d;
    } else {
        this->valid_read = 1;
        this->vram_buffer_d = data;
        this->vram_buffer_w = address;
    }*/
    address = address & 0x4000;
    if (address <= 0x1FFF) {
        bus->cartridge.chrData[address] = data;
    } else if (address <= 0x3EFF) {
        address = (address & 0x0400);
        if (address < 0x0200) {
            name_table[address] = data;
        } else {
            attr_table[address & 0x0200] = data;
        }
    } else if (address < 0x3F20) {
        address = address & 0x0020;
        if (address < 0x0010) {
            image_palette[address] = data;
        } else {
            sprite_palette[address & 0x0010] = data;
        }
    }

}

void PPU2C02::reset_buff() {
    this->vram_buffer_d = 0;
    this->vram_buffer_r = 0;
    this->vram_buffer_w = 0;
    this->valid_write = 0;
    this->valid_write = 0;
}


uint8_t PPU2C02::readCPU(uint16_t address) const {
    if (address == 0x4014) {
        return this->get_oam_dma();
    }
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
    if (address == 0x4014) {
       this->set_oam_dma(data);
    }
    switch (address) {
        case 0x0000:
            this->set_ppu_ctrl(data);
            break;
        case 0x0001:
            this->set_ppu_mask(data);
            break;
        case 0x0002:
            cout << "PPU reg 0x2002 is read only.";
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
        case 0x4014:  // Todo What about this?
            this->set_oam_dma(data);
        default:
            break;
    }
}
void PPU2C02::clock() {
    //bus->renderer->updatePixel();
    scanLine = (scanLine+1);
    if (scanLine == 20000) {
        frameDone = true;
        //nmiVblank();
    }
//does nothing for now
}


void PPU2C02::display_pixel(uint16_t p) {
    // -> nesdoc page 19
    if (p > 1024) {
        return;
    }
    std::string buff_out;
    if (p % 2 == 1) p--;
    int i = p * 8;
    int j = i + 8;
    while (true) {
        uint8_t p_1 = this->VRAM[i];
        uint8_t p_2 = this->VRAM[j];
        for (int b = 0; b < 8; b++) {
            if (p_1 & 1 << b && p_2 & 1 << b) {
                buff_out.append("3 ");
            } else if (p_1 & 1 << b) {
                buff_out.append("1 ");
            } else if (p_2 & 1 << b) {
                buff_out.append("2 ");
            } else {
                buff_out.append("0 ");
            }
        }
        buff_out.append("\n");
        i++;
        j++;
        if (i % 8 == 0) {
            break;
        }
    }
    cout << buff_out.c_str() << endl;
}
