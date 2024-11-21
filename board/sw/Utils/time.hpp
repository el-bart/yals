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

inline auto operator+(Timepoint ts, Ticks t) { return Timepoint{ .value_ = ts.value_ + t.value_  }; }
inline auto operator+(Ticks t, Timepoint ts) { return Timepoint{ .value_ = t.value_  + ts.value_ }; }

inline auto operator <(Timepoint lhs, Timepoint rhs) { return lhs.value_  < rhs.value_; }
inline auto operator >(Timepoint lhs, Timepoint rhs) { return lhs.value_  > rhs.value_; }
inline auto operator==(Timepoint lhs, Timepoint rhs) { return lhs.value_ == rhs.value_; }
inline auto operator!=(Timepoint lhs, Timepoint rhs) { return lhs.value_ != rhs.value_; }

}
