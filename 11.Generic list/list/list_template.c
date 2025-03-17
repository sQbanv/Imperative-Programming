#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

struct List;
typedef void (*DataFp)(void*);
typedef void (*ConstDataFp)(const void*);
typedef  int (*CompareDataFp)(const void*, const void*);

typedef struct ListElement {
	struct ListElement *next;
	void *data;
} ListElement;

typedef struct {
	ListElement *head;
	ListElement *tail;
	ConstDataFp dump_data;
	DataFp free_data;
	CompareDataFp compare_data;
	DataFp modify_data;
} List;


typedef struct DataWord {
    char *word;
    int counter;
} DataWord;

void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if(ptr) return ptr;
	printf("malloc error\n");
	exit(EXIT_FAILURE);
}

void *safe_strdup(const char *string) {
	void *ptr = strdup(string);
	if(ptr) return ptr;
	printf("strdup error\n");
	exit(EXIT_FAILURE);
}

void dump_int(const void *d);

// --- generic functions --- for any data type

void init_list(List *p_list, ConstDataFp dump_data, DataFp free_data,
		CompareDataFp compare_data, DataFp modify_data) {
    p_list->head = NULL;
    p_list->tail = NULL;
    p_list->compare_data = compare_data;
    p_list->modify_data = modify_data;
    p_list->dump_data = dump_data;
    p_list->free_data = free_data;
}

// Print elements of the list
void dump_list(const List* p_list) {
    ListElement *ptr = p_list->head;
    while(ptr != NULL){
        p_list->dump_data(ptr->data);
        ptr = ptr->next;
    }
}

// Print elements of the list if comparable to data
void dump_list_if(List *p_list, void *data) {
    ListElement *ptr = p_list->head;

    DataWord *comparator = (DataWord*)data;

    while(ptr != NULL){
        if(p_list->compare_data(ptr->data,comparator) == 0){
            p_list->dump_data(ptr->data);
        }
        ptr = ptr->next;
    }
}

// Free element pointed by data using free_data() function
void free_element(DataFp free_data, ListElement *to_delete) {
    free_data(to_delete);
}

// Free all elements of the list
void free_list(List* p_list) {
	ListElement *curr = p_list->head;
    while(curr != NULL) {
        ListElement *to_delete = curr;
        curr = curr->next;

        p_list->free_data(to_delete->data);
        free(to_delete);
    }
    p_list->head = NULL;
    p_list->tail = NULL;
}

// Push element at the beginning of the list
void push_front(List *p_list, void *data){
    ListElement *element = safe_malloc(sizeof(ListElement));
    element->data = data;
    element->next = p_list->head;

    if(!p_list->head){
        p_list->tail = element;
    }

    p_list->head = element;
}

// Push element at the end of the list
void push_back(List *p_list, void *data) {
    ListElement *element = safe_malloc(sizeof(ListElement));
    element->data = data;
    element->next = NULL;

    if(p_list->tail != NULL){
        p_list->tail->next = element;
    }

    p_list->tail = element;

    if(p_list->head == NULL){
        p_list->head = p_list->tail;
    }
}

// Remove the first element
void pop_front(List *p_list) {
    if(p_list->head != NULL){
        ListElement *element = p_list->head;

        if(p_list->head == p_list->tail){
            p_list->head = NULL;
            p_list->tail = NULL;
        } else {
            p_list->head = element->next;
        }

        p_list->free_data(element->data);
        free(element);
    }
}

// Reverse the list
void reverse(List *p_list) {
    ListElement *prev = p_list->head;

    if(prev != NULL){
        ListElement *curr = prev->next;
        prev->next = NULL;

        ListElement *tmp;
        while(curr != NULL){
            tmp = curr->next;
            curr->next = prev;
            prev = curr;
            curr = tmp;
        }

        tmp = p_list->head;
        p_list->head = p_list->tail;
        p_list->tail = tmp;
    }
}

// find element in sorted list after which to insert given element
ListElement* find_insertion_point(const List *p_list, ListElement *p_element) {
    ListElement *curr_element = p_list->head;
    ListElement *prev = NULL;

    while(curr_element != NULL){
        if(p_list->compare_data(curr_element->data,p_element->data) > 0){
            break;
        }
        prev = curr_element;
        curr_element = curr_element->next;
    }

    return prev;
}

// Insert element after 'previous'
void push_after(List *p_list, void *data, ListElement *previous) {
    ListElement *element = (ListElement*)data;

    if(previous == NULL){
        element->next = p_list->head;
        if(p_list->tail == NULL){
            p_list->tail = element;
        }
        p_list->head = element;

    } else {
        element->next = previous->next;
        previous->next = element;

        if(previous == p_list->tail){
            p_list->tail = element;
        }
    }
}

// Insert element preserving order
void insert_in_order(List *p_list, void *p_data) {
    ListElement *element = safe_malloc(sizeof(ListElement));
    element->data = p_data;
    element->next = NULL;

    ListElement *after = find_insertion_point(p_list,element);

    if(after == NULL || p_list->compare_data(element->data,after->data) != 0){
        push_after(p_list,element,after);
    }
}

