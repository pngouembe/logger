/* Logger.h */

#ifndef LOGGER_H
#define LOGGER_H

#ifndef MAX_LOG_LINE_LENGTH
#define MAX_LOG_LINE_LENGTH 256
#endif /* MAX_LOG_LINE_LENGTH */

/* LogLevel */
typedef enum
{
    LOG_ERROR,
    LOG_WARNING,
    LOG_INFO,
    LOG_DEBUG,
} LogLevel;

static const char *LOG_LEVEL_STR[] = {"ERROR", "WARNING", "INFO", "DEBUG"};

/* LogHandler */
typedef struct LogHandler_t
{
    void (*lock)(void);
    void (*write)(const char *msg);
    void (*unlock)(void);
} LogHandler;

LogHandler *logHandlerInit();
void logHandlerDeinit(LogHandler *handler);
void logHandlerSetLockMethod(LogHandler *handler, void (*lock)(void));
void logHandlerSetWriteMethod(LogHandler *handler, void (*write)(const char *msg));
void logHandlerSetUnlockMethod(LogHandler *handler, void (*unlock)(void));

/* Formatter */

typedef struct LogFormatter_t
{
    void (*format)(const char *input, char *output, LogLevel level);
} LogFormatter;

LogFormatter *logFormatterInit();
void logFormatterDeinit(LogFormatter *formatter);
void logFormatterSetFormatMethod(LogFormatter *formatter,
                                 void (*format)(const char *input, char *output, LogLevel level));

/* Logger */

typedef struct Logger_t
{
    LogHandler *handler_list;
    int handler_count;
    LogFormatter *formatter;
    LogLevel log_level;
} Logger;

Logger *loggerInit();

Logger *getLogger();

void loggerSetLogHandler(Logger *logger, LogHandler *handler);

void loggerSetLogLevel(Logger *logger, LogLevel level);

void loggerSetFormatter(Logger *logger, LogFormatter *formatter);

void loggerDeinit(Logger *log);

/* Log methods */

void logError(const char *msg);
void logWarn(const char *msg);
void logInfo(const char *msg);
void logDebug(const char *msg);


#endif /* LOGGER_H */
