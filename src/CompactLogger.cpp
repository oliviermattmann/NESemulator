

#include "CompactLogger.h"
#include <string>
#include <chrono>
#include <iostream>
#include <fstream>

using namespace std;
using namespace std::chrono;
using days = duration<int, ratio_multiply<hours::period, ratio<24> >::type>;


CompactLogger::CompactLogger() {
    this->level = DEBUG;
    this->log_deque = std::deque<const char*>();
    this->creation = this->get_time();
}

void CompactLogger::setLevel(CompactLogger::LEVEL _level) {
    this->level = _level;
}


std::string CompactLogger::get_format(const char* function_name, LEVEL _level) {
    auto micro = to_string(
            (this->get_time() - this->creation).count()
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

void CompactLogger::out(const char* str, CompactLogger::LEVEL _level) {
    if (_level >= this->level) {
        log_deque.push_back(str);
        printf("%s", str);
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
    format.insert(0, "\n");
    format.append(str);
    this->out(format.c_str(), _level);
}

void CompactLogger::flush() {
    ofstream _logfile;
    _logfile.open (LOGFILE);
    auto it = this->log_deque.begin();
    while (it != this->log_deque.end()) {
        _logfile << ' ' << *it++;
        _logfile << '\n';
    }
    _logfile.close();
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

CompactLogger::~CompactLogger() = default;

