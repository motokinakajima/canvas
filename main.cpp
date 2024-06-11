#include <iostream>
#include "canvas.h"


int main() {
    canvas Canvas = canvas(50,50);
    //Canvas.draw_line(1,1,10,10,1);
    //Canvas.print_data();
    ///*
    model Rectangle = model(4,4,2);
    Rectangle.set_vertex({{1,1},{1,8},{8,1},{30,30}});
    Rectangle.set_side({{0,1},{1,3},{0,2},{2,3}});

    model cube = model(4,4,3);
    cube.set_vertex({{1,1,0},{1,8,0},{8,1,0},{30,30,0}});
    cube.set_side({{0,1},{1,3},{0,2},{2,3}});
    cube.print_data();

    Canvas = Rectangle.draw(Canvas);
    Canvas.print_data();
    std::cout<<std::endl;
    world World = world(10,10,10,3);
    World.add_item(Rectangle);
    World.print_items();
    //int flag;
    //flag = std::cin.get();
    return 0;
}
