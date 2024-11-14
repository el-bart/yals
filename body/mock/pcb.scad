module pcb()
{
  translate([0, 102.87, 0])
    rotate([0, 0, -90])
      rotate([90, 0, 0])
        translate([-90.8, 94.6, -1.35])
          import("pcb.stl");
}

pcb();
