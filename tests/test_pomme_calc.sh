#!/bin/zsh


curr_dir=$(dirname $PWD)
top_dir=$(dirname ${curr_dir})

exe_path="./pomme_calc"
data_path="inputs_201503.txt"

lat="21.19"
lon="158.0"
elev="0.0"

results_file="pomme_hon_201503_2.txt"
pomme_tmpfile="pom_tmp.txt"


echo "date X Y Z dst" > $results_file
{
  read
  while IFS=, read -r dates fYear est ist imf_by em f107  
  do
        # build commands and save im temporary file

        $exe_path -a $lat -o $lon -d $fYear -e $elev -E $est -I $ist -B $imf_by -R $f107 -m $em -T C > tmp.txt

        tail -n 1 tmp.txt > $pomme_tmpfile

        while IFS=, read -r fYear alt lat lon decline incline H X Y Z F
        do
          echo "$fYear,$X,$Y,$Z" >> $results_file

        done < $pomme_tmpfile

        break
  done
} < $data_path


# Open file descriptors for both files
exec 3<"pomme_HON.txt"
exec 4<$results_file

while IFS=, ; do
    read -r dyear x y z dst <&3
    read -r dyear px py pz dst<&4

    # Check if both files have reached EOF
    if [ -z "$x" ] && [ -z "$px" ]; then
        break
    fi

    if [ "$x" != "$px" ];
    then
        echo "$x and $px not equal\n"
    fi

    if [ "$y" != "$py" ];
    then
        echo "$y and $py not equal\n"
    fi

    if [ "$z" != "$px" ];
    then
        echo "$z and $pz not equal\n"
    fi
    break
done

# Close file descriptors
exec 3<&-
exec 4<&-
