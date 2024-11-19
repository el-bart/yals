#pragma once
#include <optional>
#include <array>

namespace Io
{

struct Buffer final
{
  template<typename ...Args>
  void dive_add(uint8_t b1, uint8_t b2, Args... args)
  {
    dive_add(b1);
    dive_add(b2);
    dive_add(args...);
  }

  void dive_add(uint8_t b)
  {
    if(size_ == max_size)
      trim_by(1);
    data_[size_] = b;
    ++size_;
  }

  void dive_add() { }

  void consume_leading_garbage()
  {
    if(size_ == 0u)
      return;
    auto const gs = leading_garbage_size();
    trim_by(gs);
  }

  void trim_by(uint8_t trim_size)
  {
    if(size_ < trim_size)
    {
      size_ = 0;
      return;
    }
    auto const new_size = size_ - trim_size;
    for(auto i=0u; i<new_size; ++i)
      data_[i] = data_[trim_size + i];
    size_ = new_size;
  }

  static constexpr uint8_t max_size = 128;
  uint8_t size_{0};
  std::array<uint8_t, max_size> data_;

private:
  uint8_t leading_garbage_size() const
  {
    uint8_t c = 0;
    for(auto i=0u; i<size_; ++i, ++c)
      if(data_[i] == '!')
        return c;
    return c;
  }
};

}
