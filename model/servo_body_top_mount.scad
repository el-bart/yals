use <m3d/fn.scad>
use <m3d/engine/dc/mt44.scad>
include <m3d/math.scad>
include <detail/config.scad>


module spaced_engine_slot(dy=0, spacing=servo_body_engine_mount_spacing_r)
{
  rotate([-90, 0, 0])
    linear_extrude(engine_box_size.z + dy)
      offset(r=spacing)
        engine_dc_mt44_crosssection_2d($fn=fn(100));
}


module servo_body_top_mount()
{
  wall = servo_body_wall;
  lenght = engine_box_size.z;
  es = [ engine_box_size.x, engine_box_size.z, engine_box_size.y ];
  base_size = [ es.x+2*(2*wall+servo_body_threaded_insert_slot_d), es.y, wall ];
  screw_d = servo_body_mount_screw_d + 0.5;

  module core()
  {
    module block()
    {
      spaced_engine_slot(dy=0, spacing=wall+servo_body_engine_mount_spacing_r);
      translate([-base_size.x/2, 0, 0])
        cube(base_size);
    }

    difference()
    {
      block();
      // hollow body
      translate([0, -eps, 0])
        spaced_engine_slot(dy=2*eps);
      // remove bottom part
      cut = [2*(es.x + 2*wall), lenght+2*eps, es.z];
      translate(-[cut.x/2, eps, cut.z])
        cube(cut);
    }
  }

  difference()
  {
    core();
    // screw holes
    for(dx=[-1,+1])
      translate([dx*(base_size.x/2 - screw_d/2 - wall), lenght/2, -eps])
        cylinder(d=screw_d, h=wall+2*eps, $fn=fn(50));
  }
}


rotate([90, 0, 0])
  servo_body_top_mount();
