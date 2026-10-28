#include "logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

// Text color helpers
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_RESET "\x1b[0m"

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

	switch (severity) {
	case LOG_SEVERITY_ERROR:
		fprintf(stderr, ANSI_COLOR_RED "[%s] %s: %s\n" ANSI_COLOR_RESET, time_str, severity_str, msg_str);
		break;
	case LOG_SEVERITY_WARN:
		fprintf(stdout, ANSI_COLOR_YELLOW "[%s] %s: %s\n" ANSI_COLOR_RESET, time_str, severity_str, msg_str);
		break;
	default:
		fprintf(stdout, "[%s] %s: %s\n", time_str, severity_str, msg_str);
		break;
	}
}

void _report_assertion_failure(const char* expression, const char* message, const char* file, u32 line) {
	if (message) {
		_log_output(LOG_SEVERITY_ERROR, "Assertion Failure: %s, message: '%s', file: %s line: %d\n", expression, message, file, line);
	} else {
		_log_output(LOG_SEVERITY_ERROR, "Assertion Failure: %s, file: %s line: %d\n", expression, file, line);
	}
}
