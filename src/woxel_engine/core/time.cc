#include "woxel_engine/core/time.hh"

#include <chrono>

namespace woxel {

// duration

duration::duration(std::chrono::steady_clock::duration d) : d_(d) {}
duration::duration(float seconds) : d_(as_duration(seconds)) {}

duration::operator float() const { return ::woxel::as_seconds(d_); }

auto duration::as_seconds() const -> float
{
    return ::woxel::as_seconds(d_);
}
auto duration::as_milliseconds() const -> float
{
    return ::woxel::as_milliseconds(d_);
}
auto duration::as_microseconds() const -> float
{
    return ::woxel::as_microseconds(d_);
}

// stopwatch

void stopwatch::start() {
    start_     = clock::now();
    lap_start_ = start_;
}

auto stopwatch::elapsed() -> duration
{
    return clock::now() - start_;
}

auto stopwatch::restart() -> duration {
    auto const &now = clock::now();
    lap_start_      = now;
    last_lap_total_ = now - std::exchange(start_, now);
    return last_lap_total_;
}

auto stopwatch::lap() -> duration
{
    auto const &now = clock::now();
    last_lap_       = now - std::exchange(lap_start_, now);
    last_lap_total_ = now - start_;
    return last_lap_;
}

auto stopwatch::last_lap() const -> duration
{
    return last_lap_;
}

auto stopwatch::last_lap_total() const -> duration
{
    return last_lap_total_;
}

} // namespace woxel
