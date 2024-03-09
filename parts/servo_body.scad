use <mock/engine.scad>
use <mock/lin_pot.scad>
use <mock/bearing.scad>
use <coupler.scad>
use <m3d/fn.scad>
include <m3d/math.scad>
include <detail/config.scad>

module servo_body_engine_pos()
{
  translate([0, 0, servo_body_bottom_h]) // space for bototm core block
    translate([0, -engine_size_shaft_h-engine_size_shaft_hold_h, 0]) // 
      translate([0, -engine_size_len, engine_size_d/2]) // axis-centered
        rotate([0, 90, 0])
          rotate([-90, 0, 0])
            children();
}


module servo_body_lin_pot_pos()
{
  translate([0, 0, servo_body_bottom_h])
    translate([-lin_pot_knob_size.x/2, 0, -lin_pot_size.z/2]) // mount position
      translate([-lin_pot_size.x, 0, -lin_pot_size.z/2 + engine_size_d/2]) // axis-centered
        children();
}


module servo_body_coupler_pos()
{
  translate([0, 0, servo_body_bottom_h])
    translate([0, -coupler_engine_in, engine_size_d/2])
      rotate([-90, 0, 0])
        children();
}


module servo_body(mocks=true)
{
  module main_support()
  {
    lps = lin_pot_size;
    ks = lin_pot_knob_size;
    bh = servo_body_bottom_h;
    thickness = 3.9;
    extra_h = 3;

    module lin_pot_screw_mounts()
    {
      module blocks()
      {
        module lin_pot_screw_mount()
        {
          s = [thickness, lin_pot_knob_pos_range[0]-lin_pot_knob_size.y/2, lin_pot_size.z];
          translate([0, 0, -extra_h])
            cube(s + [0, 0, extra_h]);
        }
        for(dy=[0, lin_pot_knob_pos_range[1]+ks.y/2])
          translate([lin_pot_size.x, dy, 0])
            servo_body_lin_pot_pos()
              lin_pot_screw_mount();
      }

      difference()
      {
        blocks();
        // screw holes
        for(dy=lin_pot_mount_screw_pos)
          translate([-eps+lin_pot_size.x, dy, lin_pot_size.z/2])
            servo_body_lin_pot_pos()
              rotate([0, 90, 0])
                cylinder(d=lin_pot_screw_d+0.5, h=thickness+2*eps, $fn=fn(50));
      }
    }

    module bearing_support()
    {
      s = [lin_pot_knob_size.x, thickness, engine_size_d/2+engine_size_d/4];
      pos = lin_pot_knob_pos_range + [-s.y-lin_pot_knob_size.y/2, lin_pot_knob_size.y/2];

      module support()
      {
        difference()
        {
          cube(s);
          translate([s.x/2, -eps, engine_size_d/2])
          {
            // rod
            rotate([-90, 0, 0])
              cylinder(d=screw_rod_d+1, h=lin_pot_size.y, $fn=fn(40));
            rotate([-90, 0, 0])
              bearing_slot();
          }
        }
      }

      for(dy=pos)
        translate([-s.x/2, dy, servo_body_bottom_h])
          support();
    }

    // base
    translate([-lps.x - ks.x/2, 0, 0])
      cube([lps.x + ks.x, lps.y, bh]);
    lin_pot_screw_mounts();
    bearing_support();
  }

  main_support();

  %if(mocks)
    servo_body_lin_pot_pos()
    {
      lin_pot(  0/*%*/);
      lin_pot( 50/*%*/);
      lin_pot(100/*%*/);
    }
  %if(mocks)
    servo_body_engine_pos()
      engine();
  %if(mocks)
    servo_body_coupler_pos()
      coupler();
}


servo_body();
