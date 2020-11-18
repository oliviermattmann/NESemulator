

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
    /**
     * The constructor method of the logger takes an argument filename that is
     * a (const char*) to a filename. This instance of logger will log to that file
     * and overwrite previous information in that file.
     * @param filename out-put-file name. (put .txt in there).
     */
    explicit CompactLogger(const char *filename);

    /**
     * Destructor
     */
    ~CompactLogger();


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
    * Enum defining the different possible log-levels.
    */
    enum LEVEL {
        DEBUG = 0,
        INFO = 1,
        WARNING = 2,
        FATAL = 3,
    };

    /**
     * Current level of the logger instance.
     */
    LEVEL level;

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
     * Indicates wether the logger
     */
    bool printsTerminal;

    /**
     * Name of out-put-file corresponding to this logger.
     */
    const char* file_name;

    /**
     * Writes string to terminal and file according to
     * loglevel.
     * @param str
     */
    void out(const std::string& str, LEVEL level) const;

    /**
     * Logs something.
     * @param function_name
     * @param str
     * @param _level
     */
    void log(const char* function_name, const char* str, LEVEL _level);


public:

    /**
     * Sets the level of the logger instance.
     * Can be used to switch levels during runtime.
     * @param level
     */
    void setLevel(LEVEL level);

    /**
     * Sets the boolean deciding if logger does stdout.
     * @param printsTerminal
     */
    void setTerminal(bool printsTerminal);


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


};


#endif //NESEMULATOR_COMPACTLOGGER_H
