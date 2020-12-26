

#include "CompactLogger.h"
#include <string>
#include <chrono>
#include <iostream>
#include <fstream>

using namespace std;
using namespace std::chrono;
using days = duration<int, ratio_multiply<hours::period, ratio<24> >::type>;


CompactLogger::CompactLogger(const char* file_name) {
    this->level = DEBUG;
    this->creation = CompactLogger::get_time();
    this->file_name = file_name;
    this->printsTerminal = true;
    // Overwrite existing file.
    std::ofstream file(file_name, std::ofstream::trunc);
    file << "";
    file.flush();
    file.close();
}

void CompactLogger::setLevel(CompactLogger::LEVEL _level) {
    this->level = _level;
}


std::string CompactLogger::get_format(const char* function_name, LEVEL _level) {
    auto micro = to_string(
            (CompactLogger::get_time() - this->creation).count()
            );
    std::string buf(micro);
    buf.append(" mu");
    buf.append("\t\t\t[");
    buf.append(get_level(_level));
    buf.append("] ");
    buf.append(function_name);
    buf.append("() -> ");
    return buf;
}

void CompactLogger::out(const std::string& str, CompactLogger::LEVEL _level) const {
    if (_level >= this->level) {
        if (this->printsTerminal) {
            cout << str.c_str() << endl;
        }
        std::ofstream file(this->file_name, std::ofstream::app);
        file << str.c_str() << endl;
        file.flush();
        file.close();
    }
}

const char *CompactLogger::get_level(LEVEL _level) {
    switch (_level) {
        case DEBUG:
            return "DEBUG";
        case INFO:
            return "INFO";
        case WARNING:
            return "WARNING";
        case FATAL:
            return "FATAL";
    }
}

void CompactLogger::debug(const char* function_name, const char *str) {
    this->log(function_name, str, DEBUG);
}

void CompactLogger::info(const char *function_name, const char *str) {
    this->log(function_name, str, INFO);
}

void CompactLogger::warning(const char *function_name, const char *str) {
    this->log(function_name, str, WARNING);
}

void CompactLogger::fatal(const char *function_name, const char *str) {
    this->log(function_name, str, FATAL);
}

void CompactLogger::log(const char *function_name, const char *str, LEVEL _level) {
    std::string format = get_format(function_name, _level);
    format.append(str);
    this->out(format, _level);
}

std::chrono::system_clock::duration CompactLogger::get_time() {
    system_clock::duration tp = system_clock::now().time_since_epoch();
    days d = duration_cast<days>(tp);
    tp -= d;
    hours h = duration_cast<hours>(tp);
    tp -= h;
    minutes m = duration_cast<minutes>(tp);
    tp -= m;
    seconds s = duration_cast<seconds>(tp);
    tp -= s;
    return std::chrono::duration_cast<microseconds>(tp);
}

void CompactLogger::setTerminal(bool _printsTerminal) {
    this->printsTerminal = _printsTerminal;
}

CompactLogger::~CompactLogger() = default;

