#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define N_CARDS 52
#define N_PLAYERS 2

int rand_from_interval(int a, int b){
    if(a>b){
        return INT_MIN;
    } else if(b-a > RAND_MAX){
        return INT_MAX;
    } else if(a==b){
        return a;
    } else {
        return (rand() % (b - a + 1)) + a;
    }
}

void swap(int arr[], int i, int k){
    int temp = arr[i];
    arr[i] = arr[k];
    arr[k] = temp;
}

void rand_permutation(int n, int arr[]){
    if(n >= 0){
        for(int i=0;i<n;i++){
            arr[i] = i;
        }
        for(int i=0;i<n-1;i++){
            int k = rand_from_interval(i,n-1);
            swap(arr,i,k);
        }
    }
}

void handing_out_cards(int n, int arr[], int arrA[], int arrB[]){
    int mid = n / 2;
    int k = 0;
    for(int i=0;i<mid;i++){
        arrA[i] = arr[i];
        arrB[i] = arr[i+mid];
    }
}

// Queue with cyclic buffer

#define CBUFF_SIZE 53

int out_A = 0, len_A = N_CARDS/2;
int out_B = 0, len_B = N_CARDS/2;

//int out_A = 0, len_A = 8;
//int out_B = 0, len_B = 8;

void cbuff_push(int num, int arr[], char player) {
    if(player == 'A'){
        arr[(out_A+len_A)%CBUFF_SIZE] = num;
        len_A++;
    } else {
        arr[(out_B+len_B)%CBUFF_SIZE] = num;
        len_B++;
    }
}

int cbuff_pop(int arr[], char player){
    if(player == 'A'){
        if(len_A==0){
            return -1;
        } else {
            int val = arr[out_A%CBUFF_SIZE];
//            arr[out_A%CBUFF_SIZE] = -1;
            out_A++;
            len_A--;
            return val;
        }
    } else {
        if(len_B==0){
            return -1;
        } else {
            int val = arr[out_B%CBUFF_SIZE];
//            arr[out_B%CBUFF_SIZE] = -1;
            out_B++;
            len_B--;
            return val;
        }
    }
}

void print_arr(int n, int arr[]){
    for(int i=0;i<n;i++){
        printf("%d ",arr[i]);
    }
    printf("\n");
}

void cbuff_print(int arr[], char player){
//    printf("out_A: %d; len_A: %d; out_B: %d; len_B: %d\n",out_A,len_A,out_B,len_B);
//    print_arr(CBUFF_SIZE,arr);
    if(player == 'A'){
        int curr_len = len_A;
        for(int i=out_A%CBUFF_SIZE;i<CBUFF_SIZE;i++){
            if(curr_len==0){
                break;
            }
            printf("%d ", arr[i]);
            curr_len--;
        }
        for(int i=0;i<out_A%CBUFF_SIZE;i++){
            if(curr_len==0){
                break;
            }
            printf("%d ", arr[i]);
            curr_len--;
        }
    } else {
        int curr_len = len_B;
        for(int i=out_B%CBUFF_SIZE;i<CBUFF_SIZE;i++){
            if(curr_len==0){
                break;
            }
            printf("%d ", arr[i]);
            curr_len--;
        }
        for(int i=0;i<out_B%CBUFF_SIZE;i++){
            if(curr_len==0){
                break;
            }
            printf("%d ", arr[i]);
            curr_len--;
        }
    }
}

