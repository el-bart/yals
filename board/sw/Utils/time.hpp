#pragma once
#include <cinttypes>

namespace Utils
{

struct Timepoint final
{
  uint64_t value_{};
};

struct Ticks final
{
  uint64_t value_{};
};

inline auto operator+(Timepoint lhs, Ticks     rhs) { return Timepoint{ .value_ = lhs.value_ + rhs.value_ }; }
inline auto operator+(Ticks     lhs, Timepoint rhs) { return Timepoint{ .value_ = lhs.value_ + rhs.value_ }; }

inline auto operator <(Timepoint lhs, Timepoint rhs) { return lhs.value_  < rhs.value_; }
inline auto operator >(Timepoint lhs, Timepoint rhs) { return lhs.value_  > rhs.value_; }
inline auto operator==(Timepoint lhs, Timepoint rhs) { return lhs.value_ == rhs.value_; }
inline auto operator!=(Timepoint lhs, Timepoint rhs) { return lhs.value_ != rhs.value_; }

inline auto operator-(Timepoint lhs, Timepoint rhs) { return Ticks{ .value_ = lhs.value_ - rhs.value_ }; }
inline auto operator/(Ticks     lhs, Ticks     rhs) { return (1.0 * lhs.value_) / rhs.value_; }

}
