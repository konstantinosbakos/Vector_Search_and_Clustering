#include <cmath>
#include <climits>
#include <algorithm>

#include "reverse_search.hpp"
#include "../../math_functions/math_functions.hpp"

int min_distance_q(image_array *q){ //Find the minimum distance between all centroids.
    int IMG_S    = q->get_img_s();
    int min_dist = INT_MAX;
    int num_of_q = q->get_num_of_images();

    for(int i=0; i<num_of_q; i++){
        for(int j=0; j<num_of_q; j++){
            std::vector<double> q_i = q->get_images()[i]->get_image_vector_d();
            int new_dist = int(dist_k(q_i, q->get_images()[j]->get_image_vector_d(),IMG_S,2));

            if(min_dist > new_dist){
                min_dist = new_dist;
            }
        }
    }

    return min_dist;
}

int flag_check(int n, int q_num, std::vector<std::vector<int>> one_hot,
               std::vector<std::vector<int>> one_hot_old){
    int check = 0;

    for(int k=0; k<q_num; k++){     //Check how many clusters had points added to them.
        for(int i=0; i<n; i++){
            if(one_hot[i][k] != one_hot_old[i][k]){
                check++;
                break;
            }
        }
    }

    return check;
}

void delete_lsh_q(LSH *lsh, image_array *q){
    delete q;
    q = NULL;

    delete lsh;
    lsh = NULL;
}

void delete_cube_q(Cube *cube, image_array *q){
    delete q;
    q = NULL;

    delete cube;
    cube = NULL;
}

void resolve_conflicts(int n, int q_num, std::vector<std::vector<int>> *one_hot,
                       image_array *input_images, image_array *q){
    int IMG_S = input_images->get_img_s();

    for(int i=0; i<n; i++){
        if((*one_hot)[i][q_num] > 1){   //If a point is assigned to more than one clusters,
            std::vector<double> i_dist(q_num,std::numeric_limits<double>::max());
            
            for(int r=0; r<q_num; r++){ //find the distance to said clusters,
                if((*one_hot)[i][r]){
                    i_dist[r] = dist_k(input_images->get_images()[i]->get_image_vector(),
                                        q->get_images()[r]->get_image_vector_d(),IMG_S,2);
                }
            }

            int closest_r = std::distance(i_dist.begin(),std::min_element(i_dist.begin(),i_dist.end()));
                                        //find the closest cluster,
            for(int r=0; r<q_num;r++){
                if(r != closest_r){     //remove the assignment from other clusters
                    (*one_hot)[i][r] = 0;
                }
            }

            (*one_hot)[i][q_num] = 1;   //and denote that it is assigned only to one cluster.
        }
    }
}

void update_one_hot(int q_num, std::vector<std::vector<int>> *one_hot, NEIGHBOUR_SET **ranges){
    for(int r=0; r<q_num; r++){
        for(std::set<prio_queue>::iterator apx = ranges[r]->begin(); 
            apx != ranges[r]->end(); apx++){

            (*one_hot)[(*apx)._image->get_file_id()][r]      = 1;
            (*one_hot)[(*apx)._image->get_file_id()][q_num] += 1;
        }
    }
}

void match_unmatched_points(int n, int q_num, std::vector<std::vector<int>> *one_hot, std::vector<int> *cluster_size,
                            std::vector<int> *closest_cluster, image_array *input_images, image_array *q){
    int IMG_S = input_images->get_img_s();
    
    for(int i=0; i<n; i++){
        if((*one_hot)[i][q_num] == 0){            //If not matched,
            std::vector<double> i_dist(q_num,std::numeric_limits<double>::max());

            for(int r=0; r<q_num; r++){           //find the distance to all the clusters,
                i_dist[r] = dist_k(input_images->get_images()[i]->get_image_vector(),
                                    q->get_images()[r]->get_image_vector_d(),IMG_S,2);
            }

            int closest_r = std::distance(i_dist.begin(),std::min_element(i_dist.begin(),i_dist.end()));
                                                  //find the closest cluster,
            (*one_hot)[i][q_num]     = 1;         //denote that the point belongs to a cluster,
            (*cluster_size)[closest_r]++;         //increase the cluster size,
            (*closest_cluster)[i]    = closest_r; //assign the closest cluster,
            (*one_hot)[i][closest_r] = 1;         //and assign the point to said cluster.
        }
        else{
            for(int r=0; r<q_num; r++){
                if((*one_hot)[i][r]){             //If assigned,
                    (*cluster_size)[r]++;         //increase cluster size
                    (*closest_cluster)[i] = r;    //and assign the closest cluster.
                    break;
                }
            }
        }
    }
}

