#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define MAX_RATE 4
#define MEMORY_ALLOCATION_ERROR  (-1)

typedef union {
	int int_data;
	char char_data;
	// ... other primitive types used
	void *ptr_data;
} data_union;

typedef struct ht_element {
	struct ht_element *next;
	data_union data;
} ht_element;

typedef void (*DataFp)(data_union);
typedef void (*DataPFp)(data_union*);
typedef int (*CompareDataFp)(data_union, data_union);
typedef size_t (*HashFp)(data_union, size_t);
typedef data_union (*CreateDataFp)(void*);

typedef struct {
	size_t size;
	size_t no_elements;
	ht_element *ht;
	DataFp dump_data;
	CreateDataFp create_data;
	DataFp free_data;
	CompareDataFp compare_data;
	HashFp hash_function;
	DataPFp modify_data;
} hash_table;

// ---------------------- functions to implement

// initialize table fields
void init_ht(hash_table *p_table, size_t size, DataFp dump_data, CreateDataFp create_data,
		 DataFp free_data, CompareDataFp compare_data, HashFp hash_function, DataPFp modify_data) {
    p_table->size = size;
    p_table->no_elements = 0;
    p_table->ht = malloc(sizeof(ht_element)*size);
    for(int i=0;i<size;i++){
        p_table->ht[i].next = NULL;
    }
    p_table->dump_data = dump_data;
    p_table->create_data = create_data;
    p_table->free_data = free_data;
    p_table->compare_data = compare_data;
    p_table->hash_function = hash_function;
    p_table->modify_data = modify_data;
}

// print elements of the list with hash n
void dump_list(const hash_table* p_table, size_t n) {
    ht_element *curr = p_table->ht[n].next;
    while(curr != NULL) {
        p_table->dump_data(curr->data);
        curr = curr->next;
    }
}

// Free element pointed by data_union using free_data() function
void free_element(DataFp free_data, ht_element *to_delete) {
    if(free_data != NULL){
        free_data(to_delete->data);
    }
    free(to_delete);
}

// free all elements from the table (and the table itself)
void free_table(hash_table* p_table) {
    for(int i=0;i<p_table->size;i++){
        ht_element *curr = p_table->ht[i].next;
        while(curr != NULL){
            ht_element *tmp = curr->next;
            free_element(p_table->free_data,curr);
//            free(curr);
            curr = tmp;
        }
    }
    free(p_table->ht);
}

// calculate hash function for integer k
size_t hash_base(int k, size_t size) {
	static const double c = 0.618033988; // (sqrt(5.) – 1) / 2.;
	double tmp = k * c;
	return (size_t)floor((double)size * (tmp - floor(tmp)));
}

void rehash(hash_table *p_table) {
    p_table->size *= 2;
    ht_element *new_ht = malloc(sizeof(ht_element)*p_table->size);
    for(int i=0;i<p_table->size;i++){
        new_ht[i].next = NULL;
    }
    for(int i=0;i<(int)p_table->size/2;i++){
        ht_element *curr = p_table->ht[i].next;
        while(curr != NULL){
            size_t new_index = p_table->hash_function(curr->data,p_table->size);
//            printf("%zu %d\n",new_index,curr->data.int_data);
            ht_element *tmp = curr->next;
            curr->next = new_ht[new_index].next;
            new_ht[new_index].next = curr;
            curr = tmp;
        }
    }
    free(p_table->ht);
    p_table->ht = new_ht;
}

// find element; return pointer to previous
ht_element *find_previous(hash_table *p_table, data_union data) {
}

// return pointer to element with given value
ht_element *get_element(hash_table *p_table, data_union *data) {
    size_t index = p_table->hash_function(*data,p_table->size);
//    printf("%zu\n",index);
//    p_table->dump_data(*data);//
    ht_element *curr = p_table->ht[index].next;
    while(curr != NULL){
        if(p_table->compare_data(*data,curr->data) == 0){
//            p_table->dump_data(curr->data);
            break;
        }
        curr = curr->next;
    }
    return curr;
}

// insert element
void insert_element(hash_table *p_table, data_union *data) {
    size_t index = p_table->hash_function(*data,p_table->size);
//    printf("%zu\n",index);
    ht_element *element = malloc(sizeof(ht_element));
    element->data = *data;
    ht_element *curr = p_table->ht[index].next;
    int flag = 1;
    while(curr != NULL){
        if(p_table->compare_data(element->data,curr->data) == 0){
            flag = 0;
            break;
        }
        curr = curr->next;
    }
    if(flag == 1){
        element->next = p_table->ht[index].next;
        p_table->ht[index].next = element;
        p_table->no_elements += 1;
    }
    if(p_table->no_elements/p_table->size > MAX_RATE){
        rehash(p_table);
    }
}

// remove element
void remove_element(hash_table *p_table, data_union data) {
    size_t index = p_table->hash_function(data,p_table->size);
//    printf("%zu\n",index);
    ht_element *prev = &p_table->ht[index];
    ht_element *curr = p_table->ht[index].next;
    while(curr != NULL){
        if(p_table->compare_data(data,curr->data) == 0){
            prev->next = curr->next;
            free_element(p_table->free_data,curr);
            p_table->no_elements -= 1;
            break;
        }
        curr = curr->next;
        prev = prev->next;
    }
}

