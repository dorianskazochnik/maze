using namespace std;

#include <iostream>
#include <fstream>
#include <iterator>
#include <random>
#include <utility>
#include <queue>
#include <map>
#include <vector>
#include <stack>
#include <exception>

class Maze : public multimap<tuple<int, int, int>, tuple<int, int, int> >
{
protected:
    int rows;
    int columns;
    int levels;
    multimap<tuple<int, int, int>, tuple<int, int, int> > maze;
    tuple<int, int, int> start;
    tuple<int, int, int> finish;
public:
    Maze(int r = 0, int c = 0, int l = 0) : multimap()
    {
        rows = r;
        columns = c;
        levels = l;
        start = make_tuple(0, 0, 0);
        finish = make_tuple(0, 0, 0);
    }
    Maze(const Maze& m)
    {
        maze = m.maze;
        rows = m.rows;
        columns = m.columns;
        levels = m.levels;
        start = m.start;
        finish = m.finish;
    }

    inline bool choose_orientation(int width, int height)
    {
        bool k = width == height;
        switch (k)
        {
            case true:
            {
                srand(time(0));
                return rand() % (2) == 0;
            }
        }
        return width < height;
    }

    inline void generate_3d_maze(vector<vector<vector<char> > >& v)
    {
        int current_level = 0;
        for (vector<vector<vector<char> > >::iterator it = v.begin(); it != v.end(); ++it, current_level++)
        {
            time_t* t = new time_t(current_level);
            srand(time(t));
            *it = generate();
            //генерация входа и выхода
            switch (current_level)
            {
                case 0:
                {
                    start = make_tuple((rand() % (levels * 2)) / 2, (1 + rand() % (rows * 2)) / 2, (1 + rand() % (columns * 2)) / 2);
                    finish = make_tuple(((rand() % (levels * 2)) % (levels * 2)) / 2, ((1 + rand() % (columns * 2)) % (rows * 2)) / 2, ((1 + rand() % (rows * 2)) % (columns * 2)) / 2);
                    break;
                }
            }
            delete t;
        }
    }

    //генерация уровня
    inline vector<vector<char> > generate()
    {
        //создание и заполнение сетки
        vector<vector<char> > v(rows * 2 + 1, vector<char>(columns * 2 + 1));
        for (vector<vector<char> >::iterator it = v.begin(); it != v.end(); ++it)
            for (vector<char>::iterator jt = it->begin(); jt != it->end(); ++jt)
                *jt = '1';
        //генерация внутренних стен
        r_division(v, make_pair(0, 0), make_pair(rows, columns), choose_orientation(rows, columns));
        //рамка
        int i = 0;
        for (vector<char>::iterator jt = v.front().begin(); jt != v.front().end(); ++jt)
            *jt = '#';
        for (vector<char>::iterator jt = v.back().begin(); jt != v.back().end(); ++jt)
            *jt = '#';
        for (vector<vector<char> >::iterator it = v.begin(); it != v.end(); ++it)
        {
            it->back() = '#';
            it->front() = '#';
        }
        return v;
    }

