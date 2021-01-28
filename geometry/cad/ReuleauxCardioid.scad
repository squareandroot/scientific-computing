//$fn = 100;

radius = 100;
R = 18;
r = 6;
height = sqrt(pow(radius,2)-pow(radius/2,2));

cardioid = [for(t = [0:10:360]) [(R+r)*cos(t)-r*cos((R+r)/r*t),(R+r)*sin(t)-r*sin((R+r)/r*t)]];

difference() {
    intersection() {
        circle(r=radius);
        translate([radius,0]) circle(r=radius);
        translate([radius/2, height]) circle(r=radius);
    }
    
    color("blue")
    translate([radius/2, height/3])
    rotate(90)
    polygon(cardioid);
}