int main(void){
    int seed, game, max_conflicts, conflict_counter;
    int arr[N_CARDS];
    int arrA[CBUFF_SIZE], arrB[CBUFF_SIZE];

    scanf("%d %d %d", &seed, &game, &max_conflicts);
    srand((unsigned) seed);

    rand_permutation(N_CARDS,arr);
    handing_out_cards(N_CARDS,arr,arrA,arrB);

//    int arrB[] = {51,50,49,48,47,46,45,44};
//    int arrA[] = {7,6,5,4,3,2,1,0};

//    print_arr(N_CARDS,arr);
//    print_arr(CBUFF_SIZE,arrA);
//    print_arr(CBUFF_SIZE,arrB);

    switch(game){
        case 0:
            conflict_counter = 0;
            while(conflict_counter < max_conflicts){
                int card_A[N_CARDS];
                int card_B[N_CARDS];
                int curr_A = cbuff_pop(arrA,'A');
                int curr_B = cbuff_pop(arrB,'B');
//                printf("[K] curr_A: %d curr_B: %d\n",curr_A,curr_B);
//                print_arr(CBUFF_SIZE,arrA);
//                print_arr(CBUFF_SIZE,arrB);
//                printf("out_A: %d; len_A: %d; out_B: %d; len_B: %d\n",out_A,len_A,out_B,len_B);
                if (curr_B == -1){ //wygrana gracza A
                    printf("2 ");
                    printf("%d",conflict_counter);
                    return 2;
                } else if (curr_A == -1){ //wygrana gracza B
                    printf("3\n");
                    printf("%d ",curr_B);
                    cbuff_print(arrB,'B');
                    return 3;
                } else {
                    card_A[0] = curr_A;
                    card_B[0] = curr_B;
                }
                if ((card_A[0]/4)==(card_B[0])/4){
                    conflict_counter++;
                    int i = 0;
                    do {
                        curr_A = cbuff_pop(arrA,'A');
                        curr_B = cbuff_pop(arrB,'B');
//                        printf("[W] curr_A: %d curr_B: %d\n",curr_A,curr_B);
//                        print_arr(CBUFF_SIZE,arrA);
//                        print_arr(CBUFF_SIZE,arrB);
//                        printf("out_A: %d; len_A: %d; out_B: %d; len_B: %d\n",out_A,len_A,out_B,len_B);
                        if (curr_B == -1 || curr_A == -1){ //nieroztrzygniete (zabraklo kart)
                            for(int j=0;j<=i;j++){
                                cbuff_push(card_A[j],arrA,'A');
                            }
                            for(int j=0;j<=i;j++){
                                cbuff_push(card_B[j],arrB,'B');
                            }
                            if(curr_A != -1){
                                cbuff_push(curr_A,arrA,'A');
                            } else if (curr_B != -1){
                                cbuff_push(curr_B,arrB,'B');
                            }
                            printf("1 ");
                            printf("%d ", len_A);
                            printf("%d", len_B);
                            return 1;
                        } else {
                            i++;
                            card_A[i] = curr_A;
                            card_B[i] = curr_B;
                        }
                        curr_A = cbuff_pop(arrA,'A');
                        curr_B = cbuff_pop(arrB,'B');
//                        printf("[K] curr_A: %d curr_B: %d\n",curr_A,curr_B);
//                        print_arr(CBUFF_SIZE,arrA);
//                        print_arr(CBUFF_SIZE,arrB);
//                        printf("out_A: %d; len_A: %d; out_B: %d; len_B: %d\n",out_A,len_A,out_B,len_B);
                        if (curr_B == -1 || curr_A == -1){ //nieroztrzygniete (zabraklo kart)
                            for(int j=0;j<=i;j++){
                                cbuff_push(card_A[j],arrA,'A');
                            }
                            for(int j=0;j<=i;j++){
                                cbuff_push(card_B[j],arrB,'B');
                            }
                            if(curr_A != -1){
                                cbuff_push(curr_A,arrA,'A');
                            } else if (curr_B != -1){
                                cbuff_push(curr_B,arrB,'B');
                            }
                            printf("1 ");
                            printf("%d ", len_A);
                            printf("%d", len_B);
                            return 1;
                        } else {
                            i++;
                            card_A[i] = curr_A;
                            card_B[i] = curr_B;
                        }
                        conflict_counter++;
                    }while((card_A[i]/4)==(card_B[i]/4));
                    if((card_A[i]/4) > (card_B[i]/4)){
                        for(int j=0;j<=i;j++){
                            cbuff_push(card_A[j],arrA,'A');
                        }
                        for(int j=0;j<=i;j++){
                            cbuff_push(card_B[j],arrA,'A');
                        }
                    } else {
                        for(int j=0;j<=i;j++){
                            cbuff_push(card_B[j],arrB,'B');
                        }
                        for(int j=0;j<=i;j++){
                            cbuff_push(card_A[j],arrB,'B');
                        }
                    }
                } else if ((card_A[0]/4) > (card_B[0]/4)){
                    cbuff_push(card_A[0],arrA,'A');
                    cbuff_push(card_B[0],arrA,'A');
                    conflict_counter++;
                } else {
                    cbuff_push(card_B[0],arrB,'B');
                    cbuff_push(card_A[0],arrB,'B');
                    conflict_counter++;
                }
            }
            if(conflict_counter >= max_conflicts){ //niedokonczenie gry
                printf("0 ");
                printf("%d ", len_A);
                printf("%d", len_B);
            }
            break;
        case 1:
            conflict_counter = 0;
            while(conflict_counter < max_conflicts){
                int card_A[N_CARDS];
                int card_B[N_CARDS];
                int curr_A = cbuff_pop(arrA,'A');
                int curr_B = cbuff_pop(arrB,'B');
//                printf("[K] curr_A: %d curr_B: %d\n",curr_A,curr_B);
//                print_arr(CBUFF_SIZE,arrA);
//                print_arr(CBUFF_SIZE,arrB);
//                printf("out_A: %d; len_A: %d; out_B: %d; len_B: %d\n",out_A,len_A,out_B,len_B);
                if (curr_B == -1){ //wygrana gracza A
                    printf("2 ");
                    printf("%d",conflict_counter);
                    return 2;
                } else if (curr_A == -1){ //wygrana gracza B
                    printf("3\n");
                    printf("%d ",curr_B);
                    cbuff_print(arrB,'B');
                    return 3;
                } else {
                    card_A[0] = curr_A;
                    card_B[0] = curr_B;
                }
                if ((card_A[0]/4)==(card_B[0])/4){
                    cbuff_push(card_A[0],arrA,'A');
                    cbuff_push(card_B[0],arrB,'B');
                    conflict_counter++;
                } else if ((card_A[0]/4) > (card_B[0]/4)){
                    cbuff_push(card_A[0],arrA,'A');
                    cbuff_push(card_B[0],arrA,'A');
                    conflict_counter++;
                } else {
                    cbuff_push(card_B[0],arrB,'B');
                    cbuff_push(card_A[0],arrB,'B');
                    conflict_counter++;
                }
            }
            if(conflict_counter >= max_conflicts){ //niedokonczenie gry
                printf("0 ");
                printf("%d ", len_A);
                printf("%d", len_B);
            }
            break;
        default:
            printf("Error\n");
            break;
    }
    return 0;
}