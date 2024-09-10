#include <fstream>

#include "image.hpp"

//~Used only in this file~//

bool endianness(){ //True if the machine is little endian.
    uint16_t num = 1;
    return (*((char*) &(num)));
}

//~Private~//

void image_array::delete_images(){
    if(this->delete_flag){
        if(this->images){
            for(int i=0; i<int(this->number_of_images); i++){
                delete this->images[i];
                this->images[i] = NULL;
            }
            delete [] this->images;
            this->images = NULL;
        }
    }
    else{
        if(this->images){
            for(int i=0; i<int(this->number_of_images); i++){
                this->images[i] = NULL;
            }
            delete [] this->images;
            this->images = NULL;
        }
    }
}

//~Public~//

image_array::image_array(){
    this->images           = NULL;
    this->delete_flag      = true;
    this->number_of_images = -1;
}

image_array::image_array(int k, bool flag){
    this->number_of_images = k;
    this->delete_flag      = flag;
    this->images           = new image*[k];

    for(int i=0; i<k; i++){
        this->images[i] = NULL;
    }
}

image_array::~image_array(){
    this->delete_images();
}

bool image_array::search_image(image *_image){
    for(int i=0; i<int(this->number_of_images); i++){
        if(_image == this->images[i]){
            return true;
        }
        if(this->images[i] == NULL){
            return false;
        }
    }

    return false;
}

void image_array::read_images(std::string file){
    std::ifstream ifile(file,std::ios::binary);             //Open the file to read (in binary mode).

    if(ifile.is_open()){                                    //Check if the file is open.
        uint32_t img_h  = 0;
        uint32_t img_w  = 0;
        uint32_t dummy1 = 0;                                //Check ReadMe.

        ifile.read((char*)&dummy1,sizeof(dummy1));
        ifile.read((char*)&this->number_of_images,sizeof(this->number_of_images)); 
                                                            //Get the number of images contained.
        ifile.read((char*)&img_h,sizeof(img_h));
        ifile.read((char*)&img_w,sizeof(img_w));
        if(endianness()){                                   //The number is contained in big endian order.
                                                            //If the system is little endian, swap the bytes of the number.
            this->number_of_images = __builtin_bswap32(this->number_of_images);
            img_h                  = __builtin_bswap32(img_h);
            img_w                  = __builtin_bswap32(img_w);

        }

        this->img_s  = img_h * img_w;
        this->images = new image*[this->number_of_images];  //Create the image array.

        for(int i=0; i<int(this->number_of_images); i++){
            std::vector<int> curr_image;

            curr_image.resize(this->get_img_s());

            for(int j=0; j<this->get_img_s(); j++){
                unsigned char byte = 0;

                ifile.read((char*)&byte,sizeof(byte));
                curr_image[j]= (int)byte;
            }

            this->images[i] = new image(i,curr_image);      //Save the position in the file as ID,
                                                            //and the image vector in the array.
        }
        ifile.close();
    }
}

void image_array::update_images(std::string file){
    this->delete_images();                                  //Delete current images.
    this->read_images(file);                                //Read new images.
}

void image_array::add_image(int pos, image *_image){
    this->images[pos] = _image;
}

void image_array::add_image(int pos, std::vector<double> image_vector){
    this->images[pos] = new image(pos, image_vector);
}
