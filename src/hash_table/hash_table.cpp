#include <cmath>

#include "hash_table.hpp"

//~Item~//

item::item(image *_image){
    this->next   = NULL;
    this->_image = _image;
}

item::~item(){
    this->next   = NULL;
    this->_image = NULL;
}

//~Bucket~//

bucket::bucket(){
    this->size      = 0;
    this->item_list = NULL;
}

bucket::~bucket(){
    item *temp = NULL;

    while(this->item_list){
        temp = this->item_list->get_next();

        delete this->item_list;
        this->item_list = NULL;

        this->item_list = temp;
    }
}

void bucket::add_item(image *_image){
    item *new_item  = new item(_image);

    new_item->set_next(this->item_list);
    this->item_list = new_item;
    
    this->size++;
}

//~Hash Table~//

hash_table::hash_table(int n, G *hash_function, image **input_images){
    this->n               = n;
    this->bucket_num      = floor(n/8);
    this->bucket_array    = new bucket*[this->bucket_num];
    this->G_hash_function = hash_function;
    this->F_hash_function = NULL;

    for(int i=0; i<this->bucket_num; i++){
        this->bucket_array[i] = new bucket();
    }

    for(int i=0; i<this->n; i++){ //Initialise the hash table, adding all the images to
                                  //their respective bucket.
        int hash_value = this->G_hash_function->calculate_g(input_images[i],this->bucket_num);

        this->bucket_array[hash_value]->add_item(input_images[i]);
    }
}

hash_table::hash_table(int n, int k, F *hash_function, image **input_images){
    this->n               = n;
    this->bucket_num      = pow(2,k);
    this->bucket_array    = new bucket*[this->bucket_num];
    this->G_hash_function = NULL;
    this->F_hash_function = hash_function;

    for(int i=0; i<this->bucket_num; i++){
        this->bucket_array[i] = new bucket();
    }

    for(int i=0; i<this->n; i++){ //Initialise the hash table, adding all the images to
                                  //their respective bucket.
        int hash_value = this->F_hash_function->calculate_f(input_images[i]);

        this->bucket_array[hash_value]->add_item(input_images[i]);
    }
}

hash_table::~hash_table(){
    for(int i=0; i<this->bucket_num; i++){
        delete this->bucket_array[i];
        this->bucket_array[i] = NULL;
    }

    delete [] this->bucket_array;
    this->bucket_array = NULL;

    if(this->G_hash_function){
        delete this->G_hash_function;
        this->G_hash_function = NULL;
    }

    if(this->F_hash_function){
        delete this->F_hash_function;
        this->F_hash_function = NULL;
    }

}

bucket *hash_table::search_item_G(image *q){
    int hash_value = this->G_hash_function->calculate_g(q,this->bucket_num);

    return this->bucket_array[hash_value]; //Return the bucket of the corresponding hash value of q.
}

bucket *hash_table::search_item_F(image *q){
    int hash_value = this->F_hash_function->calculate_f(q);

    return this->bucket_array[hash_value]; //Return the bucket of the corresponding hash value of q.
}
