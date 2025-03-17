#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct {
	int first;
	int second;
} pair;

void insertion_sort(int *array, int n){
    for(int i=1;i<n;i++){
        int key = array[i];
        int j = i - 1;
        while(j >= 0 && array[j] > key){
            array[j+1] = array[j];
            j--;
        }
        array[j+1] = key;
    }
}

// Add pair to existing relation if not already there
int add_relation (pair*, int, pair);

// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair *relation, int n){
    int flags[MAX_RANGE];
    for(int i=0;i<n;i++){
        if(relation[i].first == relation[i].second){
            flags[relation[i].first] = 1;
        }
    }
    int flag=1;
    for(int i=0;i<n;i++){
        if(flags[relation[i].first] == 0){
            flag = 0;
            break;
        }
    }
    return flag;
};

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair *relation, int n){
    int flag = 1;
    for(int i=0;i<n;i++){
        if(relation[i].first == relation[i].second){
            flag = 0;
            break;
        }
    }
    return flag;
};

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair *relation, int n){
    for(int i=0;i<n;i++){
        int flag = 0;
        for(int j=0;j<n;j++){
            if(i==j){
                continue;
            }
            if(relation[i].first == relation[j].second && relation[i].second == relation[j].first){
                flag = 1;
                break;
            }
        }
        if(flag == 0){
            return flag;
        }
    }
    return 1;
};

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair *relation, int n){
    int flag = 1;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(i==j){
                continue;
            }
            if(relation[i].first == relation[j].second && relation[i].second == relation[j].first){
                if(relation[i].first != relation[i].second){
                    flag = 0;
                    break;
                }
            }
        }
        if(flag == 0){
            return flag;
        }
    }
    return 1;
};

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair *relation, int n){
    int flag = 1;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(relation[i].first == relation[j].second && relation[i].second == relation[j].first){
                flag = 0;
                break;
            }
        }
        if(flag == 0){
            return flag;
        }
    }
    return flag;
};

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair *relation, int n){
    for(int i=0;i<n;i++){
        int flag = 0;
        int curr_x = relation[i].first;
        int curr_y = relation[i].second;
        for(int j=0;j<n;j++){
            if(i==j){
                continue;
            }
            if(relation[j].first == curr_y){
                int curr_z = relation[j].second;
                for(int k=0;k<n;k++){
                    if(relation[k].first == curr_x && relation[k].second == curr_z){
                        flag = 1;
                        break;
                    }
                }
                if(flag == 0){
                    return flag;
                }
            }
        }
    }
    return 1;
};

// Case 2:

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair *relation, int n){
    if(is_reflexive(relation,n) == 1 && is_antisymmetric(relation,n) == 1 && is_transitive(relation,n)){
        return 1;
    } else {
        return 0;
    }
};

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair *relation, int n){
    int x[MAX_REL_SIZE], y[MAX_REL_SIZE];
    int x_size = 0, y_size = 0;
    for(int i=0;i<n;i++){
        int curr_x = relation[i].first;
        int flag = 1;
        for(int j=0;j<x_size;j++){
            if(x[j] == curr_x){
                flag = 0;
                break;
            }
        }
        if(flag == 1){
            x[x_size] = curr_x;
            x_size++;
        }
        int curr_y = relation[i].second;
        flag = 1;
        for(int j=0;j<y_size;j++){
            if(y[j] == curr_y){
                flag = 0;
                break;
            }
        }
        if(flag == 1){
            y[y_size] = curr_y;
            y_size++;
        }
    }
    for(int i=0;i<x_size;i++){
        int curr_x = x[i];
        int flag = 0;
        for(int j=0;j<y_size;j++){
            int curr_y = y[j];
            for(int k=0;k<n;k++){
                if((curr_x == relation[k].first && curr_y == relation[k].second)||(curr_x == relation[i].second && curr_y == relation[i].first)){
                    flag = 1;
                    break;
                }
            }
            if(flag == 0){
                return 0;
            }
        }
    }
    return 1;
};

