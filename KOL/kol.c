#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

//Część 1

static int glob_columns;

typedef struct _matrix{
    int rows;
    int cols;
    double **data;
}Matrix;

int create_matrix(Matrix *pmatrix, int rows, int cols){
    if(rows < 1 || cols < 1){
        return 0;
    } else {
        double **new_data = malloc(sizeof(double*)*rows);
        for(int i=0;i<cols;i++){
            new_data[i] = malloc(sizeof(double)*cols);
        }

        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                new_data[i][j] = 0;
            }
        }

        pmatrix->rows = rows;
        pmatrix->cols = cols;
        pmatrix->data = new_data;

        return 1;
    }
}

Matrix random_matrix(int rows, int cols){
    srand(time(NULL));

    double **new_data = malloc(sizeof(double*)*rows);
    for(int i=0;i<rows;i++){
        double *new_row = malloc(sizeof(double)*cols);
        for(int j=0;j<cols;j++){
            new_row[j] = (double)rand() / RAND_MAX;
        }
    }

    Matrix new_matrix;
    new_matrix.rows = rows;
    new_matrix.cols = cols;
    new_matrix.data = new_data;

    return new_matrix;
}

void inplace_mul_scalar(Matrix *pm, double v){
    for(int i=0;i<pm->rows;i++){
        for(int j=0;j<pm->cols;j++){
            pm->data[i][j] *= v;
        }
    }
}

Matrix copy_matrix(const Matrix *psrc){
    double **new_data = malloc(sizeof(double*)*psrc->rows);
    for(int i=0;i<psrc->rows;i++){
        double *new_row = malloc(sizeof(double)*psrc->cols);
        for(int j=0;j<psrc->cols;j++){
            new_row[j] = psrc->data[i][j];
        }
        new_data[i] = new_row;
    }
    Matrix new_matrix;
    new_matrix.rows = psrc->rows;
    new_matrix.cols = psrc->cols;
    new_matrix.data = new_data;

    return new_matrix;
}

Matrix mul_by_scalar(const Matrix *pm, double v){
    double **new_data = malloc(sizeof(double*)*pm->rows);
    for(int i=0;i<pm->rows;i++){
        double *new_row = malloc(sizeof(double)*pm->cols);
        for(int j=0;j<pm->cols;j++){
            new_row[j] = pm->data[i][j] * v;
        }
        new_data[i] = new_row;
    }
    Matrix new_matrix;
    new_matrix.rows = pm->rows;
    new_matrix.cols = pm->cols;
    new_matrix.data = new_data;

    return new_matrix;
}

int compare(const void *a, const void *b){
    const double* row1 = *((const double**)a);
    const double* row2 = *((const double**)a);

    double sum1 = 0.0;
    double sum2 = 0.0;

    for(int i=0; i<glob_columns;i++){
        sum1 += row1[i];
    }

    for(int i=0;i<glob_columns;i++){
        sum2 += row2[i];
    }

    if(sum1 > sum2){
        return 1;
    } else if(sum1 < sum2){
        return -1;
    } else {
        return 0;
    }
}

//Część 2

enum Drive_type {
    combustion, electric, d_ndef
};

typedef struct {int d; int m; int y;} Date;

enum Fuel_type {gasoline,diesel,gas,hydrogen,f_d_ndef};
enum Battery_type {Li_Ion, Na_Ion, NiMH, b_d_ndef};

typedef struct{
    char owner[30];
    Date valid_date;
    enum Drive_type d_type;
    union {
        struct{
           char emmision[15];
           enum Fuel_type  fuel_t;
        }Comb_data;
        struct{
            enum Battery_type  bat_t;
            float capacity;
        }El_data;
    };
}Vehicle;

typedef union{
    struct Comb_data {
        char emmision[15];
        enum Fuel_type fuel_t;
    }cs;
    struct El_data {
        enum Battery_type bat_t;
        float capacity;
    }es;
}dvr_union;

int cmp_dates(Date date1, Date date2){
    if(date1.y > date2.y){
        return 1;
    } else if(date1.y < date2.y){
        return -1;
    }

    if(date1.m > date2.m){
        return 1;
    } else if(date1.m < date2.m){
        return -1;
    }

    if(date1.d > date2.d){
        return 1;
    } else if(date1.d < date2.d){
        return -1;
    }

    return 0;
}

int delayed(const Vehicle *tv, int size, const Date *base_date, int **pdelayed_owners){
    int counter = 0;
    int *delayedOwners = NULL;

    for(int i=0;i<size;i++){
        if(cmp_dates(tv[i].valid_date,*base_date) < 0){
            delayedOwners = realloc(delayedOwners, (counter + 1) * sizeof(int));
            delayedOwners[counter] = i;
            counter++;
        }
    }

    if(counter == 0){
        *pdelayed_owners = NULL;
    } else {
        *pdelayed_owners = delayedOwners;
    }
    return counter;
}

void print_owners(const Vehicle *tv, int *t_ind, size_t size){
    for(int i=0;i<size;i++){
        int curr_index = t_ind[i];
        printf("%s\n",tv[curr_index].owner);
    }
}

Vehicle *create_vehicle(void){
    Vehicle *new_v = malloc(sizeof(Vehicle));
    char new_owner[] = "";
    strcpy(new_v->owner,new_owner);
    Date new_date = {1,1,1970};
    new_v->valid_date = new_date;
    new_v->d_type = d_ndef;

    return new_v;
}

void print_drive_data(const Vehicle *v){
    if(v->d_type == 0 || v->d_type == 1){
        printf("Owner: %s, ",v->owner);
        printf("Valid_Date: %d.%d.%d, ",v->valid_date.d,v->valid_date.m,v->valid_date.y);
        if(v->d_type == combustion){
            printf("emmision: %s, ",v->Comb_data.emmision);
            printf("Fuel_type: %d",v->Comb_data.fuel_t);
        } else {
            printf("Battery_type: %d",v->El_data.bat_t);
            printf("Capacity: %f",v->El_data.capacity);
        }
    }
}

int put_data(Vehicle *v, enum Drive_type type, const void *data){
    if(type == d_ndef || type == combustion || type == electric){
        if(type == combustion){
            struct Comb_data *curr = (struct Comb_data*)data;
            strcpy(v->Comb_data.emmision,curr->emmision);
            v->Comb_data.fuel_t = curr->fuel_t;
        } else if(type == electric){
            struct El_data *curr = (struct El_data*)data;
            v->El_data.bat_t = curr->bat_t;
            v->El_data.capacity = curr->capacity;
        }
        return 1;
    } else {
        return 0;
    }
}

//Część 3

int main(){
//    freopen("1.txt","r",stdin);
    return 0;
}