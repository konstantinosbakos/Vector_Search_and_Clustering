#ifndef __REV_S__
#define __REV_S__

#include "../../LSH/LSH.hpp"
#include "../../cube/cube.hpp"
#include "../centroid/centroid.hpp"

#define LSH_M       1
#define CUBE_M      2

std::vector<int> reverse_search_lsh(int k_lsh, int L, std::vector<int> *cluster_size,
                                    centroid_array *centroids, image_array *input_images);
std::vector<int> reverse_search_cube(int k_cube, int M, int probes, std::vector<int> *cluster_size,
                                    centroid_array *centroids, image_array *input_images);

#endif