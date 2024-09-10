#include <fstream>
#include <iomanip>
#include <iostream>

#include "macqueen/macqueen.hpp"
#include "k_means_pp/k_means_pp.hpp"
#include "silhouette/silhouette.hpp"
#include "reverse_search/reverse_search.hpp"
#include "../math_functions/math_functions.hpp"

//~Private~//

int *CLS::get_conf_settings(std::string conf_file){
    int           i             = 0;
    int          *conf_settings = NULL;
    std::fstream  conf_stream;

    conf_stream.open(conf_file);

    if(conf_stream.is_open()){
        std::string line    = "";
        std::string numbers = "0123456789";

        conf_settings = new int[6];

        while(std::getline(conf_stream, line) || i < 6){
            if(line.find_first_of(numbers) != std::string::npos){
                int pos = line.find_first_of(numbers);

                conf_settings[i] = std::stoi(line.substr(pos));
            }
            else{
                conf_stream.close();

                delete [] conf_settings;
                conf_settings = NULL;

                return NULL;
            }

            i++;
        }       
    }

    return conf_settings;
}

void CLS::set_method(std::string method){
    this->method = METHOD_ERR;

    if(method == "Hypercube"    || method == "hypercube"){
        this->method = CUBE_M;
    }
    if(method == "LSH"     || method == "Lsh"     || method == "lsh"){
        this->method = LSH_M;
    }
    if(method == "CLASSIC" || method == "Classic" || method == "classic"){
        this->method = CLASSIC_M;
    }

    if(this->method == METHOD_ERR){
        this->init_status = false;
    } 
}

void CLS::set_conf_settings(int *conf_settings){
    if(conf_settings){
        this->K           = conf_settings[0];
        this->L           = conf_settings[1];
        this->k_LSH       = conf_settings[2];
        this->M_Hype      = conf_settings[3];
        this->k_Hype      = conf_settings[4];
        this->probes      = conf_settings[5];
        this->init_status = true;
    }
    else{
        this->K           = -1;
        this->L           = -1;
        this->k_LSH       = -1;
        this->M_Hype      = -1;
        this->k_Hype      = -1;
        this->probes      = -1;
        this->init_status = false;
    }
}

