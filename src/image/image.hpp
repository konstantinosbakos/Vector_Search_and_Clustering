#ifndef __IMG__
#define __IMG__

#include <vector>
#include <string>

class image{
    private:
        int file_id;            //The position of the image in the input file.
        std::vector<int> image_vector_i;
        std::vector<double> image_vector_d;
    public:
        inline image(int file_id, std::vector<int> image_vector){
            this->file_id        = file_id;
            this->image_vector_i = image_vector;
        }
        inline image(int file_id, std::vector<double> image_vector){
            this->file_id        = file_id;
            this->image_vector_d = image_vector;
        }

        inline int get_file_id(){return this->file_id;};
        inline std::vector<int> get_image_vector(){return this->image_vector_i;};
        inline std::vector<double> get_image_vector_d(){return this->image_vector_d;};
};

class image_array{
    private:
        int        img_s;
        bool       delete_flag; //Used to determined if the deletion of the image is required.
        image    **images;
        uint32_t   number_of_images;

        void delete_images();
    public:
        image_array();
        image_array(int k, bool flag=false);
        ~image_array();

        bool search_image(image *_image);

        void read_images(std::string file); 
        void update_images(std::string file);
        void add_image(int pos, image *_image);
        void add_image(int pos, std::vector<double> image_vector);

        inline int get_img_s(){return this->img_s;};
        inline void set_img_s(int img_s){this->img_s = img_s;};
        inline void update_image(int pos, image *new_image){this->images[pos] = new_image;};
        inline image **get_images(){return this->images;};
        inline uint32_t get_num_of_images(){return this->number_of_images;};
};

#endif