use <m3d/fn.scad>
include <m3d/math.scad>
include <detail/config.scad>

module coupler()
{
  // engine shaft
  e_h = coupler_engine_in;
  e_d = 2;
  // threaded shaft
  s_h = coupler_shaft_in;
  s_d = 3;

  h = coupler_len;
  difference()
  {
    cylinder(d=coupler_d, h=h, $fn=fn(30));
    translate([0, 0, -eps])
      cylinder(d=e_d, h=e_h+eps, $fn=fn(60));
    translate([0, 0, h-s_h])
      cylinder(d=s_d, h=s_h+eps, $fn=fn(60));
  }
}


coupler();
