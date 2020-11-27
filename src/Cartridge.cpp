//
// Created by matia on 25/11/2020.
//

#include <fstream>
#include "Cartridge.h"

Cartridge::Cartridge(const std::string& fileName) {



    getHeader(fileName);
    std::cout << (int)nBanksChr << " " << (int)nBanksPrg << " "<<  (int) mapper_id;


}

void Cartridge::getHeader(const std::string& fileName) {
    ifstream inFile(fileName, ios::binary);
    uint8_t size = 16;
    uint8_t result [16];
    char byte;
    for(int i = 0; i < size; i++) {
        inFile.read(&byte, 1);
        result[i] = byte;
        std::cout <<(int)byte<< " ";
    }



    nBanksPrg =  result[4];
    nBanksChr =  result[5];

    std::cout << (result[7] >> 4) << " " <<  (result[6] >> 4) << " ";
    mapper_id = (((result[7] >> 4) << 4)| ((result[6]) >> 4));






    inFile.close();
}
