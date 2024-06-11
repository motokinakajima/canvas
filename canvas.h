#ifndef GRAPHICS_CANVAS_H
#define GRAPHICS_CANVAS_H
#include <iostream>
#include <cmath>
#include <utility>
#include <vector>

class canvas {
public:
    canvas() = default;
    canvas(int x,int y){
        data = std::vector<std::vector<int>>(y,std::vector<int>(x,0));
    }

    void print_data(){
        for(int i = 0;i < data.size();i++){
            for(int j = 0;j < data.at(0).size();j++){
                std::cout<<data[i][j]<<" ";
            }
            std::cout<<"\n";
        }
    }

    void draw_pixel(int x, int y, int content){
        data[y][x] = content;
    }

    void draw_line(int x1,int y1, int x2, int y2, int content){
        if(std::abs(x2-x1)<std::abs(y2-y1)){
            //std::cout<<"y base"<<std::endl;
            if(y1 > y2){
                int tmp_x = x1, tmp_y = y1;
                tmp_x = x1;
                tmp_y = y1;
                x1 = x2;
                x2 = tmp_x;
                y1 = y2;
                y2 = tmp_y;
            }
            //std::cout<<"x1: "<<x1<<" y1: "<<y1<<" x2: "<<x2<<" y2: "<<y2<<std::endl;
            for(int i = y1; i <= y2; i++){
                draw_pixel(std::round((i-y1)*(x2+1-x1)/(y2+1-y1))+x1,i,content);
                //std::cout<<"x: "<<std::round((i-y1)*(x2+1-x1)/(y2+1-y1))+x1<<" y: "<<i<<std::endl;
            }
        }else{
            //std::cout<<"x base"<<std::endl;
            if(x1 > x2){
                int tmp_x = x1, tmp_y = y1;
                tmp_x = x1;
                tmp_y = y1;
                x1 = x2;
                x2 = tmp_x;
                y1 = y2;
                y2 = tmp_y;
            }
            //std::cout<<"x1: "<<x1<<" y1: "<<y1<<" x2: "<<x2<<" y2: "<<y2<<std::endl;
            for(int i = x1; i <= x2; i++){
                draw_pixel(i,std::round((i-x1)*(y2+1-y1)/(x2+1-x1))+y1,content);
                //std::cout<<"x: "<<i<<" y: "<<std::round((i-x1)*(y2+1-y1)/(x2+1-x1))+y1<<std::endl;
            }
        }
    }
private:
    std::vector<std::vector<int>> data;
};

class model {
public:
    model() = default;
    model(int vertex_num,int side_num,int model_dimention){
        dimention = model_dimention;
        vertex = std::vector<std::vector<double>> (vertex_num,std::vector<double>(dimention));
        side = std::vector<std::vector<double>> (side_num,std::vector<double>(2));
    }

    void set_vertex(std::vector<std::vector<double>> input){
        vertex = std::move(input);
    }

    void set_side(std::vector<std::vector<double>> input){
        side = std::move(input);
    }

    std::vector<std::vector<double>> get_vertex(){
        return vertex;
    }

    std::vector<std::vector<double>> get_side(){
        return side;
    }

    canvas draw(canvas Canvas){
        if(dimention != 2){
            std::cout<<"This model is not 2d"<<std::endl;
            return Canvas;
        }
        for(int i = 0;i < side.size();i++){
            Canvas.draw_line(vertex[side[i][0]][0],vertex[side[i][0]][1],vertex[side[i][1]][0],vertex[side[i][1]][1],1);
            //std::cout<<"first: "<<side[i][0]<<"  second: "<<side[i][1]<<std::endl;
            //std::cout<<"x1: "<<vertex[side[i][0]][0]<<"  y1: "<<vertex[side[i][0]][1]<<"  x2: "<<vertex[side[i][1]][0]<<"  y2: "<<vertex[side[i][1]][1]<<std::endl;
            //Canvas.print_data();
            //std::cout<<std::endl;
        }
        return Canvas;
    }

    void print_data(){
        for(int i = 0;i < vertex.size();i++){
            std::cout<<"vertex["<<i<<"]: {";
            for(int j = 0;j < vertex.at(0).size();j++){
                if(j == vertex.at(0).size() - 1){
                    std::cout<<vertex[i][j]<<"}"<<std::endl;
                    break;
                }else{
                    std::cout<<vertex[i][j]<<",";
                }
            }
        }
        std::cout<<std::endl;
        for(int i = 0;i < side.size();i++){
            std::cout<<"side["<<i<<"]: {";
            for(int j = 0;j < side.at(0).size();j++){
                if(j == side.at(0).size() - 1){
                    std::cout<<side[i][j]<<"}"<<std::endl;
                    break;
                }else{
                    std::cout<<side[i][j]<<",";
                }
            }
        }
        std::cout<<std::endl<<std::endl;
    }
private:
    std::vector<std::vector<double>> vertex;
    std::vector<std::vector<double>> side;
    int dimention;
};

class world {
public:
    world(double max_width, double max_height, double max_depth, int max_item_num)
        : width(max_width), height(max_height), depth(max_depth), items(max_item_num, model()) {}

    void add_item(model Model){
        for(int i = 0;i < items.size();i++){
            if(items[i].get_side().size() == 0){
                items[i] = std::move(Model);
                break;
            }
        }
    }

    void print_items(){
        for(int i = 0;i < items.size();i++){
            items[i].print_data();
        }
    }
    double get_width() const { return width; }
    double get_height() const { return height; }
    double get_depth() const { return depth; }
    int get_max_item_num() const { return items.size(); }
private:
    double width, height, depth;
    std::vector<model> items;
};

class camera {
public:
    /*
    camera(std::vector<double> camera_coordinate,double camera_depth, const world& camera_world){
        World(camera_world.get_width(), camera_world.get_height(), camera_world.get_depth(), camera_world.get_max_item_num());
        coordinate = std::move(camera_coordinate);
        depth = camera_depth;
    }
    */
    camera(std::vector<double> camera_coordinate, double camera_depth, const world& camera_world)
        : World(camera_world), coordinate(std::move(camera_coordinate)), depth(camera_depth) {}

    std::vector<double> get_coordinate() const { return coordinate; }

    double get_depth() const { return depth; }

    void set_coordinate(std::vector<double>&& camera_coordinate) & {
        coordinate = std::move(camera_coordinate);
    }

    void set_depth(double camera_depth) & {
        depth = camera_depth;
    }
private:
    world World;
    std::vector<double> coordinate;
    double depth;
};

#endif //GRAPHICS_CANVAS_H
