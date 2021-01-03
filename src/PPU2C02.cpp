
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
    renderState = PreRender;
    // Buffer
    this->reset_buff();
    // disable PPU2C02 NMI and rendering
    this->set_ppu_ctrl(NMI_ENABLE, false);
    this->set_ppu_mask(SPRITE_ENABLE, false);
    this->set_ppu_mask(BACKGROUND_ENABLE, false);
    this->set_ppu_mask(BACKGR_LEFT_COL_ENABLE, false);
    this->set_ppu_mask(SPRITE_LEFT_COL_ENABLE, false);



    for (int i = 0; i < 64; i++) {
        this->colors[i] = sf::Color {colorData[i].r, colorData[i].g, colorData[i].b, 255};
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
        //cout << int(address) << endl;
        //cout << int((address & 0x1000) >> 12) << endl;
        temp = bus->cartridge.chrData[address];//patternTable[(address & 0x1000) >> 12][(address & 0x0FFF)];
        temp = temp;
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
        patternTable[(address & 0x1000) >> 12][(address & 0x0FFF)] = data;
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
            dataOut = get_ppu_stat() & 0xE0 | (dataBuffer&0x1F);
            set_ppu_stat(STAT_MASK::VBLANK, false);
            writeToggle = false;
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
            dataOut = dataBuffer;
            dataBuffer = readPPU(absLoopy);
            if (absLoopy >= 0x3F00) {
                dataOut = dataBuffer;
            }

            absLoopy += (get_ppu_ctrl(CTRL_MASK::INCR_MODE)? 32 : 1);
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
            setNameTableID(data&0x03);
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
            if (!writeToggle) {
                if (data != 0) {
                    cout << "problem" << endl;
                }
                //this->set_ppu_scro(data);
                setCoarseX(data >> 3);
                setFineX(data & 0x07);
                writeToggle = true;
            } else {
                if (data != 0) {
                    cout << "problem" << endl;
                }
                //this->set_ppu_scro(data);
                setCoarseY(data >> 3);
                setFineY(data & 0x07);
                writeToggle = false;
            }
            break;
        case 0x0006:
            //cout << ((get_ppu_stat(STAT_MASK::VBLANK) > 0) ? 1:0) << endl;
            if (!writeToggle) {
                //tempLoopy = (uint16_t)((data&0x3F)<<8) | (tempLoopy & 0x00FF);
                setFineY(((data & 0x30) >> 4)&0x03);
                setNameTableID(((data & 0x0C) >> 2)&0x03);
                setCoarseYHi(data & 0x03);
                writeToggle = true;
            } else {
               //setCoarseYLow((data & 0xE0) >> 5);
               //setCoarseX(data & 0x1F);
               tempLoopy = (tempLoopy & 0xFF00) | data;

               absLoopy = tempLoopy;
               writeToggle = false;
            }
            break;
        case 0x0007:
            this->writePPU(absLoopy, data);
            absLoopy += (get_ppu_ctrl(CTRL_MASK::INCR_MODE)? 32 : 1);
            //cout <<int (get_ppu_ctrl(CTRL_MASK::INCR_MODE)) << endl;
            break;
        case 0x4014:  // Todo What about this?
            this->set_oam_dma(data);
        default:
            break;
    }
}
void PPU2C02::clock() {


    switch (renderState) {
        case PreRender:
            if ((scanLine == -1) && cycle == 1) {
                set_ppu_stat(STAT_MASK::VBLANK, false);
                set_ppu_stat(STAT_MASK::SPRITE_ZERO_HIT, false);
                set_ppu_stat(STAT_MASK::SPRITE_OVERFLOW, false);
                nameTableFetch = readPPU(getTileAddress(absLoopy));
            }
            if ((scanLine == -1) && cycle > 1) {
                if (cycle < 256 || (cycle > 320 && cycle < 337)) {
                    uint16_t tempAddress = ((nameTableFetch*16) + (absLoopy >> 12)&0x07) | ((get_ppu_ctrl(CTRL_MASK::BT_SELECT))<<12);
                    switch (cycle % 8) {
                        case 0:
                            incrementCoarseX();
                            loadShifters();
                            break;
                        case 1:
                            //cout << "CoarseX: " << int(getCoarseX()) << endl;
                            //cout << "absLoopy: " << bitset<16> (absLoopy) << endl;
                            nameTableFetch = readPPU(getTileAddress(absLoopy));
                            //cout << int(nameTableFetch) << endl;
                            break;
                        case 3:
                            attributeFetch = readPPU(getAttributeAddress(absLoopy));
                            //the Attribute Fetch variable need to be adjusted to the section the tile is in
                            if ((getCoarseX() & 0x02) && (getCoarseY() & 0x02)) {
                                attributeFetch >>= 6;
                            } else if (!(getCoarseX() & 0x02) && (getCoarseY() & 0x02)) {
                                attributeFetch >>= 4;
                            } else if ((getCoarseX() & 0x02) && !(getCoarseY() & 0x02)) {
                                attributeFetch >>= 2;
                            }
                            attributeFetch &= 0x03;
                            break;
                        case 5:
                            //cout << int ((get_ppu_ctrl(CTRL_MASK::BT_SELECT) << 12)+ nameTableFetch*16 + getFineY() + 0) << endl;
                            patternFetchLsb = readPPU(((get_ppu_ctrl(CTRL_MASK::BT_SELECT)>>5) << 12)+ nameTableFetch*16 + getFineY() + 0);
                            //cout << bitset<8> (patternFetchLsb) << endl;
                            break;
                        case 7:
                            patternFetchMsb = readPPU(((get_ppu_ctrl(CTRL_MASK::BT_SELECT)>>5) << 12)+ nameTableFetch*16 + getFineY() + 8);
                            break;
                    }
                    shiftShifters();
                }
                //cout << "fineY: " << int(getFineY()) << " , CoarseY: " << int(getCoarseY()) << endl;
                //cout << "fineX: " << int(getFineX()) << " , CoarseX: " << int(getCoarseX()) << endl;
                //cout << "cycle: " << int(cycle) << " , Scanline: " << int(scanLine) << endl;
                if (cycle == 256) {
                    incrementY();
                }
                if (cycle == 257) {
                    //cout << "x transfer before (temp)" << bitset<16>(tempLoopy) << endl;
                    //cout << "x transfer before (abs)" << bitset<16>(absLoopy) << endl;
                    updateLoopyX();
                    //cout << "x transfer after (temp)" << bitset<16>(tempLoopy) << endl;
                    //cout << "x transfer after (abs)" << bitset<16>(absLoopy) << endl;

                    tempLoopy = tempLoopy;
                }
                if ((cycle > 279) || (cycle < 305)) {
                    if (ppu_mask & MASK_MASK::BACKGROUND_ENABLE || ppu_mask & MASK_MASK::SPRITE_ENABLE) {
                        //cout << "Y transfer before (temp)" << bitset<16>(tempLoopy) << endl;
                        //cout << "Y transfer before (abs)" << bitset<16>(absLoopy) << endl;
                        updateLoopyY();
                        //cout << "Y transfer after (temp)" << bitset<16>(tempLoopy) << endl;
                        //cout << "Y transfer after (abs)" << bitset<16>(absLoopy) << endl;
                        tempLoopy = tempLoopy;
                    }
                }
                if (cycle == 337 || cycle == 339) {
                    nameTableFetch = readPPU(getTileAddress(absLoopy));
                }
            }
            cycle++;

            if ((scanLine == -1) && cycle == 341) {
                cycle = 0;
                scanLine = 0;
                renderState = Render;
            }
            break;
        case Render:
            if (ppu_mask & MASK_MASK::BACKGROUND_ENABLE || ppu_mask & MASK_MASK::SPRITE_ENABLE) {
                //cout << bitset<8> (ppu_mask) << endl;
            }
            if (cycle == 0) {

            }
            if ((cycle > 0 && cycle < 256) || (cycle > 320 && cycle < 337)) {
                uint16_t tempAddress = ((nameTableFetch*16) + (absLoopy >> 12)&0x07) | ((get_ppu_ctrl(CTRL_MASK::BT_SELECT))<<12);
                switch (cycle % 8) {
                    case 0:
                        incrementCoarseX();
                        loadShifters();
                        break;
                    case 1:
                        nameTableFetch = readPPU(getTileAddress(absLoopy));
                        break;
                    case 3:
                        attributeFetch = readPPU(getAttributeAddress(absLoopy));
                        //the Attribute Fetch variable need to be adjusted to the section the tile is in
                        if ((getCoarseX() & 0x02) && (getCoarseY() & 0x02)) {
                            attributeFetch >>= 6;
                        } else if (!(getCoarseX() & 0x02) && (getCoarseY() & 0x02)) {
                            attributeFetch >>= 4;
                        } else if ((getCoarseX() & 0x02) && !(getCoarseY() & 0x02)) {
                            attributeFetch >>= 2;
                        }
                        attributeFetch &= 0x03;
                        break;
                    case 5:

                        //cout << "lsb " <<int(tempAddress) << endl;
                        patternFetchLsb = bus->cartridge.chrData[(256*16) + nameTableFetch*16 + getFineY()];

                        //cout << bitset<8> (patternFetchLsb) << endl;

                        //cout << bitset<8> (patternTableRow[0]) << endl;
                        if (patternFetchLsb != 0) {
                           // cout << "lsb not zero" << endl;
                        }
                        break;
                    case 7:
                        //cout << "msb" << int(tempAddress + 8) << endl;
                        patternFetchMsb = bus->cartridge.chrData[(256*16) + nameTableFetch*16 + getFineY() + 8];
                        if (patternFetchMsb != 0) {
                            //cout << "msb not zero" << endl;
                            //cout << bitset<8> (patternFetchMsb) << endl;
                            //cout << bitset<8> (patternTableRow[1]) << endl;
                        }
                        break;
                }
                if ((cycle > 320 && cycle < 337)) {
                    shiftShifters();
                }
            }
            //cout << "fineY: " << int(getFineY()) << " , CoarseY: " << int(getCoarseY()) << endl;
            //cout << "fineX: " << int(getFineX()) << " , CoarseX: " << int(getCoarseX()) << endl;
            //cout << "cycle: " << int(cycle) << " , Scanline: " << int(scanLine) << endl;
            if (cycle == 256) {
                incrementY();
            }
            if ((cycle > 0 && cycle < 257)) {// && ppu_mask & MASK_MASK::BACKGROUND_ENABLE) {
                fineXMultiplexer = 0x8000 >> getFineX();
                if (patternTableDataShift_bg_lsb != 0 || patternTableDataShift_bg_msb != 0) {
                    //cout << "should draw something" << endl;
                }
                uint8_t lsbC = (((patternTableDataShift_bg_lsb & fineXMultiplexer) > 0) ? 1 : 0) << 1;
                uint8_t msbC = ((patternTableDataShift_bg_msb & fineXMultiplexer) > 0) ? 1 : 0;
                uint8_t bgPixelColorID = (msbC | lsbC);
                uint8_t bgPaletteID = (((paletteAttributesShift_bg_msb & fineXMultiplexer) > 0) << 1) |
                                      (paletteAttributesShift_bg_lsb & fineXMultiplexer) > 0;
                //cout << "x transfer before (abs)" << bitset<16>(absLoopy) << endl;
                ppuScreen.setPixel(cycle - 1, scanLine, getColor(bgPaletteID, bgPixelColorID));
                /*cout << bitset<16> (patternTableDataShift_bg_lsb) << endl;
                cout << bitset<16> (patternTableDataShift_bg_msb) << endl;
                cout << bitset<16> (fineXMultiplexer) << endl;*/
                shiftShifters();
                /*cout << bitset<16> (patternTableDataShift_bg_lsb) << endl;
                cout << bitset<16> (patternTableDataShift_bg_msb) << endl;
                cout << bitset<16> (fineXMultiplexer) << endl;*/
            }
            if (cycle == 257) {
                updateLoopyX();
            }
            if (cycle == 337 || cycle == 339) {
                nameTableFetch = readPPU(getTileAddress(absLoopy));
            }

            cycle++;
            if (cycle == 341 && scanLine != 239) {
                cycle = 0;
                scanLine++;
            }
            if (scanLine == 239 && cycle == 341) {
                cycle = 0;
                scanLine = 240;
                renderState = PostRender;
            }
            break;
        case PostRender:
            cycle++;
            if (scanLine == 240 && cycle == 341) {
                scanLine = 241;
                cycle = 0;
                renderState = VerticalBlank;
            }

            break;

        case VerticalBlank:
            if (scanLine == 241 && cycle == 1) {
                set_ppu_stat(STAT_MASK::VBLANK);
                frameDone = true;
                if (get_ppu_ctrl(CTRL_MASK::NMI_ENABLE)) {
                    nmiVblank();
                    //cout << "nmi" << endl;
                }
            }
            if (scanLine < 261 && cycle < 340) {
                cycle++;
            }
            if (scanLine < 260 && cycle == 340) {
                cycle = 0;
                scanLine++;
            }
            if (scanLine == 260 && cycle == 340) {
                cycle = 0;
                scanLine = -1;
                renderState = PreRender;
            }
            break;


    }
}
   /* if (scanLine >= -1 && scanLine < 240) {
        if (scanLine == 0 && cycle == 0) {
            cycle = 1;
        }
        if (scanLine == -1 && cycle == 1) {
            set_ppu_stat(STAT_MASK::VBLANK, false);
            set_ppu_stat(STAT_MASK::SPRITE_OVERFLOW, false);
            set_ppu_stat(STAT_MASK::SPRITE_OVERFLOW, false);
        }
        if ((cycle >= 2 && cycle < 258) || (cycle >= 321 && cycle < 338)) {

                shiftShifters();

            switch ((cycle-1) % 8) {

                case 0:
                    //cout << bitset<8> (patternFetchLsb) << endl;
                    //cout << bitset<8> (patternFetchMsb) << endl;
                    loadShifters();
                    if (patternTableDataShift_bg_lsb != 0 || patternTableDataShift_bg_msb != 0) {

                        /*cout << bitset<16> (patternTableDataShift_bg_lsb) << endl;
                        cout << bitset<16> (patternTableDataShift_bg_msb) << endl;
                        cout << int(getCoarseX()) << endl;
                        cout << int(getCoarseY()) << endl;*/
                        //cout << "fuck you" << endl;*/

                   /* }
                    nameTableFetch = readPPU(getTileAddress(absLoopy));
                    //cout << int(nameTableFetch) << endl;
                    break;
                case 2:
                    attributeFetch = readPPU(getAttributeAddress(absLoopy));
                    //the Attribute Fetch variable need to be adjusted to the section the tile is in
                    if ((getCoarseX() & 0x02) && (getCoarseY() & 0x02)) {
                        attributeFetch >>= 6;
                    } else if (!(getCoarseX() & 0x02) && (getCoarseY() & 0x02)) {
                        attributeFetch >>= 4;
                    } else if ((getCoarseX() & 0x02) && !(getCoarseY() & 0x02)) {
                        attributeFetch >>= 2;
                    }
                    attributeFetch &= 0x03;
                    break;
                case 4:
                    patternFetchLsb = readPPU(((ppu_ctrl&CTRL_MASK::BT_SELECT)<<12) + (nameTableFetch<<4) + getFineY());
                    if (patternFetchLsb != 0) {
                        //cout << bitset<8>(patternFetchLsb) << endl;
                        break;
                    }
                    break;
                case 6:
                    patternFetchMsb = readPPU(((ppu_ctrl&CTRL_MASK::BT_SELECT)<<12) + (nameTableFetch<<4) + getFineY() + 8);
                    if (patternFetchMsb != 0) {
                        //cout << bitset<8>(patternFetchMsb) << endl;
                        break;
                    }
                    break;
                case 7:
                    incrementCoarseX();
                    break;
            }
            //uint16_t coarseY = scanLine/8;
            //uint16_t coarseX = cycle/8;
            //updatePatternTileRowVar(nameTable[0][coarseY * 32 + coarseX] + 256);

            if (cycle == 256) {
                incrementY();
            }

            if (cycle == 257) {
                //only done when rendering is enabled
                    loadShifters();
                    updateLoopyX();

            }
            if (((cycle >= 280) && (cycle <= 304))) {

                updateLoopyY();

            }
           /* if ((cycle - 1) == 337 || (cycle-1)==339) {
                nameTableFetch = readPPU(getTileAddress(absLoopy));
            }*/



  /*      }

        if (scanLine == 240) {

        }


        //uint8_t paletteID = getPaletteID(cycle, scanLine);
        //uint8_t colorID = getColorID(cycle&0x07);

        //ppuScreen.setPixel(cycle, scanLine, getColor(paletteID, colorID));
    }
    if (false) {
        cout << "Loopy: " << "cX = " << (int) getCoarseX() << " ,x = " << int(getFineX()) << endl;
        cout << "cY = " << int(getCoarseY()) << " ,y = " << int(getFineY()) << endl;
        cout << "scanline: " << int(scanLine) << " ,cycle: " << int(cycle) << endl;
    }

    if (scanLine == 241 && cycle == 1) {
        set_ppu_stat(STAT_MASK::VBLANK, true);
        frameDone = true;
        if (get_ppu_ctrl(CTRL_MASK::NMI_ENABLE)) {
            nmiVblank();
            cout << "nmi" << endl;
        }
    }
    if (scanLine < 240 && scanLine > -1 && ppu_mask & MASK_MASK::BACKGROUND_ENABLE) {
        fineXMultiplexer = 0x8000 >> getFineX();
        //cout << bitset<16> (patternTableDataShift_bg_lsb) << endl;
        //cout << bitset<16> (patternTableDataShift_bg_msb) << endl;
        //cout << bitset<16> (fineXMultiplexer) << endl;

        uint8_t lsbC = (((patternTableDataShift_bg_lsb & fineXMultiplexer) > 0) ? 1:0) << 1;
        uint8_t msbC = ((patternTableDataShift_bg_msb & fineXMultiplexer) > 0) ? 1:0;
        //cout << int(msbC | lsbC) << endl;
        uint8_t bgPixelColorID = (msbC | lsbC);
        uint8_t bgPaletteID = (((paletteAttributesShift_bg_msb & fineXMultiplexer) > 0) << 1) | (paletteAttributesShift_bg_lsb & fineXMultiplexer) >0;
        ppuScreen.setPixel(cycle-1, scanLine, getColor(bgPaletteID, bgPixelColorID));
    }
    cycle++;
    if (cycle == 341) {
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

/*
 * Updates the patternTileRowVar variable for 8 pixels in 1 row
 */
