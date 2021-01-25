#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct point
{
    double x, y;

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
            {
                fs.get(c);
            }
        }
    }
    cout << endl;
}

void write_vtk(string filename, vector<point> &points, vector<triangle> &triangles, vector<line> &lines)
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
    {
        fs << points[i].x << " " << points[i].y << " " << 0.0 << endl;
    }
    fs << endl;

    fs << "POLYGONS " << n_triangles << " " << 4 * n_triangles << endl;
    for (int i = 0; i < n_triangles; i++)
    {
        fs << 3 << " " << triangles[i].i1 << " " << triangles[i].i2 << " " << triangles[i].i3 << endl;
    }
    fs << endl;

    fs << "POINT_DATA " << n_points << endl;
    fs << "SCALARS value double 1" << endl;
    fs << "LOOKUP_TABLE default" << endl;
    for (int i = 0; i < n_points; i++)
    {
        fs << i << endl;
    }
}

int main(int argc, char *argv[])
{
    // read data
    vector<point> points;
    vector<triangle> triangles;
    vector<line> lines;
    read_mesh("simple.msh", points, triangles, lines);

    cout << "Number of points:    " << points.size() << endl;
    cout << "Number of triangles: " << triangles.size() << endl;

    write_vtk("simple.vtk", points, triangles, lines);
}
