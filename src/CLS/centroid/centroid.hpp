#ifndef __CEN__
#define __CEN__

#include "../../image/image.hpp"

class centroid{
    private:
        std::vector<double> *image_vector; //This image vector is double, because except when
                                           //initialised by kmeans++, the values are mean().
        std::vector<int> *sum;             //Used by the Classic algorithm for efficiency.
    public:
        centroid(centroid &old);
        centroid(std::vector<int> image_vector, int IMG_S);
        centroid(std::vector<double> image_vector, int IMG_S);
        centroid(std::vector<double> image_vector, std::vector<int> sum);
        ~centroid();

        void reset_sum(int IMG_S);

        inline std::vector<int> get_sum(){return this->sum[0];};
        inline std::vector<double> get_image_vector(){return this->image_vector[0];};
};

class centroid_array{
    private:
        int        size; //K
        int        IMG_S;
        centroid **array;
    public:
        centroid_array(int k, int IMG_S);
        centroid_array(centroid_array &old);
        ~centroid_array();

        bool search(std::vector<int> q);
        bool search(std::vector<double> q);

        void reset_sums();
        void add_centroid(std::vector<int> image_vector);
        void add_centroid(std::vector<double> image_vector);
        void update_centroid(int k, std::vector<double> image_vector);
        void update_centroid(int k, std::vector<double> image_vector, std::vector<int> sum);

        image_array *get_img_array_of_centroids();

        inline int get_size(){return this->size;};
        inline centroid *get_centroid(int k){return this->array[k];};
        inline std::vector<double> get_centroid_vector(int i){return this->array[i]->get_image_vector();};
};

#endif