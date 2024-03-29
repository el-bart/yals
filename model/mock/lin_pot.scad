// CDE23N-60-B50K
// 50kR linear potentiometer
// +-20%
include<../detail/config.scad>
include<../m3d/math.scad>
use<../m3d/fn.scad>


module lin_pot_knob()
{
  translate([0, -lin_pot_knob_size.y/2, -lin_pot_knob_size.z/2])
    cube(lin_pot_knob_size);
}


module lin_pot_mount_screws_pos()
{
  for(dy=lin_pot_mount_screw_pos)
    translate([1, dy, lin_pot_size.z/2])
      rotate([0, 90, 0])
        children();
}


// knob_pos - 0-100[%]
module lin_pot(knob_pos=50/*%*/)
{
  assert(0 <= knob_pos);
  assert(knob_pos <= 100);

  k_pos_min = lin_pot_knob_pos_range[0];
  k_pos_max = lin_pot_knob_pos_range[1];
  k_pos = k_pos_min + (k_pos_max-k_pos_min)*knob_pos/100;

  // * 1<->3 is 50kR
  // * 2 is in between 1 and 3 (i.e. moving part)
  module back_connectors()
  {
    module connector()
    {
      cs = [8.7, 2.6, 0.5];
      translate([-cs.x, 0, 0])
        cube(cs);
    }
    // 1
    translate([0, 8.1, 8.5])
      connector();
    // 2
    translate([0, 3, 3])
      connector();
    // 3
    translate([0, 77.9, 8.5])
      connector();
  }

  difference()
  {
    cube(lin_pot_size);
    lin_pot_mount_screws_pos()
      cylinder(d=lin_pot_mount_screw_d, h=lin_pot_size.x, $fn=fn(20));
  }
  translate([lin_pot_size.x, k_pos, lin_pot_size.z/2])
    lin_pot_knob();
  back_connectors();
}

lin_pot();
