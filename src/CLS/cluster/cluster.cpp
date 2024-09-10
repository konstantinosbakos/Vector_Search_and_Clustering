#include "cluster.hpp"

cluster::cluster(int cluster_size, centroid *_centroid){
    this->_centroid      = _centroid;
    this->next_null      = 0;
    this->cluster_size   = cluster_size;
    this->cluster_points = new image_array(this->cluster_size);
}

cluster::~cluster(){
    delete this->cluster_points;
    this->cluster_points = NULL;
}

void cluster::add_point(image *point){
    if(this->next_null != this->cluster_size){
        this->cluster_points->add_image(this->next_null++,point);
    }
}

cluster_array::cluster_array(std::vector<int> cluster_size, centroid_array *centroids){
    this->K     = centroids->get_size();
    this->array = new cluster*[this->K];

    for(int k=0; k<this->K; k++){
        this->array[k] = new cluster(cluster_size[k],centroids->get_centroid(k));
    }
}

cluster_array::~cluster_array(){
    for(int k=0; k<this->K; k++){
        delete this->array[k];
        this->array[k] = NULL;
    }

    delete [] this->array;
    this->array = NULL;
}