// A total order relation is a partial order relation that is connected
int is_total_order(pair *relation, int n){
    if(is_partial_order(relation,n) == 1 && is_connected(relation,n) == 1){
        return 1;
    } else {
        return 0;
    }
};

int find_max_elements(pair *relation, int n, int *max_elements){
    int max_size = 0;
    for(int i=0;i<n;i++){
        int curr = relation[i].first;
        int flag = 1;
        for(int j=0;j<n;j++){
            if(i == j){
                continue;
            }
            if(relation[j].first == curr){
                flag = 0;
                break;
            }
        }
        if(flag == 1){
            max_elements[max_size] = curr;
            max_size++;
        }
    }
    insertion_sort(max_elements,max_size);
    return max_size;
};
int find_min_elements(pair *relation, int n, int *min_elements){
    int min_size = 0;
    for(int i=0;i<n;i++){
        int curr = relation[i].second;
        int flag = 1;
        for(int j=0;j<n;j++){
            if(i == j){
                continue;
            }
            if(relation[j].second == curr){
                flag = 0;
                break;
            }
        }
        if(flag == 1){
            min_elements[min_size] = curr;
            min_size++;
        }
    }
    insertion_sort(min_elements,min_size);
    return min_size;
};
int get_domain(pair *relation, int n, int *domain){
    int domain_size = 0;
    for(int i=0;i<n;i++){
        int flag = 1;
        for(int j=0;j<domain_size;j++){
            if(relation[i].first == domain[j]){
                flag = 0;
                break;
            }
        }
        if(flag == 1){
            domain[domain_size] = relation[i].first;
            domain_size++;
        }
        flag = 1;
        for(int j=0;j<domain_size;j++){
            if(relation[i].second == domain[j]){
                flag = 0;
                break;
            }
        }
        if(flag == 1){
            domain[domain_size] = relation[i].second;
            domain_size++;
        }
    }
    insertion_sort(domain,domain_size);
    return domain_size;
};

// Case 3:

int composition (pair *relation_1, int n_1, pair *relation_2, int n_2, pair *comp_relation){
    int comp_size = 0;
    for(int i=0;i<n_1;i++){
        int curr_y = relation_1[i].second;
        for(int j=0;j<n_2;j++){
            if(curr_y == relation_2[j].first){
                int x = relation_1[i].first;
                int y = relation_2[j].second;
                int flag = 1;
                for(int k=0;k<comp_size;k++){
                    if(comp_relation[k].first == x && comp_relation[k].second == y){
                        flag = 0;
                        break;
                    }
                }
                if(flag == 1){
                    comp_relation[comp_size].first = x;
                    comp_relation[comp_size].second = y;
                    comp_size++;
                }
            }
        }
    }
    return comp_size;
};

// Comparator for pair
int cmp_pair (const void *a, const void *b) {
}

int insert_int (int *tab, int n, int new_element) {
}

// Add pair to existing relation if not already there
int add_relation (pair *tab, int n, pair new_pair) {
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation) {
    int size = 0;
    scanf("%d",&size);
    int k = 0;
    for(int i=0;i<2*size;i++){
        int curr;
        scanf("%d",&curr);
        if(i%2==0){
            relation[k].first = curr;
        } else {
            relation[k].second = curr;
            k += 1;
        }
    }
//    for(int i=0;i<size;i++){
//        printf("%d %d\n",relation[i].first,relation[i].second);
//    }
    return size;
}

void print_int_array(int *array, int n) {
    printf("%d\n",n);
    for(int i=0;i<n;i++){
        printf("%d ",array[i]);
    }
    printf("\n");
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];
	int max_elements[MAX_REL_SIZE];
	int min_elements[MAX_REL_SIZE];

//    freopen("2.txt","r",stdin); //plik z ktorego czyta program

    scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

