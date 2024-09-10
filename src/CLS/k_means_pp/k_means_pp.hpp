#ifndef __KPP__
#define __KPP__

#include "../cluster/cluster.hpp"
#include "../centroid/centroid.hpp"

double k_means_objective_func_original(int K, cluster_array *clusters);
double k_means_objective_func_latent(int K, cluster_array *clusters, image_array *input_images);

centroid_array *k_means_pp_init(int k, image_array *images);

#endif
