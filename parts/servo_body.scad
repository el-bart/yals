use<mock/engine.scad>
use<mock/lin_pot.scad>
use<coupler.scad>
include<detail/config.scad>

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
    translate([-lin_pot_knob_size.x/2, 0, -engine_size_d/2/2]) // mount position
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
  module base()
  {
    cube([1,1,1]);
  }

  base();

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
