#ifndef __F_G_H__
#define __F_G_H__

#include "../cube/skip_list/skip_list.hpp"
#include "../image/image.hpp"

class H{
    private:
        int    d; //size of vectors.
        int    w;
        double t;
        std::vector<double> v;
    public:
        H(int d, int w);

        int calculate(std::vector<int> p); 
};

class G{
    private:
        H   **h_; //h_i() functions of a g() functions, where i belongs to [0,k].
        int   k;
        int   M;
        int  *r_;
    public:
        G(unsigned int M, int d, int w, int k);
        ~G();

        unsigned int calculate_id(image *p);
        unsigned int calculate_g(image *p, int table_size);

        inline   int calculate_g(int id, int table_size){return id%table_size;};
};

class F{
    private:
        H   **h_;
        int   k;

        skip_list *SL;
    public:
        F(int IMG_S, int k);
        ~F();

        int calculate_f(image *p);
};

#endif
