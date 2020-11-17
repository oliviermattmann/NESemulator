

#ifndef NESEMULATOR_COMPACTLOGGER_H
#define NESEMULATOR_COMPACTLOGGER_H
#include <deque>
#include <string>
#include <chrono>


/**
 * A quickie of a basic Logger.
 *
 * HOW TO SETUP:
 *
 * Create a global instance of the class.
 * Finished.
 *
 * HOW TO USE:
 *
 * Call one of the log functions {debug(), info()...}
 * with the arguments __FUNCTION__ and your message string.
 */
class CompactLogger {

public:
    enum LEVEL {
        DEBUG = 0,
        INFO = 1,
        WARNING = 2,
        FATAL = 3,
    };

    LEVEL level;

    /**
     * Sets the level of the logger instance.
     * Can be used to switch levels during runtime.
     * @param level
     */
    void setLevel(LEVEL level);


private:

    /**
     * Saves time of logger instantiation.
     */
    std::chrono::system_clock::duration creation{};

    /**
     * Gets current time since epoch in microseconds.
     * @return
     */
    static std::chrono::system_clock::duration get_time();

    /**
     * Determines the maximum number of lines in a logfile before
     * autoflush.
     */
    static const int MAX_NUMBER_OF_LINES_IN_LOG = 2000;

    /**
     * Counts the logfiles created.
     */
    int log_file_counter;

    /**
     * Fills up with logs to dump into textfile.
     */
    std::deque<std::string> log_deque;

    /**
     * Returns a string containing time and
     * context of the message.
     * @return
     */
    std::string get_format(const char* function_name, LEVEL _level);

    /**
     * Returns the level of the Logger.
     * @return
     */
    static const char* get_level(LEVEL _level);

    /**
     * Writes string to terminal and file according to
     * loglevel.
     * @param str
     */
    void out(const std::string& str, LEVEL level);

    /**
     * Logs something.
     * @param function_name
     * @param str
     * @param _level
     */
    void log(const char* function_name, const char* str, LEVEL _level);



public:
    /**
     * Constructor.
     */
    CompactLogger();

    /**
     * Destructor
     */
    ~CompactLogger();

    /**
     * Logs msg on debug level.
     * @param str
     */
    void debug(const char* function_name, const char* str);

    /**
     * Logs msg on info level.
     * @param str
     */
    void info(const char* function_name, const char* str);

    /**
     * Logs msg on warning level.
     * @param str
     */
    void warning(const char* function_name, const char* str);

    /**
     * Logs msg on fatal level.
     * @param str
     */
    void fatal(const char* function_name, const char* str);


    /**
     * To print to a file all the log-statements that have been collected
     * to this point. The files can be named individually. Can be used
     * successively.
     * @param output_filename name of the file. (put a .txt in there).
     */
    void flush();


};


#endif //NESEMULATOR_COMPACTLOGGER_H
