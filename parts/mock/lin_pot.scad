// CDE23N-60-B50K
// 50kR linear potentiometer
// +-20%
include<../detail/config.scad>
include<../m3d/math.scad>


module lin_pot_knob()
{
  translate([0, -lin_pot_knob_size.y/2, -lin_pot_knob_size.z/2])
    cube(lin_pot_knob_size);
}


// knob_pos - 0-100[%]
module lin_pot(knob_pos=50/*%*/)
{
  assert(0 <= knob_pos);
  assert(knob_pos <= 100);

  k_pos_min = lin_pot_knob_pos_range[0];
  k_pos_max = lin_pot_knob_pos_range[1];
  k_pos = k_pos_min + (k_pos_max-k_pos_min)*knob_pos/100;

  difference()
  {
    cube(lin_pot_size);
    for(dy=lin_pot_mount_screw_pos)
      translate([-eps, dy, lin_pot_size.z/2])
  }
  translate([lin_pot_size.x, k_pos, lin_pot_size.z/2])
    lin_pot_knob();
}

lin_pot();
