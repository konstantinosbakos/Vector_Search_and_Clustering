#include <algorithm>

#include "macqueen.hpp"

void mac_queen_update_step(int pos, int k, int cluster_size, centroid_array *centroids,
                           image_array *input_images){
    int              IMG_S      = input_images->get_img_s();
    std::vector<int> sum        = centroids->get_centroid(k)->get_sum();
                                            //Get the centroid sum.
    std::vector<int> curr_image = input_images->get_images()[pos]->get_image_vector();
                                            //Get the vector of the added image.
    
    std::transform(sum.begin(),sum.end(),curr_image.begin(),sum.begin(),std::plus<int>()); 
                                            //Add the image to the sum.

    std::vector<double> mean(IMG_S,cluster_size);

    std::transform(sum.begin(),sum.end(),mean.begin(),mean.begin(),std::divides<double>()); 
                                            //mean = sum/cluster_size.
    centroids->update_centroid(k,mean,sum); //Update the centroid.
}

void mac_queen_update(int K, std::vector<int> assignment, std::vector<int> cluster_size, 
                      centroid_array *centroids,image_array *input_images){
                                                                    //This is the same function as above,
                                                                    //but essentially implemented for all
                                                                    //points.
    int n     = input_images->get_num_of_images();
    int IMG_S = input_images->get_img_s();

    std::vector<std::vector<int>> sum(K,std::vector<int>(IMG_S,0)); //Array for the sum of each centroid. 

    for(int i=0; i<n; i++){
        std::vector<int> curr_image_vector = input_images->get_images()[i]->get_image_vector();

        std::transform(sum[assignment[i]].begin(),sum[assignment[i]].end(),
                       curr_image_vector.begin(),sum[assignment[i]].begin(),std::plus<int>());
    }

    std::vector<std::vector<double>> mean(K,std::vector<double>(IMG_S,0.0));

    for(int k=0; k<K; k++){
        std::fill(mean[k].begin(),mean[k].end(),cluster_size[k]);
        std::transform(sum[k].begin(),sum[k].end(),mean[k].begin(),
                       mean[k].begin(),std::divides<double>());

        centroids->update_centroid(k,mean[k]);
    }
}