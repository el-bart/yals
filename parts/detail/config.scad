// CDE23N-60-B50K linear potentiometer (horizontal, on side, knob to the right)
lin_pot_size = [11, 88.5, 13];
lin_pot_mount_screw_d = 3;
lin_pot_mount_screw_pos = [4, lin_pot_size.y-4];
lin_pot_knob_size = [15.3, 5, 1.2];
lin_pot_knob_pos_range = [14, lin_pot_size.y-14];
lin_pot_screw_d = 2;

// MT83 engine: 12V, on side
engine_size_d = 32;
engine_size_len = 19.7;
engine_size_shaft_d = 2;
engine_size_shaft_h = 8;
engine_size_shaft_hold_h = 3.5;

// screw row
screw_rod_d = 3;

// engine-to-shaft coupler
coupler_d = 6;
coupler_shaft_in = 5;
coupler_engine_in = 5;
coupler_spacing = 2;
coupler_len = coupler_shaft_in + coupler_spacing + coupler_engine_in;

// servo body
servo_body_bottom_h = 2;
servo_body_wall = 2;
servo_body_mount_screw_d = 3;
servo_body_threaded_insert_slot_d = 5;
servo_body_threaded_insert_slot_h = 6;

// bearing size [mm]
bearing_size = [3, 7, 3]; // internal_d, external_d, h

// carriage
carriage_magnet_d = 5;
carriage_magnet_h = 5;
carriage_wall = 2;
//carriage_rod_h = carriage_wall + lin_pot_knob_size.z+0.5 + carriage_wall + screw_rod_d/2 + 10;
carriage_rod_h = carriage_wall + lin_pot_knob_size.z+0.5 + carriage_wall + screw_rod_d/2 + 10;
carriage_size = [ 10,
                  lin_pot_knob_size.y + 2*carriage_wall,
                  carriage_rod_h + screw_rod_d/2 + carriage_wall + carriage_magnet_h ];
