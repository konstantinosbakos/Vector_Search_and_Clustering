#*********************************************************************************************

HTN       ?= false
CXX        = g++
CXXFLAGS   = -Wall -g -O3 -std=c++11#-fsanitize=address
HCXXFLAGS  = -Wall -g -O3 -std=c++11 -D NLOOP#-fsanitize=address

#*********************************************************************************************

all: lsh cube cluster graph_search

#*********************************************************************************************

lsh: main_LSH.o f.o g.o h.o LSH.o image.o hash_table.o LSH_output.o brute_force.o\
                               UI_messages.o skip_node.o LSH_var.o math_functions.o\
                               skip_list_public.o skip_list_private.o
	$(CXX) $(CXXFLAGS) -o lsh main_LSH.o f.o g.o h.o image.o LSH.o hash_table.o\
                               LSH_output.o brute_force.o UI_messages.o skip_node.o\
                               LSH_var.o math_functions.o skip_list_public.o\
                               skip_list_private.o

#*********************************************************************************************

cube: main_cube.o f.o g.o h.o cube.o image.o hash_table.o cube_output.o brute_force.o\
                               UI_messages.o skip_node.o math_functions.o cube_var.o\
                               skip_list_public.o skip_list_private.o
	$(CXX) $(CXXFLAGS) -o cube main_cube.o f.o g.o h.o cube.o image.o hash_table.o\
                               cube_output.o brute_force.o UI_messages.o skip_node.o\
                               math_functions.o cube_var.o skip_list_public.o\
                               skip_list_private.o

#*********************************************************************************************

cluster: main_cluster.o CLS.o LSH.o f.o g.o h.o cube.o image.o Lloyd.o cluster.o centroid.o\
                               hash_table.o macqueen.o UI_messages.o skip_node.o k_means_pp.o\
                               silhouette.o k_means_obj.o math_functions.o skip_list_public.o\
                               skip_list_private.o cluster_var.o reverse_search.o
	$(CXX) $(CXXFLAGS) -o cluster main_cluster.o CLS.o LSH.o f.o g.o h.o cube.o image.o\
                               Lloyd.o cluster.o centroid.o hash_table.o macqueen.o\
                               UI_messages.o skip_node.o k_means_pp.o silhouette.o\
							   k_means_obj.o math_functions.o skip_list_public.o\
							   cluster_var.o skip_list_private.o reverse_search.o

#***********************************************************************************************

graph_search: SoG.o GNNS.o main_graph_search.o graph_search_vars.o algorithm.o g.o h.o f.o LSH.o\
                               image.o hash_table.o brute_force.o UI_messages.o skip_node.o\
                               math_functions.o skip_list_public.o skip_list_private.o
	$(CXX) $(CXXFLAGS) -o graph_search SoG.o GNNS.o main_graph_search.o graph_search_vars.o algorithm.o\
                               g.o h.o f.o LSH.o image.o hash_table.o brute_force.o\
                               UI_messages.o skip_node.o math_functions.o\
                               skip_list_public.o skip_list_private.o

#*********************************************************************************************

main_LSH.o: main_LSH.cpp
ifeq ($(HTN),false)
		$(CXX) $(CXXFLAGS) -c main_LSH.cpp
else
		$(CXX) $(HCXXFLAGS) -c main_LSH.cpp
endif
main_cube.o: main_cube.cpp
ifeq ($(HTN),false)
		$(CXX) $(CXXFLAGS) -c main_cube.cpp
else
		$(CXX) $(HCXXFLAGS) -c main_cube.cpp
endif
SoG.o: ../src/SoG/SoG.cpp
		$(CXX) $(CXXFLAGS) -c ../src/SoG/SoG.cpp
main_cluster.o: main_cluster.cpp
		$(CXX) $(CXXFLAGS) -c main_cluster.cpp
g.o: ../src/f_g_h/g.cpp
		$(CXX) $(CXXFLAGS) -c ../src/f_g_h/g.cpp
h.o: ../src/f_g_h/h.cpp
		$(CXX) $(CXXFLAGS) -c ../src/f_g_h/h.cpp
f.o: ../src/f_g_h/f.cpp
		$(CXX) $(CXXFLAGS) -c ../src/f_g_h/f.cpp
LSH.o: ../src/LSH/LSH.cpp
		$(CXX) $(CXXFLAGS) -c ../src/LSH/LSH.cpp
CLS.o: ../src/CLS/CLS.cpp
		$(CXX) $(CXXFLAGS) -c ../src/CLS/CLS.cpp
GNNS.o: ../src/GNNS/GNNS.cpp
		$(CXX) $(CXXFLAGS) -c ../src/GNNS/GNNS.cpp
