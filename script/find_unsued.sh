#!/bin/sh

if [ -e unused.txt ];then
rm unused.txt
fi

unset file
for file in $(find app/src)
do
	filef=0
	list=$(ctags -x $file |sed -n 's/variable.*//p')
	for i in $list
	do
		l=$(grep -rwn $i app/src|grep -v $file | wc -l)
		if [ $l -eq 0 ]
		then
			j=$(grep -wn $i $file|wc -l)
			if [ $j -eq 1 ]; then
				if [ $filef -eq 0 ];then 
					echo ========================================= >> unused.txt
					echo $file >> unused.txt
					echo ========================================= >> unused.txt
				fi
				filef=1
				echo $i >> unused.txt
			fi
		fi
	done
done
for func in $(cat unused.txt)
do
	echo "function -> $func"
	grep -rwn $func app/src
	echo "===="
done
