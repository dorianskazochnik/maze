using namespace std;

#include <iostream>
#include <map>
#include <set>
#include <iterator>
#include <random>
#include <utility>
#include <windows.h>

class Maze : public map < pair<int, int>, set<pair<int, int> > >
{
protected:
    map < pair<int, int>, set<pair<int, int> > > maze;
    map < tuple<int, int, int>, set<tuple<int, int, int> > > maze_3d;
    int rows;
    int columns;
    int levels;
public:
    Maze(int r = 0, int c = 0) : map()
    {
        rows = r;
        columns = c;
    }
    Maze(const Maze& m) : map(maze)
    {
        rows = m.rows;
        columns = m.columns;
    }
    ~Maze()
    {
        for (map < pair<int, int>, set<pair<int, int> > >::iterator it = maze.begin(); it != maze.end(); ++it)
            it->second.clear();
        maze.clear();
    }

    bool choose_orientation(int width, int height)
    {
        if (width == height)
        {
            srand(time(0));
            return rand() % (2) == 0;
        }
        return width < height;
    }

    //генератор лабиринта 2d
    void r_division(pair<int, int> coord, pair<int, int> hw, bool orientation)
    {
        if (hw.first < 2 || hw.second < 2)
            return;
        srand(time(0));
        pair<int, int> wall(coord.first + (orientation ? 0 : 1 + rand() % (hw.first - 1)), coord.second + (orientation ? 1 + rand() % (hw.second - 1) : 0));
        pair<int, int> passage(coord.first + (orientation ? 1 + rand() % (hw.first) : 0), coord.second + (orientation ? 0 : 1 + rand() % (hw.second)));
        pair<int, int> direction(orientation ? 1 : 0, orientation ? 0 : 1);
        int length = orientation ? hw.first : hw.second;
        for (int i = 0; i < length; i++)
        {
            if (wall.first != passage.first || wall.second != passage.second)
            {
                map < pair<int, int>, set<pair<int, int> > >::iterator it = maze.find(wall);
                if (it != maze.end())
                {
                    if (orientation)
                    {
                        it->second.insert(make_pair(wall.first, wall.second + 1));
                    }
                    else
                    {
                        it->second.insert(make_pair(wall.first + 1, wall.second));
                    }
                }
                else
                {
                    maze[wall] = { make_pair(wall.first + 1, wall.second) };
                }
            }
            wall = make_pair(wall.first | direction.first, wall.second | direction.second);
        }
        pair<int, int> n = coord;
        int width = orientation ? hw.first : wall.first - n.first;
        int height = orientation ? wall.second - n.second : hw.second;
        r_division(n, make_pair(width, height), choose_orientation(width, height));

        n = make_pair(orientation ? coord.first : wall.first, orientation ? wall.second : coord.second);
        width = orientation ? hw.first : coord.first + hw.first - wall.first;
        height = orientation ? coord.second + hw.second - wall.second : hw.second;
        r_division(n, make_pair(width, height), choose_orientation(width, height));
    }

    void generate_3d()
    {
        for (int i = 0; i < levels; i++)
        {
            //
        }
    }

    //вывод лабиринта
    void display()
    {
        vector<vector<char> > v(rows * 2 + 1, vector<char>(columns * 2 + 1));
        for (int i = 0; i < rows * 2 + 1; i++)
        {
            for (int j = 0; j < columns * 2 + 1; j++)
            {
                v[i][j] = ' ';
            }
        }
        for (map < pair<int, int>, set<pair<int, int> > >::iterator it = maze.begin(); it != maze.end(); ++it)
        {
            for (set<pair<int, int> >::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
            {
                if (it->first.first + 1 == jt->first)
                {
                    v[it->first.first * 2 + 1][it->first.second * 2] = '#';
                    if (it->first.second != 0)
                        v[it->first.first * 2 + 1][it->first.second * 2 - 1] = '#';
                }
                else if (it->first.second + 1 == jt->second)
                {
                    v[it->first.first * 2][it->first.second * 2 + 1] = '#';
                    if (it->first.first != 0)
                        v[it->first.first * 2 - 1][it->first.second * 2 + 1] = '#';
                }
                v[it->first.first * 2 + 1][it->first.second * 2 + 1] = '#';
            }
        }
        cout << '\n';
        for (int i = 0; i < rows * 2 - 2; i++)
        {
            cout << '#';
            for (int j = 0; j < columns * 2 - 2; j++)
            {
                cout << v[i][j];
            }
            cout << "# \n";
        }
    }

    void input_table()
    {
        //
    }

    void input_coords()
    {
        //
    }

    void a_star()
    {
        //
    }
};

int main()
{
    Maze maze(15, 15);
    maze.r_division(make_pair(0, 0), make_pair(15, 15), maze.choose_orientation(15, 15));
    cout << "\n\n\n ------------ \n\n\n";
    maze.display();
    int k;
    cin >> k;
}
