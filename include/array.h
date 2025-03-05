#pragma once
 
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 
#include <stdarg.h>

 
typedef struct{
	float  * data;
	size_t len;
	size_t capacity ;
	
}Array; //this is a string ref



Array array_create(size_t size);

void array_append(Array arr [static 1],float * data,size_t len);
 
void array_insert(Array arr [static 1],float * data, size_t len, size_t pos);

void array_free(Array arr [static 1]);



#ifdef ARRAY_IMPLEMENTATION
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 
#include <stdarg.h>
 
 
  
Array array_create(size_t size){
 
	Array  arr ={0};
	
	arr.data = calloc(size,sizeof(float));
	if(arr.data)
	{
		arr.capacity = size;
	}
	return arr;
}
 
void array_append(Array arr [static 1], float s[static 1],size_t len){
	printf("array_append\n");

	array_insert(arr,s,len,arr->len);
}
 
static void print_array(Array arr [static 1]){
	printf("print_buffer\n");
	int len = arr->len;
	for(int i=0;i<len;i++)
	{
		printf("%02x ",arr->data[i]);
	}
	printf("\n");
	
}

void array_insert(Array arr [static 1], float s[static 1],size_t len,size_t pos){
	printf("array_insert\n");
	
	 
	size_t left = arr->capacity - arr->len  ;
	printf("ins.str:%s\n",s);
	printf("b.str:%s\n",arr->data);
	print_array(arr);
	printf("ins: cap:%d, b.len:%d, left:%d, s.len:%d, pos:%d \n",
		arr->capacity,
		arr->len,
		left,
		len,
		pos
	);
	if(len >= left)
	{
		size_t new_cap = arr->capacity * 2 ;
		if( arr->capacity / len == 0)
		{
			new_cap = len * 2 ;
		}
	 

		//printf("new cap:%d\n",new_cap);
		arr->data = realloc(arr->data,new_cap);
		arr->capacity = new_cap;
	
	}
	if(pos < arr->len)
	{

		memcpy(&arr->data[arr->len],&arr->data[pos] , len);
		memcpy(&arr->data[pos], s, len);
		
	}else {
		memcpy(&arr->data[pos], s, len);
	}
	arr->len+= len;
 
 
};

void array_free(Array arr [static 1]){
	if( arr->data)
	{
		free(arr->data);
		
	}
	*arr=(Array){0};
}
 
   
#endif