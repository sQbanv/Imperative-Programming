#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

int get(int cols, int row, int col, const int *A) {
    int index;
    index = row + col*cols;
//    printf("%d\n",A[index]);
    return index;
}

void set(int cols, int row, int col, int *A, int value) {
}

void prod_mat(int rowsA, int colsA, int colsB, int *A, int *B, int *AB) {
    int prod = 0;
    int index = 0;
    for(int i=0;i<rowsA;i++){
        for(int x=0;x<rowsA;x++){
            for(int j=0;j<colsA;j++){
                prod += A[i * colsA + j] * B[get(colsB,0,j,B) + x];
            }
            AB[index] = prod;
            prod = 0;
            index++;
        }
    }
}

void read_mat(int rows, int cols, int *t) {
    int index = 0;
    int curr;
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            scanf("%d",&curr);
            t[index] = curr;
            index++;
        }
    }
}

void print_mat(int rows, int cols, int *t) {
    for(int i=0;i<rows*cols;i++){
        printf("%d ",t[i]);
        if(i%cols == cols - 1){
            printf("\n");
        }
    }
}

int read_char_lines(char *array[]) {
    char buff[BUF_SIZE];
    int i = 0;
    while(fgets(buff,BUF_SIZE,stdin)){
//        printf("%s\n",buff);
        char *curr_str = malloc(sizeof curr_str * strlen(buff));
        for(int j=0;j<strlen(buff);j++){
            if(buff[j] == '\n'){
                curr_str[j] = '\0';
                break;
            }
            curr_str[j] = buff[j];
            if(j==0){
                array[i] = &curr_str[j];
                i += 1;
            }
        }
//        free(curr_str);
    }
    return i;
}

void write_char_line(char *array[], int n) {
    char *curr_address = array[n];
    while(*curr_address != '\0'){
        printf("%c",*curr_address);
        curr_address += 1;
    }
    printf("\n");
}

void delete_lines(char *array[]) {
}

int read_int_lines_cont(int *ptr_array[]) {
    char buff[100];
    int digits[1000];
    int k = 0;
    int i = 0;
    while(fgets(buff,100,stdin)){
//        printf("%s\n",buff);
        int digit;
        for(int j=0;j<strlen(buff);j+=2){
            if(buff[j] == '-'){
                digit = -1*(buff[j+1]-'0');
//                printf("%d\n",digit);
                j++;
            } else {
                digit = buff[j]-'0';
//                printf("%d\n",digit);
            }
            digits[k] = digit;
            if(j == 0){
                ptr_array[i] = &digits[k];
                i += 1;
            }
            k += 1;
            if(buff[j+1] == '\n'){
                break;
            }
        }
    }
    return i;
}

void write_int_line_cont(int *ptr_array[], int n) {
    int* curr_address = ptr_array[n];
    while(curr_address != ptr_array[n+1]){
        printf("%d ",*curr_address);
        curr_address += 1;
    }
    printf("\n");
}

typedef struct {
	int *values;
	int len;
	double average;
} line_type;

int read_int_lines(line_type lines_array[]) {
    char buff[BUF_SIZE];
    int i = 0;
    int k = 0;
    int *pointer;
    while(fgets(buff,BUF_SIZE,stdin)){
//        printf("%s\n",buff);
        char *token = strtok(buff," ");
        double sum = 0;
        int counter = 0;
        int *curr = malloc(sizeof curr * BUF_SIZE);
        while(token != NULL){
            int digit = atoi(token);
//            printf("%d\n",digit);


            curr[i] = digit;
            if(counter == 0){
                pointer = &curr[i];
            }
            i += 1;

            sum += digit;
            counter += 1;

            token = strtok(NULL, " ");
        }
        line_type sequence;
        sequence.average = sum/(double)counter;
        sequence.len = counter;
        sequence.values = pointer;
        lines_array[k] = sequence;
        k++;
    }
    return k;
}

void write_int_line(line_type lines_array[], int n) {
    int *curr_address = lines_array[n].values;
    int number_of_elements = lines_array[n].len;
    for(int i=0;i<number_of_elements;i++){
        printf("%d ",*curr_address);
        curr_address += 1;
    }
    printf("\n");
    printf("%0.2f",lines_array[n].average);
    printf("\n");
}

void delete_int_lines(line_type array[], int line_count) {
}

int cmp (const void *a, const void *b) {
    line_type *A = (line_type*)a;
    line_type *B = (line_type*)b;
    if(A->average > B->average){
        return 1;
    } else if(A->average < B->average){
        return -1;
    } else {
        return 0;
    }
}

void sort_by_average(line_type lines_array[], int line_count) {
    qsort(lines_array,line_count,sizeof(*lines_array),cmp);
}

typedef struct {
	int r, c, v;
} triplet;

