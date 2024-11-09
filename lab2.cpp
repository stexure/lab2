#include<iostream>
#include <cassert>

template <typename T>
class Grid final {
public:
using value_type = T;
using size_type = unsigned;
private:
T * data;
size_type y_size, x_size;
bool construction_flag=1;       //Если мы динамически выделяем память в конструкторе, тогда и только тогда мы должны освободить её в деструкторе.
public:
Grid(T *data, size_type y_size, size_type x_size):
             data(data), y_size(y_size), x_size(x_size),construction_flag(0) { }
/*
Grid(Grid<T> const &) = delete;
Grid(Grid<T>&&) = delete;
Grid<T>& operator=(Grid<T>&) = delete;
Grid<T>& operator=(Grid<T>&&) = delete;
*/
    Grid(Grid<T> &G): data(new T[G.get_x_size()*G.get_y_size()]),y_size(G.get_y_size()),x_size(G.get_x_size()) {
        for ( auto x = 0u;  x != x_size; ++x)
            for(auto y=0u; y!= y_size; ++y)
                data[y*x_size+x] = G[y][x];
    }

    Grid<T>& operator=(Grid<T>& G) {
        x_size = G.get_x_size();
        y_size = G.get_y_size();
        delete [] data;
        data = (new T[G.get_x_size()*G.get_y_size()]);
        for ( auto x = 0u;  x != x_size; ++x)
            for(auto y=0u; y!= y_size; ++y)
                data[y*x_size+x] = G[y][x];
        return *this;
    }

    Grid(Grid<T> &&G): data(new T[G.get_x_size()*G.get_y_size()]),y_size(G.get_y_size()),x_size(G.get_x_size()) {
        for ( auto x = 0u;  x != x_size; ++x)
            for(auto y=0u; y!= y_size; ++y)
                data[y*x_size+x] = G[y][x];
    }

    Grid<T>& operator=(Grid<T>&& G) {
        x_size = G.get_x_size();
        y_size = G.get_y_size();
        delete [] data;
        data = (new T[G.get_x_size()*G.get_y_size()]);
        for ( auto x = 0u;  x != x_size; ++x)
            for(auto y=0u; y!= y_size; ++y)
                data[y*x_size+x] = G[y][x];
        return *this;
    }

Grid(T const &t):data(new T[1]{t}),y_size(1),x_size(1){}//1

Grid(size_type y_size,size_type x_size): y_size(y_size),x_size(x_size),data(new T [x_size * y_size]){}//2

Grid(size_type y_size,size_type x_size, T const &t) :data(new T [x_size * y_size]{}), y_size(y_size),x_size(x_size)  //3
                {
                    *this=t;
/*                  for ( auto it = data, end = data + x_size * y_size;  it != end; ++it)
                     *it = t;
*/
                 }

T operator()(size_type y_idx, size_type x_idx) const {
return data[y_idx * x_size + x_idx];
}

T& operator()(size_type y_idx, size_type x_idx) {
return data[y_idx * x_size + x_idx];
}

T* operator[](size_type y_idx){
    return (data + y_idx * x_size); //Возвращаю указатель на начало строчки, а вторая индексация просто массива.
}

const T* operator[](size_type y_idx) const { //для константного объекта
    return (data + y_idx * x_size);
}

Grid<T>& operator=(T const &t) {
for ( auto it = data, end = data + x_size * y_size;  it != end; ++it)
         *it = t;
return *this;
}

size_type get_y_size() const { return y_size; }
size_type get_x_size() const { return x_size; }

~Grid(){if(construction_flag)delete[] data;}
};


int main() {
Grid<float> g1(3, 2, 0.0f);
Grid<float> g(g1);
assert(3 == g.get_y_size());
assert(2 == g.get_x_size());

using  gsize_t = Grid<float>::size_type;

for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
assert(0.0f == g[y_idx][x_idx]);

for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
g[y_idx][x_idx] = 1.0f;

for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
assert(1.0f == g(y_idx, x_idx));

return 0;

}
