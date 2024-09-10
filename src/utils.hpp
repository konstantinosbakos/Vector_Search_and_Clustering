#ifndef __UTILS__
#define __UTILS__

#include <set>
#include <chrono>

#include "image/image.hpp"

#define EXEC_TIME     std::chrono::microseconds
#define NEIGHBOUR_SET std::set<prio_queue,Compare> //Used as a priority queue with unique entries
                                                   //that can be iterated.
class prio_queue{ 
    public:
        double dist;
        image *_image;

        prio_queue(double dist, image *_image){
            this->dist   = dist;
            this->_image = _image;
        }
};

struct Compare{                                    //First item of the list is the closest to the query.
        bool operator()(prio_queue d1, prio_queue d2){return d1.dist < d2.dist;}
};

#endif