    //генератор уровня лабиринта в виде двумерного поля (вектора)
    inline void r_division(vector<vector<char> >& v, pair<int, int> coord, pair<int, int> hw, bool orientation)
    {
        if (hw.first < 2 || hw.second < 2)
            return;
        srand(time(0));
        pair<int, int> wall(coord.first + (orientation ? 0 : 1 + rand() % (hw.first - 1)), coord.second + (orientation ? 1 + rand() % (hw.second - 1) : 0));
        pair<int, int> passage(coord.first + (orientation ? 1 + rand() % (hw.first) : 0), coord.second + (orientation ? 0 : 1 + rand() % (hw.second)));
        pair<int, int> direction(orientation ? 1 : 0, orientation ? 0 : 1);
        int length = orientation ? hw.first : hw.second;
        pair<int, int> entry((1 + rand() % (rows * 2)) / 2, (1 + rand() % (columns * 2)) / 2);
        v[entry.first][entry.second] = 'x';
        pair<int, int> exit(((1 + rand() % (columns * 2)) % (rows * 2)) / 2, ((1 + rand() % (rows * 2)) % (columns * 2)) / 2);
        v[exit.first][exit.second] = '0';
        for (int i = 0; i < length; i++)
        {
            bool wp = wall != passage;
            switch (wp)
            {
                case true:
                {
                    switch (orientation)
                    {
                        case true:
                        {
                            v[wall.first * 2][wall.second * 2 + 1] = '#';
                            if (wall.first != 0)
                                v[wall.first * 2 - 1][wall.second * 2 + 1] = '#';
                            break;
                        }
                        default:
                        {
                            v[wall.first * 2 + 1][wall.second * 2] = '#';
                            if (wall.second != 0)
                                v[wall.first * 2 + 1][wall.second * 2 - 1] = '#';
                            break;
                        }
                    }
                    v[wall.first * 2 + 1][wall.second * 2 + 1] = '#';
                    break;
                }
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

    //вывод лабиринта 3d
    inline void display(vector<vector<vector<char> > >& v)
    {
        int current_level = 0;
        for (vector<vector<vector<char> > >::iterator it = v.begin(); it != v.end(); ++it, current_level++)
        {
            display(current_level, *it);
        }
    }

    //вывод лабиринта 2d
    inline void display(int current_level, vector<vector<char> >& v)
    {
        cout << "-level " << current_level << "- \n";
        for (vector<vector<char> >::iterator it = v.begin(); it != v.end(); ++it)
        {
            for (vector<char>::iterator jt = it->begin(); jt != it->end(); ++jt)
            {
                switch (*jt)
                {
                    case '1':
                    {
                        cout << ' ';
                        break;
                    }
                    default:
                    {
                        cout << *jt;
                        break;
                    }
                }
            }
            cout << '\n';
        }
    }

    //превратить из вектора векторов в дерево
    inline void construct_maze(vector<vector<vector<char> > >& v)
    {
        for (int i = 0; i < rows * 2 - 1; i++)
        {
            for (int j = 0; j < columns * 2 - 1; j++)
            {
                for (int k = 0; k < levels; k++)
                {
                    bool c = (i % 2 == 0 && j % 2 == 0);
                    bool u1 = c && (v[k][i][j] == '1' || v[k][i][j] == 'x' || v[k][i][j] == '0') && (v[k][i + 1][j] == '1' || v[k][i][j] == 'x' || v[k][i][j] == '0') && (v[k][i + 2][j] == '1' || v[k][i][j] == 'x' || v[k][i][j] == '0');
                    bool u2 = c && (v[k][i][j] == '1' || v[k][i][j] == 'x' || v[k][i][j] == '0') && (v[k][i][j + 1] == '1' || v[k][i][j] == 'x' || v[k][i][j] == '0') && (v[k][i][j + 2] == '1' || v[k][i][j] == 'x' || v[k][i][j] == '0');
                    bool u3 = c && (j >= 2) && (v[k][i][j] == '1' || v[k][i][j] == 'x' || v[k][i][j] == '0') && (v[k][i][j - 1] == '1' || v[k][i][j] == 'x' || v[k][i][j] == '0') && (v[k][i][j - 2] == '1' || v[k][i][j] == 'x' || v[k][i][j] == '0');
                    bool u4 = c && (i >= 2) && (v[k][i][j] == '1' || v[k][i][j] == 'x' || v[k][i][j] == '0') && (v[k][i - 1][j] == '1' || v[k][i][j] == 'x' || v[k][i][j] == '0') && (v[k][i - 2][j] == '1' || v[k][i][j] == 'x' || v[k][i][j] == '0');
                    bool u5 = v[k][i][j] == 'x';
                    bool u6 = v[k][i][j] == '0';
                    bool u51 = k != 0;
                    bool u61 = k < levels - 1;
                    switch (u1)
                    {
                        case true:
                        {
                            tuple<int, int, int> f(make_tuple(k, i / 2, j / 2));
                            tuple<int, int, int> s(make_tuple(k, i / 2 + 1, j / 2));
                            maze.insert(make_pair(f, s));
                            break;
                        }
                    }
                    switch (u2)
                    {
                        case true:
                        {
                            tuple<int, int, int> f(make_tuple(k, i / 2, j / 2));
                            tuple<int, int, int> s(make_tuple(k, i / 2, j / 2 + 1));
                            maze.insert(make_pair(f, s));
                            break;
                        }
                    }
                    switch (u3)
                    {
                        case true:
                        {
                            tuple<int, int, int> f(make_tuple(k, i / 2, j / 2));
                            tuple<int, int, int> s(make_tuple(k, i / 2, j / 2 - 1));
                            maze.insert(make_pair(f, s));
                            break;
                        }
                    }
                    switch (u4)
                    {
                        case true:
                        {
                            tuple<int, int, int> f(make_tuple(k, i / 2, j / 2));
                            tuple<int, int, int> s(make_tuple(k, i / 2 - 1, j / 2));
                            maze.insert(make_pair(f, s));
                            break;
                        }
                    }
                    switch (u5)
                    {
                        case true:
                        {
                            switch (u51)
                            {
                                case true:
                                {
                                    v[k - 1][i][j] = '0';
                                    tuple<int, int, int> f(make_tuple(k, i / 2, j / 2));
                                    tuple<int, int, int> s(make_tuple(k - 1, i / 2, j / 2));
                                    maze.insert(make_pair(f, s));
                                    maze.insert(make_pair(s, f));
                                    break;
                                }
                                default:
                                {
                                    v[k][i][j] = '1';
                                    break;
                                }
                            }
                        }
                    }
                    switch (u6)
                    {
                        case true:
                        {
                            switch (u61)
                            {
                                case true:
                                {
                                    v[k + 1][i][j] = 'x';
                                    tuple<int, int, int> f(make_tuple(k, i / 2, j / 2));
                                    tuple<int, int, int> s(make_tuple(k + 1, i / 2, j / 2));
                                    maze.insert(make_pair(f, s));
                                    maze.insert(make_pair(s, f));
                                    break;
                                }
                                default:
                                {
                                    v[k][i][j] = '1';
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        //добавить проверку на входы и выходы
        v[get<0>(start)][get<1>(start) * 2][get<2>(start) * 2] = 's';
        v[get<0>(finish)][get<1>(finish) * 2][get<2>(finish) * 2] = 'f';
    }

    inline void path(vector<vector<vector<char> > >& v, vector<tuple<int, int, int> >& used)
    {
        for (vector<tuple<int, int, int> >::iterator it = used.begin(); it != used.end(); ++it)
        {
            v[get<0>(*it)][get<1>(*it) * 2][get<2>(*it) * 2] = '*';
        }
    }

    //поиск в глубину
    inline bool DFS(vector<vector<vector<char> > >& v)
    {
        vector<vector<vector<int> > > used(levels, vector<vector<int> >(rows * 2 + 1, vector<int>(columns * 2 + 1, -1)));
        vector<tuple<int, int, int> > res;
        vector<vector<vector<tuple<int, int, int> > > > p(levels, vector<vector<tuple<int, int, int> > >(rows * 2 + 1, vector<tuple<int, int, int> >(columns * 2 + 1)));
        bool flag = false;
        stack<tuple<int, int, int> > s;
        s.push(start);
        p[get<0>(start)][get<1>(start)][get<2>(start)] = start;
        while (!s.empty())
        {
            tuple<int, int, int> current = s.top();
            s.pop();
            for (multimap<tuple<int, int, int>, tuple<int, int, int> >::iterator it = maze.equal_range(current).first; it != maze.equal_range(current).second; ++it)
            {
                switch (used[get<0>(it->second)][get<1>(it->second)][get<2>(it->second)])
                {
                    case -1:
                    {
                        s.push(it->second);
                        used[get<0>(it->second)][get<1>(it->second)][get<2>(it->second)] = 0;
                        p[get<0>(it->second)][get<1>(it->second)][get<2>(it->second)] = current;
                        if (it->second == finish)
                            flag = true;
                        break;
                    }
                }
            }
            while (current != start && flag)
            {
                res.push_back(current);
                current = p[get<0>(current)][get<1>(current)][get<2>(current)];
                break;
            }
        }
        switch (flag)
        {
            case false:
            {
                cout << "no escape";
                return false;
            }
            default:
            {
                path(v, res);
                return true;
            }
        }
    }
    
    //поиск в ширину
    inline bool BFS(vector<vector<vector<char> > >& v)
    {
        bool flag = false;
        queue<tuple<int, int, int> > q;
        q.push(start);
        vector<vector<vector<int> > > used(levels, vector<vector<int> >(rows * 2 + 1, vector<int>(columns * 2 + 1, -1)));
        vector<tuple<int, int, int> > res;
        vector<vector<vector<tuple<int, int, int> > > > p(levels, vector<vector<tuple<int, int, int> > >(rows * 2 + 1, vector<tuple<int, int, int> >(columns * 2 + 1)));
        used[get<0>(start)][get<1>(start)][get<2>(start)] = 0;
        while (!q.empty() && !flag)
        {
            tuple<int, int, int> current = q.front();
            q.pop();
            for (multimap<tuple<int, int, int>, tuple<int, int, int> >::iterator it = maze.equal_range(current).first; it != maze.equal_range(current).second && !flag; ++it)
            {
                tuple<int, int, int> neighbor = it->second;
                switch (used[get<0>(neighbor)][get<1>(neighbor)][get<2>(neighbor)])
                {
                    case -1:
                    {
                        q.push(neighbor);
                        used[get<0>(neighbor)][get<1>(neighbor)][get<2>(neighbor)] = used[get<0>(current)][get<1>(current)][get<2>(current)] + 1;
                        p[get<0>(neighbor)][get<1>(neighbor)][get<2>(neighbor)] = current;
                        if (neighbor == finish)
                            flag = true;
                        break;
                    }
                }
            }
            while (current != start && flag)
            {
                res.push_back(current);
                current = p[get<0>(current)][get<1>(current)][get<2>(current)];
            }
        }
        switch (flag)
        {
            case false:
            {
                cout << "no escape";
                return false;
            }
            default:
            {
                path(v, res);
                return true;
            }
        }
    }
};

inline ifstream& operator >> (ifstream& stream, vector<vector<vector<char> > >& v)
{
    for (vector<vector<vector<char> > >::iterator it = v.begin(); it != v.end(); ++it)
        for (vector<vector<char> >::iterator jt = it->begin(); jt != it->end(); ++jt)
            for (vector<char>::iterator kt = jt->begin(); kt != jt->end(); ++kt)
                stream >> *kt;
    return stream;
}

inline ofstream& operator << (ofstream& stream, const vector<vector<vector<char> > >& v)
{
    for (vector<vector<vector<char> > >::const_iterator it = v.begin(); it != v.end(); ++it)
        for (vector<vector<char> >::const_iterator jt = it->begin(); jt != it->end(); ++jt)
            for (vector<char>::const_iterator kt = jt->begin(); kt != jt->end(); ++kt)
                stream << *kt;
    return stream;
}

int main()
{
    try
    {
        cout << "Welcome to the maze. Generate (g) it or open (o) from already existing maze.txt? ( g / o ) \n";
        char og;
        cin >> og;
        cout << "If you want to generate, set number of rows, columns and levels. (int, int, int) If you chose open from file, tou can set any 3 numbers. \n";
        int r = 0, c = 0, l = 0;
        cin >> r >> c >> l;
        Maze maze(r, c, l);
        //заготовка для поля
        vector<vector<vector<char> > > v(l, vector<vector<char> >(r * 2 + 1, vector<char>(c * 2 + 1, '1')));
        switch (og)
        {
            case 'o':
            {
                ifstream in("maze.txt");
                in >> r >> c >> l;
                in >> v;
                break;
            }
            case 'g':
            {
                maze.generate_3d_maze(v);
                break;
            }
            default:
            {
                throw runtime_error("Wrong input");
            }
        }
        maze.construct_maze(v);
        cout << "This is your maze. \n\n";
        maze.display(v);
        cout << "\n\n\n ------------ \n\n\n";
        cout << "Which method would you like to use to sove it? BFS or DFS or automatically chosen? ( 1 / 2 / 3 ) \n";
        char bda;
        cin >> bda;
        switch (bda)
        {
            case '1':
            {
                bool bfs = maze.BFS(v);
                if (bfs)
                    maze.display(v);
                break;
            }
            case '2':
            {
                bool dfs = maze.DFS(v);
                if (dfs)
                    maze.display(v);
                break;
            }
            case '3':
            {
                bool dfs = maze.DFS(v);
                if (!dfs)
                {
                    bool bfs = maze.BFS(v);
                    cout << '\n';
                    if (bfs)
                        maze.display(v);
                }
                else
                {
                    maze.display(v);
                }
                break;
            }
            default:
            {
                throw runtime_error("Wrong input");
            }
        }
        cout << "\n\n\n ------------ \n\n\n";
        //сохраняет с решением//
        cout << "Would you like to save this maze in maze.txt (all data already existing there will be lost)? ( y / n ) \n";
        char yn;
        cin >> yn;
        switch (yn)
        {
            case 'y' :
            {
                ofstream out("maze.txt");
                out << r << " " << c << " " << l << "\n";
                out << v;
                cout << "Data is saved. \n";
                cout << "Thank you for using this maze. \n";
                break;
            }
            case 'n' :
            {
                cout << "Thank you for using this maze. \n";
                break;
            }
            default:
            {
                throw runtime_error("Wrong input");
            }
        }
        int k;
        cin >> k;
    }
    catch (const exception& X)
    {
        cout << X.what();
    }
}