std::vector<int> reverse_search_lsh(int k_lsh, int L, std::vector<int> *cluster_size,
                                    centroid_array *centroids, image_array *input_images){
    int          n     = input_images->get_num_of_images();
    int          flag  = floor(centroids->get_size() / 2.0);               //If not enough clusters are updated,
                                                                           //stop the loop.
    int          check = 0;                                                //Counts how many clusters are updated.
    image_array *q     = centroids->get_img_array_of_centroids();
    int          q_num = q->get_num_of_images();
    int          R     = min_distance_q(q)/2;                              //Initialize first R = min_dist/2 (between centroids).
    LSH         *lsh   = new LSH(k_lsh,L,-1,R,q_num,input_images);

    std::vector<int> closest_cluster(n,-1);                                //Array denoting the closest cluster of each point.
    std::vector<std::vector<int>> one_hot(n,std::vector<int>(q_num+1,0));  //A one hot encoding array with a count at the end.
                                                                           //0 means that the point does not belong to a cluster.
                                                                           //1 means the point belongs to the cluster.
                                                                           //The last cell shows in how many clusters a point belongs to.
                                                                           //It is used to find and resolve conflicts (and then it is
                                                                           //set to 1).
    std::vector<std::vector<int>> one_hot_old(n,std::vector<int>(q_num+1,0)); 

    while(check > flag){
        one_hot_old = one_hot;

        lsh->Approximate(q);
        NEIGHBOUR_SET **ranges = lsh->get_approx_neighbours();

        update_one_hot(q_num,&one_hot,ranges);                             //Update the array based on the new additions (if any).
        resolve_conflicts(n,q_num,&one_hot,input_images,q);                //All points belonging to more than 1 ranges,
                                                                           //get placed to the closest.

        check = flag_check(n,q_num,one_hot,one_hot_old);                   //Check how many clusters had points added to them.

        R *= 2;                                                            //Update the values for the next range.
        lsh->update_R(R);
        lsh->update_approx();
        check = 0;
    }

    match_unmatched_points(n,q_num,&one_hot,cluster_size,&closest_cluster,input_images,q);
                                                                           //All points that ended up in no clusters,
                                                                           //get placed to the closest.
    delete_lsh_q(lsh,q);

    return closest_cluster;
}

std::vector<int> reverse_search_cube(int k_cube, int M, int probes, std::vector<int> *cluster_size,
                                    centroid_array *centroids, image_array *input_images){
    int          n     = input_images->get_num_of_images();
    int          flag  = floor(centroids->get_size() / 2.0);               //If not enough clusters are updated,
                                                                           //stop the loop.
    int          check = 0;                                                //Counts how many clusters are updated.
    image_array *q     = centroids->get_img_array_of_centroids();
    int          q_num = q->get_num_of_images();
    int          R     = min_distance_q(q)/2;                              //Initialize first R = min_dist/2 (between centroids).
    Cube         *cube   = new Cube(k_cube,M,-1,R,probes,q_num,input_images);

    std::vector<int> closest_cluster(n,-1);                                //Array denoting the closest cluster of each point.
    std::vector<std::vector<int>> one_hot(n,std::vector<int>(q_num+1,0));  //A one hot encoding array with a count at the end.
                                                                           //0 means that the point does not belong to a cluster.
                                                                           //1 means the point belongs to the cluster.
                                                                           //The last cell shows in how many clusters a point belongs to.
                                                                           //It is used to find and resolve conflicts (and then it is
                                                                           //set to 1).
    std::vector<std::vector<int>> one_hot_old(n,std::vector<int>(q_num+1,0)); 

    while(check > flag){
        one_hot_old = one_hot;

        cube->range_search(q);
        NEIGHBOUR_SET **ranges = cube->get_range_neighbours();

        update_one_hot(q_num,&one_hot,ranges);                             //Update the array based on the new additions (if any).
        resolve_conflicts(n,q_num,&one_hot,input_images,q);                //All points belonging to more than 1 ranges,
                                                                           //get placed to the closest.

        check = flag_check(n,q_num,one_hot,one_hot_old);                   //Check how many clusters had points added to them.

        R *= 2;                                                            //Update the values for the next range.
        cube->update_R(R);
        cube->update_approx();
        check = 0;
    }

    match_unmatched_points(n,q_num,&one_hot,cluster_size,&closest_cluster,input_images,q);
                                                                           //All points that ended up in no clusters,
                                                                           //get placed to the closest.
    delete_cube_q(cube,q);

    return closest_cluster;
}