// -----------------------------------------------------------
// --- type-specific definitions

// int element

void dump_int(const void *d) {
    int *v = (int*)d;
    printf("%d ",*v);
}

void free_int(void *d) {
    int *data = (int*)d;
    free(data);
}

int cmp_int(const void *a, const void *b) {
    int* v1 = (int*)a;
    int* v2 = (int*)b;
    if(*v1 > *v2){
        return 1;
    } else if(*v1 < *v2){
        return -1;
    } else {
        return 0;
    }
}

int *create_data_int(int v) {
    int* ptr = safe_malloc(sizeof(int));
    *ptr = v;
    return ptr;
}

// Word element

void dump_word (const void *d) {
    DataWord *data = (DataWord*)d;
    printf("%s ",data->word);
}

void free_word(void *d) {
    DataWord *data = (DataWord*)d;
    free(data->word);
    free(data);
}

int cmp_word_alphabet(const void *a, const void *b) {
    DataWord *word1 = (DataWord*)a;
    DataWord *word2 = (DataWord*)b;

    return strcmp(word1->word,word2->word);
}

int cmp_word_counter(const void *a, const void *b) {
    DataWord *word1 = (DataWord*)a;
    DataWord *word2 = (DataWord*)b;
//    printf("%s %d | %s %d\n",word1->word,word1->counter,word2->word,word2->counter);
    if(word1->counter > word2->counter){
        return 1;
    } else if(word1->counter < word2->counter){
        return -1;
    } else {
        return 0;
    }
}

void modify_word(void *p) {
    DataWord *data = (DataWord*)p;
    data->counter += 1;
}

void *create_data_word(const char *string, int counter) {
    DataWord *data = (DataWord*)safe_malloc(sizeof(DataWord));
    data->word = strdup(string);
    data->counter = counter;
    return data;
}

void insert_in_order_word(List *p_list, void *p_data) {
    ListElement *element = safe_malloc(sizeof(ListElement));
    element->data = p_data;
    element->next = NULL;

    ListElement *after = find_insertion_point(p_list,element);

    if(after == NULL || p_list->compare_data(element->data,after->data) != 0){
        push_after(p_list,element,after);
    } else {
        modify_word(after->data);
    }
}

// read text, parse it to words, and insert those words to the list.
// Order of insertions is given by the last parameter of type CompareDataFp.
// (comparator function address). If this address is not NULL the element is
// inserted according to the comparator. Otherwise, read order is preserved.
void stream_to_list(List *p_list, FILE *stream, CompareDataFp cmp) {
    DataWord *ptr;
    char buff[BUFFER_SIZE];
    char limit[] = " \t\n\r\v\f.,?!:;-";
    if(cmp != NULL){
        p_list->compare_data = cmp;
    }
    while(fgets(buff,BUFFER_SIZE,stream)){
//        printf("%s",buff);
        char *curr = strtok(buff,limit);
        while(curr != NULL){
            ptr = create_data_word(curr, 1);
            if(cmp != NULL){
                for(int i=0;i<strlen(ptr->word);i++){
                    ptr->word[i] = tolower(ptr->word[i]);
                }
//                printf("%s ",ptr->word);
                insert_in_order_word(p_list,ptr);
            } else {
                push_back(p_list, ptr);
            }
            curr = strtok(NULL, limit);
        }
    }
}

// test integer list
void list_test(List *p_list, int n) {
    char op;
	int v;
	for (int i = 0; i < n; ++i) {
		scanf(" %c", &op);
		switch (op) {
			case 'f':
				scanf("%d", &v);
				push_front(p_list, create_data_int(v));
				break;
			case 'b':
				scanf("%d", &v);
				push_back(p_list, create_data_int(v));
				break;
			case 'd':
				pop_front(p_list);
				break;
			case 'r':
				reverse(p_list);
				break;
			case 'i':
				scanf("%d", &v);
				insert_in_order(p_list, create_data_int(v));
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
}

int main(void) {
	int to_do, n;
	List list;

//    freopen("3.txt","r",stdin); //plik na wejście

	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer list
			scanf("%d",&n);
			init_list(&list, dump_int, free_int, cmp_int, NULL);
			list_test(&list, n);
			dump_list(&list);
			free_list(&list);
			break;
		case 2: // read words from text, insert into list, and print
			init_list(&list, dump_word, free_word, NULL, NULL);
			stream_to_list(&list, stdin, NULL);
			dump_list(&list);
			free_list(&list);
			break;
		case 3: // read words, insert into list alphabetically, print words encountered n times
			scanf("%d",&n);
			init_list(&list, dump_word, free_word, NULL, modify_word);
			stream_to_list(&list, stdin, cmp_word_alphabet);
			list.compare_data = cmp_word_counter;
			DataWord data = { NULL, n };
			dump_list_if(&list, &data);
			free_list(&list);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

