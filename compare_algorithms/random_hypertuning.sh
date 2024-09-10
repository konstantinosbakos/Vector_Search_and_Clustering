#!/bin/bash

algo_runs=$1                                                     #Get the number of runs for each algorithm.
query_file=$2                                                    #Get the query file full path.
input_file=$3                                                    #Get the input file full path.

if (( algo_runs == "-h"))                                        #Check if help box is requested.
then
    echo " _________________________________________________________   "
    echo "|>To run the script please call it like this:             |  "
    echo "|./random_hypertuning reruns(int) query(.dat) input(.dat) |  "
    echo "|\"reruns\" is a number that indicates how many times to run|"
    echo "|each algorithm.                                          |  "
    echo " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   "
    exit 0                                                       #If yes, exit after printing it.
fi

(cd ../bin && make HTN=true)                                     #Make the executables for the algorithms.
(cd ../../bin && make lsh HTN=true)
(cd ../../bin && make cube HTN=true)

comp_file="output_comparison.txt"                                #Create an output file to store avg_time, true_time
                                                                 #and MAF for each run.

if [ -f "$comp_file" ]                                           #If such a file already exists,
then
    rm "$comp_file"                                              #delete it.
fi

if [ -d "output_logs" ]                                          #If the "output_logs" folder exists,
then
    rm -rf output_logs                                           #delete it.
fi

touch $comp_file                                                 #Create the output file.
mkdir output_logs                                                #Create the output folder.
mkdir output_logs/LSH                                            #Create a folder to save the output of each run of LSH.
mkdir output_logs/Cube                                           #Create a folder to save the output of each run of Cube.
mkdir output_logs/GNNS                                           #Create a folder to save the output of each run of GNNS.
mkdir output_logs/SoG                                            #Create a folder to save the output of each run of SoG.

echo -e

#LSH
echo "/~~~\\" >> $comp_file
echo "|LSH|"  >> $comp_file
echo "\~~~/"  >> $comp_file
echo -e       >> $comp_file

i=0
while (( i < algo_runs))
do
    echo "Running LSH $(($i + 1))..."

    output_file="output_logs/LSH/test_LSH_$(($i + 1)).txt"

    k=$(($RANDOM % 30 + 1))                                      #Choose k randomly between 1-30.
    L=$(($RANDOM % 30 + 1))                                      #Choose L randomly between 1-30.
    R=0                                                          #Range search is not relevant, so set R to 0.
    N=1                                                          #Only the first Neighbour is important
                                                                 #(to calculate MAF as instructed).
    ../../bin/lsh -k $k -L $L -R $R -N $N -d $input_file -q $query_file -o $output_file
                                                                 #Call the algorithm.
    echo "/~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" >> $comp_file
    echo "Run         : $(($i + 1))"      >> $comp_file
    echo "Running for :-k $k -L $L -N $N" >> $comp_file
    echo "------------------------------" >> $comp_file
    ./output_processing.sh 0 $output_file $comp_file
                                                                 #Call output_processing.sh to calculate MAF and mean times.
    echo "------------------------------" >> $comp_file
    echo "\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" >> $comp_file

    i=$(($i + 1))
done

echo -e

#Cube
echo -e        >> $comp_file
echo "/~~~~\\" >> $comp_file
echo "|Cube|"  >> $comp_file
echo "\~~~~/"  >> $comp_file
echo -e        >> $comp_file

i=0
while (( i < algo_runs))
do
    echo "Running Cube $(($i + 1))..."

    output_file="output_logs/Cube/test_Cube_$(($i + 1)).txt"

    k=$(($RANDOM % 0011     + 0005))                             #Choose k randomly between 5-15.
    M=$(($RANDOM % 3001     + 3000))                             #Choose M randomly between 3000-6000.
    probes=$(($RANDOM % 701 + 0100))                             #Choose probes randomly between 100-800.
    R=0
    N=1
    ../../bin/cube -k $k -M $M -probes $probes -R $R -N $N -d $input_file -q $query_file -o $output_file

    echo "/~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"     >> $comp_file
    echo "Run         : $(($i + 1))"                             >> $comp_file
    echo "Running for : -k $k -M $M -probes $probes -R $R -N $N" >> $comp_file
    echo "------------------------------"                        >> $comp_file
    ./output_processing.sh 1 $output_file $comp_file
                                                                 #Call output_processing.sh to calculate MAF and mean times.
    echo "------------------------------"                        >> $comp_file
    echo "\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"     >> $comp_file

    i=$(($i + 1))
done

echo -e

#GNNS
echo -e        >> $comp_file
echo "/~~~~\\" >> $comp_file
echo "|GNNS|"  >> $comp_file
echo "\~~~~/"  >> $comp_file
echo -e        >> $comp_file

i=0
while (( i < algo_runs))
do
    echo "Running GNNS $(($i + 1))..."

    output_file="output_logs/GNNS/test_GNNS_$(($i + 1)).txt"

    k=$(($RANDOM % 151 + 50))                                    #Choose k randomly between 50-200.

    E=0
    condition=$(($k/10))
    while (( E < condition || E == k))                           #If E < (k/10) or E = k, recalculate.
    do
        E=$(($RANDOM % $k + 1))
    done

    R=$(($RANDOM % 15 + 1))                                      #Choose R randomly between 1-15.
    N=1

    ../bin/graph_search -m 1 -k $k -E $E -R $R -N $N -d $input_file -q $query_file -o $output_file

    echo "/~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"  >> $comp_file
    echo "Run         : $(($i + 1))"               >> $comp_file
    echo "Running for : -k $k -E $E -R $R -N $N"   >> $comp_file
    echo "-------------------------------"         >> $comp_file
    tail -n 3 $output_file                         >> $comp_file #Get the last 3 lines of the output file
                                                                 #of the algorithm's output (tAvg, tTrue, MAF).
    echo "-------------------------------"         >> $comp_file
    echo "\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"  >> $comp_file

    i=$(($i + 1))
done

echo -e

#SoG
echo -e       >> $comp_file
echo "/~~~\\" >> $comp_file
echo "|SoG|"  >> $comp_file
echo "\~~~/"  >> $comp_file
echo -e       >> $comp_file

i=0
while (( i < algo_runs))
do
    echo "Running SoG $(($i + 1))..."

    output_file="output_logs/SoG/test_SoG_$(($i + 1)).txt"

    N=1
    l=$(($RANDOM % 501 + 100))                                    #Choose l randomly between 100-500.

    ../bin/graph_search -m 2 -l $l -N $N -d $input_file -q $query_file -o $output_file

    echo "/~~~~~~~~~~~~~~~~~~~~~~~~"       >> $comp_file
    echo "Run         : $(($i + 1))"       >> $comp_file
    echo "Running for : -l $l -N $N"       >> $comp_file
    echo "-------------------------------" >> $comp_file
    tail -n 3 $output_file                 >> $comp_file
    echo "-------------------------------" >> $comp_file
    echo "\~~~~~~~~~~~~~~~~~~~~~~~~"       >> $comp_file

    i=$(($i + 1))
done

echo -e

(cd ../bin && make clean)                                        #Clean files.
(cd ../../bin && make clean)

exit 0