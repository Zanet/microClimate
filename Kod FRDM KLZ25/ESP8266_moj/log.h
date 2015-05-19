#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "mbed.h"

#define LOG_OUTPUT_ERROR              (0)
#define LOG_OUTPUT_WARN               (0)
#define LOG_OUTPUT_INFO               (0)
#define LOG_OUTPUT_DEBUG              (0)
#define LOG_OUTPUT_VERBOSE            (0)

#define LOG_OUTPUT_ERROR_PREFIX       (1)
#define LOG_OUTPUT_WARN_PREFIX        (1)
#define LOG_OUTPUT_INFO_PREFIX        (0)
#define LOG_OUTPUT_DEBUG_PREFIX       (0)
#define LOG_OUTPUT_VERBOSE_PREFIX     (0)

extern Serial pc;

#define logOut pc.printf

#define logError(fmt, args...)\
    do {\
        if (LOG_OUTPUT_ERROR)\
        {\
            if(LOG_OUTPUT_ERROR_PREFIX)\
                logOut("[LOG Error:%s,%d,%s] ",__FILE__,__LINE__,__FUNCTION__);\
            logOut(fmt, ##args);\
            logOut("\r\n");\
        }\
    } while(0)

#define logWarn(fmt, args...)\
    do {\
        if (LOG_OUTPUT_WARN)\
        {\
            if(LOG_OUTPUT_WARN_PREFIX)\
                logOut("[LOG Warn:%s,%d,%s] ",__FILE__,__LINE__,__FUNCTION__);\
            logOut(fmt, ##args);\
            logOut("\r\n");\
        }\
    } while(0)

#define logDebug(fmt, args...)\
    do {\
        if (LOG_OUTPUT_DEBUG)\
        {\
            if (LOG_OUTPUT_DEBUG_PREFIX)\
                logOut("[LOG Debug:%s,%d,%s] ",__FILE__,__LINE__,__FUNCTION__);\
            logOut(fmt, ##args);\
            logOut("\r\n");\
        }\
    } while(0)

#define logInfo(fmt, args...)\
    do {\
        if (LOG_OUTPUT_INFO)\
        {\
            if (LOG_OUTPUT_INFO_PREFIX)\
                logOut("[LOG Info:%s,%d,%s] ",__FILE__,__LINE__,__FUNCTION__);\
            logOut(fmt, ##args);\
            logOut("\r\n");\
        }\
    } while(0)
        

#define logVerbose(fmt, args...)\
    do {\
        if (LOG_OUTPUT_VERBOSE)\
        {\
            if (LOG_OUTPUT_VERBOSE_PREFIX)\
                logOut("[LOG Verbose:%s,%d,%s] ",__FILE__,__LINE__,__FUNCTION__);\
            logOut(fmt, ##args);\
            logOut("\r\n");\
        }\
    } while(0)


#endif /* #ifndef __DEBUG_H__ */