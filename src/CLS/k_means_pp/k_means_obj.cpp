#include <cmath>
#include <limits>
#include <algorithm>

#include "k_means_pp.hpp"
#include "../../math_functions/math_functions.hpp"

double k_means_objective_func_original(int K, cluster_array *clusters){
    int    IMG_S = clusters->get_cluster(0)->get_points()->get_images()[0]->get_image_vector().size();
    double vC    = 0.0;

    for(int c=0; c<K; c++){
        cluster  *curr_cluster  = clusters->get_cluster(c);
        centroid *curr_centroid = curr_cluster->get_centroid();

        for(int i=0; i<curr_cluster->get_size(); i++){
            image *x = curr_cluster->get_points()->get_images()[i];

            vC += pow(dist_k(x->get_image_vector(),curr_centroid->get_image_vector(),IMG_S,2),2);
            //vC += d(x_i,C)^2
        }
    }

    return vC;
}

double k_means_objective_func_latent(int K, cluster_array *clusters, image_array *input_images){
    int    IMG_S = input_images->get_img_s();
    double vC    = 0.0;

    for(int c=0; c<K; c++){
        cluster  *curr_cluster  = clusters->get_cluster(c);

        std::vector<int> sum(IMG_S,0);
        std::vector<double> og_centroid(IMG_S,0.0); //Find the centroid as the mean 
                                                    //of all the points in the original
                                                    //dimension.
        for(int i=0; i<curr_cluster->get_size(); i++){
            image *x = input_images->get_images()[curr_cluster->get_points()->get_images()[i]->get_file_id()];

            std::transform(sum.begin(),sum.end(),
                       x->get_image_vector().begin(),sum.begin(),std::plus<int>());
        }

        std::fill(og_centroid.begin(),og_centroid.end(),curr_cluster->get_size());
        std::transform(sum.begin(),sum.end(),og_centroid.begin(),
                       og_centroid.begin(),std::divides<double>());


        for(int i=0; i<curr_cluster->get_size(); i++){
            image *x = input_images->get_images()[curr_cluster->get_points()->get_images()[i]->get_file_id()];

            vC += pow(dist_k(x->get_image_vector(),og_centroid,IMG_S,2),2);
            //vC += d(x_i,C)^2
        }
    }

    return vC;
}
