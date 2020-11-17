#include "Bus.h"
#include "CPU6502.h"
#include <cstdio>
#include "CompactLogger.h"

void test_logger() {
    auto* logger = new CompactLogger();
    logger->info(__FUNCTION__ , "This part of the code didn't work.");
    logger->fatal(__FUNCTION__ , "This part also the code didn't work.");
    delete logger;
}

void test_this() {
    Bus *bus = new Bus();
    CPU6502 *cpu = new CPU6502(bus);
    cpu->write(0x0000, 0x10);
    cpu->write(0x0001, 0x15);
    cpu->write(0x0002, 0x00);
    cpu->write(0x0015, 0x12);
    cpu->write(0x0012, 0X04);
    cpu->write(0x0026, 0x04);

    cpu->run();

    delete bus;
    delete cpu;
}