cube.o: ../src/cube/cube.cpp
		$(CXX) $(CXXFLAGS) -c ../src/cube/cube.cpp
image.o: ../src/image/image.cpp
		$(CXX) $(CXXFLAGS) -c ../src/image/image.cpp
main_graph_search.o: main_graph_search.cpp
ifeq ($(HTN),false)
		$(CXX) $(CXXFLAGS) -c main_graph_search.cpp
else
		$(CXX) $(HCXXFLAGS) -c main_graph_search.cpp
endif
Lloyd.o: ../src/CLS/Lloyd/Lloyd.cpp
		$(CXX) $(CXXFLAGS) -c ../src/CLS/Lloyd/Lloyd.cpp
graph_search_vars.o: ../src/graph_search_vars/graph_search_vars.cpp
		$(CXX) $(CXXFLAGS) -c ../src/graph_search_vars/graph_search_vars.cpp
algorithm.o: ../src/algorithm/algorithm.cpp
		$(CXX) $(CXXFLAGS) -c ../src/algorithm/algorithm.cpp
cluster.o: ../src/CLS/cluster/cluster.cpp
		$(CXX) $(CXXFLAGS) -c ../src/CLS/cluster/cluster.cpp
macqueen.o: ../src/CLS/macqueen/macqueen.cpp
		$(CXX) $(CXXFLAGS) -c ../src/CLS/macqueen/macqueen.cpp
centroid.o: ../src/CLS/centroid/centroid.cpp
		$(CXX) $(CXXFLAGS) -c ../src/CLS/centroid/centroid.cpp
hash_table.o: ../src/hash_table/hash_table.cpp
		$(CXX) $(CXXFLAGS) -c ../src/hash_table/hash_table.cpp
LSH_output.o: ../src/output/LSH/LSH_output.cpp
		$(CXX) $(CXXFLAGS) -c ../src/output/LSH/LSH_output.cpp
brute_force.o: ../src/brute_force/brute_force.cpp
		$(CXX) $(CXXFLAGS) -c ../src/brute_force/brute_force.cpp
cube_output.o: ../src/output/cube/cube_output.cpp
		$(CXX) $(CXXFLAGS) -c ../src/output/cube/cube_output.cpp
UI_messages.o: ../src/UI_messages/UI_messages.cpp
		$(CXX) $(CXXFLAGS) -c ../src/UI_messages/UI_messages.cpp
skip_node.o: ../src/cube/skip_list/skip_node.cpp
		$(CXX) $(CXXFLAGS) -c ../src/cube/skip_list/skip_node.cpp
k_means_pp.o: ../src/CLS/k_means_pp/k_means_pp.cpp
		$(CXX) $(CXXFLAGS) -c ../src/CLS/k_means_pp/k_means_pp.cpp
silhouette.o: ../src/CLS/silhouette/silhouette.cpp
		$(CXX) $(CXXFLAGS) -c ../src/CLS/silhouette/silhouette.cpp
k_means_obj.o: ../src/CLS/k_means_pp/k_means_obj.cpp
		$(CXX) $(CXXFLAGS) -c ../src/CLS/k_means_pp/k_means_obj.cpp
LSH_var.o: ../src/main_var_classes/LSH/LSH_var.cpp
		$(CXX) $(CXXFLAGS) -c ../src/main_var_classes/LSH/LSH_var.cpp
math_functions.o: ../src/math_functions/math_functions.cpp
		$(CXX) $(CXXFLAGS) -c ../src/math_functions/math_functions.cpp
cube_var.o: ../src/main_var_classes/cube/cube_var.cpp
		$(CXX) $(CXXFLAGS) -c ../src/main_var_classes/cube/cube_var.cpp
skip_list_public.o: ../src/cube/skip_list/skip_list_public.cpp
		$(CXX) $(CXXFLAGS) -c ../src/cube/skip_list/skip_list_public.cpp
skip_list_private.o: ../src/cube/skip_list/skip_list_private.cpp
		$(CXX) $(CXXFLAGS) -c ../src/cube/skip_list/skip_list_private.cpp
cluster_var.o: ../src/main_var_classes/CLS/cluster_var.cpp
		$(CXX) $(CXXFLAGS) -c ../src/main_var_classes/CLS/cluster_var.cpp
reverse_search.o: ../src/CLS/reverse_search/reverse_search.cpp
		$(CXX) $(CXXFLAGS) -c ../src/CLS/reverse_search/reverse_search.cpp

#*********************************************************************************************

clean:
	rm -rf lsh cube cluster graph_search *o *txt
