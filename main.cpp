#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*структура точки*/
struct Point {
    double x, y;
    void print() {
        cout << x << ' ' << y << '\n';
    }
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


vector<Point> graham(vector<Point> &p) {
    int n = p.size();
    auto p0 = p[0];

    /* Находим самую самую левую нижнюю точку */
    for (int i = 1; i < n; ++i) {
        if (p[i].x < p0.x || (p[i].x == p0.x && p[i].y < p0.y)) {
            p0 = p[i];
        }
    }

    /*сделаем точку p0 началом координат*/
    for (auto &i : p) {
        i.x -= p0.x;
        i.y -= p0.y;
    }

    /*сортируем точки относительно угла*/
    sort(p.begin(), p.end(), cmp);


    /*создаем финальный вектор значений вершин оболочки*/
    vector<Point> st;
    st.push_back(p[0]);
    st.push_back(p[1]);


    for (int i = 2; i < n; i++) {
        /*удаляем последнюю точку МВО пока она образует невыпуклость*/
        while (!IsLeft(st[st.size() - 2], st[st.size() - 1], p[i])) {
            st.pop_back();
        }
        st.push_back(p[i]);
    }


    /*возращаем в обычное состояние*/
    for (auto &i : st) {
        i.x += p0.x;
        i.y += p0.y;
    }
    return st;
}


int main() {

}