#pragma once 
#include "defines.h"

#define VENABLE_TRACE

typedef enum log_severity {
    LOG_SEVERITY_TRACE,
    LOG_SEVERITY_INFO,
    LOG_SEVERITY_WARN,
    LOG_SEVERITY_ERROR,
} LogSeverity;

void _report_assertion_failure(const char* expression, const char* message, const char* file, u32 line);
void _log_output(LogSeverity severity, const char* msg, ...);

#ifdef VENABLE_TRACE
#define VLOG_TRACE(...) _log_output(LOG_SEVERITY_TRACE, __VA_ARGS__)
#else
#define VLOG_TRACE(...) ((void)0)
#endif

#define VLOG_INFO(...) _log_output(LOG_SEVERITY_INFO,  __VA_ARGS__)
#define VLOG_WARN(...) _log_output(LOG_SEVERITY_WARN,  __VA_ARGS__)
#define VLOG_ERROR(...) _log_output(LOG_SEVERITY_ERROR, __VA_ARGS__)

#define VASSERT(expr) { \
  if (expr) { \
  } else { \
    _report_assertion_failure(#expr, NULL, __FILE__, __LINE__); \
    VDEBUG_BREAK(); \
  } \
} \

#define VASSERT_MSG(expr, message) { \
  if (expr) { \
  } else { \
    _report_assertion_failure(#expr, message, __FILE__, __LINE__); \
    VDEBUG_BREAK(); \
  } \
} \
