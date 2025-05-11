#!/bin/bash

curr_dir=$PWD
top_dir=$(dirname ${curr_dir})
c_exe_file="${top_dir}/pomme/cpomme/build/pomme_calc"

echo $c_exe_file

inputs_file="${curr_dir}/pomme_inputs.csv"

echo $inputs_file

results_file="pomme_testvalues.txt"

echo "date,lat,lon,alt,x,y,z,h,f,dec,inc" > $results_file
{
  read
  while IFS=',' read -r date lat lon alt est ist imf_by f107 em
  do

     ${c_exe_file} -a ${lat} -o $lon -d $date -e $alt -E $est -I $ist -B $imf_by -R $f107 -m $em -T C > tmp.txt

     tail -n 1 tmp.txt > pomm_tmp.txt

     while IFS=, read -r fYear ALT LAT LON decline incline H X Y Z F
     do
       echo "${fYear},${LAT},${LON},${ALT},$X,$Y,$Z,$H,$F,${decline},${incline}" >> $results_file
     done < pomm_tmp.txt



  done
} < ${inputs_file}