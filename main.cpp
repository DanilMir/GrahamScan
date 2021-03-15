#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <time.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

/*структура точки*/
struct Point {
    double x, y;
};

/*компаратор для сортировки по углу*/
bool cmp(Point a, Point b) {
    double cTemp = a.x * b.y - a.y * b.x;
    return cTemp > 0 || cTemp == 0 && a.x * a.x + a.y * a.y < b.x * b.x + b.y * b.y;
}

/*компаратор для определения левого поворота, использауется
обобщение векторного произведения на двумерное пространство*/
bool IsLeft(Point &a, Point &b, Point &c) {
    Point u = {b.x - a.x, b.y - a.y};
    Point v = {c.x - b.x, c.y - b.y};
    return (u.x * v.y - u.y * v.x) >= 0;
}

/*сокращенный вариант компаратора, но менее понятный*/
bool IsLeft2(Point &a, Point &b, Point &c) {
    return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y) >= 0;
}

template<typename T>
void insertionSort(vector<T> & data, int n) {
    int i, j;
    T tmp;

    for (i = 1; i < n; i++) {
        j = i;
        tmp = data[i];
        while (j > 0 && cmp(tmp, data[j - 1])) {
            data[j] = data[j - 1];
            j--;
        }
        data[j] = tmp;
    }
}

vector<Point> graham(vector<Point> &p, int & iteration) {
    int n = p.size();
    if(n <= 1)
        return p;
    auto p0 = p[0];

    /* Находим самую самую левую нижнюю точку */
    for (int i = 1; i < n; ++i) {
        iteration++;
        if (p[i].x < p0.x || (p[i].x == p0.x && p[i].y < p0.y)) {
            p0 = p[i];
        }
    }

    /*сделаем точку p0 началом координат*/
    for (auto &i : p) {
        iteration++;
        i.x -= p0.x;
        i.y -= p0.y;
    }

    /*сортируем точки относительно угла*/

    insertionSort(p, n);

    /*создаем финальный вектор значений вершин оболочки*/
    vector<Point> st;
    st.push_back(p[0]);
    st.push_back(p[1]);


    for (int i = 2; i < n; i++) {
        /*удаляем последнюю точку МВО пока она образует невыпуклость*/
        while (!IsLeft(st[st.size() - 2], st[st.size() - 1], p[i])) {
            st.pop_back();
            iteration++;
        }
        st.push_back(p[i]);
    }


    /*возращаем в обычное состояние*/
    for (auto &i : st) {
        iteration++;
        i.x += p0.x;
        i.y += p0.y;
    }
    return st;
}


int main() {
    string inputPath = R"(C:\Users\Danil Mirgayazov\Desktop\temp.txt)"; //файл считывания
    string outputPath = R"(C:\Users\Danil Mirgayazov\Desktop\output.txt)"; //файл вывода
    ifstream input(inputPath);
    ofstream output(outputPath);

    int count;
    input >> count;
    for (int i = 0; i < count; ++i) {
        int n;
        int iteration = 0;
        input >> n;
        vector<Point> p {};
        for (int j = 0; j < n; ++j) {
            double x, y;
            input >> x >> y;
            Point tt = {x, y};
            p.push_back(tt);
        }
        clock_t start = clock();
        auto temp = graham(p, iteration);
        int ms = (clock() - start) / (double) (CLOCKS_PER_SEC / 1000);
        output << n << ' ' << ms << ' ' << iteration << '\n';
    }
}