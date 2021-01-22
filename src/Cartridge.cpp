//
// Created by matia on 25/11/2020.
//

#include <fstream>
#include "Cartridge.h"

Cartridge::Cartridge() {
    return;
}


Cartridge::Cartridge(const std::string& fileName) {
    //https://stackoverflow.com/questions/15138353/how-to-read-a-binary-file-into-a-vector-of-unsigned-chars
    std::ifstream stream(fileName, std::ios::in | std::ios::binary);
    std::vector<uint8_t> contents((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    std::cout << "file size: " << contents.size() << std::endl;
    stream.close();

    getHeader(contents);
    std::cout << (int)nBanksChr << " " << (int)nBanksPrg << " "<<  (int) mapper_id << endl;

    vector<uint8_t>::const_iterator firstPrg = contents.begin() + 16 + trainer*512;
    vector<uint8_t>::const_iterator lastPrg = contents.begin() + (16 + trainer*512) + BANKSIZE*nBanksPrg;
    vector<uint8_t>::const_iterator lastChr = contents.begin() + (16 + trainer*512) + BANKSIZE*nBanksPrg + (BANKSIZE/2)*nBanksChr;
    vector<uint8_t> newVec1(firstPrg, lastPrg);
    vector<uint8_t> newVec2(lastPrg,lastChr);

    prgData = newVec1;
    if(nBanksPrg == 1) {
        prgData.insert(prgData.end(), newVec1.begin(), newVec1.end());
    }

    chrData = newVec2;

    std::cout << std::endl << prgData.size() << " " << chrData.size() << endl;
}

void Cartridge::getHeader(const vector<uint8_t> content) {
    nBanksPrg =  content[4];
    nBanksChr =  content[5];
    mapper_id = (((content[7] >> 4) << 4)| ((content[6]) >> 4));
    trainer = content[6] & THIRD;
    mirroring = content[6] & FIRST;
}
