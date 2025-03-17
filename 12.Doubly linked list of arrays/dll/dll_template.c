#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// list node
typedef struct Node {
	int *data;
	size_t array_size;
	struct Node* next;
	struct Node* prev;
} Node;

// doubly linked list
typedef struct List {
	Node *head;
	Node *tail;
} List;

// iterator
typedef struct iterator {
	struct Node* node_ptr;
	size_t position;
} iterator;

// forward initialization
iterator begin(Node* head) {
	iterator it = { head, 0 };
	return it;
}

// backward initialization;
// points to the element following the last one
iterator end(Node* tail) {
	iterator it = { tail, tail->array_size };
	return it;
}

void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if (ptr) return ptr;
	exit(EXIT_FAILURE);
}

void *safe_realloc(void *old_ptr, size_t size) {
	void *ptr = realloc(old_ptr, size);
	if (ptr) return ptr;
	free(old_ptr);
	exit(EXIT_FAILURE);
}

Node *create_node(int *data, size_t array_size, Node *next, Node *prev) {
	Node *node = safe_malloc(sizeof(Node));
	node->data = data;
	node->array_size = array_size;
	node->next = next;
	node->prev = prev;
	return node;
}

// initialize list
// creates the front and back sentinels
void init(List *list) {
	list->head = create_node(NULL, 0, NULL, NULL);
	list->tail = create_node(NULL, 0, NULL, list->head);
	list->head->next = list->tail;
}

// to implement ...

// append node to the list
void push_back(List *list, int *data, size_t array_size) {
    Node *curr = safe_malloc(sizeof(Node));
    curr->data = data;
    curr->array_size = array_size;
    curr->next = NULL;
    curr->prev = NULL;

    if(list->tail->prev->data != NULL){
        list->tail->prev->next = curr;
        curr->prev = list->tail->prev;
    }

    curr->next = list->tail;
    list->tail->prev = curr;

    if(list->head->next->data == NULL){
        list->head->next = list->tail->prev;
        curr->prev = list->head;
    }
}

// set iterator to move n elements forward from its current position
void skip_forward(iterator* itr, size_t n) {
    size_t curr_n = n;
    while(itr->node_ptr->array_size < curr_n) {
        curr_n -= itr->node_ptr->array_size;
        itr->node_ptr = itr->node_ptr->next;
    }
    itr->position = curr_n;
}

// forward iteration - get n-th element in the list
int get_forward(List *list, size_t n) {
    iterator it = begin(list->head->next);
    skip_forward(&it,n);
    return it.node_ptr->data[it.position-1];
}

// set iterator to move n elements backward from its current position
void skip_backward(iterator* itr, size_t n) {
    size_t curr_n = n;
    while(itr->node_ptr->array_size < curr_n){
        curr_n -= itr->node_ptr->array_size;
        itr->node_ptr = itr->node_ptr->prev;
    }
    itr->position = itr->node_ptr->array_size - curr_n;
}

// backward iteration - get n-th element from the end of the list
int get_backward(List *list, size_t n) {
    iterator it = end(list->tail->prev);
    skip_backward(&it,n);
    return it.node_ptr->data[it.position];
}

void remove_node(Node *node_ptr) {
    free(node_ptr);
}

// remove n-th element; if array empty remove node
void remove_at(List *list, size_t n) {
    iterator it = begin(list->head->next);
    skip_forward(&it,n);
    if(it.node_ptr->array_size > 1){
        if(it.position == it.node_ptr->array_size){
            it.node_ptr->array_size -= 1;
        } else {
            for(size_t i=it.position-1;i<it.node_ptr->array_size-1;i++){
                it.node_ptr->data[i] = it.node_ptr->data[i+1];
            }
            it.node_ptr->array_size -= 1;
        }
    } else {
        Node *tmp = it.node_ptr->prev;
        it.node_ptr->prev->next = it.node_ptr->next;
        it.node_ptr->next->prev = tmp;
        remove_node(it.node_ptr);
    }
}

// return the number of digits of number n
size_t digits(int n) {
    if(n == 0) return 1;
    return floor(log10(abs(n)))+1;
}

// inserts 'value' to the node with the same digits' count
// otherwise insert new node
void put_in_order(List *list, int value) {
    size_t curr_digits = digits(value);
    Node *curr = list->head->next;
    if(list->head->next == list->tail && list->tail->prev == list->head){
        int *tab = safe_malloc(sizeof(int));
        *tab = value;
        push_back(list,tab,1);
    } else {
        while(curr->next != NULL || curr->data!=NULL){
            if(curr_digits == digits(curr->data[0])){
                curr->data = safe_realloc(curr->data, sizeof(int) * (curr->array_size+1));
                curr->array_size += 1;

                size_t i;
                for(i=0;i<curr->array_size-1;i++){
                    if(value < curr->data[i]){
                        break;
                    }
                }
                for(size_t j=curr->array_size-1;j>i;j--){
                    curr->data[j] = curr->data[j-1];
                }
                curr->data[i] = value;
                break;
            } else if(curr_digits < digits(curr->data[0])){
                int *tab = safe_malloc(sizeof(int));
                *tab = value;
                Node *c_node = safe_malloc(sizeof(Node));
                c_node->data = tab;
                c_node->array_size = 1;
                c_node->next = NULL;
                c_node->prev = NULL;

                c_node->prev = curr->prev;
                c_node->next = curr;
                curr->prev->next = c_node;
                curr->prev = c_node;
                break;
            }
            curr = curr->next;
        }
        if(curr == list->tail){
            int *tab = safe_malloc(sizeof(int));
            *tab = value;
            push_back(list,tab,1);
        }
    }
}

// -------------------------------------------------------------
// helper functions

// print list
void dumpList(const List *list) {
	for(Node *node = list->head->next; node != list->tail; node = node->next) {
		printf("-> ");
		for (int k = 0; k < node->array_size; k++) {
			printf("%d ", node->data[k]);
		}
		printf("\n");
	}
}

// free list
void freeList(List *list) {
	Node *to_delete = list->head->next, *next;
	while(to_delete != list->tail) {
		next = to_delete->next;
		remove_node(to_delete);
		to_delete = next;
	}
}

// read int vector
void read_vector(int tab[], size_t n) {
	for (size_t i = 0; i < n; ++i) {
		scanf("%d", tab + i);
	}
}

// initialize the list and push data
void read_list(List *list) {
	int n;
	size_t size;
	scanf("%d", &n); // number of nodes
	for (int i = 0; i < n; i++) {
		scanf("%zu", &size); // length of the array in i-th node
		int *tab = (int*) safe_malloc(size * sizeof(int));
		read_vector(tab, size);
		push_back(list, tab, size);
	}
}

int main() {
	int to_do, value;
	size_t size, m;
	List list;
	init(&list);

//    freopen("5.txt","r",stdin); //plik na wejscie

	scanf("%d", &to_do);
	switch (to_do) {
		case 1:
			read_list(&list);
			dumpList(&list);
			break;
		case 2:
			read_list(&list);
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%zu", &m);
				printf("%d ", get_forward(&list, m));
			}
			printf("\n");
			break;
		case 3:
			read_list(&list);
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%zu", &m);
				printf("%d ", get_backward(&list, m));
			}
			printf("\n");
			break;
		case 4:
			read_list(&list);
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%zu", &m);
				remove_at(&list, m);
			}
			dumpList(&list);
			break;
		case 5:
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%d", &value);
				put_in_order(&list, value);
			}
			dumpList(&list);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	freeList(&list);

	return 0;
}
