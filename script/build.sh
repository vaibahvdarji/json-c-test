#!/bin/sh

WORKSPACE=$1
company=$2
date=$(date +%d'_'%m'_'%Y'_'%H'-'%M'-'%S)
release_dir="Release_$date"
release_path="/tmp/$release_dir"

mkdir -p $release_path

for proj in "strip" "lc52_pwm" "lc52_analog" "gateway"
do
	cd "$WORKSPACE"/proj/$proj/
	make clean_all all WALA=0 OUTDOOR_LIGHTS=0
	mkdir -p $release_path/$proj
	cp build/$company/"$proj"_"$company"_*.hex $release_path/$proj
	cp build/$company/parse_application_init.bin $release_path/$proj
	cp build/$company/parse_application.bin $release_path/$proj
	if [ "$proj" = "lc52_pwm" ]; then
		make clean_all all WALA=0 OUTDOOR_LIGHTS=1
		mkdir -p $release_path/$proj"_outdoor"
		cp build/$company/"$proj"_"$company"_*.hex $release_path/$proj"_outdoor"
		cp build/$company/parse_application_init.bin $release_path/$proj"_outdoor"
		cp build/$company/parse_application.bin $release_path/$proj"_outdoor"
	fi
	cd -
done

cd /tmp
zip -rq $release_dir.zip $release_dir
cd -
