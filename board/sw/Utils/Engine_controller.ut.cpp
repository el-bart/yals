#include "catch2/catch.hpp"
#include "Utils/Engine_controller.hpp"
#include "Utils/Config/settings.hpp"
#include "Hal/Sim.hpp"

using Hal::sim;
using EC = Utils::Engine_controller;
using namespace Utils::Config;

namespace
{

TEST_CASE("Engine_controller")
{
  auto constexpr pos_tolerance = Utils::Config::servo_position_tolerance_mm / Utils::Config::servo_traven_len_mm;
  auto constexpr eng_full_travel_time_s = Hal::Sim::eng_full_travel_time_s;
  auto constexpr dt_step_s = 0.001f;
  sim().reset();
  sim().position_ = 0.5;
  Hal::Engine eng;
  Hal::Clock const clock;
  EC ec{eng, clock, sim().position_};

  SECTION("on init no engine force is applied")
  {
    CHECK( sim().position_ == 0.5f );
    CHECK( sim().engine_force_ == 0u );
  }

  SECTION("when present matches current position, engine is stopped")
  {
    sim().update(500 * dt_step_s);  // make some time pass
    ec.update(sim().position_, sim().position_);
    CHECK( sim().position_ == 0.5f );
    CHECK( sim().engine_force_ == 0u );
  }

  SECTION("when present matches current position with a given tolerance, engine is stopped")
  {
    auto const off = servo_position_tolerance_mm / servo_traven_len_mm;
    sim().update(500 * dt_step_s);  // make some time pass
    // +
    ec.update(sim().position_ + off, sim().position_);
    CHECK( sim().position_ == 0.5f );
    CHECK( sim().engine_force_ == 0u );
    // -
    ec.update(sim().position_ - off, sim().position_);
    CHECK( sim().position_ == 0.5f );
    CHECK( sim().engine_force_ == 0u );
  }

  SECTION("chnging preset triggers movement")
  {
    auto const setpoint = 0.75f;
    INFO("setpoint = " << setpoint);
    auto prev_delta_pos  = setpoint - sim().position_;
    auto prev_pos_offset = setpoint - sim().position_;
    for(auto t = 0.0; t < eng_full_travel_time_s; t += dt_step_s)
    {
      auto const prev_pos = sim().position_;
      ec.update(setpoint, sim().position_);
      sim().update(dt_step_s);

      auto const delta_pos  = sim().position_ - prev_pos;
      auto const pos_offset = setpoint - sim().position_;
      INFO("t="       << t)
      INFO("pos="     << sim().position_);
      INFO("force="   << sim().engine_force_ / 65535.0);
      INFO("d_pos="   << delta_pos);
      INFO("pos_off=" << pos_offset);
      CHECK( fabs(delta_pos) <= fabs(prev_delta_pos) );
      CHECK( fabs(pos_offset) <= fabs(prev_pos_offset) );
      prev_delta_pos  = delta_pos;
      prev_pos_offset = pos_offset;
    }
    CHECK( sim().position_ == Approx(setpoint).epsilon(pos_tolerance) );
    CHECK( sim().engine_force_ == 0u );
  }
}

}
