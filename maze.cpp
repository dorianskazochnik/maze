using namespace std;

#include <iostream>
#include <map>
#include <iterator>
#include <random>
#include <utility>
#include <windows.h>

class Maze : public multimap<pair<int, int>, int>
{
protected:
    multimap<pair<int, int>, int> maze;
    int rows;
    int columns;
public:
    Maze(int r = 0, int c = 0): multimap()
    {
        rows = r;
        columns = c;
    }
    Maze(const Maze& m) : multimap(maze)
    {
        rows = m.rows;
        columns = m.columns;
    }
    ~Maze()
    {
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
                if (maze.size() != 0)
                {
                    multimap< pair<int, int>, int>::iterator it = maze.find(wall);
                    if (it != maze.end())
                    {
                        it->second = int(it->second | !(orientation));
                    }
                    else
                    {
                        maze.insert(make_pair(wall, !(orientation)));
                    }
                }
                else if (maze.size() == 0)
                {
                    maze.insert(make_pair(wall, !(orientation)));
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
        for (multimap<pair<int, int>, int>::iterator it = maze.begin(); it != maze.end(); ++it)
        {
            for (auto jt = maze.equal_range(it->first).first; jt != maze.equal_range(it->first).second; ++jt)
            {
                if (it->second == 0)
                {
                    v[it->first.first * 2][it->first.second * 2 + 1] = '#';
                    if (it->first.first != 0)
                        v[it->first.first * 2 - 1][it->first.second * 2 + 1] = '#';
                }
                else
                {
                    v[it->first.first * 2 + 1][it->first.second * 2] = '#';
                    if (it->first.second != 0)
                        v[it->first.first * 2 + 1][it->first.second * 2 - 1] = '#';
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
