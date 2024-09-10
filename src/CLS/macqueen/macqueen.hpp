#ifndef __MCQ__
#define __MCQ__

#include "../centroid/centroid.hpp"

void mac_queen_update_step(int pos, int k, int cluster_size, centroid_array *centroids,
                           image_array *input_images);
    //Calculate the new centroid(only one), after only one point insertion.
void mac_queen_update(int K, std::vector<int> assignment, std::vector<int> cluster_size, 
                      centroid_array *centroids,image_array *input_images);
    //Calculate the new centroids(all of them), after all the points are inserted.

#endif