use <m3d/fn.scad>

module coupler()
{
  eps = 0.01;
  // engine shaft
  e_h = 5;
  e_d = 2;
  // threaded shaft
  s_h = 5;
  s_d = 3;

  h = e_h+2+s_h;
  difference()
  {
    cylinder(d=6, h=h, $fn=fn(30));
    translate([0, 0, -eps])
      cylinder(d=e_d, h=e_h+eps, $fn=fn(60));
    translate([0, 0, h-s_h])
      cylinder(d=s_d, h=s_h+eps, $fn=fn(60));
  }
}


coupler();
