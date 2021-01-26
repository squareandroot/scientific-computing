//$fn = 20;

teeth_factor = 1;

union() {
    difference() {
        cylinder(d=60, h=50);
        
        for(angle=[0:40*teeth_factor:360])
            rotate([0,0,angle])
            color("blue")
            translate([0,-30,-10])
            linear_extrude(70)
            scale([teeth_factor, teeth_factor])
            polygon([[-9,-4],[9,-4],[3,10],[-3,10]]);
    }

    difference() {
        translate([0,0,-40])
        cylinder(d=30, h=150);
        
        difference() {
            translate([0,0,70])
            cylinder(d=35, h=60);
            
            translate([0,0,70])
            cylinder(d=24, h=50);
        }
        
        difference() {
            translate([0,0,-45])
            cylinder(d=35, h=30);
            
            translate([0,0,-45])
            cylinder(d=24, h=30);
        }
        
        color("green")
        translate([-4,13,82])
        rotate([90,0,0])
        linear_extrude(7)
        
        union() {
            square([8, 16]);
            translate([4,0,0]) circle(d=8);
            translate([4,16,0]) circle(d=8);
        }
    }
}
