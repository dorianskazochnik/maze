using namespace std;

#include <iostream>
#include <iterator>
#include <random>
#include <utility>
#include <queue>
#include <map>
#include <vector>
#include <stack>
#include <windows.h>

class Maze : public multimap<pair<int, int>, pair<int, int> >
{
protected:
    multimap<pair<int, int>, pair<int, int> > maze;
    pair<int, int> entry;
    pair<int, int> exit;
    int rows;
    int columns;
    int levels;
public:
    Maze(int r = 0, int c = 0, int l = 0) : multimap()
    {
        rows = r;
        columns = c;
        levels = l;
    }
    Maze(const Maze& m)
    {
        maze = m.maze;
        rows = m.rows;
        columns = m.columns;
        levels = m.levels;
    }
    ~Maze()
    {
        maze.~multimap();
    }

    pair<int, int> getEntry() { return entry; }
    pair<int, int> getExit() { return exit; }

    bool choose_orientation(int width, int height)
    {
        if (width == height)
        {
            srand(time(0));
            return rand() % (2) == 0;
        }
        return width < height;
    }

    vector<vector<char> > generate()
    {
        //создание и заполнение сетки
        vector<vector<char> > v(rows * 2 + 1, vector<char>(columns * 2 + 1));
        for (int i = 0; i < rows * 2 + 1; i++)
        {
            for (int j = 0; j < columns  * 2 + 1; j++)
            {
                v[i][j] = ' ';
            }
        }
        //генерация внутренних стен
        r_division(v, make_pair(0, 0), make_pair(rows, columns), choose_orientation(rows, columns));
        srand(time(0));
        //генерация входа и выхода
        entry = make_pair((1 + rand() % (rows * 2)) / 2, (1 + rand() % (columns * 2)) / 2);
        exit = make_pair(((1 + rand() % (columns * 2)) % (rows * 2)) / 2, ((1 + rand() % (rows * 2)) % (columns * 2)) / 2);
        for (int i = 0; i < rows * 2 + 1; i++)
        {
            if (i == 0 || i == rows * 2)
                for (int j = 0; j < columns * 2 + 1; j++)
                    v[i][j] = '#';
            else
            {
                v[i][0] = '#';
                v[i][columns * 2] = '#';
            }
        }
        v[entry.first * 2][entry.second * 2] = 'x';
        v[exit.first * 2][exit.second * 2] = '!';
        //вывод
        display(v);

        //создание очереди - основной структуры лабиринта
        construct_maze(v);
        //output();
        return v;
    }

