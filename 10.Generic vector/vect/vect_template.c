#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_STR_LEN 64

typedef struct Vector {
	void *data;
	size_t element_size;
	size_t size;
	size_t capacity;
} Vector;

typedef struct Person {
	int age;
	char first_name[MAX_STR_LEN];
	char last_name[MAX_STR_LEN];
} Person;

typedef int(*cmp_ptr)(const void*, const void*);
typedef int(*predicate_ptr)(void*);
typedef void(*read_ptr)(void*);
typedef void(*print_ptr)(const void*);

// Allocate vector to initial capacity (block_size elements),
// Set element_size, size (to 0), capacity
void init_vector(Vector *vector, size_t block_size, size_t element_size) {
    vector->data = malloc(block_size*1000);
//    vector->data = malloc(element_size);
    vector->element_size = element_size;
    vector->size = 0;
    vector->capacity = 1;
}

// If new_capacity is greater than the current capacity,
// new storage is allocated, otherwise the function does nothing.
void reserve(Vector *vector, size_t new_capacity) {
    if(new_capacity > vector->capacity){
        vector->capacity = vector->capacity*2;
//        vector->data = realloc(vector->data,vector->capacity*vector->element_size);
    }
}

// Resizes the vector to contain new_size elements.
// If the current size is greater than new_size, the container is
// reduced to its first new_size elements.

// If the current size is less than new_size,
// additional zero-initialized elements are appended
void resize(Vector *vector, size_t new_size) {
    if(vector->size > new_size){
        vector->size = new_size;
        vector->capacity = new_size;
//        vector->data = realloc(vector->data,vector->capacity*vector->element_size);
    } else {
//        vector->data = realloc(vector->data,new_size);
        void *ptr = vector->data + vector->element_size*vector->size;
        for(int i=(int)vector->size;i<new_size;i++){
            *(int*)ptr = 0;
            ptr += vector->element_size;
        }
        vector->size = new_size;
        vector->capacity = new_size;
//        vector->data = realloc(vector->data,vector->capacity*vector->element_size);
    }
}

// Add element to the end of the vector
void push_back(Vector *vector, void *value) {
//    resize(vector,vector->size+1);
    reserve(vector,vector->size+1);
    void *curr_address = vector->data + vector->size*vector->element_size;
    memcpy(curr_address,value,vector->element_size);
//    *curr_address = *(int*)value;
    vector->size += 1;
}

// Remove all elements from the vector
void clear(Vector *vector) {
    vector->size = 0;
    vector->capacity = 1;
//    vector->data = realloc(vector->data,vector->capacity*vector->element_size);
}

// Insert new element at index (0 <= index <= size) position
void insert(Vector *vector, size_t index, void *value) {
//    resize(vector,vector->size+1);
    reserve(vector,vector->size+1);
    void *ptr = vector->data;
    memmove(ptr + vector->element_size * (index+1),ptr + vector->element_size * index,vector->element_size*(vector->size-index));
    memcpy(ptr + vector->element_size * index,value,vector->element_size);
    vector->size += 1;
}

// Erase element at position index
void erase(Vector *vector, size_t index) {
    void *ptr = vector->data;
    memmove(ptr+vector->element_size * index,ptr + vector->element_size*(index+1),vector->element_size*(vector->size - index));
    vector->size -= 1;
}

// Erase all elements that compare equal to value from the container
void erase_value(Vector *vector, void *value, cmp_ptr cmp) {
    void *ptr = vector->data;
    int counter = 0;
    for(int i=0;i<vector->size;i++){
        if(cmp(ptr,value) == 0){
            memmove(ptr,ptr + vector->element_size,vector->element_size*(vector->size-i));
//            vector->size -= 1;
            counter += 1;
//            printf("%c %c\n",*(char*)ptr,*(char*)value);
        } else {
            ptr += vector->element_size;
        }
    }
    vector->size -= counter;
}

// Erase all elements that satisfy the predicate from the vector
void erase_if(Vector *vector, int (*predicate)(void *)) {
    void *ptr = vector->data;
    int counter = 0;
    for(int i=0;i<vector->size;i++){
        if(predicate(ptr)){
            memmove(ptr,ptr + vector->element_size,vector->element_size*(vector->size-i));
//            vector->size -= 1;
            counter += 1;
        } else {
            ptr += vector->element_size;
        }
    }
    vector->size -= counter;
}

// Request the removal of unused capacity
void shrink_to_fit(Vector *vector) {
    vector->capacity = vector->size;
//    vector->data = realloc(vector->data,vector->capacity*vector->element_size);
}

// integer comparator
int int_cmp(const void *v1, const void *v2) {
    int* val_1 = (int*)v1;
    int* val_2 = (int*)v2;
    if(*val_1 == *val_2){
        return 0;
    } else if(*val_1 > *val_2){
        return 1;
    } else {
        return -1;
    }
}

