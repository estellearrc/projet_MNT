#!/bin/bash
#Run this in terminal

mkdir build
cd build
cmake ..
make

./create_raster ../data/Guerledan_Feb19_50cm_wgs84.txt 800

firefox ../doc/html/index.html
#exit 0