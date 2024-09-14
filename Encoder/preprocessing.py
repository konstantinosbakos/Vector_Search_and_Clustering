import numpy   as np
import struct
from   os.path import exists

def normalize_df(df):                                                         #Divide using the max value to contain all values between 0 and 1.
    return (df.astype('float64')/df.max().astype('float64'))

def denormalise_df(df,new_max):                                               #Convert to new max, maintaining the distribution correctly.
    return np.round(((df - df.min())*new_max)/(df.max() - df.min()))

def help():
    print(' __________________________________________________ ')
    print('|>reduce.py -d <dataset> -q <queryset>             |')
    print('| -od <output_dataset_file> -oq <output_query_file>|')
    print(' ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ')

def get_cmd_input(argv):                                                      #Get the command line input.
    if '-h' in argv:
        help()
        exit()
    elif '-d' in argv and '-q' in argv and '-od' in argv and '-oq' in argv:
        cmd_input_dict = {}

        cmd_input_dict['dataset']            = argv[argv.index('-d')  + 1]
        cmd_input_dict['queryset']           = argv[argv.index('-q')  + 1]
        cmd_input_dict['out_dataset_file']   = argv[argv.index('-od') + 1]
        cmd_input_dict['out_dataset_q_file'] = argv[argv.index('-oq') + 1]

        return cmd_input_dict
    else:
        print('Please provide the command line input correctly.')
        help()
        exit()

def create_np_df(file_path):                                                  #Read MNIST file.
    if exists(file_path):
        with open(file_path,'rb') as dataset:
            _, size, rows, cols = struct.unpack('>IIII', dataset.read(16))    #First 4 numbers are 16 bytes.
            df                  = np.fromfile(dataset, dtype=np.dtype(np.uint8
                                                                     ).newbyteorder('>')
                                             ).reshape((size, rows, cols, 1)) #Read as big endian and reshape 
                                                                              #to the correct dimensions.
        return df
    else:
        print('File does not exist')                                          #If file does not exist print the error.
        return -1