void PPU2C02::updatePatternTileRowVar(uint16_t index) {
    //index *= 16;
    patternTableRow[0] = bus->cartridge.chrData[index];// + (scanLine & 0x07)];
    patternTableRow[1] = bus->cartridge.chrData[index+8];// + (scanLine & 0x07)+8];
}

uint8_t PPU2C02::getPaletteID(uint16_t xInd, uint16_t yInd) {
    //get the right attribute byte in the nametable by determining the right Block
    uint8_t paletteByte = nameTable[0][960 + (yInd/32)*8 + (xInd/32)];
    //each block is divided into 2x2 tile sections, the section is determined by the x and y coordinate of the pixel
    uint8_t sectionID = ((((xInd & 0x1f)) > 15) ? 1 : 0) | ((((yInd & 0x1f) > 15) ? 1 : 0) << 1);
    return ((paletteByte >> (sectionID * 2)) & 0x03);
}

uint8_t PPU2C02::getColorID(uint8_t xOffset) {
    uint8_t lsbColor = ((patternTableRow[0] >> (7 - xOffset)) & 0x01);
    uint8_t msbColor = ((patternTableRow[1] >> (7 - xOffset)) & 0x01);
    return (msbColor << 1) | lsbColor;
}

sf::Color PPU2C02::getColor(uint8_t palette, uint8_t colorIndex) {
    return colors[readPPU(0x3F00 + (palette << 2) + colorIndex) & 0x3F];
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

//only used for displaying the pattern tables
sf::Color PPU2C02::getColor(int8_t index) {
    return {colorData[index].r, colorData[index].g, colorData[index].b, 255};
}

/*
 * Loopy Register helper methods
 */
void PPU2C02::setCoarseX(uint8_t value) {
    tempLoopy &= (~0x001F);
    tempLoopy |= value;
}
uint8_t PPU2C02::getCoarseX() {
    return (absLoopy & 0x001F);
}
void PPU2C02::setCoarseYHi(uint8_t value) {
    tempLoopy &= (~0x0300);
    tempLoopy |= (value << 8);
}
void PPU2C02::setCoarseYLow(uint8_t value) {
    tempLoopy &= (~0x00E0);
    tempLoopy |= (value << 5);
}
void PPU2C02::setCoarseY(uint16_t value) {
    tempLoopy &= (~0x03E0);
    tempLoopy |= (value << 5);
}
uint8_t PPU2C02::getCoarseY() {
    return (absLoopy & 0x03E0) >> 5;
}
void PPU2C02::setNameTableID(uint8_t value) {
    tempLoopy &= (~0x0C00);
    tempLoopy |= (value << 10);
}
uint8_t PPU2C02::getNameTableID() {
    return (absLoopy & 0x0C00) >> 10;
}

void PPU2C02::setFineY(uint8_t value) {
    tempLoopy &= (~0x7000);
    tempLoopy |= (value << 12);
}

uint8_t PPU2C02::getFineY() {
    return (absLoopy & 0x7000) >> 12;
}

void PPU2C02::setFineX(uint8_t value) {
    fineX = value;
}

uint8_t PPU2C02::getFineX() {
    return fineX;
}
/*
 * for details on implementation of the next 4 methods see and credits also go to
 * https://wiki.nesdev.com/w/index.php/PPU_scrolling
 */
void PPU2C02::incrementCoarseX() {
    if (ppu_mask&MASK_MASK::BACKGROUND_ENABLE || ppu_mask&MASK_MASK::SPRITE_ENABLE) {
        if ((absLoopy & 0x001F) == 31) {
            absLoopy &= ~0x001F;
            absLoopy ^= 0x0400;
        } else {
            absLoopy++;
        }
    }
}
void PPU2C02::incrementY() {
    if (ppu_mask&MASK_MASK::BACKGROUND_ENABLE || ppu_mask&MASK_MASK::SPRITE_ENABLE) {
        if ((absLoopy & 0x7000) != 0x7000) {
            int8_t fineY = getFineY();
            absLoopy += 0x1000;
        } else {
            absLoopy &= (~0x7000);
            uint16_t y = getCoarseY();
            if (y == 29) {
                y = 0;
                absLoopy ^= 0x0800;
            } else if (y == 31) {
                y = 0;
            } else {
                y++;
            }

            absLoopy &= (~0x03E0);
            absLoopy |= (y << 5);
        }
    }
}

uint16_t  PPU2C02::getTileAddress(uint16_t loopy) {
    return 0x2000 | (loopy & 0x0FFF);
}

uint16_t PPU2C02::getAttributeAddress(uint16_t loopy) {
    return 0x23C0 | (loopy & 0x0C00) | (loopy >> 4) & 0x38 | ((loopy >> 2) & 0x07);
}

/*
 *Happens every tile, the data loaded in the previous tile is now loaded into the the least significant byte of the
 * shift registers
*/
void PPU2C02::loadShifters() {
    (patternTableDataShift_bg_lsb&=0xFF00) |= patternFetchLsb;
    (patternTableDataShift_bg_msb&=0xFF00) |= patternFetchMsb;

    //The attributeFetch byte has a structure like AaBbCcDd
    //where the uppercase Letter of the pairs represents the msb and the lowercase letter the lsb
    //each letter pair represents 1 of 4 Sections in the Attribute byte, which section the tile is in
    //is determined by coarseX and coarseY and the attributeFetch variable is set accordingly, eg:
    //tile is in section B so attributeFetch = 0b000000Bb, this is then inflated to 8 bits, because the palette given
    //by the section applies to all pixels in a tile
    paletteAttributesShift_bg_lsb |= ((attributeFetch & 0b01) ? 0xFF : 0x00);
    paletteAttributesShift_bg_msb |= ((attributeFetch & 0b10) ? 0xFF : 0x00);
}

void PPU2C02::shiftShifters() {
    if (ppu_mask & MASK_MASK::BACKGROUND_ENABLE) {


        patternTableDataShift_bg_lsb <<= 1;
        patternTableDataShift_bg_msb <<= 1;



        paletteAttributesShift_bg_lsb <<= 1;
        paletteAttributesShift_bg_msb <<= 1;
    }
}

void PPU2C02::updateLoopyX() {
    if (ppu_mask&MASK_MASK::BACKGROUND_ENABLE || ppu_mask&MASK_MASK::SPRITE_ENABLE) {
        //update coarseX
        absLoopy &= (~0x001F);
        absLoopy |= (tempLoopy & 0x001F);
        //update nametable
        absLoopy &= (~0x0400);
        absLoopy |= (tempLoopy & 0x0400);
    }
}

void PPU2C02::updateLoopyY() {
    //update fineY
    if (ppu_mask&MASK_MASK::BACKGROUND_ENABLE || ppu_mask&MASK_MASK::SPRITE_ENABLE) {
        absLoopy &= (~0x7000);
        absLoopy |= (tempLoopy & 0x7000);
        //update nametable
        absLoopy &= (~0x0800);
        absLoopy |= (tempLoopy & 0x0800);
        //update coarse Y
        absLoopy &= (~0x03E0);
        absLoopy |= (tempLoopy & 0x03E0);
    }
}