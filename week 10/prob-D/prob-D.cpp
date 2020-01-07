#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>
#include <limits>
#include <queue>
#include <cmath>
#include <set>

using namespace std;

struct point_t {
    int x, y;

    bool operator<(const point_t& other) const {
        if (other.x != this->x)
            return other.x < this->x;
        return other.y < this->y;
    }
};

struct line_segment_t {
    point_t start_p, end_p;
};

float calcTriangleArea(int x1, int y1, int x2, int y2, int x3, int y3) {
    return abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0);
}

// Checks whether a given point is inside a given triangle
// From: https://www.geeksforgeeks.org/check-whether-a-given-point-lies-inside-a-triangle-or-not/
bool isInsideTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y) {
    /* Calculate area of triangle ABC */
    float A = calcTriangleArea(x1, y1, x2, y2, x3, y3);

    /* Calculate area of triangle PBC */
    float A1 = calcTriangleArea(x, y, x2, y2, x3, y3);

    /* Calculate area of triangle PAC */
    float A2 = calcTriangleArea(x1, y1, x, y, x3, y3);

    /* Calculate area of triangle PAB */
    float A3 = calcTriangleArea(x1, y1, x2, y2, x, y);

    /* Check if sum of A1, A2 and A3 is same as A */
    return (A == A1 + A2 + A3);
}

bool checkPointInsideTriangle(point_t x0p, point_t y0p, point_t y1p, point_t y2p){
    return isInsideTriangle(y0p.x, y0p.y, y1p.x, y1p.y, y2p.x, y2p.y, x0p.x, x0p.y);
}


// Below code from https://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/
// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool onSegment(point_t p, point_t q, point_t r) {
    return q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
           q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y);
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(point_t p, point_t q, point_t r) {
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0; // colinear
    return (val > 0)? 1: 2; // clock or counterclock wise
}

// The function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool doIntersect(point_t p1, point_t q1, point_t p2, point_t q2)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    // p1, q1 and p2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

    // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    return o4 == 0 && onSegment(p2, q1, q2);
}

// // taken from: https://stackoverflow.com/questions/2303278/find-if-4-points-on-a-plane-form-a-rectangle
bool isOrthogonal(point_t a, point_t b, point_t c) {
    return (b.x - a.x) * (b.x - c.x) + (b.y - a.y) * (b.y - c.y) == 0;
}

bool isRectangle(point_t a, point_t b, point_t c, point_t d) {
    return isOrthogonal(a, b, c) && isOrthogonal(b, c, d) && isOrthogonal(c, d, a);
}

bool isRectangleAnyOrder(point_t a, point_t b, point_t c, point_t d) {
    return isRectangle(a, b, c, d) ||
           isRectangle(b, c, a, d) ||
           isRectangle(c, a, b, d);
}

bool isSquare(point_t x0, point_t x1, point_t x2, point_t x3) {
    return isRectangleAnyOrder(x0, x1, x2, x3);
}

// Checks whether the given solution is valid: triangle segments should not intersect square segments
bool checkValidSolution(const vector<point_t> &corners, int x0, int x1, int x2, int x3, int y0, int y1, int y2) {

    bool result = \
        doIntersect(corners[x0], corners[x1], corners[y0], corners[y1]) ||
        doIntersect(corners[x0], corners[x1], corners[y0], corners[y2]) ||
        doIntersect(corners[x0], corners[x1], corners[y1], corners[y2]) ||

        doIntersect(corners[x0], corners[x2], corners[y0], corners[y1]) ||
        doIntersect(corners[x0], corners[x2], corners[y0], corners[y2]) ||
        doIntersect(corners[x0], corners[x2], corners[y1], corners[y2]) ||

        doIntersect(corners[x0], corners[x3], corners[y0], corners[y1]) ||
        doIntersect(corners[x0], corners[x3], corners[y0], corners[y2]) ||
        doIntersect(corners[x0], corners[x3], corners[y1], corners[y2]) ||

        doIntersect(corners[x1], corners[x2], corners[y0], corners[y1]) ||
        doIntersect(corners[x1], corners[x2], corners[y0], corners[y2]) ||
        doIntersect(corners[x1], corners[x2], corners[y1], corners[y2]) ||

        doIntersect(corners[x1], corners[x3], corners[y0], corners[y1]) ||
        doIntersect(corners[x1], corners[x3], corners[y0], corners[y2]) ||
        doIntersect(corners[x1], corners[x3], corners[y1], corners[y2]) ||

        doIntersect(corners[x2], corners[x3], corners[y0], corners[y1]) ||
        doIntersect(corners[x2], corners[x3], corners[y0], corners[y2]) ||
        doIntersect(corners[x2], corners[x3], corners[y1], corners[y2]);

    return ! result;
}

/**
 * Returns true if the point p lies inside the poly with n vertices
 * From: https://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/
 * @param poly
 * @param p
 * @return
 */
