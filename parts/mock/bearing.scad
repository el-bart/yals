// 3x7x3 [mm] bearing
include<../detail/config.scad>
include<../m3d/math.scad>
use<../m3d/fn.scad>


module bearing_slot(delta_r=0.5, delta_h=0)
{
  bs = bearing_size;
  cylinder(d=bs[1]+delta_r, h=bs[2]+delta_h, $fn=fn(80));
}


module bearing()
{
  bs = bearing_size;
  difference()
  {
    cylinder(d=bs[1], h=bs[2], $fn=fn(50));
    translate([0, 0, -eps])
      cylinder(d=bs[0], h=bs[2]+2*eps, $fn=fn(20));
  }
}


bearing();
