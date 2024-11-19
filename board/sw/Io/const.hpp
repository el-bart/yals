#pragma once

namespace Io
{
constexpr uint8_t max_frame_size  = 16;
constexpr uint8_t max_mtu_size    = 1 + 2 + 2 * max_frame_size + 2 + 1;
constexpr uint8_t max_buffer_size = 2 * max_mtu_size;
}
