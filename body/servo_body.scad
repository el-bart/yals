use <mock/lin_pot.scad>
use <mock/bearing.scad>
use <mock/universal_joint.scad>
use <mock/pcb.scad>
use <servo_body_top_mount.scad>
use <carriage.scad>
use <m3d/fn.scad>
use <m3d/engine/dc/mt44.scad>
include <m3d/math.scad>
include <detail/config.scad>

module engine()
{
  engine_dc_mt44();
}

module engine_crosssecion_2d()
{
  engine_dc_mt44_crosssection_2d();
}


module servo_body_engine_pos()
{
  translate([0, -engine_size_total_len - universal_joint_center_spacing, base_to_axis_h]) // axis-centered
    rotate([-90, 0, 0])
      children();
}


module servo_body_lin_pot_pos()
{
  translate([0, 0, servo_body_bottom_h])
    translate([-lin_pot_knob_size.x/2, 0, -lin_pot_size.z/2]) // mount position
      translate([-lin_pot_size.x, 0, lin_pot_size.z/2]) // axis-centered
        children();
}


module servo_body_pcb_pos()
{
  translate([-5, -14.3, -servo_body_bottom_h+1])
    servo_body_lin_pot_pos()
      children();
}


module servo_body_universal_joint_pos()
{
  translate([0, -universal_joint_full_len/2, base_to_axis_h])
    rotate([-90, 0, 0])
      children();
}


module servo_body_carriage_pos()
{
  knob_center = lin_pot_knob_pos_range[0] + (lin_pot_knob_pos_range[1] - lin_pot_knob_pos_range[0]) / 2;
  translate([ lin_pot_size.x + lin_pot_knob_size.x/2,
              knob_center,
              lin_pot_size.z/2 - lin_pot_knob_size.z/2 - carriage_wall - carriage_knob_slot_spacing/2 ]) // move to final spot
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
    thickness = 3.5;

    module lin_pot_screw_mounts()
    {
      module blocks()
      {
        module lin_pot_screw_mount()
        {
          s = [thickness, lin_pot_knob_pos_range[0]-lin_pot_knob_size.y/2, lin_pot_size.z];

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

          phased_cube(size=s, phase=[2/3*s.x, s.x]);
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
      h = base_to_axis_h - servo_body_bottom_h + bearing_size[1]/2 + thickness;
      s = [lin_pot_knob_size.x, thickness, h];
      pos = lin_pot_knob_pos_range + [-s.y-lin_pot_knob_size.y/2, lin_pot_knob_size.y/2];

      module support()
      {
        difference()
        {
          cube(s);
          translate([s.x/2, -eps, base_to_axis_h-servo_body_bottom_h])
          {
            // rod
            rotate([-90, 0, 0])
              cylinder(d=screw_rod_d+1, h=s.y+2*eps, $fn=fn(40));
            rotate([-90, 0, 0])
              bearing_slot();
          }
        }
      }

      translate([-s.x/2, 0, servo_body_bottom_h])
      {
        // supports
        for(dy=pos)
          translate([0, dy, 0])
            support();
        // stiffners
        ss = [thickness, pos[1]-pos[0], thickness/2];
        for(dx=[0, s.x-ss.x])
          translate([dx, pos[0], 0])
            cube(ss);
      }
    }

    module engine_support()
    {
      wall = servo_body_wall;
      es = [ engine_box_size.x, engine_box_size.z, engine_box_size.y ];
      screw_mount_space = wall + servo_body_threaded_insert_slot_d + wall;
      sb = [ es.x + 2*screw_mount_space,
             engine_size_total_len + universal_joint_center_spacing + wall,
             servo_body_bottom_h ];

      module base_engine_mount()
      {
        to_eng_base = base_to_axis_h - servo_body_bottom_h - es.z/2;
        eng_sup = [ sb.x, es.y, base_to_axis_h - servo_body_bottom_h ];

        module support_block()
        {
          module core()
          {
            // engine block holder
            translate([0, wall, 0 ])
            {
              cube(eng_sup);
            }
            // engine-blocking elements
            union()
            {
              sblk = wall*[1,1,0] + [0, 0, to_eng_base+wall];
              for(dy=[0, wall + es.y])
                translate([sb.x/2-sblk.x/2, dy, 0])
                  cube(sblk);
            }
          }

          translate(-[sb.x/2, sb.y, 0])
          {
            delta_y = servo_body_engine_mount_spacing_h;
            // base
            cube(sb);
            // elements on top
            translate([0, 0, servo_body_bottom_h])
            {
              difference()
              {
                core();
                translate([sb.x/2, wall-delta_y, base_to_axis_h-servo_body_bottom_h])
                  spaced_engine_slot(dy=2*delta_y);
              }
            }
          }
        }

        module threaded_insert_slot(h)
        {
          rotate([180, 0, 0])
          {
            cylinder(d=servo_body_threaded_insert_slot_d,
                     h=servo_body_threaded_insert_slot_h,
                     $fn=fn(20));
            translate([0, 0, servo_body_threaded_insert_slot_h-eps])
              cylinder(d=servo_body_mount_screw_d+0.5, h=h+eps, $fn=fn(40));
          }
        }

        difference()
        {
          support_block();
          for(dy=[-1,+1])
            translate([ dy*(sb.x/2-servo_body_threaded_insert_slot_d/2-wall),
                        -sb.y + wall + es.y/2,
                        base_to_axis_h+eps ])
              threaded_insert_slot(h=base_to_axis_h);
        }
      }

      base_engine_mount();
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
    translate([0, universal_joint_center_spacing, base_to_axis_h])
      rotate([-90, 0, 0])
        cylinder(d=screw_rod_d,
                 h=lin_pot_size.y-universal_joint_center_spacing,
                 $fn=fn(40)); // main rod
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
    servo_body_universal_joint_pos()
      universal_joint();
  %if(mocks)
    translate([0, -eps, 0])
      servo_body_engine_pos()
        rotate([90, 0, 0])
          servo_body_top_mount();
  %if(mocks)
    servo_body_carriage_pos()
      carriage();
  %if(mocks)
    servo_body_pcb_pos()
      pcb();
}


servo_body();
