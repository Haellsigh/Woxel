#pragma once

#include <chrono>

namespace woxel {

using f_sec   = std::chrono::duration<float>;
using f_milli = std::chrono::duration<float, std::milli>;
using f_micro = std::chrono::duration<float, std::micro>;

using clock      = std::chrono::high_resolution_clock;
using time_point = clock::time_point;

constexpr clock::duration as_duration(float seconds) {
    return std::chrono::duration_cast<clock::duration>(f_sec(seconds));
}
constexpr float as_seconds(clock::duration d) { return std::chrono::duration_cast<f_sec>(d).count(); }
constexpr float as_milliseconds(clock::duration d) { return std::chrono::duration_cast<f_milli>(d).count(); }
constexpr float as_microseconds(clock::duration d) { return std::chrono::duration_cast<f_micro>(d).count(); }

struct duration {
    duration(clock::duration d);
    duration(float seconds);

    static constexpr auto zero() { return clock::duration::zero(); }

    operator float() const;

    float as_seconds() const;
    float as_milliseconds() const;
    float as_microseconds() const;

  private:
    clock::duration d_;
};

/*!
 * \brief The stopwatch class measures the amount of time elapsed.
 */
class stopwatch {
  public:
    /*!
     * \brief Starts the stopwatch.
     */
    void start();

    /*!
     * \brief Returns the time elapsed since the timer was started.
     */
    duration elapsed();

    /*!
     * \brief Restarts the stopwatch.
     */
    duration restart();

    /*!
     * \brief Starts a new lap.
     */
    duration lap();

    /*!
     * \brief Returns the elapsed time between the last two laps.
     */
    duration last_lap() const;

    /*!
     * \brief Returns the elapsed time from start to the last lap.
     */
    duration last_lap_total() const;

  private:
    time_point start_, lap_start_;
    duration last_lap_total_ = duration::zero(), last_lap_ = duration::zero();
};

} // namespace woxel