// char comparator
int char_cmp(const void *v1, const void *v2) {
    char* c1 = (char*)v1;
    char* c2 = (char*)v2;
    if(*c1 == *c2){
        return 0;
    } else if(c1 > c2){
        return 1;
    } else {
        return -1;
    }
}

// Person comparator:
// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int person_cmp(const void *p1, const void *p2) {
    Person *P1 = (Person*)p1;
    Person *P2 = (Person*)p2;
    if(P1->age < P2->age){
        return 1;
    } else if(P1->age > P2->age){
        return -1;
    } else {
        if(strcmp(P1->first_name,P2->first_name) > 0){
            return 1;
        } else if(strcmp(P1->first_name,P2->first_name) < 0){
            return -1;
        } else {
            if(strcmp(P1->last_name,P2->last_name) > 0){
                return 1;
            } else if(strcmp(P1->last_name,P2->last_name) < 0){
                return -1;
            } else {
                return 0;
            }
        }
    }
}

// predicate: check if number is even
int is_even(void *value) {
    int *v = (int*)value;
    if(*v%2 == 0){
        return 1;
    } else {
        return 0;
    }
}

// predicate: check if char is a vowel
int is_vowel(void *value) {
    char* c = (char*)value;
    int lowercase_vovel = (*c == 'a' || *c == 'e' || *c == 'i' || *c == 'o' || *c == 'u' || *c == 'y');
    int uppercase_vovel = (*c == 'A' || *c == 'E' || *c == 'I' || *c == 'O' || *c == 'U' || *c == 'Y');
    if(lowercase_vovel || uppercase_vovel){
        return 1;
    } else {
        return 0;
    }
}

// predicate: check if person is older than 25
int is_older_than_25(void *person) {
    Person *p = (Person*)person;
    if(p->age > 25){
        return 1;
    } else {
        return 0;
    }
}

// print integer value
void print_int(const void *v) {
    int *int_v = (int*)v;
    printf("%d ",*int_v);
}

// print char value
void print_char(const void *v) {
    char *c = (char*)v;
    printf("%c ",*c);
}

// print structure Person
void print_person(const void *v) {
    Person *p = (Person*)v;
    printf("%d %s %s\n",p->age,p->first_name,p->last_name);
}

// print capacity of the vector and its elements
void print_vector(Vector *vector, print_ptr print) {
    printf("%d\n",(int)vector->capacity);
    size_t n = vector->size;
    void *ptr = vector->data;
    for(int i=0;i<n;i++){
        print(ptr);
        ptr += vector->element_size;
    }
}

// read int value
void read_int(void* value) {
    int *v = (int*)value;
    scanf("%d",v);
}

// read char value
void read_char(void* value) {
    char *c = (char*)value;
    scanf(" %c",c);
}

// read struct Person
void read_person(void* value) {
    Person *v = (Person*)value;
    scanf("%d",&v->age);
    scanf("%s",v->first_name);
    scanf("%s",v->last_name);
}

void vector_test(Vector *vector, size_t block_size, size_t elem_size, int n, read_ptr read,
		 cmp_ptr cmp, predicate_ptr predicate, print_ptr print) {
    init_vector(vector, block_size, elem_size);
	void *v = malloc(vector->element_size);
	size_t index, size;
	for (int i = 0; i < n; ++i) {
		char op;
		scanf(" %c", &op);
		switch (op) {
			case 'p': // push_back
				read(v);
				push_back(vector, v);
				break;
			case 'i': // insert
				scanf("%zu", &index);
				read(v);
				insert(vector, index, v);
				break;
			case 'e': // erase
				scanf("%zu", &index);
				erase(vector, index);
				break;
			case 'v': // erase
				read(v);
				erase_value(vector, v, cmp);
				break;
			case 'd': // erase (predicate)
				erase_if(vector, predicate);
				break;
			case 'r': // resize
				scanf("%zu", &size);
				resize(vector, size);
				break;
			case 'c': // clear
				clear(vector);
				break;
			case 'f': // shrink
				shrink_to_fit(vector);
				break;
			case 's': // sort
				qsort(vector->data, vector->size,
				      vector->element_size, cmp);
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
	print_vector(vector, print);
	free(vector->data);
	free(v);
}

int main(void) {
	int to_do, n;
	Vector vector_int, vector_char, vector_person;

//    freopen("1_1.txt","r",stdin); //plik na wejście

	scanf("%d%d", &to_do, &n);

	switch (to_do) {
		case 1:
			vector_test(&vector_int, 4, sizeof(int), n, read_int, int_cmp,
				is_even, print_int);
			break;
		case 2:
			vector_test(&vector_char, 2, sizeof(char), n, read_char, char_cmp,
				is_vowel, print_char);
			break;
		case 3:
			vector_test(&vector_person, 2, sizeof(Person), n, read_person,
				person_cmp, is_older_than_25, print_person);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}

	return 0;
}

