include <../m3d/engine/dc/mt44_conf.scad>

// CDE23N-60-B50K linear potentiometer (horizontal, on side, knob to the right)
lin_pot_travel = 60;
lin_pot_size = [11, lin_pot_travel+28.3, 13];
lin_pot_mount_screw_d = 3;
lin_pot_mount_screw_dist_from_edge = ( lin_pot_size.y - lin_pot_travel - 20 ) / 2;
lin_pot_mount_screw_pos = [4, lin_pot_size.y-4];
lin_pot_knob_size = [15.3, 5, 1.2];
lin_pot_knob_dist_from_edge = ( lin_pot_size.y - lin_pot_travel ) / 2;
lin_pot_knob_pos_range = [lin_pot_knob_dist_from_edge, lin_pot_size.y - lin_pot_knob_dist_from_edge];

// MT44 engine - mock sizes taken from m3d
engine_box_size = engine_dc_mt44_box_size;
engine_size_shaft_d = engine_dc_mt44_shaft_d;
engine_size_shaft_h = engine_dc_mt44_shaft_h + engine_dc_mt44_bearing_top_h;
engine_size_total_len = engine_box_size.z + engine_size_shaft_h;

// universal joint
universal_joint_part_bottom_h = 2.8;
universal_joint_part_top_h = 8.75;
universal_joint_part_joint_h = 6.5;
universal_joint_full_len = 2 * universal_joint_part_joint_h;
universal_joint_d = 7;
universal_joint_shaft_d = 3;
universal_joint_wall = 1;
assert( universal_joint_shaft_d + 2*universal_joint_wall <= universal_joint_d );
universal_joint_center_spacing = universal_joint_part_joint_h - universal_joint_part_bottom_h; // distance to be kept from the center of the joint

// screw row
screw_rod_d = 3;

// servo body
servo_body_bottom_h = 2;
servo_body_wall = 2;
servo_body_mount_screw_d = 3;
servo_body_threaded_insert_slot_d = 4.6;
servo_body_threaded_insert_slot_h = 5.7;
servo_body_engine_mount_spacing_r = 0.15; // around the engine
servo_body_engine_mount_spacing_h = 0.15; // before and after engine (dy)

// bearing size [mm]
bearing_size = [3, 7, 3]; // [ internal_d, external_d, h ]

// combined, utility metrics
base_to_axis_h = servo_body_bottom_h + lin_pot_size.z;

// servo body: mount tower dimensions
servo_body_mount_tower_hole_dist_to_wall = 3;
servo_body_mount_tower_size = ( servo_body_mount_screw_d + 2 * servo_body_mount_tower_hole_dist_to_wall ) * [1,1,0] +
                              [ 0,0, base_to_axis_h +
                                engine_box_size.y/2 +
                                servo_body_engine_mount_spacing_r +
                                servo_body_wall ];

// carriage
carriage_magnet_d = 5;
carriage_magnet_h = 5;
carriage_wall = 2;
carriage_knob_slot_spacing = 0.5;
carriage_rod_h = carriage_wall + (carriage_knob_slot_spacing + lin_pot_knob_size.z)/2 + // center to potentiometer's knob center
                 base_to_axis_h - servo_body_bottom_h - lin_pot_size.z/2;               // move to height of the rod
carriage_size = [ 10,
                  lin_pot_knob_size.y + 2*carriage_wall,
                  carriage_rod_h + screw_rod_d/2 + carriage_wall + carriage_magnet_h ];
carriage_threaded_insert_slot_d = 4.6;
carriage_threaded_insert_slot_h = 5.7;
