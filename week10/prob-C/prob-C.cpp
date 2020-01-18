#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <limits>

#define MAX_ERROR   1e-6

using namespace std;

// Circle packing problem (with restrictions on radii)
// See: https://en.wikipedia.org/wiki/Circle_packing

template <class Comparable>
struct point_t {
    Comparable x, y;

    bool operator<(const point_t& other) const {
        if (other.x != this->x)
            return other.x < this->x;
        return other.y < this->y;
    }
};

double circle_area(double radius) {
    return radius * radius * M_PI;
}

double euclidean_distance(const point_t<double>& p1, const point_t<double>& p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

class TestCase {
    int n_cameras;
    vector<point_t<double>> cameras;
public:
    explicit TestCase(istream& in) {
        in >> n_cameras;
        cameras.resize(n_cameras);
        for (int i = 0; i < n_cameras; ++i)
            in >> cameras[i].x >> cameras[i].y;
    }

    double solve() {
        // Build matrix of distances between cameras
        vector<vector<double>> distances(n_cameras, vector<double>(n_cameras));
        for (int i = 0; i < n_cameras; ++i) {
            for (int j = 0; j < n_cameras; ++j) {
                distances[i][j] = (i == j) ? 0 : euclidean_distance(cameras[i], cameras[j]);
            }
        }

        // NOTE use k-center clustering ?
        // How to apply radii restrictions ?

        if (2 == n_cameras)
            return M_PI * distances[0][1] * distances[1][0];

        // 1st case: 1st camera gets maximal radius
        // Get closest 2nd camera
        double first_cam_rad = numeric_limits<double>::max();
        for (int i = 1; i < n_cameras; ++i)
            first_cam_rad = min(first_cam_rad, distances[0][i]);

        // 2nd case: other cameras pick radius first
        double other_radii = numeric_limits<double>::max();
        for (int i = 0; i < n_cameras; ++i) {
            for (int j = i + 1; j < n_cameras; ++j)
                other_radii = min(other_radii, distances[i][j] * (i == 0 ? 1 : 0.5));
        }
        // 2.2. pick 1st camera radius (maximal radius under restrictions from other cameras' radii)
        double scnd_case_first_cam_rad = numeric_limits<double>::max();
        for (int i = 1; i < n_cameras; ++i)
            scnd_case_first_cam_rad = min(scnd_case_first_cam_rad, distances[0][i] - other_radii);

        double frst_case_result = circle_area(first_cam_rad);
        double scnd_case_result = (n_cameras - 1) * circle_area(other_radii) + circle_area(scnd_case_first_cam_rad);
        return max(frst_case_result, scnd_case_result);
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);

    uint num_cases;
    cin >> num_cases;
    for (uint i = 0; i < num_cases; ++i) {
        cout << "Case #" << i + 1 << ": " << fixed << setprecision(8) << TestCase(cin).solve() << endl;
    }

    return EXIT_SUCCESS;
}
