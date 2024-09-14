import sys
from   output                  import output_to_file,encode_datasets
from   Autoencoder             import Autoencoder,train_model
from   preprocessing           import get_cmd_input,create_np_df, normalize_df
from   print_functions         import ask_to_tune
from   sklearn.model_selection import train_test_split

def main():
    cmd_input_dict           = get_cmd_input(sys.argv)                    #Read the command line input.
    
    #Normalize the datasets (from [0,255] to [0,1])
    data_df                  = normalize_df(create_np_df(cmd_input_dict.get('dataset')))
    query_df                 = normalize_df(create_np_df(cmd_input_dict.get('queryset')))

    data_train, data_test    = train_test_split(data_df,test_size=0.10,random_state=42,shuffle=False)
    data_train, data_valid   = train_test_split(data_train,test_size=0.10,random_state=42,shuffle=False)
    
    flag = ask_to_tune()

    if flag == 1:
        hyperparameters = {}                                                 #Other values that were tested:
        hyperparameters.update({'epochs':[10,15]})                           #5,20,30
        hyperparameters.update({'l_dims':[20,30]})                           #5,10,15
        hyperparameters.update({'filters':[(3,3)]})                          #(4,4)
        hyperparameters.update({'batch_size':[8,16]})                        #32,64,128

        best_model = train_model(data_train,data_valid,data_test,hyperparameters)
                                                                            #Run hypertuning.

        print('Chosen Model       : Epochs:',best_model.get('epochs'),', batch size:',
            best_model.get('batch_size'),',filter:',best_model.get('filter'),
            ',latent dimension:',best_model.get('l_dim'),'.')
        print('Chosen Model\'s loss:', best_model.get('loss'))

        encoded_train,encoded_test = encode_datasets(best_model.get('autoencoder'),best_model.get('encoder'),data_df,query_df)

        output_to_file(encoded_train,cmd_input_dict.get('out_dataset_file')) #Create the latent datasets.
        output_to_file(encoded_test,cmd_input_dict.get('out_dataset_q_file'))

    elif flag == 2:
        autoencoder,encoder = Autoencoder(l_dim=30,shape=data_train[0].shape,_filter_=(3,3)) 
                                                                             #Create the models.
        autoencoder.compile(optimizer='adam', loss='mean_squared_error')     #Compile them.
        autoencoder.fit(x=data_train, y=data_train,epochs=15,                #Fit and save the results to history.
            batch_size=8,shuffle=True,validation_data=(data_valid,data_valid),
        )

        predict_loss = autoencoder.evaluate(data_test,data_test)             #Evaluate on test set.

        print('Chosen Model       : Epochs: 15, batch size:,filter: (3,3), latent dimension: 30.')
        print('Chosen Model\'s loss:', predict_loss)

        encoded_train,encoded_test = encode_datasets(autoencoder,encoder,data_df,query_df)

        output_to_file(encoded_train,cmd_input_dict.get('out_dataset_file')) #Create the latent datasets.
        output_to_file(encoded_test,cmd_input_dict.get('out_dataset_q_file'))

main()