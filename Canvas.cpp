#include "Canvas.h"

Canvas::Canvas(int x, int y) { Canvas::data = std::vector<std::vector<int>> (y, std::vector<int>(x, 0)); }

void Canvas::print_data() {
    for (auto &i : Canvas::data) {
        for (int j : i) {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
}

void Canvas::draw_pixel(int x, int y, int color) {
    if (y >= data.size() || y < 0 || x >= data.at(0).size() || x < 0) {
        return;
    }
    data[y][x] = color;
}

void Canvas::fill_circle(int cx, int cy, int r, int color) {
    for (int y = -r;y <= r;y++) {
        for (int x = -r;x <= r;x++) {
            if (x * x + y * y <= r * r) {
                Canvas::draw_pixel(cx + x, cy + y, color);
            }
        }
    }
}

void Canvas::draw_line(int x1, int y1, int x2, int y2, int thickness, int color) {
    if (std::abs(x2 - x1) < std::abs(y2 - y1)) {
        if (y1 > y2) {
            int tmp_x, tmp_y = y1;
            x1 = x2;
            x2 = tmp_x;
            y1 = y2;
            y2 = tmp_y;
        }
        for (int i = y1; i <= y2; i++) {
            fill_circle(std::round((i - y1) * (x2 + 1 - x1) / (y2 + 1 - y1)) + x1, i, thickness, color);
        }
    } else {
        if (x1 > x2) {
            int tmp_x = x1, tmp_y = y1;
            x1 = x2;
            x2 = tmp_x;
            y1 = y2;
            y2 = tmp_y;
        }
        for (int i = x1; i <= x2; i++) {
            fill_circle(i, std::round((i - x1) * (y2 + 1 - y1) / (x2 + 1 - x1)) + y1, thickness, color);
        }
    }
}

unsigned long long int Canvas::get_width() { return Canvas::data[0].size(); }

unsigned long long int Canvas::get_height() { return Canvas::data.size(); }

void Object::draw(Canvas& canvas) {
    dimension = vertex.at(0).size();
    if (dimension != 2) {
        std::cout << "Error: Dimension not supported" << std::endl;
        return;
    }
    for (auto &i : side) {
        canvas.draw_line(vertex.at(i.first).at(0), vertex.at(i.first).at(1), vertex.at(i.second).at(0), vertex.at(i.second).at(1), 1, 1);
    }
}

std::vector<double> Camera::get_vector() {
    double y_rad = rotation[0] * (M_PI / 180.0);
    double z_rad = rotation[1] * (M_PI / 180.0);
    return std::vector<double>{cos(z_rad) * -1 * cos(y_rad), sin(z_rad) * -1 * cos(y_rad), sin(y_rad)};
}

void World::render(int render_model_index, Canvas& render_canvas) {
    std::vector<std::vector<double>> model_vertex = items[render_model_index].get_vertex();
    std::vector<std::vector<double>> rendered_vertex(model_vertex.size(), std::vector<double>(2, 0));
    for (int i = 0; i < model_vertex.size(); i++) {
        rendered_vertex[i] = projection(model_vertex[i], render_canvas.get_width(), render_canvas.get_height());
    }
    Object rendered_model(rendered_vertex, items[render_model_index].get_side());
    rendered_model.draw(render_canvas);
}

std::vector<double> World::projection(std::vector<double> coordinate, double screen_width, double screen_height) {
    std::vector<double> camera_coordinate = camera.get_coordinate();
    std::vector<double> camera_vector = camera.get_vector();
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
    double fovRadians = camera.get_FOV_by_rad();

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