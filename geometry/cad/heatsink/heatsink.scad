num_fins=13;
fin_size=0.2;

difference() {
    translate([-2.5,0,0]) square([5,1.5]);
    
    translate([0,0.3])
    for(i=[0:num_fins-2]) {
        cutout_size = (5.0 - num_fins*fin_size)/(num_fins-1);
        x_left = - 2.5 + i*(fin_size+cutout_size) + fin_size;
        
        translate([x_left,0,0]) square([cutout_size, 1.5]);
    }
}
