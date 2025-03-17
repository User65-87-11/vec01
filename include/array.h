#pragma once

#include <assert.h>
#include <stdlib.h>
typedef struct Array Array;



void array_create(Array ** ref ,size_t elem_num);
void array_free(Array ** ref);
void array_inset_at(Array * ref,size_t pos, void * data);
void array_rem(Array * ref, size_t pos);
void array_get(Array * ref, size_t pos);
void array_push(Array * ref,void * data);
void array_pop(Array * ref,void * data);
void array_resize(Array * ref,size_t size);




typedef struct Array {
	size_t length;
	size_t cap;
	size_t data_sz;
	void * data;

} Array;





void array_create(Array ** ref ,size_t elem_num){
	assert(*ref == nullptr);

	*ref = calloc(1, elem_num);
	Array *a = (*ref);
	a->cap = elem_num;
	a->length = elem_num;
 
	
}
void array_free(Array ** ref){
	assert(*ref);
	free(*ref);
}
void array_inset_at(Array * ref,size_t pos, void * data){
	assert(ref->length >= pos);

	
}
void array_push(Array * ref,void * data);
void array_pop(Array * ref,void * data);
void array_resize(Array * ref,size_t size);
