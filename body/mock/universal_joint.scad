// 3mm universal joint
include<../detail/config.scad>
include<../m3d/math.scad>
use<../m3d/fn.scad>


module universal_joint()
{
  bh = universal_joint_part_bottom_h;
  th = universal_joint_part_top_h;
  d_ext = universal_joint_d;
  d_shaft = universal_joint_shaft_d;
  wall = universal_joint_wall;
  joint_h = universal_joint_part_joint_h;

  module element()
  {
    module body()
    {
      // bottom block
      cylinder(d=d_ext, h=bh, $fn=fn(30));
      // side rods
      difference()
      {
        cylinder(d=d_ext, h=th, $fn=fn(30));
        cylinder(d=d_ext-2*wall, h=th+eps, $fn=fn(30));
        translate([0, 0, bh])
        {
          s = [d_ext-2*wall, d_ext+2*eps, th];
          translate(-[s.x/2, s.y/2, 0])
            cube(s);
        }
      }
    }

    module joint()
    {
      // center top joing (connector)
      translate([-d_ext/2, 0, joint_h])
        rotate([0, 90, 0])
          cylinder(d=wall, h=d_ext, $fn=fn(20));
    }
    

    difference()
    {
      body();
      // shaft slot
      translate([0, 0, -eps])
        cylinder(d=d_shaft, h=bh+2*eps, $fn=fn(15));
    }
    joint();
  }

  element();
  translate([0, 0, 2*joint_h])
    rotate([180, 0, 90])
      element();
}


universal_joint();
