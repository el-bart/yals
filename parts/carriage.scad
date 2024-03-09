use <m3d/fn.scad>
use <m3d/screw_head_hex.scad>
include <m3d/math.scad>
include <detail/config.scad>


module carriage()
{
  s = carriage_size;
  w = carriage_wall;
  d = screw_rod_d + 0.5;

  ks = lin_pot_knob_size + 0.5*[0,1,1];
  rod_pod = [s.x/2, 0, carriage_rod_h];

  difference()
  {
    cube(s);
    // main rod
    translate(rod_pod)
      translate([0, -eps, 0])
        rotate([-90, 0, 0])
          cylinder(d=d, h=s.y+2*eps, $fn=fn(50));
    // knob slot
    translate([-eps, -ks.y/2 + s.y/2, w])
      cube(ks);
    // magnet slot
    translate([s.x/2, s.y/2, s.z-carriage_magnet_h])
      cylinder(d=carriage_magnet_d, h=carriage_magnet_h+eps, $fn=fn(50));
    // screw slots
    assert(screw_rod_d == 3); // M3 is assumed here
    for(dy=[0, s.y-2.5+2*eps])
      translate([0, dy, 0])
        translate(rod_pod)
          translate([0, -eps, 0])
            rotate([-90, 0, 0])
              screw_head_hex_m3(dy=0.5, dh=eps);
  }
}


rotate([0, -90, 0])
  carriage();
