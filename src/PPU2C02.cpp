
#include <functional>
#include "PPU2C02.h"

PPU2C02::PPU2C02(Bus *busRef, Screen &screenRef) :
    ppuScreen(screenRef)
{
    //establish a connection to the bus and vice versa
    bus = busRef;
    bus->ppu = this;
    std::memset(primaryOAM, 0xFF, sizeof(primaryOAM));
    std::memset(secondaryOAM, 0xFF, sizeof(secondaryOAM));

    // memory init
    for (uint8_t & text : this->nameTable[0]) {
        text = 0x00;
    }
    for (uint8_t & text : this->nameTable[1]) {
        text = 0x00;
    }

    renderState = PreRender;
    spriteZeroDrawn = false;
    spriteZeroOnScanLine = false;
    spriteZeroHitInFrame = false;
    writeToggle = false;
    ppu_ctrl = 0x00;
    ppu_mask = 0x00;
    ppu_stat = 0b10100000;
    ppu_scro = 0x00;
    ppu_addr = 0x00;
    ppu_data = 0x00;
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

/*
 * GET AND SET METHODS FOR REGISTERS
 */
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

/* Read and Write Operations inside the ppu */

uint8_t PPU2C02::readPPU(uint16_t address) {
    address = address & 0x3FFF;
    uint8_t dataOut = 0x00;
    if (address < 0x2000) {
        //since the patterntable is usually not written to we read directly from the cartridge
        dataOut = bus->cartridge.chrData[address];
    } else if (address <= 0x3EFF) {
        address = (address & 0x0FFF);
        //details can be found in write ppu
        if (bus->cartridge.mirroring) {
            if (address >= 0x0000 && address <= 0x03FF)
                dataOut = nameTable[0][address & 0x03FF];
            if (address >= 0x0400 && address <= 0x07FF)
                dataOut = nameTable[1][address & 0x03FF];
            if (address >= 0x0800 && address <= 0x0BFF)
                dataOut = nameTable[0][address & 0x03FF];
            if (address >= 0x0C00 && address <= 0x0FFF)
                dataOut = nameTable[1][address & 0x03FF];
        } else {
            if (address >= 0x0000 && address <= 0x03FF)
                dataOut = nameTable[0][address & 0x03FF];
            if (address >= 0x0400 && address <= 0x07FF)
                dataOut = nameTable[0][address & 0x03FF];
            if (address >= 0x0800 && address <= 0x0BFF)
                dataOut = nameTable[1][address & 0x03FF];
            if (address >= 0x0C00 && address <= 0x0FFF)
                dataOut = nameTable[1][address & 0x03FF];
        }
    } else if (address >= 0x3F00 && address <= 0x3FFF) {
        //bottom 5 bits are masked as only they are relevant
        address &= 0x001F;
        //first color of background palettes (0x0000, 0x0004, 0x0008, 0x000C) is mirrored to the background color at 0x3F00
        if (address == 0x0004) {
            address = 0x0000;
        }
        if (address == 0x0008) {
            address = 0x0000;
        }
        if (address == 0x000C) {
            address = 0x0000;
        }
        //these four addresses are mapped to the colors in the background palettes
        //for details on how the mirroring of colors work refer to: https://wiki.nesdev.com/w/index.php/PPU_palettes
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
        dataOut = paletteTable[address];
    }

    return dataOut;
}

void PPU2C02::writePPU(uint16_t address, uint8_t data) {
    address = address & 0x3FFF;
    if (address < 0x2000) {
        patternTable[(address & 0x1000) >> 12][(address & 0x0FFF)] = data;
        //usually the pattern table is not written to, i.e. is static
    } else if (address <= 0x3EFF) {
        //the size of the nametables is not 0x3EFF bytes, so it gets mirrored down to 0x0FFF bytes
        address = (address & 0x0FFF);
        //depending on the mirroring we read and write from different nametables
        /*
         * It can be visualized like this
         * o-----o-----o
         * |  0  |  1  |
         * o-----o-----o
         * |  1  |
         * o-----o
         * depending on the mirroring (vertical or horizontal) given by the cartridge we write to certain nametables when
         * in a certain address range to make scrolling possible. In Theory the NES has 4 nametables, but it only has memory
         * for 2, so we use 2 and and use them depending on the mirroring. More info at https://wiki.nesdev.com/w/index.php/PPU_nametables
         */
        if (bus->cartridge.mirroring) {
            //Vertical Mirroring
            if (address >= 0x0000 && address <= 0x03FF)
                nameTable[0][address & 0x03FF] = data;
            if (address >= 0x0400 && address <= 0x07FF)
                nameTable[1][address & 0x03FF] = data;
            if (address >= 0x0800 && address <= 0x0BFF)
                nameTable[0][address & 0x03FF] = data;
            if (address >= 0x0C00 && address <= 0x0FFF)
                nameTable[1][address & 0x03FF] = data;

        } else {
            //Horizontal Mirroring
            if (address >= 0x0000 && address <= 0x03FF)
                nameTable[0][address & 0x03FF] = data;
            if (address >= 0x0400 && address <= 0x07FF)
                nameTable[0][address & 0x03FF] = data;
            if (address >= 0x0800 && address <= 0x0BFF)
                nameTable[1][address & 0x03FF] = data;
            if (address >= 0x0C00 && address <= 0x0FFF)
                nameTable[1][address & 0x03FF] = data;
        }
    //Palette Address Range
    } else if (address >= 0x3F00 && address <= 0x3FFF) {
        //the bottom 5 bits are masked as they are the only relevant ones
        address &= 0x001F;
        //These 4 addresses of the foreground palette/color are mirrored to the respective background palette/color
        //These four colors in the background palettes are actually not used by the background, only foreground
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


uint8_t PPU2C02::readCPU(uint16_t address)  {
    uint8_t dataOut = 0x00;
    switch (address & 0x0007) {
        //Control
        case 0x0000:
            //not readable
            break;
        //Mask
        case 0x0001:
            //not readable
            break;
        //Status
        case 0x0002:
            //only 3 most significant bits are relevant
            dataOut = get_ppu_stat() & 0xE0;
            //reading from this register sets VBlank to false
            set_ppu_stat(STAT_MASK::VBLANK, false);
            //also resets the write latch
            writeToggle = false;
            break;
        //OAM Address
        case 0x0003:
            //OAM is handled with dma, so this register is unused
            break;
        //OAM Data
        case 0x0004:
            //OAM is handled with dma, so this register is unused
            break;
        //Scroll
        case 0x0005:
            //only written to
            break;
        //PPU Address
        case 0x0006:
           //only written to
            break;
        //PPU Data
        case 0x0007:
            //when the cpu reads data from the ppu it is delayed by one read; the first read is discarded
            //an exception is made when reading reading from palette memory
            dataOut = dataBuffer;
            dataBuffer = readPPU(absLoopy);
            if (absLoopy >= 0x3F00) {
                dataOut = dataBuffer;
            }
            absLoopy += (get_ppu_ctrl(CTRL_MASK::INCR_MODE)? 32 : 1);
            break;
    }
    return dataOut;
}

void PPU2C02::writeCPU(uint16_t address, uint8_t data) {
    if (address == 0x4014) {
        bus->dma = true;
        bus->dmaPage = data;
        bus->dmaAddress = 0x00;
    }
    switch (address & 0x0007) {
        case 0x0000:
            //set the control register to the data
            this->set_ppu_ctrl(data);
            //the loopy registers is updated with new nametableID for x and y
            setNameTableID(data & 0x03);
            break;
        case 0x0001:
            this->set_ppu_mask(data);
            break;
        case 0x0002:
            //status register is only readable
            break;
        case 0x0003:
            //OAM is handled with dma, so this register is unused
            break;
        case 0x0004:
            //OAM is handled with dma, so this register is unused
            break;
        //Scroll register
        case 0x0005:
            if (!writeToggle) {
                //with the latch on false the X components of the tempLoopy registers are updated (scroll register not used)
                setCoarseX(data >> 3);
                setFineX(data & 0x07);
                writeToggle = true;
            } else {
                // with the latch on true the Y components of the tempLoopy register are updated (scroll register not used)
                setCoarseY(data >> 3);
                setFineY(data & 0x07);
                writeToggle = false;
            }
            break;
        case 0x0006:
            if (!writeToggle) {
                //specific Byte of the data go to different parts in the tempLoopy register
                setFineY(((data & 0x30) >> 4)&0x03);
                setNameTableID(((data & 0x0C) >> 2)&0x03);
                setCoarseYHi(data & 0x03);
                writeToggle = true;
            } else {
                //the rest of the tempLoopy register is updated on the second write
               setCoarseYLow((data & 0xE0) >> 5);
               setCoarseX(data & 0x1F);
               //absLoopy is set to tempLoopy
               absLoopy = tempLoopy;
               writeToggle = false;
            }
            break;
        case 0x0007:
            //absLoopy keeps track where to write to
            this->writePPU(absLoopy, data);
            absLoopy += (get_ppu_ctrl(CTRL_MASK::INCR_MODE)? 32 : 1);
            break;
        default:
            break;
    }
}
void PPU2C02::clock() {

    //for details see diagram:
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
                    fetchPipeline();
                    if (cycle != 336) shiftShifters();
                }

                if (cycle == 256) {
                    incrementY();
                }

                if (cycle == 257) {
                    updateLoopyX();
                    loadScanlineSprites(scanLine);
                }

                if ((cycle > 279) || (cycle < 305)) {
                    if (ppu_mask & MASK_MASK::BACKGROUND_ENABLE || ppu_mask & MASK_MASK::SPRITE_ENABLE) {
                        updateLoopyY();
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
            }
            //1 cycle is skipped
            if (cycle == 0 && scanLine == 1 && odd) {
                cycle = 1;
            }
            if ((cycle > 0 && cycle < 256) || (cycle > 320 && cycle < 337)) {
                fetchPipeline();
                if ((cycle > 320 && cycle < 336)) {
                    shiftShifters();
                }
            }
            if (cycle == 256) {
                incrementY();
            }
            if ((cycle >= 0 && cycle < 257) ){
                uint8_t spritePixelColorID = 0x00;
                uint8_t spritePaletteID = 0x00;
                uint8_t bgPixelColorID = 0x00;
                uint8_t bgPaletteID = 0x00;
                uint8_t finalPixelColorID = 0x00;
                uint8_t finalPaletteID = 0x00;
                bool spritePriority = false;

                if (ppu_mask & MASK_MASK::BACKGROUND_ENABLE) {
                    fineXMultiplexer = 0x8000 >> getFineX();
                    if (patternTableDataShift_bg_lsb != 0 || patternTableDataShift_bg_msb != 0) {
                    }
                    uint8_t lsbC = (((patternTableDataShift_bg_lsb & fineXMultiplexer) > 0) ? 1 : 0);
                    uint8_t msbC = (((patternTableDataShift_bg_msb & fineXMultiplexer) > 0) ? 1 : 0) << 1;
                    bgPixelColorID = (msbC | lsbC);
                    bgPaletteID = (((paletteAttributesShift_bg_msb & fineXMultiplexer) > 0 ? 1:0) << 1) |
                                          ((paletteAttributesShift_bg_lsb & fineXMultiplexer) > 0 ? 1:0);


                    shiftShifters();
                }
                if (ppu_mask & MASK_MASK::SPRITE_ENABLE) {
                    bool pixelSet = false;
                   for (int i = 0; i < 8; i++) {

                       //TODO Reset dont know what
                       if (spriteCounter[i] < -7) {
                           //do nothing
                       } else if (spriteCounter[i] > 0) { //Have we hit the beginning of this sprite yet?
                           spriteCounter[i]--;

                       } else if (!pixelSet){
                           spritePixelColorID = (((spriteShift[i][1] & 0x80) > 0 ? 1:0) << 1) | (((spriteShift[i][0] & 0x80) > 0 ? 1:0));
                           spritePaletteID = (spriteAttribute[i]&0x03);
                           spritePriority = spriteAttribute[i]&0x20;
                           spriteShift[i][0] <<= 1;
                           spriteShift[i][1] <<= 1;
                           spriteCounter[i]--;
                           if (spritePixelColorID != 0) {
                               if (i == 0) {
                                   spriteZeroDrawn = true;
                               }
                               pixelSet = true;
                           }


                       } else {
                           spriteShift[i][0] <<= 1;
                           spriteShift[i][1] <<= 1;
                           spriteCounter[i]--;
                       }
                   }

                }
                if ((ppu_mask & MASK_MASK::SPRITE_ENABLE) || (ppu_mask & MASK_MASK::BACKGROUND_ENABLE)) {
                    if (bgPixelColorID == 0 && spritePixelColorID == 0) {
                        finalPixelColorID = bgPixelColorID;
                        finalPaletteID = bgPaletteID;
                    } else if (bgPixelColorID == 0 && spritePixelColorID != 0) {
                        finalPixelColorID = spritePixelColorID;
                        finalPaletteID =  4+spritePaletteID;
                    } else if (bgPixelColorID != 0 && spritePixelColorID == 0) {
                        finalPixelColorID = bgPixelColorID;
                        finalPaletteID = bgPaletteID;
                    } else {
                        //sprite priority of 1 means sprite is behind background
                        if (spritePriority) {
                            finalPixelColorID = bgPixelColorID;
                            finalPaletteID = bgPaletteID;
                        } else {
                            finalPixelColorID = spritePixelColorID;
                            finalPaletteID = 4+spritePaletteID;
                        }
                    }


                }
                if (spriteZeroOnScanLine && spriteZeroDrawn) {
                    //see wiki on when it happens and when not
                    //http://wiki.nesdev.com/w/index.php/PPU_OAM
                    //it can only occur if both renderings are enabled, because its a collision between the background
                    //and the foreground
                    if (get_ppu_mask(MASK_MASK::SPRITE_ENABLE) && get_ppu_mask(MASK_MASK::BACKGROUND_ENABLE)) {
                        //only when they are not opaque (not zero)
                        if (bgPixelColorID!=0 && spritePixelColorID!=0) {
                            //if either of those bits in the mask register are zero we ignore cycle 0 to 7
                            if (!get_ppu_mask(MASK_MASK::SPRITE_LEFT_COL_ENABLE) || !get_ppu_mask(MASK_MASK::BACKGR_LEFT_COL_ENABLE)) {
                                if (cycle >= 8 && cycle < 255) {
                                    set_ppu_stat(STAT_MASK::SPRITE_ZERO_HIT, true);
                                    spriteZeroDrawn = false;
                                }
                            } else {
                                if (cycle >= 0 && cycle < 255) {
                                    set_ppu_stat(STAT_MASK::SPRITE_ZERO_HIT, true);
                                    spriteZeroDrawn = false;
                                }
                            }
                        }
                    }
                }

                ppuScreen.setPixel(cycle , scanLine, getColor(finalPaletteID, finalPixelColorID));
            }
            if (cycle == 257) {
                updateLoopyX();
                if (scanLine < 239) {
                    loadScanlineSprites(scanLine);
                }
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
                //cout << "----------------------------------------------------------------------------------------------------------" << endl;
                for (int i = 0; i < 64; i++) {
                    //cout << "Y-Address: " << int(primaryOAM[i*4]) << ", Pattern Index: " << int(primaryOAM[i*4+1]) << ", Attribute Byte: " << bitset<8>(primaryOAM[i*4+2]) << ", X-Position: " << int(primaryOAM[i*4+3])<< endl;
                }

            }


            break;

        case VerticalBlank:
            if (scanLine == 241 && cycle == 1) {
                set_ppu_stat(STAT_MASK::VBLANK, true);
                frameDone = true;
                if (get_ppu_ctrl(CTRL_MASK::NMI_ENABLE)) {
                    nmiVblank();
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
void PPU2C02::fetchPipeline() {
    switch (cycle % 8) {
        case 0:
            //on every 8th cycle (cycle 0 excluded) the CoarseX segment of the absLoopy register is incremented
            //additionally the shifters for the Background are loaded for the tile after the one that just started
            incrementCoarseX();
            loadShifters();
            break;
        case 1:
            //the Tile index is fetched from the nametable
            nameTableFetch = readPPU(getTileAddress(absLoopy));
            break;
        case 3:
            //the Attribute byte of Tile is fetched from the nametable aswell
            attributeFetch = readPPU(getAttributeAddress(absLoopy));
            //the Attribute Fetch variable need to be adjusted to the section the tile is in
            //the Nametable is seperated into 8x8 Blocks, each one of these blocks have an attribute byte saved
            //in the end of the nametable. Each of these blocks are seperated into 4 sections, these sections are
            //4x4 Tiles. Each sections palette is determined by 2 bits.
            /*
             * o----o----o      Represents a block
             * | 0  o 1  |      The bits in the attribute byte are set like this:
             * o----o----o      3322 1100
             * | 2  | 3  |
             * o----o----o
             */
            if ((getCoarseX() & 0x02) && (getCoarseY() & 0x02)) {
                //section 3
                attributeFetch >>= 6;
            } else if (!(getCoarseX() & 0x02) && (getCoarseY() & 0x02)) {
                //section 2
                attributeFetch >>= 4;
            } else if ((getCoarseX() & 0x02) && !(getCoarseY() & 0x02)) {
                //section 1
                attributeFetch >>= 2;
            }
            //section 0 can just be masked
            attributeFetch &= 0x03;
            break;
        case 5:
            //Fetch of the least significant bits. The address in structured in the following way:
            //Background Tile Select Bit in the control register selects the pattern Table (0 or 1) and is shifted
            //to the correct position in the address. The previously fetched Tile Index is multiplied by 16 (each Tile is
            // 16 bytes of information) and added to the address, finally fineY is the row offset so get the correct byte

            patternFetchLsb = readPPU(((get_ppu_ctrl(CTRL_MASK::BT_SELECT) > 0 ? 1:0) << 12)
                                      + ((uint16_t)nameTableFetch * 16)
                                      + getFineY());
            break;
        case 7:
            //same as above but 8 is added to the address because the pattern bytes come in pairs and the first 8 bytes
            // are the lsb bytes and the second 8 bytes of the Tile are the msb bytes
            patternFetchMsb = readPPU(((get_ppu_ctrl(CTRL_MASK::BT_SELECT) > 0 ? 1:0) << 12)
                                      +((uint16_t)nameTableFetch * 16)
                                      + getFineY() + 8);

            break;
    }
}







sf::Color PPU2C02::getColor(uint8_t palette, uint8_t colorIndex) {
    return colors[(readPPU(0x3F00 | (palette << 2) | colorIndex)) & 0x3F];
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
    return 0x23C0 | (loopy & 0x0C00) | ((loopy >> 4) & 0x38) | ((loopy >> 2) & 0x07);
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
    //by the section applies to all pixels in a section
    paletteAttributesShift_bg_lsb = (paletteAttributesShift_bg_lsb&0xFF00) | ((attributeFetch & 0x01) > 0 ? 0x00FF : 0x0000);
    paletteAttributesShift_bg_msb = (paletteAttributesShift_bg_msb&0xFF00) |((attributeFetch & 0x02) > 0 ? 0x00FF : 0x0000);
}

void PPU2C02::shiftShifters() {
    if (ppu_mask & MASK_MASK::BACKGROUND_ENABLE) {

        patternTableDataShift_bg_lsb <<= 1;
        patternTableDataShift_bg_msb <<= 1;

        paletteAttributesShift_bg_lsb <<= 1;
        paletteAttributesShift_bg_msb <<= 1;
    }
}

/*
 * Each visible
 */
void PPU2C02::loadScanlineSprites(int16_t nextScanLine) {
    //byte 0 y position of the top of sprite
    //byte 1 Tile index number
    //byte 2 Attributes
    //byte 3 left x position of sprite
    std::memset(spriteAttribute, 0, sizeof(spriteAttribute));
    std::memset(spriteCounter, 0, sizeof(spriteCounter));
    std::memset(spriteShift, 0x00, sizeof(spriteShift));
    std::memset(secondaryOAM, 0xFF, sizeof(secondaryOAM));
    spriteCount = 0;
    uint8_t count = 0;
    spriteZeroOnScanLine = false;
    while (spriteCount < 9 && count < 64) {

        if (primaryOAM[count * 4] <= (nextScanLine)) {

            int16_t difference = (nextScanLine) - uint16_t(primaryOAM[count * 4]);

            if ((difference < 16 && get_ppu_ctrl(CTRL_MASK::SPRITE_HEIGHT))
                || (difference < 8 && !get_ppu_ctrl(CTRL_MASK::SPRITE_HEIGHT))) {
                if (spriteCount == 8) {
                    set_ppu_stat(STAT_MASK::SPRITE_OVERFLOW, true);
                    break;
                }
                if (count == 0) {

                    spriteZeroOnScanLine = true;
                }
                std::memcpy(secondaryOAM + sizeof(uint8_t) * 4 * spriteCount, primaryOAM + sizeof(uint8_t) * 4 * count,
                            sizeof(uint8_t) * 4);
                spriteAttribute[spriteCount] = primaryOAM[count * 4 + 2];
                spriteCounter[spriteCount] = primaryOAM[count * 4 + 3];
                //check bit 7 and 6 to determine if the sprite needs to be flipped
                bool verticalFlip = ((spriteAttribute[spriteCount] & 0x80) > 0);
                bool horizontalFlip = ((spriteAttribute[spriteCount] & 0x40) > 0);
                //check if sprite is 8x16 or 8x8
                if (get_ppu_ctrl(CTRL_MASK::SPRITE_HEIGHT)) {
                    //sprite is 8x16
                    //is is vertically flipped?
                    if (verticalFlip) {
                            //YES!
                            //How the address is calculated:
                            //bit zero of the Tile index byte (secondary[sprite*4+1]) determines the which patterntable to use
                            //Bit 1 to 7 determines the actual Index in the pattern table, multiply by 16 because each Tile
                            //has 16 bytes of information
                            //difference determines the offset into the sprite/Tile, so to get the flipped offset we take the row
                            //(15-difference) (the address range of both tiles is contiguous)
                            spriteShift[spriteCount][0] = readPPU(((secondaryOAM[spriteCount * 4 + 1] & 0x01) << 12) +
                                                                          ((secondaryOAM[spriteCount * 4 + 1] & 0xFE)) * 16 +
                                                                  (15 - difference));
                            spriteShift[spriteCount][1] = readPPU(((secondaryOAM[spriteCount * 4 + 1] & 0x01) << 12) +
                                                                  ((secondaryOAM[spriteCount * 4 + 1] & 0xFE)) * 16 +
                                                                  (15 - difference) + 8);
                    } else {
                            //same as with vertical flipping, but the offset is the difference
                            spriteShift[spriteCount][0] = readPPU(((secondaryOAM[spriteCount * 4 + 1] & 0x01) << 12) +
                                                                  (secondaryOAM[spriteCount * 4 + 1] & 0xFE) * 16 +
                                                                  difference);
                            spriteShift[spriteCount][1] = readPPU(((secondaryOAM[spriteCount * 4 + 1] & 0x01) << 12) +
                                                                  (secondaryOAM[spriteCount * 4 + 1] & 0xFE) * 16 +
                                                                  difference + 8);

                    }
                } else {
                    //Sprite is 8x8
                    //is is vertically flipped?
                    if (verticalFlip) {
                        //YES!
                        //How the address is calculated:
                        //Sprite Tile select bit in the Control register determines which pattern table to use
                        //The Tile Index byte of the sprite determines the Index into the pattern table which is multiplied by 16
                        //because each tile is 16 bytes
                        //the row offset difference is then flipped (7 - difference) to get the correct row
                        spriteShift[spriteCount][0] = readPPU(((get_ppu_ctrl(CTRL_MASK::ST_SELECT) > 0 ? 1 : 0) << 12) +
                                                              secondaryOAM[spriteCount * 4 + 1] * 16 +
                                                              (7 - difference));
                        spriteShift[spriteCount][1] = readPPU(((get_ppu_ctrl(CTRL_MASK::ST_SELECT) > 0 ? 1 : 0) << 12) +
                                                              secondaryOAM[spriteCount * 4 + 1] * 16 +
                                                              (7 - difference) + 8);
                    } else {
                        //No vertical flip
                        //Address same as above but the difference determines the correct row
                        spriteShift[spriteCount][0] = readPPU(((get_ppu_ctrl(CTRL_MASK::ST_SELECT) > 0 ? 1 : 0) << 12) +
                                                              secondaryOAM[spriteCount * 4 + 1] * 16 + difference);
                        spriteShift[spriteCount][1] = readPPU(((get_ppu_ctrl(CTRL_MASK::ST_SELECT) > 0 ? 1 : 0) << 12) +
                                                              secondaryOAM[spriteCount * 4 + 1] * 16 + difference + 8);
                    }
                }
                if (horizontalFlip) {
                    flipHorizonally(spriteCount);
                }


                spriteCount++;
            }
        }
        count++;
    }
}

/*
 * Method to transfer X components from tempLoopy to absLoopy
 */
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

/*
 * Method to transfer Y components from tempLoopy to absLoopy
 */
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
/*
 * Method used to flip bits of the pattern byte horizontally, for example
 * 0b01100000 -> 0x00000110, used for sprites
 */
void PPU2C02::flipHorizonally(uint8_t spriteIndex) {
    for (int i = 0; i < 2; i++) {
        uint8_t temp = 0x00;
        for (int j = 0; j < 8; j++) {
            temp >>= 1;
            temp |= ((spriteShift[spriteIndex][i]&0x80));
            spriteShift[spriteIndex][i] <<= 1;
        }
        spriteShift[spriteIndex][i] = temp;
    }

}