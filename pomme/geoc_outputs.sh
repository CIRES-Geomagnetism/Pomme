#!/bin/bash

pomme_exe="./build/pomme_calc"
inp_file="pomme_inputs_202310_202409.txt"
out_file="pomme_202310_202409_30.csv"

pomme_tmpfile="pom_tmp.txt"
elev="0.0"

echo "decimal_year,lat,lon,Bt,Br,Bp" > $out_file
        
for ((lat=-90; lat<=90; lat+=30))
do  
    for (( lon=0; lon<=360; lon+=30 ))
    do
        tail -n +2 ${inp_file} | while IFS=, read -r dYear est ist imf_by em f107 dst
        do
           if [[ $lat == "90" ]]; then
                $pomme_exe -a 89.999 -o $lon -d $dYear -e $elev -E $est -I $ist -B $imf_by -R $f107 -m $em -T C > tmp.txt
           elif [[ $lat == "-90" ]]; then
                $pomme_exe -a -89.999 -o $lon -d $dYear -e $elev -E $est -I $ist -B $imf_by -R $f107 -m $em -T C > tmp.txt
                
           else
                $pomme_exe -a $lat -o $lon -d $dYear -e $elev -E $est -I $ist -B $imf_by -R $f107 -m $em -T C > tmp.txt
           fi

           tail -n 1 tmp.txt > $pomme_tmpfile

           while IFS=, read -r fYear alt latitude longtitude decline incline H Bt Bp Br F
           do
             echo "$fYear,$latitude,$longtitude,$Bt,$Bp,$Br" >> $out_file
            
           done < $pomme_tmpfile
        done  
       
    done
    
done
