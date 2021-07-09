#include "woxel_engine/core/log.hh"

#include <memory>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <vector>

namespace woxel {

void log::init() {
    std::array<spdlog::sink_ptr, 2> log_sinks = {
        std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
        std::make_shared<spdlog::sinks::basic_file_sink_mt>("woxel.log", true)};

    log_sinks[0]->set_pattern("%^[%T] %n: %v%$");
    log_sinks[1]->set_pattern("[%T] [%l] %n: %v");

    logger_woxel_ = std::make_shared<spdlog::logger>("WOXEL", begin(log_sinks), end(log_sinks));
    spdlog::register_logger(logger_woxel_);
    logger_woxel_->set_level(spdlog::level::trace);
    logger_woxel_->flush_on(spdlog::level::trace);

    logger_client_ = std::make_shared<spdlog::logger>("APP", begin(log_sinks), end(log_sinks));
    spdlog::register_logger(logger_client_);
    logger_client_->set_level(spdlog::level::trace);
    logger_client_->flush_on(spdlog::level::trace);
}

} // namespace woxel
