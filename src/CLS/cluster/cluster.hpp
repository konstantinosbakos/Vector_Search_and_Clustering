#ifndef __CLUSTER__
#define __CLUSTER__

#include "../centroid/centroid.hpp"

class cluster{
    private:
        int          next_null; //Next position that is 0 (this->cluster_points[nextnull] = 0).
        int          cluster_size; 
        centroid    *_centroid;
        image_array *cluster_points;
    public:
        cluster(int cluster_size, centroid *_centroid);
        ~cluster();

        void add_point(image *point);

        inline int get_size(){return this->cluster_size;};
        inline centroid *get_centroid(){return this->_centroid;};
        inline image_array *get_points(){return this->cluster_points;};
};

class cluster_array{
    private:
        int       K;
        cluster **array;
    public:
        cluster_array(std::vector<int> cluster_size, centroid_array *centroids);
        ~cluster_array();

        inline void add_point(int k, image *point){this->array[k]->add_point(point);};

        inline cluster *get_cluster(int k){return this->array[k];};
};

#endif