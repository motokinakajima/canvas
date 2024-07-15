#include <iostream>
#include "canvas.h"


int main() {
    canvas Canvas = canvas(59,59);
    //Canvas.draw_line(1,1,10,10,1);
    //Canvas.print_data();
    ///*
    //model Rectangle = model(4,4,2);
    //Rectangle.set_vertex({{1,1},{1,8},{8,1},{30,30}});
    //Rectangle.set_side({{0,1},{1,3},{0,2},{2,3}});

    model cube = model(8,12,3);
    cube.set_vertex({{3,1,-1},{3,1,1},{5,1,-1},{5,1,1},{3,-1,-1},{3,-1,1},{5,-1,-1},{5,-1,1}});
    cube.set_side({{0,4},{2,6},{0,2},{4,6},{1,5},{3,7},{1,3},{5,7},{0,1},{2,3},{4,5},{6,7}});
    //cube.print_data();

    //Canvas = Rectangle.draw(Canvas);
    //Canvas.print_data();
    std::cout<<std::endl;
    camera myCamera = camera({-2,0,0},{-10,20},100);
    world World = world(100,100,100,3,myCamera);
    World.add_item(cube);
    World.print_items();
    Canvas = World.render(0,Canvas);
    Canvas.print_data();
    //int flag;
    //flag = std::cin.get();
    return 0;
}
