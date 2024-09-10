#!/bin/bash

type=$1                                                      #LSH(0) or Cube(1).
input=$2                                                     #The output file.
comp_file=$3                                                 #The file to output the file processing.

#!!This is why runnning with R=1 for LSH and Cube is important!!

if ((type == 0))
then
    mapfile -t tAppx  < <(sed -ne '/^tLSH/p' $input)         #Get all the lines containing approx time.
    mapfile -t dAppx  < <(sed -ne '/^distance-LSH/p' $input) #Get all the lines containing approx distance.
else
    mapfile -t tAppx  < <(sed -ne '/^tCube/p' $input)
    mapfile -t dAppx  < <(sed -ne '/^distance-Cube/p' $input)
fi

mapfile -t tTrue < <(sed -ne '/^tTrue/p' $input)             #Get all the lines containing true time.
mapfile -t dTrue < <(sed -ne '/^distance-True/p' $input)     #Get all the lines containing true distance.

i=0
mAF=0
meanAppx=0
meanTrue=0

while (( i < ${#tAppx[@]} ))
do
    currAppx=$(echo ${tAppx[$i]} | tr -dc '0-9.')            #Isolate the number from the line.
    meanAppx=$(echo "$meanAppx + $currAppx" | bc)            #Add it to the mean.
   
    currTrue=$(echo ${tTrue[$i]} | tr -dc '0-9.')
    meanTrue=$(echo "$meanTrue + $currTrue" | bc)
   
    curr_dAppx=$(echo ${dAppx[$i]} | tr -dc '0-9.')
    curr_dTrue=$(echo ${dTrue[$i]} | tr -dc '0-9.')
    curr_AF=$(echo "scale=5 ; $curr_dAppx / $curr_dTrue" | bc)
    mAF=$(echo "$mAF + $curr_AF" | bc)

    i=$((i + 1))
done

tAvgAppx=$(echo "scale=6 ; $meanAppx / $i" | bc)s            #Calculate means.
tAvgTrue=$(echo "scale=6 ; $meanTrue / $i" | bc)s
mAF=$(echo "scale=5 ; $mAF / ${#tAppx[@]}" | bc)


echo "tAverageApproximate: 0$tAvgAppx" >> $comp_file         #Print results to the requested file.
echo "tAverageTrue:        0$tAvgTrue" >> $comp_file
echo "mAF:                 $mAF"       >> $comp_file

exit 0