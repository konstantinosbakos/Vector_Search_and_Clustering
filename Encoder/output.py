import numpy           as np
from   preprocessing   import denormalise_df
from   print_functions import print_decoded_sample

def output_to_file(encoded,output_file):
    magic_num = 42                                             #Random magic number.
    size      = encoded.shape[0]                               #Size of the dataset.
    rows      = encoded.shape[1]                               #Image width.
    columns   = encoded.shape[2]                               #Image height.

    with open(output_file,'wb') as dat_file:                   #MNIST files are big endian.
        dat_file.write(magic_num.to_bytes(4, byteorder='big')) #Save the Magic_num, size, rows
        dat_file.write(     size.to_bytes(4, byteorder='big')) #and columns as the first 4 numbers
        dat_file.write(     rows.to_bytes(4, byteorder='big')) #in order to keep the same file
        dat_file.write(  columns.to_bytes(4, byteorder='big')) #structure.

        encoded.astype(np.uint8).tofile(dat_file)              #Copy the encoded images.
        #magic_num,size,rows,columns are saves as 4 bytes since they are 32 bit integers
        #in the MNIST files. The rest are saved as uint8 (0-255 values) since it is the
        #same size as an unsigned byte (0-255).

def encode_datasets(autoencoder,encoder,data_df,query_df):
    print_decoded_sample(autoencoder,data_df)

    encoded_train = encoder.predict(data_df)
    encoded_train = denormalise_df(encoded_train,255).reshape(encoded_train.shape[0],-1,1)

    encoded_test = encoder.predict(query_df)
    encoded_test = denormalise_df(encoded_test,255).reshape(encoded_test.shape[0],-1,1)

    print('New dataset  is of shape:', encoded_train.shape)    #Print the encoded dimensions.
    print('New queryset is of shape:', encoded_test.shape )

    return encoded_train,encoded_test
