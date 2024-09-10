#ifndef __LSHO__
#define __LSHO__

#include "../../LSH/LSH.hpp"
#include "../../brute_force/brute_force.hpp"

void LSH_output(image_array *query_images, std::string output_file, LSH *LSH_instance, brute_force *BF_instance);

#endif