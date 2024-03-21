// MT83 DC motor
// 12V
// 0.15A when idling
// 0.2A
include<../detail/config.scad>
include<../m3d/math.scad>
use<../m3d/fn.scad>


module engine_mounts_slots(extra_len, extra_spacing=0)
{
  dr=37/2;
  size = 3;
  s = (size+extra_spacing)/size;
  for(r=[     0,  180,
          90-dr,  90+dr,
         -90-dr, -90+dr ])
    rotate([0, 0, r])
      translate([engine_size_d/2-3*s, -3*s/2, -eps])
        scale([s, s, 1])
          cube([3+extra_len, 3, 3.5+eps]);
}
