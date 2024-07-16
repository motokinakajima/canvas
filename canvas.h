#ifndef GRAPHICS_CANVAS_H
#define GRAPHICS_CANVAS_H

#include <iostream>
#include <cmath>
#include <utility>
#include <vector>
#include <conio.h>
#include <windows.h>

class canvas {
public:
    canvas() = default;

    canvas(int x, int y) {
        data = std::vector<std::vector<int>>(y, std::vector<int>(x, 0));
    }

    void clear(){
        for(int i = 0;i < data.size();i++){
            for(int j = 0;j < data.at(0).size();j++){
                data[i][j] = 0;
            }
        }
    }

    void goto_xy(int x, int y) {
        COORD coord = { (SHORT)x, (SHORT)y };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void print_data() {
        goto_xy(0,0);
        for (int i = 0; i < data.size(); i++) {
            for (int j = 0; j < data.at(0).size(); j++) {
                std::cout << data[i][j] << " ";
            }
            std::cout << "\n";
        }
        std::cout.flush();
    }

    void draw_pixel(int x, int y, int content) {
        if (y >= data.size() || y < 0 || x >= data.at(0).size() || x < 0) {
            return;
        }
        data[y][x] = content;
    }

    void fill_circle(int cx, int cy, int radius, int content) {
        for (int y = -radius; y <= radius; y++) {
            for (int x = -radius; x <= radius; x++) {
                if (x * x + y * y <= radius * radius) {
                    draw_pixel(cx + x, cy + y, content);
                }
            }
        }
    }

    void draw_line(int x1, int y1, int x2, int y2, int thickness, int content) {
        if (std::abs(x2 - x1) < std::abs(y2 - y1)) {
            //std::cout<<"y base"<<std::endl;
            if (y1 > y2) {
                int tmp_x, tmp_y;
                tmp_x = x1;
                tmp_y = y1;
                x1 = x2;
                x2 = tmp_x;
                y1 = y2;
                y2 = tmp_y;
            }
            //std::cout<<"x1: "<<x1<<" y1: "<<y1<<" x2: "<<x2<<" y2: "<<y2<<std::endl;
            for (int i = y1; i <= y2; i++) {
                fill_circle(std::round((i - y1) * (x2 + 1 - x1) / (y2 + 1 - y1)) + x1, i, thickness / 2, content);
                //std::cout<<"x: "<<std::round((i-y1)*(x2+1-x1)/(y2+1-y1))+x1<<" y: "<<i<<std::endl;
            }
        } else {
            //std::cout<<"x base"<<std::endl;
            if (x1 > x2) {
                int tmp_x, tmp_y;
                tmp_x = x1;
                tmp_y = y1;
                x1 = x2;
                x2 = tmp_x;
                y1 = y2;
                y2 = tmp_y;
            }
            //std::cout<<"x1: "<<x1<<" y1: "<<y1<<" x2: "<<x2<<" y2: "<<y2<<std::endl;
            for (int i = x1; i <= x2; i++) {
                fill_circle(i, std::round((i - x1) * (y2 + 1 - y1) / (x2 + 1 - x1)) + y1, thickness / 2 , content);
                //std::cout<<"x: "<<i<<" y: "<<std::round((i-x1)*(y2+1-y1)/(x2+1-x1))+y1<<std::endl;
            }
        }
    }

    unsigned long long int get_width() const { return data.at(0).size(); }

    unsigned long long int get_height() const { return data.size(); }

private:
    std::vector<std::vector<int>> data;
};

class model {
public:
    model() = default;

    model(int vertex_num, int side_num, int model_dimension) {
        dimension = model_dimension;
        vertex = std::vector<std::vector<double>>(vertex_num, std::vector<double>(dimension));
        side = std::vector<std::vector<double>>(side_num, std::vector<double>(2));
    }

    void set_vertex(std::vector<std::vector<double>> input) {
        vertex = std::move(input);
    }

    void set_side(std::vector<std::vector<double>> input) {
        side = std::move(input);
    }

    std::vector<std::vector<double>> get_vertex() {
        return vertex;
    }

    std::vector<std::vector<double>> get_side() {
        return side;
    }

    canvas draw(canvas Canvas) {
        dimension = vertex.at(0).size();
        if (dimension != 2) {
            std::cout << "This model is not 2d" << std::endl;
            return Canvas;
        }
        for (auto &i: side) {
            Canvas.draw_line(vertex[i[0]][0], vertex[i[0]][1], vertex[i[1]][0], vertex[i[1]][1], 2, 1);
            //std::cout<<"first: "<<side[i][0]<<"  second: "<<side[i][1]<<std::endl;
            //std::cout<<"x1: "<<vertex[side[i][0]][0]<<"  y1: "<<vertex[side[i][0]][1]<<"  x2: "<<vertex[side[i][1]][0]<<"  y2: "<<vertex[side[i][1]][1]<<std::endl;
            //Canvas.print_data();
            //std::cout<<std::endl;
        }
        return Canvas;
    }

    void print_data() {
        for (int i = 0; i < vertex.size(); i++) {
            std::cout << "vertex[" << i << "]: {";
            for (int j = 0; j < vertex.at(0).size(); j++) {
                if (j == vertex.at(0).size() - 1) {
                    std::cout << vertex[i][j] << "}" << std::endl;
                    break;
                } else {
                    std::cout << vertex[i][j] << ",";
                }
            }
        }
        std::cout << std::endl;
        for (int i = 0; i < side.size(); i++) {
            std::cout << "side[" << i << "]: {";
            for (int j = 0; j < side.at(0).size(); j++) {
                if (j == side.at(0).size() - 1) {
                    std::cout << side[i][j] << "}" << std::endl;
                    break;
                } else {
                    std::cout << side[i][j] << ",";
                }
            }
        }
        std::cout << std::endl << std::endl;
    }

private:
    std::vector<std::vector<double>> vertex;
    std::vector<std::vector<double>> side;
    int dimension{};
};

class camera {
public:
    camera() = default;

