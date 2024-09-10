#ifndef __SIL__
#define __SIL__

#include "../CLS.hpp"

double *silhouette(CLS *CLS_instance, image_array *input_images_org);
double *silhouette(CLS *CLS_instance, image_array *input_images_lat, image_array *input_images_org);

#endif
