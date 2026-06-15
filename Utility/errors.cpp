/**
 * @file errors.cpp
 * @brief Implementation of the OKengine error-reporting functions.
 *
 * Defines the Exception constructor and the warn/recovered/error/fatal
 * functions, including message formatting and the bridge to the logger. The
 * logger dependency is confined to this translation unit.
 *
 * Part of OKengine.
 * @author Oliver Lyon
 * @date 2026-06-14
 */

#include "errors.hpp"
#include "Logger.hpp"   // the Logger dependency is now confined to this .cpp

#include <iostream>
#include <cstdlib>

namespace OKengine {
namespace {   // internal helpers -- not exposed in the header

std::string format_message(std::string_view msg,
                           Severity sev,
                           const std::source_location& loc) {
    std::string out;
    out.reserve(msg.size() + 64);
    out.append("[OKengine ");
    out.append(to_string(sev));
    out.append("] ");
    out.append(loc.file_name());
    out.push_back(':');
    out.append(std::to_string(loc.line()));
    out.append(" (");
    out.append(loc.function_name());
    out.append("): ");
    out.append(msg);
    return out;
}

::debug_level to_debug_level(Severity s) noexcept {
    switch (s) {
        case Severity::Warning:   return ::debug_level::WARN;
        case Severity::Recovered: return ::debug_level::WARN;
        case Severity::Error:     return ::debug_level::ERROR;
        case Severity::Fatal:     return ::debug_level::ERROR;
    }
    return ::debug_level::INFO;
}

std::string record(std::string_view message,
                   Severity sev,
                   const std::source_location& loc) {
    std::string formatted = format_message(message, sev, loc);
    ::Logger::GetInstance().log(formatted, to_debug_level(sev));
    return formatted;
}

} // anonymous namespace

Exception::Exception(std::string_view message,
                     Severity severity,
                     const std::source_location& loc)
    : std::runtime_error(format_message(message, severity, loc)),
      severity_(severity),
      location_(loc) {}

// NOTE: default arguments are NOT repeated here -- they may only appear once,
// in the header declaration.
void warn(std::string_view message, const std::source_location& loc) {
    std::cerr << record(message, Severity::Warning, loc) << '\n';
}

void recovered(std::string_view message, const std::source_location& loc) {
    std::cerr << record(message, Severity::Recovered, loc) << '\n';
}

void error(std::string_view message, const std::source_location& loc) {
    record(message, Severity::Error, loc);
    throw Exception(message, Severity::Error, loc);
}

void fatal(std::string_view message, const std::source_location& loc) {
    std::cerr << record(message, Severity::Fatal, loc) << std::endl;
    std::abort();
}

} // namespace OKengine