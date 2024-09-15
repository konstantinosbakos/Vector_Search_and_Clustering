# Vector Search and Clustering [![Project Status: Inactive – The project has reached a stable, usable state but is no longer being actively developed; support/maintenance will be provided as time allows.](https://www.repostatus.org/badges/latest/inactive.svg)](https://www.repostatus.org/#inactive) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Summary

This is a collection of algorithms that aim to cluster and search vectors of large dimentions, either in their original dimention (which is less time efficient since they will sparsely populate the space $R^{n}$, where $n$ is the dimention of the vectors, or in a latent dimension of choice.
If the vectors are projected to the latent dimension, the algorithms are ran in the former dimension and then the distances between the vectors are calculated in the original dimension.
The objective is to cluster the **input** vectors in such a way that any **query** vector will be matched with the (approximately) closest vector in the clustering, while being time efficient as well. 
The time and the results (distance of the selected closest neighbour of the query vector) of the algorithms are compared to the brute force method that essentially acts as a point of reference.

## Algorithms

There are four algorithms used and compared.

### LSH

Locality sensitive hashing (LSH) is an algorithm that hashes similare items into the same `hash buckets` with high probability. 

The hash function used is $g(p) = \sum^k_{i=1} r_ih_i(p) mod M\in[0,...,M)$, for some large and random $M\in\R$ and random $r_i\in\R$.  
$h_i(p)\in H$, where $H$ is a hash-function family, defined as;
$h(p) = \frac{p*v + t}{w}$, where $p$ is the vector being hashed, $v\in N(0,1)^n$ and $t \in U(0,w)$, where $w \in N^*$.

|Hyperparameters |                   Function                   |
|----------------|----------------------------------------------|
|$k$             |Number of h() functions used per g() function |
|$L$             |Number of hash tables                         |
|$R$             |Radius of search (e.g. if radius = 1000 then the algorithm will return all nodes that are in that distance from the query [distance is eucledian]) |
| **Parameters** |                                              |
|$N$             |Number of Nearest Neighbours returned         |

**Notes**:

- The algorithm return $N$ neighbours using the *LSH* method and all the neighbours that are within $R$ radius of the query.

### Hypercube

This is an algorithm that projects the data on a hypercube (a cube-like structure that spans multiple dimensions, hence the name). The probes (edges) of the hypercube act as hash buckets. Their name is a number from $0$ to $2^k+1$, where $n$ is a number such that all the vectors can fit in the hypercube, without any hash bucket overflowing (hash buckets have a maximum number of vectors that they can contain). The name of each probe is in binary.

The vectors are hashed using the same type of hash-function families $H$, as in LSH. After a vector is hashed, the output values are randomly assigned to 0 or 1 (e.g if $f(p) = [1,2,3]$ then $1$ is assigned to $0$, $2$ to $1$ and $3$ to $1$ **randomly**. If then $f(p')= [4,5,2]$, the numbers $4$ and $5$ are assigned to $0$ or $1$ randomly **but not** $2$, since it is already assigned to $1$).
After that, the vectors are "placed" in the corresponding probe of the hypercube, based on the hash-value they produced (in our example, since $p$ has the hash value of $[0,1,1]$, it would be place in the probe with the same name. In this example since we use three numbers to express the names of the hypercube we can also deduce that we have $2^3=8 + 1$ hash buckets from $[0,0,0]$ to  $[1,1,1]$).

|Hyperparameters |                   Function                   |
|----------------|----------------------------------------------|
|$k$             |Number of h() functions used per f() function and number of probes created ($2^k+1$)|
|$M$             |Number of singular node that will be checked  |
|$probes$        |Number of probes that ill be checked          |
|$R$             |Radius of search (e.g. if radius = 1000 then the algorithm will return all nodes that are in that distance from the query [distance is eucledian]) |
| **Parameters** |                                              |
|$N$             |Number of Nearest Neighbours returned         |

**Notes**:

- The hyperparameters $M$ and $probes$ interact, there is a chance that $M$ nodes will be checked before $probes$ edges get checked. The opposite is possible as well. It is up to the user and the input dataset to decide which is best.
- The algorithm return $N$ neighbours using the *Hypercube* method and all the neighbours that are within $R$ radius of the query.

### Clustering

This executable creates a graph of clusters using either the *[classic method (Lloyd's algorithm)](https://en.wikipedia.org/wiki/Lloyd%27s_algorithm)*, or the reverse search method (using either the *LSH* algorithm or the *Hypercube* algorithm to update).

The nodes are added to the graph one by one. The number of centroids is a hyperparameter $K$ that is set by the user. They are initialised using the *[k-means++ initialisation](https://en.wikipedia.org/wiki/K-means++#Improved_initialization_algorithm)*.

#### Classic Method (Lloyds)

This algorithm is the classic Lloyds method of updating a clustering. In every step a node is added to the cluster and the centroids are updated to represent the mean of their clusters. This is repeated untill all nodes are placed into the clusters.

#### Reverse search (Reverse kNN)
This algorithm runs the *LSH* or *Hypercube* method and then searches in a loop the approximate neighbours of the centroids (they are used as queries) and those neighbours are added to the respective centroid clusters. Afterwards any conflicts (nodes that are close to multiple centroids and where assigned to multiple clusters) are resolved by calculating the closest centroid to the node. To find conflicts, a one-hot vector is used that indicates in which cluster(s) a node is placed in (if it is placed in multiple clusters, then there are conficts). 

In the end, the centroids are updated to represent the mean of all the nodes in their cluster.

This is repeated untill none of the centroids change after an update. 

#### Silhouette Evaluation

For the clusterings created by any algorithm, the silhouette method is used in order to evalute it. The implementation is heavily inspired by [this paper](https://thescipub.com/pdf/jcssp.2008.252.255.pdf).

#### Hyperparameters

For this executable the user must provide a file in this exact format, in order to initialise the hyperparameters of the program:


`number_of_clusters: <int>              // K of K-means, default: 10`

`number_of_vector_hash_tables: <int>    // default: 30`

`number_of_vector_hash_functions: <int> // k of LSH for vectors, default: 5`

`max_number_M_hypercube: <int>          // M of Hypercube, default: 4700`

`number_of_hypercube_dimensions: <int>  // k of Hypercube, default: 8`

`number_of_probes: <int>                // probes of Hypercube, default: 90`


The user should replace the `<int>` positions with the number they want to provide. If no number is provided the default values will be used.
An example file is provided at `/input/conf/cluster.conf`.

### SoG (Search on Graph) with MRNG (Mutual Reciprocal Neighborhood Graph)

SoG is a simple algorithm that traverses the graph up to a radius that the user sets. The graph this algorithm traverses is created with the MRNG algorithm. Essencially MRNG uses a kNN representation of the LSH algorithm output to find the $l$ closest neighbours of each node of the graph.
For each cluster of the kNN output, a **Navigation Node** is selected. To select the navigation node, the mean (imaginary) vector of all the points in the cluster is calculated and then the closest real vector to the former is selected as the navigation node.
The algorithm runs *N* times, each time checking new *unchecked* nodes (up to $l$ nodes) and returns the closest one to the query.

|Hyperparameters |                   Function                   |
|----------------|----------------------------------------------|
|$l$             |Number of h() functions used per g() function |
| **Parameters** |                                              |
|$N$             |Number of Nearest Neighbours returned         |

### GNNS (Graph Neural Network Search)

This algorithm executes the search function directly on the LSH graph.
To initiate the search function it selects a random node (uniformally) and then walks $T$ steps on the graph, where $T$ is not a hyperparameter and it is dependent on the graph size. Each step the algorithm progresses to the node that is closest to the query. This is repeated *R* times.

Hyperparameters
|Hyperparameters |                   Function                   |
|----------------|----------------------------------------------|
|$k$             | Number of nearest neighbours in the kNN graph for each node |
|$E$             | Number of neighbours checked per step (when a new node is selected, $E$ neighbours are checked and the closest to the query is selected)|
|$R$             | Number of repetitions of the algorithm |
| **Parameters** |                                              |
|$N$             |Number of Nearest Neighbours returned         |


## Latent and Original Dimensions

The algorithms GNNS and SoG are executing the clustering procedures in a latent dimension of the data for better execution times. In the folder `/input/latent_dim_datasets/` you may find files of 10000, 1000 and 50 images in 30 dimensions (the original dimension of the MNIST dataset is $28*28=784$). The files that contain the same images in the original dimension can be found in the folder `/input/images`.
In order to reduce a file from its original dimension to a latent one, the python encoder may be used.
To run the executable please type `python reduce.py -d <dataset> -q <queryset>    -od <output_dataset_file> -oq <output_query_file>`.
The program runs on GPU and supports hypertuning to find the best parameter combination for the dimensionality reduction. As it was requested for the project, the hypertuning is manual as of now.
There is also the most optimal combination of parameters found for the MNIST dataset.

Regarding the clustering executable, that too uses latent and original dimensions and it creates the clusterings for both datasets.

## Compiling

To run the program navigate to the `/bin` folder and type one of the following:

- `make all`
- `make lsh` (for Hypercube)
- `make cube`
- `make graph_search` (for the GNNS and SoG algorithms)

## Running the executables

### lsh

`$./lsh -d <input file> -q <query file> -k <int> -L <int> -ο <output file> -Ν <number of nearest> R radius`

### cube

`./cube -d <input file> -q <query file> -k <int> -M <int> -probes <int> -ο<output file> -Ν <number of nearest> -R <radius>`

### cluster

`./cluster -il <input latent file> -c <configuration file> -io <input original file> -o <output file> -complete <optional> -m <method: Classic OR LSH or Hypercube>`

### SoG

`./graph_search -dl <input latent file> -ql <query latent file> -do <input original file> -qo <query original file> -N <int> -l <int> -m <2 for SoG> -o <output file>`  

### GNNS

`./graph_search -dl <input latent file> -ql <query latent file> -do <input original file> -qo <query original file> -k <int> -E <int> -R <int> -N <int> -m <1 for GNNS>  -o <output file>`
	
## Bash Script

In the `/compare algorithms` folder, there are two bash scripts that call the four algorithms with random hyperparameter combinations, aiming to find the best hyperparameter tuning for each algorithm on the input data.

To call the script, type `./random_hypertuning reruns<int> query<.dat> input<.dat>`, where the number of reruns is the number of times the algorithms will be called with a random combination of hyperparameters.

## Misc

- All the executable (and the bash script) can be called with the `-h` flag to print a help box for the user.
- For the implementation of the Hypercube algorithm, a [skip list](https://github.com/konstantinosbakos/skip_list) is used to store the values of the h(x) function.
- The `/input` folder contains an example of input for the algorithms (the [MNIST Dataset](https://www.kaggle.com/datasets/hojjatk/mnist-dataset)). If a different input is to be provided, please be sure to follow the file structure of the dataset specified. The query files should also have the same structure.
