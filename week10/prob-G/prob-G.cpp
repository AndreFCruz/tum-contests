#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <limits>

#define MAX_ERROR   1e-6

using namespace std;

// Circle packing problem (with equal radii)

/**
 * Compute area of a regular n-sided polygon
 * Formula from: https://www.mathopenref.com/polygonregulararea.html
 * @param n_sides
 * @param side_len
 * @return area
 */
static double calc_polygon_area(int n_sides, double side_len) {
    if (0 == n_sides)
        return M_PI * side_len * side_len;
    double k = static_cast<double> (n_sides);
    return k * side_len * side_len / (4. * tan(M_PI / k));
}

class TestCase {
    int radius;
    int n_circles;

public:
    explicit TestCase(istream& in) {
        in >> radius >> n_circles; // r, b
    }

    double solve() {
        if (1 == n_circles)
            return radius;
        if (2 == n_circles)
            return radius * 2;

        // Estimate problem as a polygon with #n_circles sides
        double side_len = radius * 2;
        double polygon_area = calc_polygon_area(n_circles, side_len);

        // Triangle packing: divide polygon in n_circles triangles with side_len base
        double triang_area = polygon_area / n_circles;
        double triang_height = 2. * triang_area / side_len;

        // Triangle's side will be distance to center of polygon
        // Result is this distance + radius of the burger (as now they all fit along the polygon's perimeter)
        double triang_side = sqrt(triang_height * triang_height + radius * radius);
        return triang_side + radius;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);

    uint num_cases;
    cin >> num_cases;
    for (uint i = 0; i < num_cases; ++i) {
        cout << "Case #" << i + 1 << ": " << setprecision(8) << TestCase(cin).solve() << endl;
    }

    return EXIT_SUCCESS;
}
