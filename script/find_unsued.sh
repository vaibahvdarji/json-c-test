#!/bin/sh

if [ -e unused.txt ];then
rm unused.txt
fi

unset file
for file in $(find app/src)
do
	list=$(ctags -x $file |sed -n 's/function.*//p')
	for i in $list
	do
		l=$(grep -rwn $i app/src|grep -v $file | wc -l)
		if [ $l -eq 0 ]
		then
			j=$(grep -wn $i $file|wc -l)
			if [ $j -eq 1 ]; then
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
