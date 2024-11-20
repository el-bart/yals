#pragma once

namespace Io
{
constexpr size_t max_line_size   = 50; // keep it relatively large, as version string is >30B
constexpr size_t max_buffer_size = 8 * ( max_line_size + 1 );
}
