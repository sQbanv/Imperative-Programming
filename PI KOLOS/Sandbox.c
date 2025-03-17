#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//ZAD1

typedef struct _matrix{
    int rows;
    int cols;
    double **data;
}Matrix;

int get(Matrix *pm, int row, int col, double *pvalue){
    if(row >= 0 && row < pm->rows && col >= 0 && col < pm->cols){
        pvalue = &pm->data[row][col];
        if(pvalue != NULL){
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

int set(Matrix *pm, int row, int col, double value){
    if(row >= 0 && row < pm->rows && col >= 0 && col < pm->cols){
        pm->data[row][col] = value;
        return 1;
    } else {
        return 0;
    }
}

void create_identity_matrix(Matrix *pm,int size){
    double *curr_matrix = malloc(sizeof(double)*size);
    pm->data = &curr_matrix;
    pm->rows = size;
    pm->cols = size;
    for(int i=0;i<size;i++){
        double *curr_row = malloc(sizeof(double)*size);
        curr_row[i] = 1;
        pm->data[i] = curr_row;
    }
}

double dot_prod_vect(const double *v1, const double *v2, int size){
    double prod = 0;
    for(int i=0;i<size;i++){
        prod += v1[i] * v2[i];
    }
    return prod;
}

void mul_vect(double *result, const Matrix *pm, const double *vect){
    double curr_col[pm->cols];
    for(int col=0;col<pm->cols;col++){
        for(int row=0;row<pm->rows;row++){
            curr_col[row] = pm->data[row][col];
        }
        result[col] = dot_prod_vect(vect,curr_col,pm->cols);
    }
}

void max_element_in_rows(double *maxdata, const Matrix *pm){
    double curr_max;
    for(int i=0;i<pm->rows;i++){
        curr_max = 0;
        for(int j=0;j<pm->cols;j++){
            if(pm->data[i][j] > curr_max){
                curr_max = pm->data[i][j];
            }
        }
        maxdata[i] = curr_max;
    }
}

//ZAD2

typedef struct {
    double *results;
    int len;
} Data;

typedef struct tagQueueElement {
    Data data;
    struct tagQueueElement *next;
}QueueElement;

typedef struct tagQueue{
    QueueElement *head;
    QueueElement *tail;
}Queue;

void free_queue(Queue *pqueue){
    QueueElement *curr = pqueue->head;
    while(curr != NULL){
        QueueElement *to_delete = curr;
        curr = curr->next;
        free(to_delete);
    }
}

void push_copy(Queue *pqueue, const Data *pdata){
    QueueElement *new_element = malloc(sizeof(QueueElement));
    new_element->data = *pdata;
    new_element->next = NULL;
    if(pqueue->tail != NULL){
        pqueue->tail->next = new_element;
        pqueue->tail = new_element;
    } else {
        pqueue->tail = new_element;
        if(pqueue->head == NULL){
            pqueue->head = new_element;
        }
    }
}

int peek(const Queue *pqueue, Data *pdata){
    if(pqueue->head != NULL){
        pdata->results = pqueue->head->data.results;
        pdata->len = pqueue->head->data.len;
        return 1;
    } else {
        return 0;
    }
}

int pop(Queue *pqueue, Data *pdata){
    if(pqueue->head != NULL){
        QueueElement *to_delete = pqueue->head;
        pqueue->head = pqueue->head->next;
        pdata->results = to_delete->data.results;
        pdata->len = to_delete->data.len;
        free(to_delete);
        if(pqueue->head != NULL){
            return 1;
        } else {
            return 0;
        }
    } else {
        return -1;
    }
}

int get_total_count(const Queue *pqueue){
    int counter = 0;
    QueueElement *curr = pqueue->head;
    while(curr != NULL){
        counter += curr->data.len;
        curr = curr->next;
    }
    return counter;
}

//ZAD3

enum VehicleType {
    car,
    bus,
    truck
};

enum PowertrainType {
    electric,
    hybrid,
    combustion
};

struct Vehicle {
    char* owner;
    int nextInspectionDate[3];
    enum VehicleType type;
    enum PowertrainType powertrain;
    union {
        struct {
            int maxPassengers;
            float enginePower;
        } carData;
        struct {
            int seatingCapacity;
            int standingCapacity;
        } busData;
        struct {
            float engineCapacity;
            float maxAxleLoad;
        } truckData;
    };
};

void new_owner(struct Vehicle *pvehicle){
    char buff[1024];
    printf("Podaj nazwe nowego wlasciciela:\n");
    fgets(buff,1024,stdin);
    if(strlen(buff) > 0 && buff[strlen(buff)-1] == '\n'){
        buff[strlen(buff)-1] = '\0';
    }
    strcpy(pvehicle->owner,buff);
}

int cmp_dates(int date1[], int date2[]){
    if(date1[2] > date2[2]){
        return 1;
    } else if(date1[2] < date2[2]){
        return -1;
    }

    if(date1[1] > date2[1]){
        return 1;
    } else if(date1[1] < date2[1]){
        return -1;
    }

    if(date1[0] > date2[0]){
        return 1;
    } else if(date1[0] < date2[0]){
        return -1;
    }

    return 0;
}

int delayed(struct Vehicle *pvehicle, int size, int *base_date, char ***pdelayed_owners){
    int counter = 0;
    char **delayedOwners = NULL;

    for(int i=0;i<size;i++){
        if(cmp_dates(pvehicle[i].nextInspectionDate,base_date) > 0){
            delayedOwners = realloc(delayedOwners,(counter + 1) * sizeof(char*));
            delayedOwners[counter] = malloc(strlen(pvehicle[i].owner)+1);
            strcpy(delayedOwners[counter],pvehicle[i].owner);
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

void print_vehicle(struct Vehicle *pvehicle){
    printf("%s\n",pvehicle->owner);
    printf("%d.%d.%d\n",pvehicle->nextInspectionDate[0],pvehicle->nextInspectionDate[1],pvehicle->nextInspectionDate[2]);
    printf("%d\n",pvehicle->type);
    printf("%d\n",pvehicle->powertrain);
    if(pvehicle->type == car){
        printf("%d\n",pvehicle->carData.maxPassengers);
        printf("%f\n",pvehicle->carData.enginePower);
    } else if(pvehicle->type == bus){
        printf("%d\n",pvehicle->busData.seatingCapacity);
        printf("%d\n",pvehicle->busData.standingCapacity);
    } else {
        printf("%f\n",pvehicle->truckData.engineCapacity);
        printf("%f\n",pvehicle->truckData.maxAxleLoad);
    }
}


int main(){

//==========================================================
//    Queue FIFO;
//    FIFO.head = NULL;
//    FIFO.tail = NULL;
//
//    double test[6] = {2.1,5.6,6.6,1.1,13,17};
//
//    freopen("1.txt","r",stdin);
//
//    char buff[1024];
//    char op;
//
//    while(fgets(buff,1024,stdin)){
//        op = buff[0];
//        switch (op) {
//            case 'i':
//                printf("i\n");
//                Data *new_element = malloc(sizeof(Data));
//                double *res = malloc(sizeof(double));
//                *res = *test;
//                new_element->results = res;
//                new_element->len = 6;
//                push_copy(&FIFO,new_element);
//                break;
//            case 'r':
//                printf("r\n");
//                Data *removed_element = malloc(sizeof(Data));
//                pop(&FIFO,removed_element);
//                free(removed_element);
//                break;
//            case 'p':
//                printf("p\n");
//                Data *element = malloc(sizeof(Data));
//                peek(&FIFO,element);
//                free(element);
//                break;
//            case 'g':
//                printf("g\n");
//                printf("%d\n", get_total_count(&FIFO));
//                break;
//            default:
//                printf("No such expression");
//                break;
//        }
//    }
//=========================================================================
//    int date[3] = {15,7,23};
//    struct Vehicle v;
//    v.owner = malloc(strlen("Jan Nowak") + 1);
//    strcpy(v.owner,"Jan Nowak");
//    v.nextInspectionDate[0] = 15;
//    v.nextInspectionDate[1] = 7;
//    v.nextInspectionDate[2] = 2023;
//    v.type = car;
//    v.powertrain = hybrid;
//    v.carData.maxPassengers = 5;
//    v.carData.enginePower = 100;
//
//    print_vehicle(&v);
//
//    new_owner(&v);
//
//    print_vehicle(&v);

    return 0;
}