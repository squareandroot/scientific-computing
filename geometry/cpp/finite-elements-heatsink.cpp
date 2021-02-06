#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

struct point
{
    double x, y;

    point()
    {
        x = 0.0;
        y = 0.0;
    }

    point(double _x, double _y)
    {
        x = _x;
        y = _y;
    }
};

struct line
{
    int i1, i2;

    line(int _i1, int _i2)
    {
        i1 = _i1;
        i2 = _i2;
    }
};

struct triangle
{
    int i1, i2, i3;

    triangle(int _i1, int _i2, int _i3)
    {
        i1 = _i1;
        i2 = _i2;
        i3 = _i3;
    }
};

void read_mesh(string filename, vector<point> &points, vector<triangle> &triangles, vector<line> &lines)
{
    ifstream fs(filename.c_str());
    string s = "";

    // read until nodes
    while (s != "$Nodes")
        fs >> s;

    int num_nodes;
    fs >> num_nodes;

    for (int i = 0; i < num_nodes; i++)
    {
        int id;
        double x, y, unused;
        fs >> id >> x >> y >> unused;
        points.push_back(point(x, y));
    }

    fs >> s;
    fs >> s;
    cout << s << " (should be $Elements)" << endl;

    int num_elements;
    fs >> num_elements;

    cout << "read element of type ";
    for (int i = 0; i < num_elements; i++)
    {
        int id, type, unused;
        fs >> id >> type >> unused >> unused >> unused;
        cout << type << " ";

        // populate the vectors lines and triangles.
        if (type == 1)
        {
            int i1, i2;
            fs >> i1 >> i2;
            lines.push_back(line(i1 - 1, i2 - 1));
        }
        else if (type == 2)
        {
            int i1, i2, i3;
            fs >> i1 >> i2 >> i3;
            triangles.push_back(triangle(i1 - 1, i2 - 1, i3 - 1));
        }
        else
        {
            // read till the end of the line
            char c = ' ';
            while (c != '\n')
                fs.get(c);
        }
    }

    cout << endl;
}

void write_vtk(string filename, vector<point> &points, vector<triangle> &triangles, vector<line> &lines, vector<double> &point_data)
{
    ofstream fs(filename.c_str());

    int n_points = points.size();
    int n_triangles = triangles.size();

    fs << "# vtk DataFile Version 2.0" << endl;
    fs << "mesh with triangles" << endl;
    fs << "ASCII" << endl;
    fs << endl;
    fs << "DATASET POLYDATA" << endl;

    fs << "POINTS " << n_points << " double" << endl;
    for (int i = 0; i < n_points; i++)
        fs << points[i].x << " " << points[i].y << " " << 0.0 << endl;
    fs << endl;

    fs << "POLYGONS " << n_triangles << " " << 4 * n_triangles << endl;
    for (int i = 0; i < n_triangles; i++)
        fs << 3 << " " << triangles[i].i1 << " " << triangles[i].i2 << " " << triangles[i].i3 << endl;
    fs << endl;

    fs << "POINT_DATA " << n_points << endl;
    fs << "SCALARS value double 1" << endl;
    fs << "LOOKUP_TABLE default" << endl;
    for (int i = 0; i < n_points; i++)
        fs << point_data[i] << endl;
}

