#include <logger.h>
#include <malloc.h>

#include <stdio.h>
#include <string.h>

void __defaultLockFunction()
{
}
void __defaultUnlockFunction()
{
}

LogHandler *logHandlerInit()
{
    LogHandler *handler = (LogHandler *)malloc(sizeof(LogHandler));
    handler->lock = __defaultLockFunction;
    handler->write = NULL;
    handler->unlock = __defaultUnlockFunction;
    return handler;
}

void logHandlerDeinit(LogHandler *handler)
{
    free(handler);
}

void logHandlerSetLockMethod(LogHandler *handler, void (*lock)(void))
{
    handler->lock = (*lock);
}
void logHandlerSetWriteMethod(LogHandler *handler, void (*write)(const char *msg))
{
    handler->write = (*write);
}
void logHandlerSetUnlockMethod(LogHandler *handler, void (*unlock)(void))
{
    handler->unlock = (*unlock);
}

/* Formatter */

void __default_formatter_function(const char *input, char *output, LogLevel level)
{
    strncpy(output, input, MAX_LOG_LINE_LENGTH);
}

LogFormatter *logFormatterInit()
{
    LogFormatter *formatter = (LogFormatter *)malloc(sizeof(LogFormatter));
    formatter->format = __default_formatter_function;
    return formatter;
}
void logFormatterDeinit(LogFormatter *formatter)
{
    free(formatter);
}

void logFormatterSetFormatMethod(LogFormatter *formatter,
                                 void (*format)(const char *input, char *output, LogLevel level))
{
    formatter->format = (*format);
}

/* Logger */

Logger *__global_logger;

Logger *loggerInit()
{
    if (__global_logger == NULL)
    {
        __global_logger = (Logger *)malloc(sizeof(Logger));
        __global_logger->handler_list = NULL;
        __global_logger->handler_count = 0;
        __global_logger->formatter = NULL;
        __global_logger->log_level = LOG_INFO;
    }
    return __global_logger;
}

Logger *getLogger()
{
    if (__global_logger == NULL)
    {
        // TODO: handle the error, loggerInit not called
        return NULL;
    }
    return __global_logger;
}

void loggerSetLogHandler(Logger *logger, LogHandler *handler)
{
    logger->handler_list = handler;
    logger->handler_count = 1;
}

void loggerSetLogLevel(Logger *logger, LogLevel level)
{
    logger->log_level = level;
}

void loggerSetFormatter(Logger *logger, LogFormatter *formatter)
{
    logger->formatter = formatter;
}

void loggerDeinit(Logger *log)
{
    free(log);
}

/* Log methods */

void __log(Logger *logger, LogLevel level, const char *msg)
{
    if (level > logger->log_level)
    {
        // Do not print
        return;
    }

    /* TODO: Format message */
    char log_line[MAX_LOG_LINE_LENGTH];
    if (logger->formatter == NULL)
    {
        __default_formatter_function(msg, log_line, level);
        // TODO: find a way to handle the error
    }
    else
    {
        logger->formatter->format(msg, log_line, level);
    }

    if (logger->handler_list == NULL)
    {
        // TODO: find a way to handle the error
    }

    LogHandler *handler_ptr = logger->handler_list;
    for (int i = 0; i < logger->handler_count; i++)
    {
        handler_ptr->lock();
        handler_ptr->write(log_line);
        handler_ptr->unlock();
        handler_ptr += sizeof(LogHandler);
    }
}

void logError(const char *msg)
{
    Logger *logger = getLogger();
    return __log(logger, LOG_ERROR, msg);
}
void logWarn(const char *msg)
{
    Logger *logger = getLogger();
    return __log(logger, LOG_WARNING, msg);
}
void logInfo(const char *msg)
{
    Logger *logger = getLogger();
    return __log(logger, LOG_INFO, msg);
}
void logDebug(const char *msg)
{
    Logger *logger = getLogger();
    return __log(logger, LOG_DEBUG, msg);
}
