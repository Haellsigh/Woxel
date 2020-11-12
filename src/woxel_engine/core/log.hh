#pragma once

#include <spdlog/spdlog.h>

#include <memory>

namespace woxel {

class log {
  public:
    static void init();

    // application logging
    template <typename... Args> inline static void trace(Args... args) {
        logger_client_->trace(std::forward<Args>(args)...);
    }
    template <typename... Args> inline static void debug(Args... args) {
        logger_client_->debug(std::forward<Args>(args)...);
    }
    template <typename... Args> inline static void info(Args... args) {
        logger_client_->info(std::forward<Args>(args)...);
    }
    template <typename... Args> inline static void warn(Args... args) {
        logger_client_->warn(std::forward<Args>(args)...);
    }
    template <typename... Args> inline static void error(Args... args) {
        logger_client_->error(std::forward<Args>(args)...);
    }
    template <typename... Args> inline static void critical(Args... args) {
        logger_client_->critical(std::forward<Args>(args)...);
    }

    // core logging
    template <typename... Args> inline static void core_trace(Args... args) {
        logger_woxel_->trace(std::forward<Args>(args)...);
    }
    template <typename... Args> inline static void core_debug(Args... args) {
        logger_woxel_->debug(std::forward<Args>(args)...);
    }
    template <typename... Args> inline static void core_info(Args... args) {
        logger_woxel_->info(std::forward<Args>(args)...);
    }
    template <typename... Args> inline static void core_warn(Args... args) {
        logger_woxel_->warn(std::forward<Args>(args)...);
    }
    template <typename... Args> inline static void core_error(Args... args) {
        logger_woxel_->error(std::forward<Args>(args)...);
    }
    template <typename... Args> inline static void core_critical(Args... args) {
        logger_woxel_->critical(std::forward<Args>(args)...);
    }

  private:
    inline static std::shared_ptr<spdlog::logger> logger_woxel_;
    inline static std::shared_ptr<spdlog::logger> logger_client_;
};

} // namespace woxel
