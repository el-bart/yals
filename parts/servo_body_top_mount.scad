use <m3d/fn.scad>
use <m3d/engine/dc/mt44.scad>
include <m3d/math.scad>
include <detail/config.scad>


module spaced_engine_slot(dy)
{
  spacing = 0.25;
  rotate([-90, 0, 0])
    linear_extrude(engine_box_size.z + dy)
      offset(r=spacing)
        engine_dc_mt44_crosssection_2d($fn=fn(100));
}

spaced_engine_slot(dy=0.42);


module servo_body_top_mount(mocks=true)
{
/*
  span = engine_size_d + 2*(3*servo_body_wall+servo_body_mount_screw_d);

  module body()
  {
    difference()
    {
      union()
      {
        cylinder(d=engine_size_d+2*servo_body_wall+0.5, h=engine_size_len, $fn=fn(50));
        s = [span, 2*servo_body_wall, engine_size_len];
        translate([-s.x/2, -s.y/2, 0])
          cube(s);
      }
      // center cut
      translate([0, 0, -eps])
        cylinder(d=engine_size_d+0.5, h=engine_size_len+2*eps, $fn=fn(50));
      // screw holes
      for(dx=[-1, +1])
        translate([dx*(span/2-servo_body_mount_screw_d/2-servo_body_wall), -eps, engine_size_len/2])
          rotate([-90, 0, 0])
            cylinder(d=servo_body_mount_screw_d+0.5, h=servo_body_wall+2*eps, $fn=fn(50));
    }
    // engine supports
    rotate([0, 0, 90])
      engine_mounts_slots(extra_len=1, extra_spacing=-0.5)
        cube([1,1,1]);
  }

  intersection()
  {
    body();
    // take half of it only
    s = [span+2*eps, engine_size_d+servo_body_wall, engine_size_len+eps];
    translate([-s.x/2, 0, 0])
      cube(s);
  }

  %if(mocks)
    rotate([0, 0, 90])
      engine();
*/
}


servo_body_top_mount();