// type-specific definitions

// int element

size_t hash_int(data_union data, size_t size) {
	return hash_base(data.int_data, size);
}

void dump_int(data_union data) {
    printf("%d ",data.int_data);
}

int cmp_int(data_union a, data_union b) {
    return a.int_data - b.int_data;
}

data_union create_int(void* value) {
    int val;
    data_union data;
    scanf("%d",&val);
    data.int_data = val;
//    printf("%d\n",val);
    if(value != NULL){

    }
    return data;
}

// char element

size_t hash_char(data_union data, size_t size) {
	return hash_base((int)data.char_data, size);
}

void dump_char(data_union data) {
    printf("%c ",data.char_data);
}

int cmp_char(data_union a, data_union b) {
    return a.char_data - b.char_data;
}

data_union create_char(void* value) {
    char c;
    scanf(" %c",&c);
//    printf("%c\n",c);
    data_union data;
    data.char_data = c;
    if(value != NULL){

    }
    return data;
}

// Word element

typedef struct DataWord {
	char *word;
	int counter;
} DataWord;

void dump_word(data_union data) {
    DataWord *word = (DataWord*)data.ptr_data;
    printf("%s %d",word->word,word->counter);
}

void free_word(data_union data) {
    DataWord *curr_word = (DataWord*)data.ptr_data;
    free(curr_word->word);
    free(curr_word);
}

int cmp_word(data_union a, data_union b) {
    DataWord *w1 = (DataWord*)a.ptr_data;
    DataWord *w2 = (DataWord*)b.ptr_data;
//    printf("%s | %s : %d\n",w1->word,w2->word, strcmp(w1->word,w2->word));
    return strcmp(w1->word,w2->word);
}

size_t hash_word(data_union data, size_t size) {
	int s = 0;
	DataWord *dw = (DataWord*)data.ptr_data;
	char *p = dw->word;
	while (*p) {
		s += *p++;
	}
	return hash_base(s, size);
}

void modify_word(data_union *data) {
    DataWord *word = (DataWord*)data->ptr_data;
    word->counter += 1;
}

data_union create_data_word(void *value) {
    data_union s;
    DataWord *new_word = malloc(sizeof(DataWord));
    char *curr_word = malloc(sizeof(char)*(strlen(value)+1));
    strcpy(curr_word,value);
    new_word->word = curr_word;
    new_word->counter = 1;

    s.ptr_data = new_word;
    return s;
}

// read text, parse it to words, and insert these words to the hashtable
void stream_to_ht(hash_table *p_table, FILE *stream) {
    data_union new_word;
    char buff[BUFFER_SIZE];
    char limit[] = " \t\n\r\v\f.,?!:;-";
    while(fgets(buff,BUFFER_SIZE,stream)){
//        printf("%s",buff);
        char *curr = strtok(buff,limit);
        while(curr != NULL){
            for(int i=0;i<strlen(curr);i++){
                curr[i] = tolower(curr[i]);
            }
            new_word = p_table->create_data(curr);
            size_t index = p_table->hash_function(new_word,p_table->size);
//            printf("%s %zu\n",curr,index);
            ht_element *element = p_table->ht[index].next;
            int flag = 1;
            while(element != NULL){
                if(p_table->compare_data(element->data,new_word) == 0){
                    flag = 0;
                    break;
                }
                element = element->next;
            }
            if(flag == 1){
                insert_element(p_table,&new_word);
            } else {
                modify_word(&element->data);
            }
            curr = strtok(NULL,limit);
        }
    }
}

// test primitive type list
void test_ht(hash_table *p_table, int n) {
	char op;
	data_union data;
	for (int i = 0; i < n; ++i) {
		scanf(" %c", &op);
//        printf("%c\n",op);
//		p_table->create_data(&data);
		data = p_table->create_data(NULL); // should give the same result as the line above
		switch (op) {
			case 'r':
				remove_element(p_table, data);
				break;
			case 'i':
				insert_element(p_table, &data);
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
}

int main(void) {
	int to_do, n;
	size_t index;
	hash_table table;
	char buffer[BUFFER_SIZE];
	data_union data;

//    freopen("3_1.txt","r",stdin); //plik na wejście

	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer hash table
			scanf("%d %zu", &n, &index);
			init_ht(&table, 4, dump_int, create_int, NULL, cmp_int, hash_int, NULL);
			test_ht(&table, n);
			printf ("%zu\n", table.size);
			dump_list(&table, index);
			break;
		case 2: // test char hash table
			scanf("%d %zu", &n, &index);
			init_ht(&table, 4, dump_char, create_char, NULL, cmp_char, hash_char, NULL);
			test_ht(&table, n);
			printf ("%zu\n", table.size);
			dump_list(&table, index);
			break;
		case 3: // read words from text, insert into hash table, and print
			scanf("%s", buffer);
			init_ht(&table, 8, dump_word, create_data_word, free_word, cmp_word, hash_word, modify_word);
			stream_to_ht(&table, stdin);
			printf ("%zu\n", table.size);
			data = table.create_data(buffer);
			ht_element *e = get_element(&table, &data);
			if (e) table.dump_data(e->data);
			if (table.free_data) table.free_data(data);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	free_table(&table);

	return 0;
}

