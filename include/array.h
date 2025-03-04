#pragma once
 
#ifdef ARRAYS_IMPLEMENTATION
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#endif
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
 

#ifdef ARRAYS_IMPLEMENTATION

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
	
	 
	size_t left = inst.capacity - inst.len  ;
	printf("ins.str:%s\n",s);
	printf("b.str:%s\n",inst.data);
	print_array(arr);
	printf("ins: cap:%d, b.len:%d, left:%d, s.len:%d, pos:%d \n",
		inst.capacity,
		inst.len,
		left,
		len,
		pos
	);
	if(len >= left)
	{
		size_t new_cap = inst.capacity * 2 ;
		if( inst.capacity / len == 0)
		{
			new_cap = len * 2 ;
		}
	 

		//printf("new cap:%d\n",new_cap);
		inst.data = realloc(inst.data,new_cap);
		inst.capacity = new_cap;
	
	}
	if(pos < inst.len)
	{

		memcpy(&inst.data[inst.len],&inst.data[pos] , len);
		memcpy(&inst.data[pos], s, len);
		
	}else {
		memcpy(&inst.data[pos], s, len);
	}
	inst.len+= len;
 
	*arr = inst;
};

void array_free(Array arr [static 1]){
	if( arr->data)
	{
		free(arr->data);
		
	}
	*arr=(Array){0};
}
 
  
#endif

 