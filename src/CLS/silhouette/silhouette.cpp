#include <cmath>
#include <numeric>
#include <algorithm>

#include "silhouette.hpp"
#include "../../math_functions/math_functions.hpp"

int get_b_cluster(int K, int IMG_S, image *point, cluster_array *clusters){
    int    a_cluster = -1;
    int    b_cluster = -1;
    double min_dist  = std::numeric_limits<double>::max();

    for(int k=0; k<K; k++){ //Find next best cluster (b).
        centroid *curr_centroid = clusters->get_cluster(k)->get_centroid();
        double    curr_dist     = dist_k(point->get_image_vector(),
                                         curr_centroid->get_image_vector(),IMG_S,2);
        if(min_dist > curr_dist){
            min_dist  = curr_dist;
            b_cluster = a_cluster;
            a_cluster = k;
        }
    }

    if(b_cluster == -1){    //If b_cluster = -1 (meaning that the cluster k=0 is a),
                            //continue and find b.
        min_dist = std::numeric_limits<double>::max();

        for(int k=0; k<K; k++){
            if(k == a_cluster){
                continue;
            }

            centroid *curr_centroid = clusters->get_cluster(k)->get_centroid();
            double    curr_dist     = dist_k(point->get_image_vector(),
                                            curr_centroid->get_image_vector(),IMG_S,2);
            if(min_dist > curr_dist){
                min_dist  = curr_dist;
                b_cluster = a_cluster;
                a_cluster = k;
            }
        }
    }

    return b_cluster;
}

void get_all_b_clusters(int **b_clusters, CLS *CLS_instance, image_array *input_images){
    //Find b for all images.
    int IMG_S         = input_images->get_img_s();
    int num_of_images = input_images->get_num_of_images();

    for(int i=0; i<num_of_images; i++){
        image *curr_point = input_images->get_images()[i];

        (*b_clusters)[i] =  get_b_cluster(CLS_instance->get_K(),IMG_S,
                                          curr_point,CLS_instance->get_clusters());
    }
}

double dist_silhouette(image *a, image *b, int *vectors_squared){
    std::vector<int> a_vector = a->get_image_vector();
    std::vector<int> b_vector = b->get_image_vector();

    return sqrt(vectors_squared[a->get_file_id()] - 2 * std::inner_product(a_vector.begin(),a_vector.end(),b_vector.begin(),0) + vectors_squared[b->get_file_id()]);
}

void get_sil_cluster_scores(double **sil, int *b_clusters, CLS *CLS_instance, image_array *input_images){
    image **images          = input_images->get_images();
    int    *vectors_squared = new int[input_images->get_num_of_images()];

    for(int i=0; i<(int)input_images->get_num_of_images(); i++){ 
                                                 //Calculate the inner product
                                                 //of each vector with itself.
        std::vector<int> curr = images[i]->get_image_vector();

        vectors_squared[i] = std::inner_product(curr.begin(),curr.end(),curr.begin(),0);
    }
    
    for(int k=0; k<CLS_instance->get_K(); k++){
        double       s_i          = 0.0;
        cluster     *a_cluster    = CLS_instance->get_clusters()->get_cluster(k);
        int          a_num_points = a_cluster->get_size();
        image_array *a_points     = a_cluster->get_points();

        for(int i=0; i<a_num_points; i++){       //For a point i in cluster k,
            int      m            = 0;
            image   *s_point      = a_points->get_images()[i];
            double   a_i          = 0.0;
            double   b_i          = 0.0;
            cluster *b_cluster    = CLS_instance->get_clusters()->get_cluster(b_clusters[s_point->get_file_id()]);
            int      b_num_points = b_cluster->get_size();
            image_array      *b_points = b_cluster->get_points();
            std::vector<int>  s_vector = s_point->get_image_vector();

            for(int j=0; j<a_num_points; j++){   //calculate a(i),
                image            *a_point  = a_points->get_images()[j];
                std::vector<int>  a_vector = a_point->get_image_vector();
                
                if(i != j){
                    a_i += dist_silhouette(s_point,a_point,vectors_squared);
                }

                if(m<b_num_points){              //start calculating b(i) for efficiency,
                    image            *b_point  = b_points->get_images()[m];
                    std::vector<int>  b_vector = b_point->get_image_vector();
                    
                    b_i += dist_silhouette(s_point,b_point,vectors_squared);

                    m++;
                }
            }

            a_i /= (a_num_points - 1);           //make it average,

            for(; m<b_num_points; m++){          //calculate (the rest, if any left) b(i),
                image            *b_point  = b_points->get_images()[m];
                std::vector<int>  b_vector = b_point->get_image_vector();
                
                b_i += dist_silhouette(s_point,b_point,vectors_squared);
            }   

            b_i /= b_num_points;                  //make it average,
            s_i += (b_i - a_i)/std::max(b_i,a_i); //get s(i) = [b(i) - a(i)]/max(a(i),b(i))
        }

        (*sil)[k]  = s_i/(a_cluster->get_size()); //and produce the average s(i) for cluster k.
    }

    delete [] vectors_squared;
    vectors_squared = NULL;
}

