#include <iostream>
#include "Canvas.h"


int main() {
    Canvas canvas(59, 59);
    //Canvas.draw_line(1,1,10,10,1);
    //Canvas.print_data();
    ///*
    //model Rectangle = model(4,4,2);
    //Rectangle.set_vertex({{1,1},{1,8},{8,1},{30,30}});
    //Rectangle.set_side({{0,1},{1,3},{0,2},{2,3}});

    Object cube = Object({{3, 1,  -1},
                          {3, 1,  1},
                          {5, 1,  -1},
                          {5, 1,  1},
                          {3, -1, -1},
                          {3, -1, 1},
                          {5, -1, -1},
                          {5, -1, 1}},

                         {{0, 4},
                          {2, 6},
                          {0, 2},
                          {4, 6},
                          {1, 5},
                          {3, 7},
                          {1, 3},
                          {5, 7},
                          {0, 1},
                          {2, 3},
                          {4, 5},
                          {6, 7}}
                          );
    //cube.print_data();

    //Canvas = Rectangle.draw(Canvas);
    //Canvas.print_data();
    std::cout << std::endl;
    Camera myCamera({-2, 0, 0}, {-10, 20}, 100);
    World world(100, 100, 100, 3, myCamera);
    world.add_item(cube);
    world.render(0, canvas);
    canvas.print_data();
    //int flag;
    //flag = std::cin.get();
    return 0;
}
