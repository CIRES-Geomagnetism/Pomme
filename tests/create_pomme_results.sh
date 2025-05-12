#!/bin/zsh


curr_dir=$(dirname $PWD)
top_dir=$(dirname ${curr_dir})

exe_path="./pomme_calc"
data_path="inputs_201503.txt"

LAT="21.19"
LON="158.0"
ELEV="0.0"

results_file="pomme_hon_201503.txt"
pomme_tmpfile="pom_tmp.txt"


echo "date,lat,lon,elevation,X,Y,Z,H,F,decline,incline" > $results_file
{
  read
  while IFS=, read -r dates fYear est ist imf_by em f107
  do
        # build commands and save im temporary file

        $exe_path -a $LAT -o $LON -d $fYear -e $ELEV -E $est -I $ist -B $imf_by -R $f107 -m $em -T C > tmp.txt

        tail -n 1 tmp.txt > $pomme_tmpfile

        while IFS=, read -r fYear ELEV LAT LON decline incline H X Y Z F
        do
          echo "$fYear,${LAT},${LON},${ELEV},$X,$Y,$Z,$H,$F,${decline},${incline}" >> $results_file

        done < $pomme_tmpfile

  done
} < $data_path