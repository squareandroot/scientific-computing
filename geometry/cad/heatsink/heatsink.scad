num_fins=8;
fin_width=0.3;
fin_height=2.5;

difference() {
    translate([-2.5,0,0]) square([5,fin_height]);
    
    translate([0,0.3])
    for(i=[0:num_fins-2]) {
        cutout_size = (5.0 - num_fins*fin_width)/(num_fins-1);
        x_left = - 2.5 + i*(fin_width+cutout_size) + fin_width;
        
        translate([x_left,0,0]) square([cutout_size, fin_height]);
    }
}