void get_sil_cluster_scores(double **sil, int *b_clusters, CLS *CLS_instance, image_array *input_images_lat, image_array *input_images_org){
    image **images          = input_images_org->get_images();
    int    *vectors_squared = new int[input_images_org->get_num_of_images()];

    for(int i=0; i<(int)input_images_org->get_num_of_images(); i++){ 
                                                 //Calculate the inner product
                                                 //of each vector with itself.
        std::vector<int> curr = images[i]->get_image_vector();

        vectors_squared[i] = std::inner_product(curr.begin(),curr.end(),curr.begin(),0);
    }
    
    for(int k=0; k<CLS_instance->get_K(); k++){
        double       s_i          = 0.0;
        cluster     *a_cluster    = CLS_instance->get_clusters()->get_cluster(k);
        int          a_num_points = a_cluster->get_size();
        image_array *a_points     = a_cluster->get_points();

        for(int i=0; i<a_num_points; i++){       //For a point i in cluster k,
            int      m            = 0;
            image   *s_point      = images[a_points->get_images()[i]->get_file_id()];
            double   a_i          = 0.0;
            double   b_i          = 0.0;
            cluster *b_cluster    = CLS_instance->get_clusters()->get_cluster(b_clusters[s_point->get_file_id()]);
            int      b_num_points = b_cluster->get_size();
            image_array      *b_points = b_cluster->get_points();
            std::vector<int>  s_vector = s_point->get_image_vector();

            for(int j=0; j<a_num_points; j++){   //calculate a(i),
                image            *a_point  = images[a_points->get_images()[j]->get_file_id()];
                std::vector<int>  a_vector = a_point->get_image_vector();
                
                if(i != j){
                    a_i += dist_silhouette(s_point,a_point,vectors_squared);
                }

                if(m<b_num_points){              //start calculating b(i) for efficiency,
                    image            *b_point  = images[b_points->get_images()[m]->get_file_id()];
                    std::vector<int>  b_vector = b_point->get_image_vector();

                    b_i += dist_silhouette(s_point,b_point,vectors_squared);

                    m++;
                }
            }

            a_i /= (a_num_points - 1);           //make it average,

            for(; m<b_num_points; m++){          //calculate (the rest, if any left) b(i),
                    image        *b_point  = images[b_points->get_images()[m]->get_file_id()];
                std::vector<int>  b_vector = b_point->get_image_vector();

                b_i += dist_silhouette(s_point,b_point,vectors_squared);
            }   

            b_i /= b_num_points;                  //make it average,
            s_i += (b_i - a_i)/std::max(b_i,a_i); //get s(i) = [b(i) - a(i)]/max(a(i),b(i))
        }

        (*sil)[k]  = s_i/(a_cluster->get_size()); //and produce the average s(i) for cluster k.
    }

    delete [] vectors_squared;
    vectors_squared = NULL;
}

void get_sil_average(double **sil, int K){
    double sum = 0.0;

    for(int k=0; k<K; k++){
        sum += (*sil)[k];
    }

    (*sil)[K] = sum/K;
}

void delete_b_clusters(int *b_clusters){
    delete [] b_clusters;
    b_clusters = NULL;
}

double *silhouette(CLS *CLS_instance, image_array *input_images_org){
    int    *b_clusters = new int[input_images_org->get_num_of_images()];
    double *sil        = new double[CLS_instance->get_K()+1]; //The last element is all
                                                              //clusters' average.
    get_all_b_clusters(&b_clusters,CLS_instance,input_images_org);
    get_sil_cluster_scores(&sil,b_clusters,CLS_instance,input_images_org);
    get_sil_average(&sil,CLS_instance->get_K());

    delete_b_clusters(b_clusters);

    return sil;
}

double *silhouette(CLS *CLS_instance, image_array *input_images_lat, image_array *input_images_org){
    int    *b_clusters = new int[input_images_lat->get_num_of_images()];
    double *sil        = new double[CLS_instance->get_K()+1]; //The last element is all
                                                              //clusters' average.
    get_all_b_clusters(&b_clusters,CLS_instance,input_images_lat);
    get_sil_cluster_scores(&sil,b_clusters,CLS_instance,input_images_lat,input_images_org);
    get_sil_average(&sil,CLS_instance->get_K());

    delete_b_clusters(b_clusters);

    return sil;
}