    camera(std::vector<double> camera_coordinate, std::vector<double> camera_rotation, double camera_FOV)
            : coordinate(std::move(camera_coordinate)), rotation(std::move(camera_rotation)), FOV(camera_FOV) {}

    std::vector<double> get_coordinate() const { return coordinate; }

    std::vector<double> get_rotation() const { return rotation; }

    void set_coordinate(std::vector<double> camera_coordinate){
        coordinate = std::move(camera_coordinate);
    }

    void set_rotation(std::vector<double> camera_rotation){
        rotation = std::move(camera_rotation);
    }

    std::vector<double> get_vector() {
        double y_rad = rotation[0] * (M_PI / 180.0);
        double z_rad = rotation[1] * (M_PI / 180.0);
        return std::vector<double>{cos(z_rad) * -1 * cos(y_rad), sin(z_rad) * -1 * cos(y_rad), sin(y_rad)};
    }

    double get_FOV_by_rad() const { return FOV * (M_PI / 180.0); }

    double get_FOV_by_deg() const { return FOV; }

private:
    std::vector<double> coordinate{1, 0, 0};
    std::vector<double> rotation{0, 0};
    double FOV = 100;
};

class world {
public:
    world(double max_width, double max_height, double max_depth, int max_item_num, camera world_camera)
            : width(max_width), height(max_height), depth(max_depth), items(max_item_num, model()),
              Camera(std::move(world_camera)) {}

    void add_item(model Model) {
        for (auto &item: items) {
            if (item.get_side().empty()) {
                item = std::move(Model);
                break;
            }
        }
    }

    void print_items() {
        for (auto &item: items) {
            item.print_data();
        }
    }

    canvas render(int render_model_index, canvas render_canvas) {
        std::vector<std::vector<double>> model_vertex = items[render_model_index].get_vertex();
        std::vector<std::vector<double>> rendered_vertex(model_vertex.size(), std::vector<double>(2, 0));
        for (int i = 0; i < model_vertex.size(); i++) {
            rendered_vertex[i] = projection(model_vertex[i], render_canvas.get_width(), render_canvas.get_height());
        }
        model rendered_model;
        rendered_model.set_vertex(rendered_vertex);
        rendered_model.set_side(items[render_model_index].get_side());
        //std::cout << "rendered model::" << std::endl;
        //rendered_model.print_data();
        render_canvas = rendered_model.draw(std::move(render_canvas));
        return render_canvas;
    }

    double get_width() const { return width; }

    double get_height() const { return height; }

    double get_depth() const { return depth; }

    unsigned long long int get_max_item_num() const { return items.size(); }

    std::vector<model> get_items() { return items; }

    camera get_camera() { return Camera; }

private:
    double width, height, depth;
    std::vector<model> items;
    camera Camera;

    std::vector<double> projection(std::vector<double> coordinate, double screen_width, double screen_height) {
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
        // Convert FOV to radians
        double fovRadians = Camera.get_FOV_by_rad();

        // Calculate projection factor
        double projectionFactor = 1.0 / tan(fovRadians / 2.0);

        // Calculate t
        double denominator_t = a * (x1 - x0) + b * (y1 - y0) + c * (z1 - z0);
        if (denominator_t == 0) {
            throw std::runtime_error("Denominator for calculating t is zero, leading to division by zero.");
        }
        double t = (a * a + b * b + c * c) / denominator_t;

        // Calculate intersection point
        double intersection_x = (1 - t) * x0 + t * x1;
        double intersection_z = (1 - t) * z0 + t * z1;

        // Calculate delta_x and delta_z
        double delta_x = intersection_x - (x0 + a);
        double delta_z = intersection_z - (z0 + c);

        // Calculate horizontal vector length and vertical vector length
        double horizontal_vector_length = sqrt(pow(b, 2) + pow(a, 2));
        double vertical_vector_length = sqrt(pow(a * c, 2) + pow(b * c, 2) + pow(a * a + b * b, 2));

        double lambda = 0.0, mu = 0.0;

        if (b != 0) {
            // Calculate lambda
            lambda = delta_x / b + (a * c * delta_z) / (b * (a * a + b * b));
        } else if (a != 0) {
            // Handle case where b is zero
            lambda = delta_x / a;
        } else {
            // Handle case where both a and b are zero (horizontal vector is zero)
            horizontal_vector_length = 0;
        }

        if (a * a + b * b != 0) {
            // Calculate mu
            mu = delta_z / (-a * a - b * b);
        } else {
            // Handle case where both a and b are zero (vertical vector is undefined)
            vertical_vector_length = sqrt(pow(c, 2)); // Vertical component is purely c
            mu = delta_z / (-c * c);
        }

        // Adjust lengths based on FOV and screen width
        double lambda_length = (lambda * horizontal_vector_length) * projectionFactor * screen_width + screen_width / 2;
        double mu_length = (mu * vertical_vector_length) * projectionFactor * screen_width + screen_height / 2;

        return std::vector<double>{lambda_length, mu_length};
    }
};

#endif //GRAPHICS_CANVAS_H
