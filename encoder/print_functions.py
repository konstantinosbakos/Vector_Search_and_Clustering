import matplotlib.pyplot as     plt
from   matplotlib        import pylab
from   preprocessing     import denormalise_df

def ask_to_tune():
    print(' _________________________ ')
    print('|>Run hypertuning      (1)|')
    print('| or                      |')
    print('| Output latent datasets  |')
    print('| using the best parameter|')
    print('| combination          (2)|')
    print(' ~~~~~~~~~~~~~~~~~~~~~~~~~ ')
    return int(input('[1/2]: '))

def print_subplot(img,x,y,pos):          #Print a single subplot.
    subp = plt.subplot(x,y,pos)

    plt.imshow(img.reshape(28, 28))
    plt.gray()

    subp.get_xaxis().set_visible(False)
    subp.get_yaxis().set_visible(False)

def print_decoded_sample(autoencoder,df): #Print the actual and the decoded images.
    decoded = denormalise_df(autoencoder.predict(df),255) 
                                         #[0-1] -> [0-255].

    fig = pylab.gcf()
    fig.canvas.manager.set_window_title('Actual and Decoded Image Comparison.')
    fig.subplots(2,5)

    for i in range(5):
        print_subplot(df[i],2,5,i+1)
    for i in range(5):
        print_subplot(decoded[i],2,5,i+6)

    plt.show()