int read_sparse(triplet *triplet_array, int n_triplets) {
    int k = 0;
    for(int i=0;i<n_triplets;i++){
        triplet curr;
        scanf("%d",&curr.r);
        scanf("%d",&curr.c);
        scanf("%d",&curr.v);
//        printf("%d %d %d\n",curr.r,curr.c,curr.v);
        triplet_array[k] = curr;
        k += 1;
    }
    return k;
}

int cmp_triplets(const void *t1, const void *t2) {
    triplet *T1 = (triplet*)t1;
    triplet *T2 = (triplet*)t2;
    if(T1->r > T2->r){
        return 1;
    } else if(T1->r == T2->r){
        if(T1->c > T2->c){
            return 1;
        } else if(T1->c < T2->c){
            return -1;
        } else {
            return 0;
        }
    } else {
        return -1;
    }
}

void make_CSR(triplet *triplet_array, int n_triplets, int rows, int *V, int *C, int *R) {
    qsort(triplet_array,n_triplets,sizeof(triplet),cmp_triplets);
//    for(int i=0;i<n_triplets;i++){
//        printf("%d\n",triplet_array[i].v);
//    }

    R[0] = 0;
    int j = 1;
    int curr_row = 0;
    int values_on_row = 0;
    for(int i=0;i<n_triplets;i++){
        if(curr_row == triplet_array[i].r){
            values_on_row += 1;
        } else {
            if(triplet_array[i].r - curr_row > 1){
                R[j] = R[j-1] + values_on_row;
                j += 1;
                for(int k=0;k<triplet_array[i].r-curr_row-1;k++){
                    R[j] = R[j-1];
                    j += 1;
                }
                curr_row = triplet_array[i].r;
                values_on_row = 1;
            } else {
                curr_row = triplet_array[i].r;
                R[j] = R[j-1] + values_on_row;
                j += 1;
                values_on_row = 1;
            }
        }
        V[i] = triplet_array[i].v;
        C[i] = triplet_array[i].c;
    }
    while(j<=rows+1){
        R[j] = n_triplets;
        j += 1;
    }
}

void multiply_by_vector(int rows, const int *V, const int *C, const int *R, const int *x, int *y) {
    for(int i=0;i<rows;i++){
        y[i] = 0;
        for(int j=R[i];j<R[i+1];j++){
            y[i] = y[i] + V[j]*x[C[j]];
        }
    }
}

void read_vector(int *v, int n) {
    for(int i=0;i<n;i++){
        scanf("%d",&v[i]);
//        printf("%d\n",v[i]);
    }
}

void write_vector(int *v, int n) {
    for(int i=0;i<n;i++){
        printf("%d ",v[i]);
    }
    printf("\n");
}

int read_int() {
	char c_buf[BUF_SIZE];
	fgets(c_buf, BUF_SIZE, stdin);
	return (int)strtol(c_buf, NULL, 10);
}

int main(void) {

//    freopen("5_test.txt","r",stdin); //plik na wejście

	int to_do = read_int();

	int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
	int n, lines_counter, rowsA, colsA, rowsB, colsB;
	int rows, cols, n_triplets;
	char *char_lines_array[TAB_SIZE] = { NULL };
	int continuous_array[TAB_SIZE];
	int *ptr_array[TAB_SIZE];
	triplet triplet_array[TAB_SIZE];
	int V[TAB_SIZE], C[TAB_SIZE], R[TAB_SIZE];
	int x[TAB_SIZE], y[TAB_SIZE];
	line_type int_lines_array[TAB_SIZE];

	switch (to_do) {
		case 1:
			scanf("%d %d", &rowsA, &colsA);
			read_mat(rowsA, colsA, A);
			scanf("%d %d", &rowsB, &colsB);
			read_mat(rowsB, colsB, B);
			prod_mat(rowsA, colsA, colsB, A, B, AB);
			print_mat(rowsA, colsB, AB);
			break;
		case 2:
			n = read_int() - 1; // we count from 1 :)
			ptr_array[0] = continuous_array;
			read_int_lines_cont(ptr_array);
			write_int_line_cont(ptr_array, n);
			break;
		case 3:
			n = read_int() - 1;
			read_char_lines(char_lines_array);
			write_char_line(char_lines_array, n);
			delete_lines(char_lines_array);
			break;
		case 4:
			n = read_int() - 1;
			lines_counter = read_int_lines(int_lines_array);
			sort_by_average(int_lines_array, lines_counter);
			write_int_line(int_lines_array, n);
			delete_int_lines(int_lines_array, lines_counter);
			break;
		case 5:
			scanf("%d %d %d", &rows, &cols, &n_triplets);
			n_triplets = read_sparse(triplet_array, n_triplets);
			read_vector(x, cols);
			make_CSR(triplet_array, n_triplets, rows, V, C, R);
			multiply_by_vector(rows, V, C, R, x, y);
			write_vector(V, n_triplets);
			write_vector(C, n_triplets);
			write_vector(R, rows + 1);
			write_vector(y, rows);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

