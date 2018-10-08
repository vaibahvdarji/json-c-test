all: 
	gcc src/json.c -I/usr/include/json-c -ljson-c -o json
