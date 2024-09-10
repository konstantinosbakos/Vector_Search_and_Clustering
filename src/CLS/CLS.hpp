#ifndef __CLS__
#define __CLS__

#include <chrono>
#include <string>

#include "Lloyd/Lloyd.hpp"
#include "cluster/cluster.hpp"
#include "reverse_search/reverse_search.hpp"
#include "../main_var_classes/CLS/cluster_var.hpp"

#define METHOD_ERR -1

class CLS{
    private:
        int  K;
        int  L;
        int  k_LSH;
        int  M_Hype;
        int  k_Hype;
        int  method;               //The method that will be used.
        int  probes;
        bool complete;             //If the output (optional) choice -complete is chosen.
        bool init_status;          //If the initialisation was correct this is true.

        EXEC_TIME      *exec_time; //The execution time of the algorithm.
        cluster_array  *clusters;  //The array of the final clusters.
        centroid_array *centroids; //The array of the current centroids.

        int *get_conf_settings(std::string conf_file);

        void set_method(std::string method); 
        void set_conf_settings(int *conf_settings);                   //Read cluster.conf.
        void classic_exec(image_array *input_images);                 //The execution of the classic algorithm.
        void init_centroids(image_array *input_images);               //Initialise the centroids (kmeans++).
        void lsh_approx_exec(image_array *input_images);              //The execution of the LSH reverse search algorithm.
        void cube_approx_exec(image_array *input_images);             //The execution of the Hypercube reverse search algorithm.
        void print_silhouette(double *silhouette_values);
        void complete_output(int IMG_S, double *silhouette_values);   //If complete = true.
        void incomplete_output(int IMG_S, double *silhouette_values); //If complete = false.
        void silhouette_exec(image_array *input_images_org, std::string output_file);
        void silhouette_exec(image_array *input_images_lat, image_array *input_images_org, std::string output_file);
        void CLS_output(int IMG_S, double *silhouette_values, std::string output_file, image_array *input_images_org, std::string type); 
                                                                      //Output to the specified file.
    public:
        CLS(CLS_main_input *CLS_vars);
        ~CLS();

        void classic_algorithm(image_array *input_images_org, std::string output_file);
        void lsh_approx_algorithm(image_array *input_images_org, std::string output_file);
        void cube_approx_algorithm(image_array *input_images_org, std::string output_file);
        void classic_algorithm(image_array *input_images_lat, image_array *input_images_org, std::string output_file);
        void lsh_approx_algorithm(image_array *input_images_lat, image_array *input_images_org, std::string output_file);
        void cube_approx_algorithm(image_array *input_images_lat, image_array *input_images_org, std::string output_file);


        inline int  get_K(){return this->K;};
        inline int  get_method(){return this->method;};
        inline bool get_init_status(){return this->init_status;};

        inline cluster_array *get_clusters(){return this->clusters;};
};

void delete_CLS(CLS *CLS);

#endif