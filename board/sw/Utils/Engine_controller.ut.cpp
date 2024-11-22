#include "catch2/catch.hpp"
#include "Utils/Engine_controller.hpp"
#include "Utils/Config/settings.hpp"
#include "Hal/Sim.hpp"

using Hal::sim;
using EC = Utils::Engine_controller;
using namespace Utils::Config;

namespace
{

template<typename F>
bool sim_move_to(EC& ec, float const setpoint, F&& on_iteration)
{
  auto prev_delta_pos  = setpoint - sim().position_;
  auto prev_pos_offset = setpoint - sim().position_;
  for(auto t = 0.0; t < servo_full_path_travel_time_s; t += control_loop_time)
  {
    auto const prev_pos = sim().position_;
    ec.update(setpoint, sim().position_);
    sim().update(control_loop_time);

    auto const delta_pos  = sim().position_ - prev_pos;
    auto const pos_offset = setpoint - sim().position_;
    INFO("t="       << t)
      INFO("pos="     << sim().position_);
    INFO("force="   << sim().engine_force_ / 65535.0);
    INFO("stall="   << (sim().simulate_stall_ ? "YES" : "no"));
    INFO("d_pos="   << delta_pos);
    INFO("pos_off=" << pos_offset);
    CHECK( fabs(delta_pos)  <= fabs(prev_delta_pos)  );
    CHECK( fabs(pos_offset) <= fabs(prev_pos_offset) );
    prev_delta_pos  = delta_pos;
    prev_pos_offset = pos_offset;
    // uncomment to get a trace of the numbers on each iteration:
    //CHECK(false);

    on_iteration(t, sim().position_);

    if( sim().engine_force_ == 0 )
      return true;
  }
  return false;
}

bool sim_move_to(EC& ec, float const setpoint)
{
  return sim_move_to(ec, setpoint, [](auto,auto){});
}

bool no_movement(EC& ec, float const setpoint)
{
  auto const prev_pos = sim().position_;
  ec.update(setpoint, sim().position_);
  sim().update(control_loop_time);
  CHECK( sim().position_ == prev_pos );
  CHECK( sim().engine_force_ == 0 );
  return sim().engine_force_ == 0;
}


TEST_CASE("Engine_controller")
{
  sim().reset();
  sim().position_ = 0.5;
  Hal::Engine eng;
  Hal::Clock const clock;
  EC ec{eng, clock, sim().position_};

  SECTION("on init no engine force is applied")
  {
    CHECK( sim().position_ == 0.5f );
    CHECK( sim().engine_force_ == 0 );
  }

  SECTION("when present matches current position, engine is stopped")
  {
    ec.update(sim().position_, sim().position_);
    CHECK( sim().position_ == 0.5f );
    CHECK( sim().engine_force_ == 0 );
  }

  SECTION("when present matches current position with a given tolerance, engine is stopped")
  {
    auto const off = 0.99f * servo_position_tolerance;
    SECTION("to the left")
    {
      CHECK( no_movement(ec, sim().position_ - off) );
    }
    SECTION("to the eight")
    {
      CHECK( no_movement(ec, sim().position_ + off) );
    }
  }

  SECTION("changing preset triggers movement")
  {
    auto const setpoint = 0.75f;
    INFO("setpoint=" << setpoint);
    INFO("servo_position_tolerance=" << servo_position_tolerance);
    REQUIRE( sim_move_to(ec, setpoint) );
    CHECK( sim().position_ == Approx(setpoint).margin(servo_position_tolerance) );
    CHECK( sim().engine_force_ == 0 );
  }

  SECTION("histeresis support")
  {
    REQUIRE( sim().engine_force_ == 0 );

    SECTION("not moving when position is inside histeresis")
    {
      auto const off = 0.99 * servo_position_histeresis;
      auto const pos = sim().position_;
      SECTION("to the left")
      {
        sim().position_ -= off;
        CHECK( no_movement(ec, pos) );
      }
      SECTION("to the right")
      {
        sim().position_ += off;
        CHECK( no_movement(ec, pos) );
      }
    }

    SECTION("moving when position is outside histeresis")
    {
      auto const off = 1.01 * servo_position_histeresis;
      auto const pos = sim().position_;
      SECTION("to the left")
      {
        sim().position_ -= off;
        CHECK( sim_move_to(ec, pos) );
      }
      SECTION("to the right")
      {
        sim().position_ += off;
        CHECK( sim_move_to(ec, pos) );
      }
    }

    SECTION("moving immediately if setpoint changes (even if new position is still within histeresis range)")
    {
      auto const off = 1.01 * servo_position_tolerance;
      SECTION("to the left")
      {
        CHECK( sim_move_to(ec, sim().position_ - off) );
      }
      SECTION("to the right")
      {
        CHECK( sim_move_to(ec, sim().position_ + off) );
      }
    }
  }

  SECTION("engine power is temporary increased when stall is detected")
  {
    auto const setpoint = 0.75f;
    auto stall_sim = [setpoint](double t, float pos) {
      auto const from = setpoint - engine_full_throttle_at_diff_mm / 2.0;
      auto const to   = setpoint - servo_position_tolerance * 1.3;
      sim().simulate_stall_ = (from < pos && pos < to);
    };
    INFO("setpoint=" << setpoint);
    INFO("servo_position_tolerance=" << servo_position_tolerance);
    REQUIRE( sim_move_to(ec, setpoint, stall_sim) );
    CHECK( sim().position_ == Approx(setpoint).margin(servo_position_tolerance) );
    CHECK( sim().engine_force_ == 0 );
  }
}

}
