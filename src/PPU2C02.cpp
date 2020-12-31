
#include <random>
#include <functional>
#include "PPU2C02.h"

PPU2C02::PPU2C02(Bus *busRef, Screen &screenRef) :
    ppuScreen(screenRef)
{
    bus = busRef;
    bus->ppu = this;
    // Todo Init Code, Bus ect...
    // Set IRQ Ignore bit
    /*
     * https://wiki.nesdev.com/w/index.php/IRQ
     */
    // memory init
    for (uint8_t & text : this->nameTable[0]) {
        text = 0x00;
    }
    for (uint8_t & text : this->nameTable[1]) {
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
/*
 * GET AND SET METHODS
 */


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
    address = address & 0x3FFF;
    uint8_t temp = 0x00;
    if (address < 0x2000) {
        temp =  bus->cartridge.chrData[address];
    } else if (address <= 0x3EFF) {
        address = (address & 0x0FFF);
        if (address >= 0x0000 && address <= 0x03FF)
            temp = nameTable[0][address & 0x03FF];
        if (address >= 0x0400 && address <= 0x07FF)
            temp = nameTable[0][address & 0x03FF];
        if (address >= 0x0800 && address <= 0x0BFF)
            temp = nameTable[1][address & 0x03FF];
        if (address >= 0x0C00 && address <= 0x0FFF)
            temp = nameTable[1][address & 0x03FF];
    } else if (address >= 0x3F00 && address <= 0x3FFF) {
        address &= 0x001F;
        if (address == 0x0010) {
            address = 0x0000;
        }
        if (address == 0x0014) {
            address = 0x0004;
        }
        if (address == 0x0018) {
            address = 0x0008;
        }
        if (address == 0x001C) {
            address = 0x000C;
        }
        temp = paletteTable[address];
    }

    return temp;
}


void PPU2C02::writePPU(uint16_t address, uint8_t data) {
    address = address & 0x3FFF;
    if (address < 0x2000) {
        return;
    } else if (address <= 0x3EFF) {
        address = (address & 0x0FFF);
        if (address >= 0x0000 && address <= 0x03FF)
            nameTable[0][address & 0x03FF] = data;
        if (address >= 0x0400 && address <= 0x07FF)
            nameTable[0][address & 0x03FF] = data;
        if (address >= 0x0800 && address <= 0x0BFF)
            nameTable[1][address & 0x03FF] = data;
        if (address >= 0x0C00 && address <= 0x0FFF)
            nameTable[1][address & 0x03FF] = data;
    } else if (address >= 0x3F00 && address <= 0x3FFF) {
        address &= 0x001F;
        if (address == 0x0010) {
            address = 0x0000;
        }
        if (address == 0x0014) {
            address = 0x0004;
        }
        if (address == 0x0018) {
            address = 0x0008;
        }
        if (address == 0x001C) {
            address = 0x000C;
        }
        paletteTable[address] = data;
    }
}

void PPU2C02::reset_buff() {
    this->vram_buffer_d = 0;
    this->vram_buffer_r = 0;
    this->vram_buffer_w = 0;
    this->valid_write = 0;
    this->valid_write = 0;
}


uint8_t PPU2C02::readCPU(uint16_t address)  {
    uint8_t dataOut = 0x00;
    if (address == 0x4014) {
        dataOut = get_oam_dma();
    }
    switch (address & 0x0007) {
        //control
        case 0x0000:
            //dataOut = get_ppu_ctrl();
            break;
        //Mask
        case 0x0001:
            //dataOut = get_ppu_mask();
            break;
        //Status
        case 0x0002:
            dataOut = get_ppu_stat() & 0xE0;//| (buffer&0x1F);
            set_ppu_stat(STAT_MASK::VBLANK, false);
            addrLatch = false;
            break;
        //OAM Address
        case 0x0003:
            //dataOut = get_oam_addr();
            break;
        //OAM Data
        case 0x0004:
            dataOut = get_oam_data();
            break;
        //Scroll
        case 0x0005:
            //dataOut = get_ppu_scro();
            break;
        //PPU Address
        case 0x0006:
            //dataOut = get_ppu_addr();
            break;
        //PPU Data
        case 0x0007:
            dataOut = buffer;
            buffer = readPPU(ppuAddress);
            if (ppuAddress >= 0x3F00) {
                dataOut = buffer;
            }
            ppuAddress += (get_ppu_ctrl(CTRL_MASK::INCR_MODE)? 32 : 1);
            break;
        //DMA, not used yet
        case 0x4014: // Todo What about this?
            dataOut = get_oam_dma();
            break;
    }
    return dataOut;
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
            cout << hex << int(bus->cpu->op_code) << endl;
            //this->set_ppu_stat(data);
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
            if (!addrLatch) {
                tempAddress =  uint16_t ((data&0x3F) << 8) | tempAddress&0x00FF;
                addrLatch = true;
            } else {
                tempAddress = (tempAddress&0xFF00) | uint16_t (data);
                ppuAddress = tempAddress;
                addrLatch = false;
            }
            break;
        case 0x0007:
            this->writePPU(ppuAddress, data);
            ppuAddress += (get_ppu_ctrl(CTRL_MASK::INCR_MODE)? 32 : 1);
            break;
        case 0x4014:  // Todo What about this?
            this->set_oam_dma(data);
        default:
            break;
    }
}
void PPU2C02::clock() {
    //bus->renderer->updatePixel();
    if (scanLine == -1 && cycle == 1) {
        set_ppu_stat(STAT_MASK::VBLANK, false);
    }

    if (scanLine == 241 && cycle == 1) {
        set_ppu_stat(STAT_MASK::VBLANK, true);
        frameDone = true;
        if (get_ppu_ctrl(CTRL_MASK::NMI_ENABLE)) {
            nmiVblank();
            std::cout << "nmi" << std::endl;
        }
        //nmiVblank();
    }
    if (++cycle == 341) {
        scanLine++;
        cycle = 0;
        if (scanLine == 261) {
            scanLine = -1;
        }
    }
}


/*void PPU2C02::display_pixel(uint16_t p) {
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
 */
//Print pattern Table atm
void PPU2C02::drawToScreen() {
    uint8_t x = 0, y = 0;
    sf::Color col;
    for (int i = 0; i < 960; i++) {
        getPatternTile(nameTable[0][i]+256);
        x = (i* 8)% 256;
        if(i%32 == 0 && i !=0) {
            y +=8;
        }
        for(int j = 0; j < 8; j++) {
            for (int k = 0; k < 8; k++) {
                switch (pixelData[j][k]) {
                    case 0: col = getColor(0x0f);
                        break;
                    case 1: col = getColor(0x15);
                        break;
                    case 2: col = getColor(0x2c);
                        break;
                    case 3: col = getColor(0x12);
                        break;
                }
                ppuScreen.setPixel(x + k, y + j, col);
            }
        }
    }
}

void PPU2C02::getPatternTile(uint16_t index) {
    index = index * 16;

    for (int i = 0; i < 8; i++) {
        uint8_t lo = bus->cartridge.chrData[index + i];
        uint8_t hi = bus->cartridge.chrData[index + i + 8];
        uint8_t temp;
        for (int j = 7; j >=0; j--) {
            if (lo & (1 << j)) {
                if (hi & (1 << j)){
                    temp = 3;
                } else {
                    temp = 1;
                }
            } else if (hi & (1 << j)) {
                temp = 2;
            } else {
                temp = 0;
            }
            pixelData[i][7-j] = temp;
        }
    }
}

sf::Color PPU2C02::getColor(int8_t index) {
    return {pallets[index].r, pallets[index].g, pallets[index].b, 255};
}