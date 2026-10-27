#include "logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

void _log_output(LogSeverity severity, const char* msg, ...) {
	char msg_str[1024];
	char time_str[10];
	const char* severity_str;

	va_list list;
	va_start(list, msg);
	vsnprintf(msg_str, sizeof(msg_str), msg, list);
	va_end(list);

	switch (severity) {
	case LOG_SEVERITY_TRACE:
		severity_str = "TRACE";
		break;
	case LOG_SEVERITY_INFO:
		severity_str = "INFO";
		break;
	case LOG_SEVERITY_WARN:
		severity_str = "WARN";
		break;
	case LOG_SEVERITY_ERROR:
		severity_str = "ERROR";
		break;
	default:
		break;
	}

	time_t t;
	time(&t);
	struct tm* tm_info = localtime(&t);
	strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);

	b8 is_error = (severity == LOG_SEVERITY_ERROR);
	FILE* console = is_error ? stderr : stdout;
	fprintf(console, "[%s] %s: %s\n", time_str, severity_str, msg_str);
}

void _report_assertion_failure(const char* expression, const char* message, const char* file, u32 line) {
	if (message) {
		_log_output(LOG_SEVERITY_ERROR, "Assertion Failure: %s, message: '%s', file: %s line: %d\n", expression, message, file, line);
	} else {
		_log_output(LOG_SEVERITY_ERROR, "Assertion Failure: %s, file: %s line: %d\n", expression, file, line);
	}
}
