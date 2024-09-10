#ifndef __HASH__
#define __HASH__

#include "../f_g_h/f_g_h.hpp"

class item{                              //An element of a bucket.
                                         //A list can be created this way for each bucket.
    private:
        image *_image;
        item  *next;
    public:
        item(image *_image);
        ~item();

        inline item  *get_next(){return this->next;};
        inline void   set_next(item *next){this->next = next;};
        inline image *get_image(){return this->_image;};
};

class bucket{                            //A bucket contains elements of the same g() value.
    private:
        int   size;
        item *item_list;
    public:
        bucket();
        ~bucket();

        void add_item(image *_image);

        inline int   get_size(){return this->size;};
        inline item *get_items(){return this->item_list;};
};

class hash_table{
    private:
        G       *G_hash_function;        //The g() hash function of the hash table.
        F       *F_hash_function;        //The f() hash function of the hash table.
        int      n;                      //Total number of images contained.
        int      bucket_num;             //Number of buckets.
        bucket **bucket_array;
    public:
        hash_table(int n, G *hash_function, image **input_images);
        hash_table(int n, int k, F *hash_function, image **input_images);
        ~hash_table();

        bucket *search_item_G(image *q); //Returns the bucket according to the hash value of q.
        bucket *search_item_F(image *q);

        inline G *get_hash_function_G(){return this->G_hash_function;};
        inline F *get_hash_function_F(){return this->F_hash_function;};
        inline bucket *get_bucket(int pos){return this->bucket_array[pos];};
};

#endif