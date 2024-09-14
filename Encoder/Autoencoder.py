import numpy        as     np
from   keras        import layers
from   tensorflow   import keras 
from   keras.models import Model

def Autoencoder(l_dim,shape,_filter_):
    #Input
    _input_     = keras.Input(shape)
    
    #Encoder
    conv1       = layers.Conv2D( 32,_filter_,activation='relu',padding='same')    (_input_)
    pool1       = layers.MaxPooling2D((2, 2))                                     (conv1)
    conv2       = layers.Conv2D( 64,_filter_,activation='relu',padding='same')    (pool1)
    pool2       = layers.MaxPooling2D((2, 2))                                     (conv2)
    conv3       = layers.Conv2D(128,_filter_,activation='relu',padding='same')    (pool2)
    conv_shape  = list(np.copy(conv3.shape))
    flattened   = layers.Flatten()                                                (conv3)
    encoder     = layers.Dense(l_dim,activation='relu')                           (flattened)

    #Decoder
    FC          = layers.Dense(np.prod(conv_shape[1:]),activation='relu')         (encoder)
    reshaped_FC = layers.Reshape(target_shape=conv_shape[1:],input_shape=FC.shape)(FC)
    conv4       = layers.Conv2D( 64,_filter_,activation='relu',padding='same')    (reshaped_FC)
    upsamp1     = layers.UpSampling2D((2,2))                                      (conv4)
    conv5       = layers.Conv2D( 32,_filter_,activation='relu',padding='same')    (upsamp1)
    upsamp2     = layers.UpSampling2D((2,2))                                      (conv5)
    decoder     = layers.Conv2D(  1,_filter_,activation='relu',padding='same')    (upsamp2)

    return Model(_input_,decoder),Model(_input_,encoder)

def train_model(train,valid,test,hyperparameters):
    best = {}
    best.update({'loss':100})

    #Hypertuning for the parameters given.
    for c_epochs in hyperparameters.get('epochs'):
        for l_dim in hyperparameters.get('l_dims'):
            for _filter_ in hyperparameters.get('filters'):
                for c_batch_size in hyperparameters.get('batch_size'):
                    print('-----------')                                                    #Print the parameters.
                    print('Running for: Epochs=\b',c_epochs,'\b, latent dimention=\b',l_dim,'\b, filter=\b',_filter_,'\b, batch size=\b',c_batch_size,'\b.')
                    print('-----------')

                    autoencoder,encoder = Autoencoder(l_dim,train[0].shape,_filter_)        #Create the models.
                    autoencoder.compile(optimizer='adam', loss='mean_squared_error')        #Compile them.
                    history             = autoencoder.fit(x=train, y=train,epochs=c_epochs, #Fit and save the results to history.
                        batch_size=c_batch_size,shuffle=True,validation_data=(valid,valid),
                    )

                    predict_loss = autoencoder.evaluate(test,test)                          #Evaluate on test set.
                    
                    if(predict_loss < best.get('loss')):                                    #Save the best model (lowest loss).
                        old_encoder = best.get('encoder')
                        old_history = best.get('history')
                        old_autoencoder = best.get('autoencoder')

                        del old_encoder
                        del old_history
                        del old_autoencoder

                        best.update({'l_dim':l_dim})
                        best.update({'encoder':encoder})
                        best.update({'history':history})
                        best.update({'loss':predict_loss})
                        best.update({'autoencoder':autoencoder})

                        best.update({'epochs':c_epochs})
                        best.update({'l_dim':l_dim})
                        best.update({'filter':_filter_})
                        best.update({'batch_size':c_batch_size})
                    else:
                        del encoder
                        del history
                        del autoencoder

                        keras.backend.clear_session()
    return best
