#pragma once

#include <stdexcept>
#include <string>
#include <string_view>
#include <source_location>

namespace OKengine {

// Severity levels, mapped to the four cases you described.
enum class Severity {
    Warning,    // FYI, keep running
    Recovered,  // engine auto-corrected, but you should fix the cause
    Error,      // recoverable failure -- thrown, meant to be caught
    Fatal       // unrecoverable -- log and abort
};

// Trivial, Logger-free, used by callers (e.g. on a caught Exception).
// Kept inline in the header on purpose: zero cost, no link dependency.
inline const char* to_string(Severity s) noexcept {
    switch (s) {
        case Severity::Warning:   return "WARNING";
        case Severity::Recovered: return "RECOVERED";
        case Severity::Error:     return "ERROR";
        case Severity::Fatal:     return "FATAL";
    }
    return "UNKNOWN";
}

// Type others catch -> its definition must be visible, so it stays in the header.
// The constructor BODY lives in errors.cpp; the trivial getters stay inline.
class Exception : public std::runtime_error {
public:
    Exception(std::string_view message,
              Severity severity,
              const std::source_location& loc);

    Severity severity() const noexcept { return severity_; }
    const std::source_location& where() const noexcept { return location_; }

private:
    Severity severity_;
    std::source_location location_;
};

// Declarations only. The default arguments live HERE and are evaluated at the
// call site, so call-site capture survives the split. No Logger.hpp needed here.
void warn(std::string_view message,
          const std::source_location& loc = std::source_location::current());

void recovered(std::string_view message,
               const std::source_location& loc = std::source_location::current());

[[noreturn]] void error(std::string_view message,
                        const std::source_location& loc = std::source_location::current());

[[noreturn]] void fatal(std::string_view message,
                        const std::source_location& loc = std::source_location::current());

} // namespace OKengine