bool isInside(vector<line_segment_t> poly, point_t p) {
    // There must be at least 3 vertices in poly
    if (poly.size() < 3)  return false;

    // Create a point for line segment from p to infinite
    point_t extreme = {1001, p.y + 500};

    // Count intersections of the above line with sides of polygon
    int count = 0, i = 0;
    do {
        int next = (i + 1) % (int) poly.size();

        // Check if the line segment from 'p' to 'extreme' intersects
        // with the line segment that is a side of the polygon
        if (doIntersect(poly[i].start_p, poly[i].end_p, p, extreme)) {
            // If the point 'p' is colinear with line segment 'i-next',
            // then check if it lies on segment. If it lies, return true,
            // otherwise false
            if (orientation(poly[i].start_p, p, poly[i].end_p) == 0)
                return onSegment(poly[i].start_p, p, poly[i].end_p);
            count++;
        }
        i = next;
    } while (i != 0);

    return count & 1; // true if odd, false otherwise
}

bool checkPointInsideSquare(point_t M, point_t A, point_t B, point_t C, point_t D){
    vector<line_segment_t> polygon{{A, B}, {B, C}, {C, D}, {D, A}};
    return isInside(polygon, M);
}


class TestCase {
    int n_corners;
    vector<point_t> corners;
    vector<point_t> result;

public:
    explicit TestCase(istream& in) {
        in >> n_corners;

        corners.resize(n_corners);
        for (int i = 0; i < n_corners; ++i)
            in >> corners[i].x >> corners[i].y;

        result = vector<point_t>(7);
    }

    bool checkValidTriangle(int xi, int xj, int xk, int xl) {
        bool resultIsValid = false;
        point_t xi_p = corners[xi];
        point_t xj_p = corners[xj];
        point_t xk_p = corners[xk];
        point_t xl_p = corners[xl];

        // check if points make a square
        if (! isSquare(xi_p, xj_p, xk_p, xl_p)) return false;

        for (int yi = 0; yi < n_corners; ++yi) {
            for (int yj = yi + 1; yj < n_corners; ++yj) {
                for (int yk = yj + 1; yk < n_corners; ++yk) {
                    if (yi == yj || yi == yk || yj == yk ||
                        yi == xi || yi == xj || yi == xk || yi == xl ||
                        yj == xi || yj == xj || yj == xk || yj == xl ||
                        yk == xi || yk == xj || yk == xk || yk == xl) {
                        continue;
                    }

                    point_t yi_p = corners[yi];
                    point_t yj_p = corners[yj];
                    point_t yk_p = corners[yk];

                    // check square inside triangle
                    if (checkPointInsideTriangle(xi_p, yi_p, yj_p, yk_p))
                        continue;

                    // triangle is always valid by the problem description
                    resultIsValid = checkValidSolution(corners, xi, xj, xk, xl, yi, yj, yk);
                    if (!resultIsValid) continue;

                    // setup result description
                    if (isRectangle(corners[xi], corners[xj], corners[xk], corners[xl])) {
                        // check if triangle inside square
                        if (checkPointInsideSquare(yi_p, xi_p, xj_p, xk_p, xl_p)) {
                            resultIsValid = false;
                            continue;
                        }

                        result[0] = corners[xi];
                        result[1] = corners[xj];
                        result[2] = corners[xk];
                        result[3] = corners[xl];
                        result[4] = yi_p;
                        result[5] = yj_p;
                        result[6] = yk_p;

                        return resultIsValid;

                    } else {
                        if (isRectangle(corners[xj], corners[xk], corners[xi], corners[xl])) {
                            // check if triangle inside square
                            if (checkPointInsideSquare(yi_p, xj_p, xk_p, xi_p, xl_p)) {
                                resultIsValid = false;
                                continue;
                            }
                        } else {
                            if (isRectangle(corners[xk], corners[xi], corners[xj], corners[xl])) {
                                // check if triangle inside square
                                if (checkPointInsideSquare(yi_p, xk_p, xi_p, xj_p, xl_p)) {
                                    resultIsValid = false;
                                    continue;
                                }

                                // cout << yi_p.x << yi_p.y << "2" << endl;
                                result[0] = corners[xk];
                                result[1] = corners[xi];
                                result[2] = corners[xj];
                                result[3] = corners[xl];
                                result[4] = yi_p;
                                result[5] = yj_p;
                                result[6] = yk_p;

                                return resultIsValid;
                            }
                        }
                    }
                }
            }
        }
        return resultIsValid;
    }

    bool computeValidResult() {
        bool resultIsValid = false;

        // from n_corners choose 4
        for (int xi = 0; xi < n_corners; ++xi) {
            for (int xj = xi + 1; xj < n_corners; ++xj) {
                for (int xk = xj + 1; xk < n_corners; ++xk) {
                    for (int xl = xk + 1; xl < n_corners; ++xl) {
                        if (xi == xj || xi == xk || xi == xl || xj == xk || xj == xl || xk == xl)
                            continue;

                        // points make square, check if we can choose three points to make triangle
                        resultIsValid = checkValidTriangle(xi, xj, xk, xl);
                        if (resultIsValid) return true;
                    }
                }
            }
        }

        return resultIsValid;
    }

    void solve() {
        if (computeValidResult()) {
            cout << "possible" << endl;
            for (auto point: result)
                cout << point.x << " " << point.y << endl;
        } else {
            cout << "impossible" << endl;
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);

    uint num_cases;
    cin >> num_cases;
    for (uint i = 0; i < num_cases; ++i) {
        cout << "Case #" << i + 1 << ": ";
        TestCase(cin).solve();
    }

    return EXIT_SUCCESS;
}
