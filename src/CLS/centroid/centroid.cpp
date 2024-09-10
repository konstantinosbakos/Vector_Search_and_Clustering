#include "centroid.hpp"

//~Centroid~//

centroid::centroid(centroid &old){
    this->image_vector = new std::vector<double>(old.image_vector[0].begin(),old.image_vector[0].end());
    this->sum          = new std::vector<int>(old.sum[0].begin(),old.sum[0].end());
}

centroid::centroid(std::vector<int> image_vector, int IMG_S){
    this->image_vector = new std::vector<double>(image_vector.begin(),image_vector.end());
    this->sum          = new std::vector<int>(IMG_S,0); 
}

centroid::centroid(std::vector<double> image_vector, int IMG_S){
    this->sum          = new std::vector<int>(IMG_S,0); 
    this->image_vector = new std::vector<double>(image_vector.begin(),image_vector.end());
}

centroid::centroid(std::vector<double> image_vector, std::vector<int> sum){
    this->sum          = new std::vector<int>(sum.begin(),sum.end()); 
    this->image_vector = new std::vector<double>(image_vector.begin(),image_vector.end());
}

centroid::~centroid(){
    delete this->sum;
    this->sum = NULL;

    delete this->image_vector;
    this->image_vector = NULL;
}

void centroid::reset_sum(int IMG_S){ //Reinitialise sum to 0.
    delete this->sum;
    this->sum = new std::vector<int>(IMG_S,0);
}

//~Centroid Array~//

centroid_array::centroid_array(int K, int IMG_S){
    this->size  = K;
    this->IMG_S = IMG_S;
    this->array = new centroid*[K];

    for(int i=0; i<K; i++){
        this->array[i] = NULL;
    }
}
centroid_array::centroid_array(centroid_array &old){
    this->size  = old.size;
    this->IMG_S = old.IMG_S;
    this->array = new centroid*[this->size];

    for(int i=0; i<this->size; i++){
        this->array[i] = new centroid(*(old.array[i]));
    } 
}

centroid_array::~centroid_array(){
    for(int i=0; i<this->size; i++){
        delete this->array[i];
        this->array[i] = NULL;
    }

    delete [] this->array;
    this->array = NULL;
}


bool centroid_array::search(std::vector<int> q){
    std::vector<double> q_double(q.begin(),q.end()); //Turn q to a double vector and search.

    for(int i=0; i<this->size; i++){
        if(this->array[i] == NULL){
            return false;
        }
        else if(this->array[i]->get_image_vector() == q_double){
            return true;
        }
    }

    return false;
}

bool centroid_array::search(std::vector<double> q){
    for(int i=0; i<this->size; i++){
        if(this->array[i] == NULL){
            return false;
        }
        else if(this->array[i]->get_image_vector() == q){
            return true;
        }
    }

    return false;
}

void centroid_array::reset_sums(){
    for(int i=0; i<this->size; i++){
        this->array[i]->reset_sum(this->IMG_S);
    }
}

void centroid_array::add_centroid(std::vector<int> image_vector){
    for(int i=0; i<this->size; i++){
        if(this->array[i] == NULL){
            this->array[i] = new centroid(image_vector,this->IMG_S);
            
            return;
        }
    }
}

void centroid_array::add_centroid(std::vector<double> image_vector){
    for(int i=0; i<this->size; i++){
        if(this->array[i] == NULL){
            this->array[i] = new centroid(image_vector,this->IMG_S);
            
            return;
        }
    }
}

void centroid_array::update_centroid(int k, std::vector<double> image_vector){
    //Update the value of a specific centroid.
    delete this->array[k];

    this->array[k] = new centroid(image_vector,this->IMG_S);
}

void centroid_array::update_centroid(int k, std::vector<double> image_vector, std::vector<int> sum){
    //Update the value and the sum of a specific centroid.
    delete this->array[k];

    this->array[k] = new centroid(image_vector,sum);
}

image_array *centroid_array::get_img_array_of_centroids(){
    image_array *array = new image_array(this->size,true);

    if(this->array[0]){
        array->set_img_s(this->array[0]->get_image_vector().size());
    }

    for(int k=0; k<this->size; k++){
        array->add_image(k,this->array[k]->get_image_vector());
    }

    return array;
}
