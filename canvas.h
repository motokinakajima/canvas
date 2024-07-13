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
        if((data.size() < y or y < 0) or (data.at(0).size() < x or x < 0)){
            return;
        }
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
    model(int vertex_num,int side_num,int model_dimension){
        dimension = model_dimension;
        vertex = std::vector<std::vector<double>> (vertex_num,std::vector<double>(dimension));
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
        if(dimension != 2){
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
    int dimension;
};

class camera {
public:
    camera() = default;
    camera(std::vector<double> camera_coordinate, std::vector<double> camera_depth)
            : coordinate(std::move(camera_coordinate)), rotation(std::move(camera_depth)) {}

    std::vector<double> get_coordinate() const { return coordinate; }

    std::vector<double> get_rotation() const { return rotation; }

    void set_coordinate(std::vector<double>&& camera_coordinate) & {
        coordinate = std::move(camera_coordinate);
    }

    void set_rotation(std::vector<double> camera_rotation) & {
        rotation = std::move(camera_rotation);
    }

    std::vector<double> get_vector(){
        return std::vector<double>{cos(rotation[1])*-1*cos(rotation[0]),sin(rotation[1])*-1*cos(rotation[0]),sin(rotation[0])};
    }
private:
    std::vector<double> coordinate {1,0,0};
    std::vector<double> rotation {0,0};
};

class world {
public:
    world(double max_width, double max_height, double max_depth, int max_item_num, camera world_camera)
        : width(max_width), height(max_height), depth(max_depth), items(max_item_num, model()), Camera(std::move(world_camera)) {}

    void add_item(model Model){
        for(int i = 0;i < items.size();i++){
            if(items[i].get_side().empty()){
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

    void render(int render_model_index, canvas render_canvas){
        std::vector<std::vector<double>> model_vertex = items[render_model_index].get_vertex();
        std::vector<std::vector<double>> rendered_vertex(model_vertex.size(),std::vector<double>(2,0));
        for(int i = 0;i < model_vertex.size();i++){
            rendered_vertex[i] = projection(model_vertex[i]);
        }
        model rendered_model;
        rendered_model.set_vertex(rendered_vertex);
        rendered_model.set_side(items[render_model_index].get_side());
        rendered_model.print_data();
        rendered_model.draw(std::move(render_canvas));
    }

    double get_width() const { return width; }
    double get_height() const { return height; }
    double get_depth() const { return depth; }
    unsigned long long int get_max_item_num() const { return items.size(); }
    std::vector<model> get_items(){ return items; }
    camera get_camera(){return Camera;}
private:
    double width, height, depth;
    std::vector<model> items;
    camera Camera;

    std::vector<double> projection(std::vector<double> coordinate){
        std::vector<double> camera_coordinate = Camera.get_coordinate();
        std::vector<double> camera_vector = Camera.get_vector();
        double a = camera_vector[0];
        double b = camera_vector[1];
        double c = camera_vector[2];
        double x0 = camera_coordinate[0];
        double y0 = camera_coordinate[1];
        double z0 = camera_coordinate[2];
        double x1 = coordinate[0];
        double y1 = coordinate[1];
        double z1 = coordinate[2];
        double t = (a*a + b*b + c*c) / (a*(x1-x0) + b*(y1-y0) + c*(z1-z0));
        double delta_x = (1 - t)*x0 + t*x1 - (x0 + a);
        double delta_z = (1 - t)*z0 + t*z1 - (z0 + c);
        double horizontal_vector_length = sqrt((pow(b,2) + pow(a,2)));
        double vertical_vector_length = sqrt(pow(a * b,2) + pow(b * c,2) + pow(pow(a,2)+pow(b,2),2));
        double lambda = delta_x/b + (a*c*delta_z)/(b*(a*a + b*b));
        double mu = delta_z/(-1*a*a + -1*b*b);
        return std::vector<double> {lambda * horizontal_vector_length,mu * vertical_vector_length};
    }
};

#endif //GRAPHICS_CANVAS_H
