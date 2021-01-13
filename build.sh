#!/bin/bash
#Run this in terminal
#+ Command to compile c++ program. here i used common one
#Use: chmod u+x build.sh to compile
mkdir build
cd build
cmake ..
make

./create_raster ../doc/Guerledan_Feb19_50cm_wgs84.txt 800

cd images
#exit 0