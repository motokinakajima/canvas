#ifndef GRAPHICS_CANVAS_H
#define GRAPHICS_CANVAS_H
#include <iostream>
#include <cmath>
#include <utility>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Canvas {
public:
    Canvas(int x, int y);

    void print_data();

    void draw_pixel(int x, int y, int color);

    void fill_circle(int x, int y, int r, int color);

    void draw_line(int x1, int y1, int x2, int y2, int thickness, int color);

    unsigned long long int get_width();

    unsigned long long int get_height();

private:
    std::vector<std::vector<int>> data;
};

class Object {
public:
    Object(std::vector<std::vector<double>> vertex, std::vector<std::pair<int, int>> side) : vertex(std::move(vertex)), side(std::move(side)) {};

    void set_vertex(std::vector<std::vector<double>> input) { vertex = std::move(input); };

    void set_side(std::vector<std::pair<int, int>> input) { side = std::move(input); };

    void draw(Canvas& canvas);

    std::vector<std::vector<double>> get_vertex() { return vertex; };

    std::vector<std::pair<int, int>> get_side() { return side; };

    int get_dimension() { return vertex.at(0).size(); };
private:
    std::vector<std::vector<double>> vertex;
    std::vector<std::pair<int, int>> side;
    int dimension{};
};

class Camera {
public:
    Camera(std::vector<double> coordinate, std::vector<double> rotation, double FOV) : coordinate(std::move(coordinate)), rotation(std::move(rotation)), FOV(FOV) {};

    std::vector<double> get_coordinate() { return coordinate; };

    std::vector<double> get_rotation() { return rotation; };

    void set_coordinate(std::vector<double> input) { coordinate = std::move(input); };

    void set_rotation(std::vector<double> input) { rotation = std::move(input); };

    std::vector<double> get_vector();

    [[nodiscard]] double get_FOV_by_rad() const { return FOV * (M_PI / 180.0); }

    [[nodiscard]] double get_FOV_by_deg() const { return FOV; }

private:
    std::vector<double> coordinate;
    std::vector<double> rotation;
    double FOV;
};

class World {
public:
    World(double width, double height, double depth, int dimension, Camera camera) : width(width), height(height), depth(depth), camera(std::move(camera)) {};

    void add_item(const Object& item) { items.push_back(item); };

    void render(int render_model_index, Canvas& render_canvas);

    void set_camera(Camera input) { camera = std::move(input); };

    Camera get_camera() { return camera; };

    void set_width(double input) { width = input; };

    void set_height(double input) { height = input; };

    void set_depth(double input) { depth = input; };

    [[nodiscard]] double get_width() const { return width; };

    [[nodiscard]] double get_height() const { return height; };

    [[nodiscard]] double get_depth() const { return depth; };

    std::vector<Object> get_items() { return items; };

    void set_items(std::vector<Object> input) { items = std::move(input); };
private:
    double width, height, depth;
    std::vector<Object> items;
    Camera camera;

    std::vector<double> projection(std::vector<double> coordinate, double screen_width, double screen_height);
};

#endif //GRAPHICS_CANVAS_H