double euclidean_distance(const point &p1, const point &p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

double length_of_adjacent_lines(const vector<point> &points, const vector<line> &lines, int i)
{
    int n_L = lines.size();

    vector<point> p(3);
    p[2] = points[i];

    int counter = 0;
    int k = 0;

    while (counter < 2 && k < n_L)
    {
        if (lines[k].i1 == i)
        {
            p[counter] = points[lines[k].i2];
            counter++;
        }
        else if (lines[k].i2 == i)
        {
            p[counter] = points[lines[k].i1];
            counter++;
        }
        k++;
    }

    return euclidean_distance(p[0], p[2]) + euclidean_distance(p[1], p[2]);
}

bool line_has_vertex(line the_line, int i)
{
    if (the_line.i1 == i || the_line.i2 == i)
        return true;

    return false;
}

bool triangle_has_vertex(triangle tr, int i)
{
    if (tr.i1 == i || tr.i2 == i || tr.i3 == i)
        return true;

    return false;
}

double area_triangle(point p1, point p2, point p3)
{
    return 0.5 * abs((p1.x - p3.x) * (p2.y - p1.y) - (p1.x - p2.x) * (p3.y - p1.y));
}

vector<double> area_triangles(const vector<point> &points, const vector<triangle> &triangles)
{
    int n_T = triangles.size();

    vector<double> areas(n_T);

    for (int i = 0; i < n_T; i++)
    {
        point p1 = points[triangles[i].i1];
        point p2 = points[triangles[i].i2];
        point p3 = points[triangles[i].i3];

        areas[i] = area_triangle(p1, p2, p3);
    }

    return areas;
}

bool on_boundary(int i, const vector<line> &lines)
{
    int n_L = lines.size();

    for (int j = 0; j < n_L; j++)
        if (lines[j].i1 == i || lines[j].i2 == i)
            return true;

    return false;
}

void compute_bc(int i, triangle t, const vector<point> &points, double &b, double &c)
{
    point p1, p2, p3;
    if (i == t.i1)
    {
        p1 = points[t.i1];
        p2 = points[t.i2];
        p3 = points[t.i3];
    }
    else if (i == t.i2)
    {
        p1 = points[t.i2];
        p2 = points[t.i1];
        p3 = points[t.i3];
    }
    else if (i == t.i3)
    {
        p1 = points[t.i3];
        p2 = points[t.i1];
        p3 = points[t.i2];
    }
    else
    {
        cout << "ERROR: vertex i is not part of triangle t" << endl;
        exit(1);
    }

    double x1 = p1.x;
    double y1 = p1.y;

    double x2 = p2.x;
    double y2 = p2.y;

    double x3 = p3.x;
    double y3 = p3.y;

    double det = x1 * y2 - x1 * y3 - x2 * y1 + x2 * y3 + x3 * y1 - x3 * y2;
    b = (y2 - y3) / det;
    c = (x3 - x2) / det;
}

vector<double> compute_H(const vector<point> &points, const vector<triangle> &triangles, const vector<line> &lines)
{
    int n_v = points.size();
    int n_T = triangles.size();

    vector<double> H(n_v, 0.0);
    vector<double> areas = area_triangles(points, triangles);

    for (int i = 0; i < n_v; i++)
            for (int k = 0; k < n_T; k++)
                if (triangle_has_vertex(triangles[k], i) == true)
                    H[i] += areas[k];

    return H;
}

vector<double> assemble_matrix(const vector<point> &points, const vector<triangle> &triangles, const vector<line> &lines, const vector<double> &H, const double &h, const double &T_fluid)
{

    int n_v = points.size();
    int n_L = lines.size();
    int n_T = triangles.size();

    // set B to zero
    vector<double> B(n_v * n_v, 0.0);

    vector<double> areas = area_triangles(points, triangles);

    for (int i = 0; i < n_v; i++)
        for (int j = 0; j < n_v; j++)
        {
            for (int k = 0; k < n_T; k++)
                if (triangle_has_vertex(triangles[k], i) == true && triangle_has_vertex(triangles[k], j) == true)
                {
                    double b_ik, b_jk, c_ik, c_jk;
                    compute_bc(i, triangles[k], points, b_ik, c_ik);
                    compute_bc(j, triangles[k], points, b_jk, c_jk);
                    B[j + n_v * i] -= areas[k] * (b_ik * b_jk + c_ik * c_jk);
                }

            if (i == j && on_boundary(i, lines))
            {
                for (int k = 0; k < n_L; k++)
                    if (i != k && line_has_vertex(lines[k], i) && on_boundary(k, lines))
                    {
                        point p1 = points[lines[k].i1];
                        point p2 = points[lines[k].i2];

                        B[k + n_v * i] -= h * euclidean_distance(p1, p2) / 6.0;
                    }

                B[j + n_v * i] -= h * length_of_adjacent_lines(points, lines, i) / 3.0;
            }

            B[j + n_v * i] /= H[i];
        }

    return B;
}

vector<double> assemble_vector(const vector<point> &points, const vector<line> &lines, const vector<double> &H, const double &sigma, const double &T_fluid)
{
    int n_v = points.size();
    vector<double> S(n_v, 0.0);

    for (int i = 0; i < n_v; i++)
    {
        if (points[i].y <= 0.3)
            S[i] = 1.0 / 3.0 * sigma;
        if (on_boundary(i, lines))
            S[i] += 1.0 / 2.0 * T_fluid * length_of_adjacent_lines(points, lines, i) / H[i];
    }

    return S;
}

vector<double> initial_value(const vector<point> &points, const vector<line> &lines, const vector<double> &S)
{
    int n_v = points.size();
    vector<double> U(n_v);

    for (int i = 0; i < n_v; i++)
        U[i] = 25.0;

    return U;
}

int main()
{
    // read data
    vector<point> points;
    vector<triangle> triangles;
    vector<line> lines;
    read_mesh("../cad/heatsink/heatsink_8_0.3.msh", points, triangles, lines);
    // read_mesh("simple.msh", points, triangles, lines);

    cout << "Number of points:    " << points.size() << endl;
    cout << "Number of triangles: " << triangles.size() << endl;

    int t_steps = 100000;
    double delta_t = 10.0 / (t_steps - 1.0);
    const string path = "./out/";
    int file_count = 1000;

    double sigma = 100.0;
    double h = 0.1;
    double T_fluid = 25.0;

    int n_v = points.size();

    vector<double> H = compute_H(points, triangles, lines);

    vector<double> B = assemble_matrix(points, triangles, lines, H, h, T_fluid);
    vector<double> S = assemble_vector(points, lines, H, sigma, T_fluid);

    vector<double> prev_U = initial_value(points, lines, S);
    vector<double> new_U(n_v);

    write_vtk(path + "out-t0.vtk", points, triangles, lines, prev_U);

    for (int i = 0; i < n_v * n_v; i++)
    {
        B[i] *= delta_t;
        if (i % (n_v + 1) == 0)
            B[i] += 1;
    }

    for (int k = 0; k < t_steps; k++)
    {
        for (int i = 0; i < n_v; i++)
        {
            double sum = 0.0;

            for (int j = 0; j < n_v; j++)
                sum += B[j + i * n_v] * prev_U[j];

            new_U[i] = sum + delta_t * S[i];
        }

        if (k % file_count == 0)
        {
            string filename = "out-t" + to_string(k + 1) + ".vtk";
            write_vtk(path + filename, points, triangles, lines, new_U);
        }

        prev_U.swap(new_U);
    }
}
