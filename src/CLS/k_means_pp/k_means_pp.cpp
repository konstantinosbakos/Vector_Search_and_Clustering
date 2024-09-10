#include <cmath>
#include <limits>
#include <algorithm>

#include "k_means_pp.hpp"
#include "../../math_functions/math_functions.hpp"

int find_r(int n_sub_t, double *P){                              //P(r-1) < x <= P(r).
    double x = uniform_real(P[0],P[n_sub_t-1]);

    for(int i=1; i<n_sub_t; i++){
        if(P[i-1] < x && x <= P[i]){
            return i;
        }
    }

    return -1;
}

void delete_arrays(double *D, double *P){
    delete [] D;
    D = NULL;

    delete [] P;
    P = NULL;

}

void normalise__D(int n_sub_t, double **D){
    double D_max = *std::max_element((*D),(*D) + n_sub_t);       //Find max element.

    for(int i=0; i<n_sub_t; i++){
        (*D)[i] /= D_max;
    }
}

void initialize_P(int n_sub_t, double *D, double **P){
    for(int i=0; i<n_sub_t; i++){
        if(i == 0){
            (*P)[i] = 0.0;                                       //P(0) = 0.0.
        }
        else{
            (*P)[i] = (*P)[i-1] + pow(D[i],2);                   //Add P(i) = P(i-1) + D(i)^2.
        }
    }

}

void initialise_D(int t, int n_sub_t, double **D, image_array *input_images, centroid_array *centroids){
    int IMG_S  = input_images->get_img_s();
    int n_real = 0;                                              //Actual value in the input_images array.
    int num_of_images = input_images->get_num_of_images();

    for(int i=0; i<n_sub_t; i++){
        if(n_real >= num_of_images){
            break;
        }

        image  *curr_image = input_images->get_images()[n_real];
        double  min_dist   = std::numeric_limits<double>::max();

        if(centroids->search(curr_image->get_image_vector())){   //If the point is a centroid, skip.
            i--;
            n_real++;
            continue;
        }

        for(int j=0;j<t; j++){                                   //Find the min_dist.
            std::vector<double> curr_centroid_vector = centroids->get_centroid_vector(j);

            double new_dist = dist_k(curr_image->get_image_vector(),
                                     curr_centroid_vector,IMG_S,2);

            if(min_dist > new_dist){
                min_dist = new_dist;
            }
        }

        (*D)[i] = min_dist;                                      //Save it.

        n_real++;
    }

}

centroid_array *k_means_pp_init(int k, image_array *input_images){
    int             IMG_S              = input_images->get_img_s();
    int             first_centroid_pos = uniform_int(0,input_images->get_num_of_images());
                                                                 //Choose a centroid uniformally at random.
    centroid_array *centroids          = new centroid_array(k,IMG_S);

    centroids->add_centroid(input_images->get_images()[first_centroid_pos]->get_image_vector());
                                                                 //Add it to the array.
    for(int t=1; t<k; t++){
        int     n_sub_t = input_images->get_num_of_images() - t; //Get number of images - t (n - t)
        double *D       = new double[n_sub_t];
        double *P       = new double[n_sub_t];

        initialise_D(t,n_sub_t,&D,input_images,centroids);       //Initialize the min_dist to the centroids
                                                                 //for each point.
        normalise__D(n_sub_t,&D);                                //Divide by max_iD(i).
        initialize_P(n_sub_t,D,&P);                              //Find P(i) partial sums.

        centroids->add_centroid(input_images->get_images()[find_r(n_sub_t,P)]->get_image_vector());

        delete_arrays(D,P);
    }

    return centroids;
}
