#include "Bus.h"
#include "CPU6502.h"
#include <cstdio>
#include "CompactLogger.h"

int main(){

    CompactLogger logger = CompactLogger();
    logger.debug(__FUNCTION__, "1");
    logger.info(__FUNCTION__, "2");
    logger.warning(__FUNCTION__, "3");
    logger.fatal(__FUNCTION__, "4");
    logger.flush();

    Bus *bus = new Bus();
    CPU6502 *cpu = new CPU6502(bus);
    cpu->write(0x0000, 0x10);
    cpu->write(0x0001, 0x15);
    cpu->write(0x0002, 0x00);
    cpu->write(0x0015, 0x12);
    cpu->write(0x0012, 0X04);
    cpu->write(0x0026, 0x04);

    cpu->run();

    return 0;

}