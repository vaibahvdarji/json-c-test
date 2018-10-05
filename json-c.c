#include <json.h>
#include <stdio.h>
#include <string.h>

/*printing the value corresponding to boolean, double, integer and strings*/
void print_json_value(json_object *jobj){
  enum json_type type;
  type = json_object_get_type(jobj); /*Getting the type of the json object*/
  switch (type) {
    case json_type_boolean: printf("json_type_boolean\n");
                         printf("value: %s\n", json_object_get_boolean(jobj)? "true": "false");
                         break;
    case json_type_double: printf("json_type_double\n");
                        printf("          value: %lf\n", json_object_get_double(jobj));
                         break;
    case json_type_int: printf("json_type_int\n");
                        printf("          value: %d\n", json_object_get_int(jobj));
                         break;
    case json_type_string: printf("json_type_string\n");
                         printf("          value: %s\n", json_object_get_string(jobj));
                         break;
	default:
			printf("error\n");
  }

}

void json_parse_array( json_object *jobj, char *key) {
  void json_parse(json_object * jobj); /*Forward Declaration*/
  enum json_type type;

  json_object *jarray = jobj; /*Simply get the array*/
  if(key) {
		printf("key[%s]\n",key);
	json_object_object_get_ex(jobj, key, &jarray); /*Getting the array if it is a key value pair*/
  }

  int arraylen = json_object_array_length(jarray); /*Getting the length of the array*/
  printf("Array Length: %d\n",arraylen);
  int i;
  json_object * jvalue;

  for (i=0; i< arraylen; i++){
    jvalue = json_object_array_get_idx(jarray, i); /*Getting the array element at position i*/
    type = json_object_get_type(jvalue);
    if (type == json_type_array) {
      json_parse_array(jvalue, NULL);
    }
    else if (type != json_type_object) {
      printf("value[%d]: ",i);
      print_json_value(jvalue);
    }
    else {
      json_parse(jvalue);
    }
  }
}

/*Parsing the json object*/
void json_parse(json_object * jobj) {
  enum json_type type;
  json_object_object_foreach(jobj, key, val) { /*Passing through every array element*/
    printf("type: ");
    type = json_object_get_type(val);
    switch (type) {
      case json_type_boolean: 
      case json_type_double: 
      case json_type_int: 
      case json_type_string: print_json_value(val);
                           break; 
      case json_type_object: printf("json_type_object\n");
							json_object_object_get_ex(jobj, key, &jobj); /*Getting the array if it is a key value pair*/
                           json_parse(jobj); 
                           break;
      case json_type_array: printf("json_type_array, ");
                          json_parse_array(jobj, key);
                          break;
	default:
			printf("error\n");
    }
  }
} 

void
printKeyVal(json_object * jobj)
{
	char *key;
	struct json_object *val __attribute__((__unused__));

	printf("jobj from str:\n---\n%s\n---\n", json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));
	for(	struct lh_entry *entrykey = json_object_get_object(jobj)->head, *entry_nextkey = ((void *)0); \
			({ if(entrykey) { \
				key = (char*)entrykey->k; \
				val = (struct json_object*)entrykey->v; \
				entry_nextkey = entrykey->next; \
			} ; entrykey; }); \
			entrykey = entry_nextkey )
	{
		enum json_type type;
		printf("key[%s]-->",key);
		type = json_object_get_type(val);
	    switch (type) {
			case json_type_boolean:
			case json_type_double:
			case json_type_int:
			case json_type_string: print_json_value(val);
							   break;
			case json_type_object: printf("json_type_object\n");
							   json_object_object_get_ex(jobj, key, &jobj);
							   printKeyVal(jobj);
							   break;
			case json_type_array: printf("json_type_array, ");
							  json_parse_array(jobj, key);
							  break;
			default:
			printf("error\n");
	    }
	}
}
int main() {
	char * string = "{\"json\":{\"string\" : \"1234\",\r\n\"array\" : [ \"1\" , [\"21\" , \"22\" ], \"3\", \"4\" ],\r\n\"obj\": { \"bool\": false, \"float\" : 1.20, \"Number\" : 10 } }}\r\n";
	printf("JSON string: %s\n", string);
	json_object * jobj = json_tokener_parse(string);
	//struct json_tokener* toknr = json_tokener_new();

	//if(toknr != NULL)
	//{
		//jobj = json_tokener_parse_ex(toknr, string, strlen(string) );
		//while(jobj != NULL){
			//printf("%u\n",json_object_get_type(jobj) );
		//}
		//json_object_put(jobj );
	//}

	if (jobj != NULL)
	{
		printKeyVal(jobj);
		json_object_put(jobj);
	}
	else
	{
		printf("invalid json\n");
	}
}
