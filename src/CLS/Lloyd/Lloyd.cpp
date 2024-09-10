#include <limits>

#include "Lloyd.hpp"
#include "../../math_functions/math_functions.hpp"

int Lloyds_assignment_step(int i, int K, int IMG_S, centroid_array *centroids, image *_image){
    //Find the min_dist from the closest centroid, and return it.
    int     closest_centroid = -1;
    double  min_dist         = std::numeric_limits<double>::max(); 

    for(int k=0; k<K; k++){
        std::vector<double> curr_centroid_vector = centroids->get_centroid_vector(k);

        double  curr_dist = dist_k(_image->get_image_vector(),
                                   curr_centroid_vector,IMG_S,2);

        if(min_dist > curr_dist){
            min_dist         = curr_dist;
            closest_centroid = k;
        }
    }

    return closest_centroid;
}
