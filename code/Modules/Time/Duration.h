#pragma once
//------------------------------------------------------------------------------
/**
    @class Oryol::Duration
    @brief Time
    @brief a duration (distance between two TimePoints)
    
    Durations are created by subtracting one TimePoint from another. Durations
    can be converted to various time units.

    @see TimePoint
*/
#include "Core/Types.h"

namespace Oryol {
    
class Duration {
public:
    /// default constructor
    Duration();
    /// copy-construct
    Duration(const Duration& rhs);
    /// construct from raw value
    Duration(int64 raw);
    
    /// assignment
    void operator=(const Duration& rhs);
    /// add a Duration
    void operator+=(const Duration& rhs);
    /// scale a Duration (result will be clamped)
    void operator*=(float64 s);
    /// subtract a Duration
    void operator-=(const Duration& rhs);
    /// equality
    bool operator==(const Duration& rhs) const;
    /// inequality
    bool operator!=(const Duration& rhs) const;
    /// less-then
    bool operator<(const Duration& rhs) const;
    /// greater-then
    bool operator>(const Duration& rhs) const;
    /// less-or-equal
    bool operator<=(const Duration& rhs) const;
    /// greater-or-equal
    bool operator>=(const Duration& rhs) const;

    /// get duration in seconds
    float64 AsSeconds() const;
    /// get duration in milliseconds
    float64 AsMilliSeconds() const;
    /// get duration in microseconds
    float64 AsMicroSeconds() const;
    /// get duration in nanoseconds
    float64 AsNanoSeconds() const;
    /// as nanoseconds integer ticks
    int64 AsTicks() const;
    
    /// get duration from seconds
    static Duration FromSeconds(float64 s);
    /// get duration from seconds
    static Duration FromSeconds(float32 s);
    /// get duration from milliseconds
    static Duration FromMilliSeconds(float64 ms);
    /// get duration from milliseconds
    static Duration FromMilliSeconds(float32 ms);
    /// get duration from microseconds
    static Duration FromMicroSeconds(float64 us);
    /// get duration from microseconds
    static Duration FromMicroSeconds(float32 us);
    /// get duration from nanoseconds
    static Duration FromNanoSeconds(float64 ns);
    /// get duration from nanoseconds
    static Duration FromNanoSeconds(float32 ns);
    
    /// get the raw value
    int64 getRaw() const;

private:
    friend class TimePoint;
    int64 val;
};

//------------------------------------------------------------------------------
inline Duration operator+(Duration a, const Duration& b) {
    a += b;
    return a;
}

//------------------------------------------------------------------------------
inline Duration operator-(Duration a, const Duration& b) {
    a -= b;
    return b;
}

//------------------------------------------------------------------------------
inline Duration operator*(Duration a, float s) {
    a *= s;
    return a;
}

//------------------------------------------------------------------------------
inline
Duration::Duration() :
val(0) {
    // empty
}

//------------------------------------------------------------------------------
inline
Duration::Duration(const Duration& rhs) :
val(rhs.val) {
    // empty
}

//------------------------------------------------------------------------------
inline
Duration::Duration(int64 raw) :
val(raw) {
    // empty
}

//------------------------------------------------------------------------------
inline void
Duration::operator=(const Duration& rhs) {
    this->val = rhs.val;
}

//------------------------------------------------------------------------------
inline void
Duration::operator+=(const Duration& rhs) {
    this->val += rhs.val;
}

//------------------------------------------------------------------------------
inline void
Duration::operator-=(const Duration& rhs) {
    this->val -= rhs.val;
}

//------------------------------------------------------------------------------
inline void
Duration::operator*=(float64 s) {
    this->val = (int64) ((float64)this->val * s);
}

//------------------------------------------------------------------------------
inline bool
Duration::operator==(const Duration& rhs) const {
    return this->val == rhs.val;
}

//------------------------------------------------------------------------------
inline bool
Duration::operator!=(const Duration& rhs) const {
    return this->val != rhs.val;
}

//------------------------------------------------------------------------------
inline bool
Duration::operator<(const Duration& rhs) const {
    return this->val < rhs.val;
}

//------------------------------------------------------------------------------
inline bool
Duration::operator>(const Duration& rhs) const {
    return this->val > rhs.val;
}

//------------------------------------------------------------------------------
inline bool
Duration::operator<=(const Duration& rhs) const {
    return this->val <= rhs.val;
}

//------------------------------------------------------------------------------
inline bool
Duration::operator>=(const Duration& rhs) const {
    return this->val >= rhs.val;
}

//------------------------------------------------------------------------------
inline int64
Duration::getRaw() const {
    // NOTE: this is not guaranteed to be in nano-seconds!
    return this->val;
}

//------------------------------------------------------------------------------
inline float64
Duration::AsSeconds() const {
    return ((float64)this->val) / 1000000000.0;
}

//------------------------------------------------------------------------------
inline Duration
Duration::FromSeconds(float64 s) {
    return Duration(int64(s * 1000000000.0));
}

//------------------------------------------------------------------------------
inline Duration
Duration::FromSeconds(float32 s) {
    return Duration(int64(s * 1000000000.0f));
}

//------------------------------------------------------------------------------
inline float64
Duration::AsMilliSeconds() const {
    return ((float64)this->val) / 1000000.0;
}

//------------------------------------------------------------------------------
inline Duration
Duration::FromMilliSeconds(float64 ms) {
    return Duration(int64(ms * 1000000.0));
}

//------------------------------------------------------------------------------
inline Duration
Duration::FromMilliSeconds(float32 ms) {
    return Duration(int64(ms * 1000000.0f));
}

//------------------------------------------------------------------------------
inline float64
Duration::AsMicroSeconds() const {
    return ((float64)this->val) / 1000.0;
}

//------------------------------------------------------------------------------
inline Duration
Duration::FromMicroSeconds(float64 us) {
    return Duration(int64(us * 1000.0));
}

//------------------------------------------------------------------------------
inline Duration
Duration::FromMicroSeconds(float32 us) {
    return Duration(int64(us * 1000.0f));
}

//------------------------------------------------------------------------------
inline float64
Duration::AsNanoSeconds() const {
    return (float64)this->val;
}

//------------------------------------------------------------------------------
inline Duration
Duration::FromNanoSeconds(float64 ns) {
    return Duration(int64(ns));
}

//------------------------------------------------------------------------------
inline Duration
Duration::FromNanoSeconds(float32 ns) {
    return Duration(int64(ns));
}

//------------------------------------------------------------------------------
inline int64
Duration::AsTicks() const {
    return this->val;
}

} // namespace Oryol