    //генератор уровня лабиринта в виде двумерного поля (вектора)
    void r_division(vector<vector<char> >& v, pair<int, int> coord, pair<int, int> hw, bool orientation)
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
                if (orientation)
                {
                    v[wall.first * 2][wall.second * 2 + 1] = '#';
                    if (wall.first != 0)
                        v[wall.first * 2 - 1][wall.second * 2 + 1] = '#';
                }
                else
                {
                    v[wall.first * 2 + 1][wall.second * 2] = '#';
                    if (wall.second != 0)
                        v[wall.first * 2 + 1][wall.second * 2 - 1] = '#';
                }
                v[wall.first * 2 + 1][wall.second * 2 + 1] = '#';
            }
            wall = make_pair(wall.first | direction.first, wall.second | direction.second);
        }
        pair<int, int> n = coord;
        int width = orientation ? hw.first : wall.first - n.first;
        int height = orientation ? wall.second - n.second : hw.second;
        r_division(v, n, make_pair(width, height), choose_orientation(width, height));

        n = make_pair(orientation ? coord.first : wall.first, orientation ? wall.second : coord.second);
        width = orientation ? hw.first : coord.first + hw.first - wall.first;
        height = orientation ? coord.second + hw.second - wall.second : hw.second;
        r_division(v, n, make_pair(width, height), choose_orientation(width, height));
    }

    //вывод лабиринта 2d
    void display(const vector<vector<char> >& v)
    {
        for (int i = 0; i < rows * 2 + 1; i++)
        {
            for (int j = 0; j < columns * 2 + 1; j++)
            {
                cout << v[i][j];
            }
            cout << '\n';
        }
    }

    //превратить из вектора векторов в дерево
    void construct_maze(vector<vector<char> >& v)
    {
        for (int i = 0; i < rows * 2 - 1; i++)
        {
            for (int j = 0; j < columns * 2 - 1; j++)
            {
                if (v[i][j] == 'x')
                    v[i][j] = ' ';
                else if (v[i][j] == '!')
                    v[i][j] = ' ';
                if (v[i][j] == ' ' && v[i + 1][j] == ' ' && v[i + 2][j] == ' ' && i % 2 == 0 && j % 2 == 0)
                {
                    pair<int, int> f(make_pair(i / 2, j / 2));
                    pair<int, int> s(make_pair(i / 2 + 1, j / 2));
                    maze.insert(make_pair(f, s));
                }
                if (v[i][j] == ' ' && v[i][j + 1] == ' ' && v[i][j + 2] == ' ' && i % 2 == 0 && j % 2 == 0)
                {
                    pair<int, int> f(make_pair(i / 2, j / 2));
                    pair<int, int> s(make_pair(i / 2, j / 2 + 1));
                    maze.insert(make_pair(f, s));
                }
                if (j >= 2)
                    if (v[i][j] == ' ' && v[i][j - 1] == ' ' && v[i][j - 2] == ' ' && i % 2 == 0 && j % 2 == 0)
                    {
                        pair<int, int> f(make_pair(i / 2, j / 2));
                        pair<int, int> s(make_pair(i / 2, j / 2 - 1));
                        maze.insert(make_pair(f, s));
                    }
                if (i >= 2)
                    if (v[i][j] == ' ' && v[i - 1][j] == ' ' && v[i - 2][j] == ' ' && i % 2 == 0 && j % 2 == 0)
                    {
                        pair<int, int> f(make_pair(i / 2, j / 2));
                        pair<int, int> s(make_pair(i / 2 - 1, j / 2));
                        maze.insert(make_pair(f, s));
                    }
            }
        }
        v[entry.first * 2][entry.second * 2] = 'x';
        v[exit.first * 2][exit.second * 2] = '!';
    }

    void path(vector<vector<char> >& v, vector<pair<int, int> >& used)
    {
        for (vector<pair<int, int> >::iterator it = used.begin(); it != used.end(); ++it)
        {
            v[it->first * 2][it->second * 2] = '*';
        }
        v[entry.first * 2][entry.second * 2] = 'x';
        v[exit.first * 2][exit.second * 2] = '!';
    }

    //вывод дерева в виде ребер
    void output()
    {
        for (multimap<pair<int, int>, pair<int, int> >::iterator it = maze.begin(); it != maze.end(); ++it)
        {
            cout << "(" << it->first.first << "; " << it->first.second << "), (" << it->second.first << it->second.second << "); \n";
        }
    }

    //вспомогательная функция для поиск в глубину
    void DFS(vector<vector<char> >& v)
    {
        vector<vector<int> > used(rows * 2 + 1, vector<int>(columns * 2 + 1, -1));
        vector<pair<int, int> > res;
        vector<vector<pair<int, int> > > p(rows * 2 + 1, vector<pair<int, int> >(columns * 2 + 1));
        bool flag = DFS(used, entry, p);
        if (!flag)
            cout << "no escape";
        pair<int, int> current = exit;
        while (current != entry && flag)
        {
            res.push_back(current);
            current = p[current.first][current.second];
        }
        path(v, res);
        cout << "\n\nentry = (" << entry.first << "; " << entry.second << "), exit = (" << exit.first << "; " << exit.second << ");\n";
    }

    //рекурсивный поиск в глубину
    bool DFS(vector<vector<int> >& used, pair<int, int> current, vector<vector<pair<int, int> > >& p)
    {
        if (used[current.first][current.second] == 0)
            return false;
        if (current == exit)
            return true;
        used[current.first][current.second] = 0;
        for (multimap<pair<int, int>, pair<int, int> >::iterator it = maze.equal_range(current).first; it != maze.equal_range(current).second; ++it)
        {
            if (used[it->second.first][it->second.second] == -1)
            {
                DFS(used, it->second, p);
                if (current == exit)
                    return true;
            }
        }
        return false;
    }

    //поиск в ширину
    void BFS(vector<vector<char> >& v)
    {
        bool flag = false;
        queue<pair<int, int> > q;
        q.push(entry);
        vector<vector<int> > used (rows * 2 + 1, vector<int> (columns * 2 + 1, -1));
        vector<vector<pair<int, int> > > p (rows * 2 + 1, vector<pair<int, int> >(columns * 2 + 1));
        vector<pair<int, int> > res;
        used[entry.first][entry.second] = 0;
        while (!q.empty() && !flag)
        {
            pair<int, int> current = q.front();
            q.pop();
            for (multimap<pair<int, int>, pair<int, int> >::iterator it = maze.equal_range(current).first; it != maze.equal_range(current).second && !flag; ++it)
            {
                pair<int, int> neighbor = it->second;
                if (used[neighbor.first][neighbor.second] == -1)
                {
                    q.push(neighbor);
                    used[neighbor.first][neighbor.second] = used[current.first][current.second] + 1;
                    p[neighbor.first][neighbor.second] = current;
                    if (neighbor == exit)
                        flag = true;
                }
            }
            while (current != entry && flag)
            {
                res.push_back(current);
                current = p[current.first][current.second];
            }
        }
        if (!flag)
            cout << "no escape";
        path(v, res);
        cout << "\n\nentry = (" << entry.first << "; " << entry.second << "), exit = (" << exit.first << "; " << exit.second << ");\n";
    }
};

int main()
{
    Maze maze(15, 15);
    vector<vector<char> > v = maze.generate();
    cout << "\n\n\n ------------ \n\n\n";
    maze.BFS(v);
    maze.display(v);
    maze.DFS(v);
    maze.display(v);
    int k;
    cin >> k;
}
