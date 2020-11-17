#include "Bus.h"
#include "CPU6502.h"
#include <cstdio>
#include "CompactLogger.h"

int main(){
/*
    CompactLogger logger = CompactLogger();
    logger.debug(__FUNCTION__, "1");
    logger.info(__FUNCTION__, "2");
    logger.warning(__FUNCTION__, "3");
    logger.fatal(__FUNCTION__, "4");
    logger.flush();*/

    Bus *bus = new Bus();
    CPU6502 *cpu = new CPU6502(bus);


    cpu->testInstruction(0x29);

    return 0;

}