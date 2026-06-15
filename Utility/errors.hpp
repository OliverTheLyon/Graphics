/**
 * @file errors.hpp
 * @brief Error handling: severity levels, the Exception type, and reporting functions.
 *
 * Declares OKengine's error vocabulary -- the Severity enum, the Exception
 * class thrown for recoverable failures, and the warn/recovered/error/fatal
 * free functions used to report problems with source-location context.
 *
 * Part of OKengine.
 * @author Oliver Lyon
 * @date 2026-06-14
 */

#pragma once

#include <stdexcept>
#include <string>
#include <string_view>
#include <source_location>

namespace OKengine {

/**
 * @brief Severity of an engine diagnostic.
 *
 * Describes how the engine reacts to a problem, from purely informational
 * through to unrecoverable. Used to tag log output and Exceptions.
 */
enum class Severity {
    Warning,    ///< FYI, keep running
    Recovered,  ///< engine auto corrected, but you should fix the cause
    Error,      ///< recoverable failure, thrown but needs to be caught
    Fatal       ///< unrecoverable, crash, log and abort
};

/**
 * @brief Converts a Severity to its uppercase string label.
 *
 * Logger-free and kept inline in the header on purpose (zero cost, no link
 * dependency) so callers can use it anywhere, e.g. when printing a caught
 * Exception.
 * @param s severity to convert
 * @return static label such as "WARNING" or "FATAL"; "UNKNOWN" if unrecognized
 */
inline const char* to_string(Severity s) noexcept {
    switch (s) {
        case Severity::Warning:   return "WARNING";
        case Severity::Recovered: return "RECOVERED";
        case Severity::Error:     return "ERROR";
        case Severity::Fatal:     return "FATAL";
    }
    return "UNKNOWN";
}

/**
 * @brief Exception carrying engine context: severity and where it was raised.
 *
 * Thrown for recoverable failures (see error()). Derives from std::runtime_error
 * so it can be caught generically, and adds the originating Severity and source
 * location. Its definition lives in the header so callers can catch it; the
 * constructor body lives in errors.cpp and the trivial getters stay inline.
 */
class Exception : public std::runtime_error {
private:
    //~~~~~~~~~~~~~~~~VARIABLES~~~~~~~~~~~~~~~~

    Severity severity_;
    std::source_location location_;

public:
    /**
     * @brief Constructs an Exception from a message, severity, and origin.
     * @param message  human-readable description of the failure
     * @param severity severity level the failure was raised with
     * @param loc      source location where the exception originated
     */
    Exception(std::string_view message,
              Severity severity,
              const std::source_location& loc);

    /** @brief Gets the severity.
     * @return the severity level this exception was raised with.
     */
    Severity severity() const noexcept { return severity_; }

    /** @brief Gets the origin.
     * @return the source location where this exception was raised.
     */
    const std::source_location& where() const noexcept { return location_; }
};

// NOTE: declarations only. The default arguments live HERE (not in errors.cpp)
// and are evaluated at the call site, so call-site capture survives the split.

/**
 * @brief Reports a warning: logged at WARN, execution continues.
 * @param message human-readable description of the issue
 * @param loc     captured automatically at the call site; do not pass explicitly
 */
void warn(std::string_view message,
          const std::source_location& loc = std::source_location::current());

/**
 * @brief Reports a recovered failure: the engine auto corrected but the cause
 * should still be fixed. Logged at WARN, execution continues.
 * @param message human-readable description of what was recovered
 * @param loc     captured automatically at the call site; do not pass explicitly
 */
void recovered(std::string_view message,
               const std::source_location& loc = std::source_location::current());

/**
 * @brief Reports a recoverable error: logs at ERROR, then throws an Exception.
 * @param message human-readable description of the failure
 * @param loc     captured automatically at the call site; do not pass explicitly
 * @throws Exception always
 */
[[noreturn]] void error(std::string_view message,
                        const std::source_location& loc = std::source_location::current());

/**
 * @brief Reports an unrecoverable failure: logs at ERROR, then aborts the process.
 * @param message human-readable description of the failure
 * @param loc     captured automatically at the call site; do not pass explicitly
 */
[[noreturn]] void fatal(std::string_view message,
                        const std::source_location& loc = std::source_location::current());

} // namespace OKengine