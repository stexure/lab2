#include<iostream>
#include <cassert>
#include <type_traits>
template <unsigned N, typename T>
class Grid final {
public:
using value_type = T;
using size_type = unsigned;
private:
    Grid <N-1,T> *data;
    size_type gsize;
//    bool construction_flag=1;       //Если мы динамически выделяем память в конструкторе, тогда и только тогда мы должны освободить её в деструкторе.
public:


    //Grid(Grid<N,T> const &) = delete;
    //Grid(Grid<N,T>&&) = delete;
    //Grid<N,T>& operator=(Grid<N,T>&) = delete;
    //Grid<N,T>& operator=(Grid<N,T>&&) = delete;
    Grid():data(nullptr),gsize(0){}
    Grid(Grid<N,T> &G): data(new Grid<N-1,T>[G.get_gsize()]),gsize(G.get_gsize()) {
        for ( auto it = 0u;  it != gsize; ++it)
        data[it] = G[it];
    }

    Grid<N,T>& operator=(Grid<N,T>& G) {
        gsize = G.get_gsize();
        delete [] data;
        data=(new Grid<N-1,T>[gsize]);
        for ( auto it = 0u;  it != gsize; ++it)
        data[it] = G[it];
        return *this;
    }

    Grid(Grid<N,T>&& G): gsize(G.get_gsize()), data(new Grid<N-1,T>[gsize]) {
        for ( auto it = 0;  it != gsize; ++it)
        data[it] = G[it];
    }

    Grid<N,T>& operator=(Grid<N,T>&& G) {
        gsize = G.get_gsize();
        if(data) delete[] data;
        data=(new Grid<N-1,T>[gsize]);
        for ( auto it = 0u;  it != gsize; ++it)
        data[it] = G[it];
        return *this;
    }

    Grid(T const &t):data(new Grid <N-1,T>[1]{t}),gsize(1){}//1
    /*template<typename S,typename ... V>
    Grid(S gsize,V ... sizes) requires (std::is_same_v<S,size_type>): data(new Grid<N-1,T>[gsize]{sizes...}),gsize(gsize){}//2*/
    template<typename ... V>
    Grid(T const &t, size_type gsize,V ... sizes) :data{new Grid<N-1,T>[gsize]},gsize{gsize}  //3 Не оптимально, поскольку перезаписывание на каждой итерации.
                {                                           //Необходимо все элементы data проинициализировать (t,sizes ...)
                    Grid<N-1,T>* tempptr = new Grid<N-1,T>(t,sizes ...); //Создаю объект, чтобы копировать его во все элементы массива.
                    for (size_type it=0;it<gsize;it++){
                        data[it] = *tempptr;
                    }
                    delete tempptr;
                }



    Grid<N-1,T>& operator[](size_type N_idx){
        //std::cout<<N_idx<<' '<<gsize<<std::endl;
        return *(data + N_idx);
    }

    Grid<N-1,T> operator[](size_type N_idx) const { //для константного объекта
        return *(data + N_idx);
        //std::cout<<N_idx<<' '<<gsize<<std::endl;
    }
    template<typename ... V>
    T operator()(size_type idx,V ... idxs) const {
    return (*this)[idx](idxs ...);
    }
    template<typename ... V>
    T& operator()(size_type idx,V ... idxs) {
    return (*this)[idx](idxs ...);
    }
    Grid<N,T>& operator=(T const &t) {
    for ( auto it = data, end = data + gsize;  it != end; ++it)
            *it = t;
    return *this;
    }

    size_type get_gsize() const { return gsize; }
    ~Grid(){if(data) delete[] data;}
};


    //Специализация для одномерного случая:

template <typename T>
    class Grid<1u,T> final {
public:
using value_type = T;
using size_type = unsigned;
private:
T * data;
size_type gsize;
bool construction_flag=1;       //Если мы динамически выделяем память в конструкторе, тогда и только тогда мы должны освободить её в деструкторе.
public:
    Grid():data(nullptr),gsize(0){}
    Grid(T *data, size_type gsize):
             data(data), gsize(gsize),construction_flag(0) { }

//    Grid(Grid<1,T> const &) = delete;
//    Grid(Grid<1,T>&&) = delete;
    Grid(Grid<1,T> &G): gsize(G.get_gsize()), data(new T[gsize]) {
        for ( auto it = 0;  it != gsize; ++it)
        data[it] = G[it];
    }

    Grid<1,T>& operator=(Grid<1,T>& G) {
        gsize = G.get_gsize();
        delete [] data;
        data = new T[gsize];
        for ( auto it = 0u;  it != gsize; ++it)
        data[it] = G[it];
        return *this;
    }

    Grid(Grid<1,T>&& G): gsize(G.get_gsize()), data(new T[gsize]) {
        for ( auto it = 0;  it != gsize; ++it)
        data[it] = G[it];
    }

    Grid<1,T>& operator=(Grid<1,T>&& G) {
        gsize = G.get_gsize();
        delete [] data;
        data = new T[gsize];
        for ( auto it = 0;  it != gsize; ++it)
        data[it] = G[it];
        return *this;
    }

    Grid(T const &t):data(new T[1]{t}),gsize(1){}//1

    //Grid(size_type gsize): data(new T [gsize]),gsize(gsize){}//2

    Grid(T const &t, size_type gsize) :data(new T [gsize]),gsize(gsize)  //3
        {
            *this=t;
        }

    T& operator[](size_type idx){
        //std::cout<<idx<<' '<<gsize<<std::endl;
        return data[idx];
    }

    T operator[](size_type idx) const { //для константного объекта возвращаем копию
        //std::cout<<idx<<' '<<gsize<<std::endl;
        return data[idx] ;
    }

    T operator()(size_type idx) const {
    return data[idx];
    }

    T& operator()(size_type idx) {
    return data[idx];
    }
    Grid<1,T>& operator=(T const &t) {
    for ( auto it = data, end = data + gsize;  it != end; ++it)
         *it = t;
    return *this;
    }

    size_type get_gsize() const { return gsize; }


    ~Grid(){if(construction_flag)delete[] data;}
};



int main() {
//using  gsize_t = Grid<float>::size_type;

Grid<3,float> const g3(1.0f,2, 3, 4);
assert(1.0f == g3(1, 1, 1));
Grid<2,float> g2(2.0f,2, 5);
assert(2.0f == g2(1, 1));
g2 = g3[1];
assert(1.0f == g2(1, 1));

return 0;

}
