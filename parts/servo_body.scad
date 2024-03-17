use <mock/engine.scad>
use <mock/lin_pot.scad>
use <mock/bearing.scad>
use <coupler.scad>
use <servo_body_top_mount.scad>
use <carriage.scad>
use <m3d/fn.scad>
include <m3d/math.scad>
include <detail/config.scad>

module servo_body_engine_pos()
{
  translate([0, 0, servo_body_bottom_h]) // space for bototm core block
    translate([0, -engine_size_shaft_h-engine_size_shaft_hold_h-servo_body_extra_space_len, 0])
      translate([0, -engine_size_len, engine_size_d/2]) // axis-centered
        rotate([0, 90, 0])
          rotate([-90, 0, 0])
            children();
}


module servo_body_lin_pot_pos()
{
  translate([0, 0, servo_body_bottom_h])
    translate([-lin_pot_knob_size.x/2, 0, -lin_pot_size.z/2]) // mount position
      translate([-lin_pot_size.x, 0, engine_size_d/2 - lin_pot_size.z/2]) // axis-centered
        children();
}


module servo_body_coupler_pos()
{
  translate([0, 0, servo_body_bottom_h])
    translate([0, -coupler_engine_in-servo_body_extra_space_len, engine_size_d/2])
      rotate([-90, 0, 0])
        children();
}


module servo_body_carriage_pos()
{
  knob_center = lin_pot_knob_pos_range[0] + (lin_pot_knob_pos_range[1] - lin_pot_knob_pos_range[0]) / 2;
  translate([ lin_pot_size.x + lin_pot_knob_size.x/2,
              knob_center,
              lin_pot_size.z/2-lin_pot_knob_size.z/2-carriage_wall -0.5/2 ]) // move to final spot
    translate([-carriage_size.x/2, -carriage_size.y/2, 0]) // center
      servo_body_lin_pot_pos()
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
          e = [0, 0, extra_h];
          se = s + e;

          module phased_cube(size, phase)
          {
            translate([0, size.y, 0])
              rotate([90, 0, 0])
                linear_extrude(size.y)
                  polygon([
                    [0, 0],
                    [0, size.z],
                    [size.x - phase[0], size.z],
                    [size.x,            size.z - phase[1]],
                    [size.x, 0],
                  ]);
          }

          translate(-e)
            phased_cube(size=se, phase=[2/3*se.x, se.x]);
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
                cylinder(d=lin_pot_mount_screw_d+0.75, h=thickness+2*eps, $fn=fn(50));
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
              cylinder(d=screw_rod_d+1, h=s.y+2*eps, $fn=fn(40));
            rotate([-90, 0, 0])
              bearing_slot();
          }
        }
      }

      for(dy=pos)
        translate([-s.x/2, dy, servo_body_bottom_h])
          support();
    }

    module engine_support()
    {
      s = [ engine_size_d + 2*(3*servo_body_wall+servo_body_mount_screw_d),
            engine_size_len + engine_size_shaft_d + engine_size_shaft_h + coupler_spacing + servo_body_extra_space_len,
            servo_body_bottom_h];
      sm = [s.x, engine_size_len, engine_size_d/2];

      module engine_pos()
      {
        translate([0, 0, s.z])
          translate([sm.x/2, -eps, engine_size_d/2])
            rotate([-90, 0, 0])
              children();
      }

      module base_engine_mount()
      {
        translate([-s.x/2, -s.y, 0])
        {
          difference()
          {
            union()
            {
              // base
              cube(s);
              // engine mount block
              translate([0, 0, s.z])
                cube(sm);
              // block to prevent engine from slipping off
              {
                bs = [5, 5, 5];
                translate([s.x/2, engine_size_len+0.75, 0])
                  translate([-bs.x/2, 0, servo_body_bottom_h])
                    cube(bs);
              }
            }
            // main body part
            engine_pos()
              cylinder(d=engine_size_d+0.5, h=engine_size_len+2*eps, $fn=fn(50));
          }
          // bottom fix
          intersection()
          {
            // all sockets
            engine_pos()
              rotate([0, 0, 90])
                engine_mounts_slots(extra_len=1, extra_spacing=-0.5)
                  cube([1,1,1]);
            // keep just the very  bottom one
            cube([sm.x, sm.y, servo_body_bottom_h+5]);
          }
        }
      }

      difference()
      {
        base_engine_mount();
        // screw holes
        for(dx=[-1,+1])
          translate([dx*(engine_size_d/2 + servo_body_mount_screw_d/2 + 2*servo_body_wall), 0, 0])
            translate([0, -s.y + engine_size_len/2, -eps])
            {
              // main screw shaft
              cylinder(d=servo_body_mount_screw_d+1, h=engine_size_d+servo_body_bottom_h, $fn=fn(50));
              // threaded insert slot
              translate([0, 0, servo_body_bottom_h+engine_size_d/2-servo_body_threaded_insert_slot_h])
                cylinder(d=servo_body_threaded_insert_slot_d, h=servo_body_threaded_insert_slot_h+2*eps, $fn=fn(40));
            }
      }
    }

    // base
    translate([-lps.x - ks.x/2, 0, 0])
      cube([lps.x + ks.x, lps.y, bh]);
    lin_pot_screw_mounts();
    bearing_support();
    engine_support();
  }

  main_support();

  %if(mocks)
    translate([0, 0, servo_body_bottom_h+engine_size_d/2])
      rotate([-90, 0, 0])
        cylinder(d=screw_rod_d, h=lin_pot_size.y, $fn=fn(40)); // main rod
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
  %if(mocks)
    translate([0, -0.5, 0])
      servo_body_engine_pos()
        rotate([0, 0, 90])
          servo_body_top_mount(mocks=false);
  %if(mocks)
    servo_body_carriage_pos()
      carriage();
}


servo_body();