void CLS::classic_exec(image_array *input_images){
    int               IMG_S         = input_images->get_img_s();
    int               loop_flag     = this->K;
    int               num_of_images = input_images->get_num_of_images();
    centroid_array   *old_centroids = NULL;
    std::vector<int>  assignment;
    std::vector<int>  cluster_size;

    this->init_centroids(input_images);

    auto start = std::chrono::high_resolution_clock::now();
    while(loop_flag){
        this->centroids->reset_sums();                          //Reset the sum values of the centroids to 0.

        if(old_centroids){
            delete old_centroids;
            old_centroids = new centroid_array(*this->centroids);
        }
        else{
            old_centroids = new centroid_array(*this->centroids);
        }

        assignment.clear();
        assignment.assign(num_of_images,-1);                    //Initialise assignment array.
        cluster_size.clear();
        cluster_size.assign(this->K,0);                         //Initialise cluster_sizes.

        for(int i=0; i<num_of_images; i++){
            assignment[i] = Lloyds_assignment_step(i,this->K,IMG_S,this->centroids,input_images->get_images()[i]);
                                                                //For each point added to a cluster,
            cluster_size[assignment[i]]++;
            mac_queen_update_step(i,assignment[i],cluster_size[assignment[i]],centroids,input_images);
                                                                //recalculate said clusters' centroid.
        }

        loop_flag = this->K;
        for(int i=0; i<this->K; i++){                           //If no centroids changed (loop_flag = 0) stop the loop.
            for(int j=0; j<this->K; j++){
                if(this->centroids->get_centroid_vector(i) == old_centroids->get_centroid_vector(j) || 
                   dist_k(this->centroids->get_centroid_vector(i),old_centroids->get_centroid_vector(j),IMG_S,2) == 0){
                    loop_flag--;
                    break;
                }
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();

    if(old_centroids){
        delete old_centroids;
        old_centroids = NULL;
    }

    this->clusters  = new cluster_array(cluster_size,centroids); //Create the final clusters.
    this->exec_time = new EXEC_TIME(std::chrono::duration_cast<std::chrono::microseconds>(end - start));

    for(int i=0; i<num_of_images; i++){                         //Add the points to their assigned clusters.
        this->clusters->add_point(assignment[i],input_images->get_images()[i]);
    }
}

void CLS::init_centroids(image_array *input_images){
    this->centroids = k_means_pp_init(this->K,input_images);
}

void CLS::lsh_approx_exec(image_array *input_images){
    int               IMG_S         = input_images->get_img_s();
    int               loop_flag     = this->K;
    int               num_of_images = input_images->get_num_of_images();
    centroid_array   *old_centroids = NULL;
    std::vector<int>  assignment;
    std::vector<int>  cluster_size;

    this->init_centroids(input_images);

    auto start = std::chrono::high_resolution_clock::now();
    while(loop_flag){
        this->centroids->reset_sums();                          //Reset the sum values of the centroids to 0.

        if(old_centroids){
            delete old_centroids;
            old_centroids = new centroid_array(*this->centroids);
        }
        else{
            old_centroids = new centroid_array(*this->centroids);
        }

        assignment.clear();
        assignment.assign(num_of_images,-1);
        cluster_size.clear();
        cluster_size.assign(this->K,0);

        assignment = reverse_search_lsh(this->k_LSH,this->L,&cluster_size,this->centroids,input_images);
                                                                //Assign all points using the reverse search (with LSH).

        mac_queen_update(this->K,assignment,cluster_size,this->centroids,input_images);
                                                                //Update all centroids.

        loop_flag = this->K;
        for(int i=0; i<this->K; i++){                           //If no centroids changed (loop_flag = 0) stop the loop.
            for(int j=0; j<this->K; j++){
                if(this->centroids->get_centroid_vector(i) == old_centroids->get_centroid_vector(j) || 
                   dist_k(this->centroids->get_centroid_vector(i),old_centroids->get_centroid_vector(j),IMG_S,2) == 0){
                    loop_flag--;
                    break;
                }
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();

    if(old_centroids){
        delete old_centroids;
        old_centroids = NULL;
    }

    this->clusters  = new cluster_array(cluster_size,centroids); //Create the final clusters.
    this->exec_time = new EXEC_TIME(std::chrono::duration_cast<std::chrono::microseconds>(end - start));

    for(int i=0; i<num_of_images; i++){                         //Add the points to their assigned clusters.
        this->clusters->add_point(assignment[i],input_images->get_images()[i]);
    }
}

void CLS::cube_approx_exec(image_array *input_images){
    int               IMG_S         = input_images->get_img_s();
    int               loop_flag     = this->K;
    int               num_of_images = input_images->get_num_of_images();
    centroid_array   *old_centroids = NULL;
    std::vector<int>  assignment;
    std::vector<int>  cluster_size;

    this->init_centroids(input_images);

    auto start = std::chrono::high_resolution_clock::now();
    while(loop_flag){
        this->centroids->reset_sums();                           //Reset the sum values of the centroids to 0.

        if(old_centroids){
            delete old_centroids;
            old_centroids = new centroid_array(*this->centroids);
        }
        else{
            old_centroids = new centroid_array(*this->centroids);
        }

        assignment.clear();
        assignment.assign(num_of_images,-1);
        cluster_size.clear();
        cluster_size.assign(this->K,0);

        assignment = reverse_search_cube(this->k_Hype,this->M_Hype,this->probes,
                                         &cluster_size,this->centroids,input_images);
                                                                 //Assign all points using the reverse search (with Hypercube).

        mac_queen_update(this->K,assignment,cluster_size,this->centroids,input_images);
                                                                 //Update all centroids.

        loop_flag = this->K;
        for(int i=0; i<this->K; i++){                            //If no centroids changed (loop_flag = 0) stop the loop.
            for(int j=0; j<this->K; j++){
                if(this->centroids->get_centroid_vector(i) == old_centroids->get_centroid_vector(j) || 
                   dist_k(this->centroids->get_centroid_vector(i),old_centroids->get_centroid_vector(j),IMG_S,2) == 0){
                    loop_flag--;
                    break;
                }
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();

    if(old_centroids){
        delete old_centroids;
        old_centroids = NULL;
    }

    this->clusters  = new cluster_array(cluster_size,centroids); //Create the final clusters.
    this->exec_time = new EXEC_TIME(std::chrono::duration_cast<std::chrono::microseconds>(end - start));

    for(int i=0; i<num_of_images; i++){                          //Add the points to their assigned clusters.
        this->clusters->add_point(assignment[i],input_images->get_images()[i]);
    }
}

void CLS::print_silhouette(double *silhouette_values){
    std::cout << "Silhouette: [";
    for(int k=0; k<=this->K; k++){
        if(k != this->K){
            std::cout << silhouette_values[k] << ", ";
        }
        else{
            std::cout << silhouette_values[k] << "]" << std::endl;
        }
    }    
}

void CLS::complete_output(int IMG_S, double *silhouette_values){
    for(int k=0; k<this->K; k++){
        cluster *curr_cluster = this->clusters->get_cluster(k);

        std::cout << "CLUSTER-" << k+1 << " ";
        std::cout << "{size: " << curr_cluster->get_size() << ", ";
        std::cout << "centroid:";

        std::cout << "[";
        std::cout << std::setprecision(7) << std::fixed;
        for(int i=0; i<IMG_S; i++){
                if(i == (IMG_S - 1)){
                    std::cout << curr_cluster->get_centroid()->get_image_vector()[i] << "], ";
                }
                else{
                    std::cout << curr_cluster->get_centroid()->get_image_vector()[i] << ", ";
                }
        }

        int size = curr_cluster->get_size();
        for(int i=0; i<size; i++){
            if(i == (size - 1)){
                std::cout << curr_cluster->get_points()->get_images()[i]->get_file_id() << "}" << std::endl;
            }
            else{
                std::cout << curr_cluster->get_points()->get_images()[i]->get_file_id() << ", ";
            }
        }
    }

    std::cout << "clustering_time: " << this->exec_time->count() / 1000000.0 << "s" << std::endl; 

    this->print_silhouette(silhouette_values);
}

void CLS::incomplete_output(int IMG_S, double *silhouette_values){
    for(int k=0; k<this->K; k++){
        cluster *curr_cluster = this->clusters->get_cluster(k);

        std::cout << "CLUSTER-" << k+1 << " ";
        std::cout << "{size: " << curr_cluster->get_size() << ", ";
        std::cout << "centroid:";

        std::cout << "[";
        std::cout << std::setprecision(7) << std::fixed;
        for(int i=0; i<IMG_S; i++){
                if(i == (IMG_S - 1)){
                    std::cout << curr_cluster->get_centroid()->get_image_vector()[i] << "]}" << std::endl;
                }
                else{
                    std::cout << curr_cluster->get_centroid()->get_image_vector()[i] << ", ";
                }
        }
    }

    std::cout << "clustering_time: " << this->exec_time->count() / 1000000.0 << "s" << std::endl; 

    this->print_silhouette(silhouette_values);
}

void CLS::silhouette_exec(image_array *input_images_org, std::string output_file){
    int     IMG_S             = input_images_org->get_img_s();
    double *silhouette_values = silhouette(this,input_images_org);                     //Find the silhouette values.

    this->CLS_output(IMG_S,silhouette_values,output_file,input_images_org,"original"); //Output to the specified file.

    delete [] silhouette_values;
    silhouette_values = NULL;  
}

void CLS::silhouette_exec(image_array *input_images_lat, image_array *input_images_org, std::string output_file){
    int     IMG_S             = input_images_lat->get_img_s();
    double *silhouette_values = silhouette(this,input_images_lat,input_images_org);    //Find the silhouette values.

    this->CLS_output(IMG_S,silhouette_values,output_file,input_images_org,"latent");   //Output to the specified file.

    delete [] silhouette_values;
    silhouette_values = NULL;  
}

void CLS::CLS_output(int IMG_S, double *silhouette_values, std::string output_file, image_array *input_images_org, std::string type){
    std::string     dimension = "";
    std::string     algorithm = "";
    std::ofstream   out_file;
    std::streambuf *old_cout  = std::cout.rdbuf();

    if(type == "original"){
        out_file.open(output_file,std::ios::app);
        dimension = " (original)";
    }
    else{
        out_file.open(output_file);
        dimension = " (latent)";
    }

    std::cout.rdbuf(out_file.rdbuf());                      //Redirect cout stream to the file.

    if(out_file.is_open()){                                 //Check if the file is open.

        if(this->method == CLASSIC_M)  {algorithm = "Lloyds";}
        else if(this->method == LSH_M) {algorithm = "Range Search LSH";}
        else if(this->method == CUBE_M){algorithm = "Range Search Hypercube";}

        std::cout << "Algorithm: " + algorithm + dimension << std::endl;

        if(this->complete){
            this->complete_output(IMG_S,silhouette_values);
        }
        else{
            this->incomplete_output(IMG_S,silhouette_values);
        }

        if(type == "original"){
            std::cout << "The objective function score is: " << k_means_objective_func_original(this->K,this->clusters) << ".\n" << std::endl;
        }
        else{
            std::cout << "The objective function score is: " << k_means_objective_func_latent(this->K,this->clusters,input_images_org) << ".\n" << std::endl;
        }

        out_file.close();
    }

    std::cout.rdbuf(old_cout);                              //Redirect the cout stream to terminal.
}

//~Public~//

CLS::CLS(CLS_main_input *CLS_vars){
    int *conf_settings = this->get_conf_settings(CLS_vars->get_conf());

    this->clusters     = NULL;
    this->complete     = CLS_vars->get_complete();
    this->centroids    = NULL;
    this->exec_time    = NULL;
    this->set_method(CLS_vars->get_method());
    this->set_conf_settings(conf_settings);

    if(conf_settings){
        delete [] conf_settings;
        conf_settings = NULL;
    }
}

CLS::~CLS(){
    if(this->centroids){
        delete this->centroids;
        this->centroids = NULL;
    }
    if(this->clusters){
        delete this->clusters;
        this->clusters = NULL;
    }
    if(this->exec_time){
        delete this->exec_time;
        this->exec_time = NULL;
    }
}

void CLS::classic_algorithm(image_array *input_images_org, std::string output_file){
    this->classic_exec(input_images_org);                          //Execute the Classic algorithm.
    this->silhouette_exec(input_images_org,output_file);
}

void CLS::lsh_approx_algorithm(image_array *input_images_org, std::string output_file){
    this->lsh_approx_exec(input_images_org);                       //Execute the LSH reverse search algorithm.
    this->silhouette_exec(input_images_org,output_file);
}

void CLS::cube_approx_algorithm(image_array *input_images_org, std::string output_file){
    this->cube_approx_exec(input_images_org);                      //Execute the Hypercube reverse search algorithm.
    this->silhouette_exec(input_images_org,output_file);
}

void CLS::classic_algorithm(image_array *input_images_lat, image_array *input_images_org, std::string output_file){
    this->classic_exec(input_images_lat);                          //Execute the Classic algorithm.
    this->silhouette_exec(input_images_lat,input_images_org,output_file);
}

void CLS::lsh_approx_algorithm(image_array *input_images_lat, image_array *input_images_org, std::string output_file){
    this->lsh_approx_exec(input_images_lat);                       //Execute the LSH reverse search algorithm.
    this->silhouette_exec(input_images_lat,input_images_org,output_file);
}

void CLS::cube_approx_algorithm(image_array *input_images_lat, image_array *input_images_org, std::string output_file){
    this->cube_approx_exec(input_images_lat);                      //Execute the Hypercube reverse search algorithm.
    this->silhouette_exec(input_images_lat,input_images_org,output_file);
}

//~Non-Class~//

void delete_CLS(CLS *CLS){
    delete CLS;
    CLS = NULL;
}
