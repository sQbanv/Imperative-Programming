#include <stdio.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		scanf("%lf", x++);
	}
}

void print_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		printf("%.4f ", x[i]);
	}
	printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			scanf("%lf", &A[i][j]);
		}
	}
}

void print_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			printf("%.4f ", A[i][j]);
		}
		printf("\n");
	}
}

// 1. Calculate matrix product, AB = A X B
// A[m][p], B[p][n], AB[m][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n) {
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            double curr_product = 0;
            for(int x=0;x<p;x++){
                curr_product += A[i][x]*B[x][j];
            }
            AB[i][j] = curr_product;
        }
    }
}


// 2. Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n) {
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(A[i][i] == 0){
                return NAN;
            }
            if(A[i][j] != 0){
                for(int x=i+1;x<n;x++){
                    double curr = A[x][j]/A[i][j];
                    for(int y=0;y<n;y++){
                        A[x][y] -= A[i][y]*curr;
                    }
                }
                break;
            }
        }
    }
    double determinant = 1;
    for(int i=0;i<n;i++){
        determinant *= A[i][i];
    }
    return determinant;
}

void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n) {
}

// 3. Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.

double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps) {
    for(int i=0;i<n;i++){
        A[i][n] = b[i];
    }
    int swaps = 0;
    for(int i=0;i<n;i++){
        for(int j=i;j<n+1;j++){
            if(A[i][j] != 0){
                for(int p=i+1;p<n;p++){
                    double curr = A[p][j]/A[i][j];
                    for(int q=0;q<n+1;q++){
                        A[p][q] -= A[i][q]*curr;
                    }
                }
                break;
            } else {
                int index_to_swap = -1;
                for(int k=i+1;k<n;k++){
                    if(A[i][k] != 0){
                        index_to_swap = k;
                        break;
                    }
                }
                if(index_to_swap != -1){
                    swaps += 1;
                    for(int k=0;k<n+1;k++){
                        double temp = A[i][k];
                        A[i][k] = A[index_to_swap][k];
                        A[index_to_swap][k] = temp;
                    }
                    for(int p=i+1;p<n;p++){
                        double curr = A[p][j]/A[i][j];
                        for(int q=0;q<n+1;q++){
                            A[p][q] -= A[i][q]*curr;
                        }
                    }
                    if(A[i][i] < eps){
                        return 0;
                    }
                    break;
                } else {
                    continue;
                }
            }
        }
    }
//    for(int i=0;i<n;i++){
//        for(int j=0;j<n+1;j++){
//            printf("%lf",A[i][j]);
//        }
//        printf("\n");
//    }

    double det = 1;
    for(int i=0;i<n;i++){
        det *= A[i][i];
    }

    if (det != 0 && b != NULL && x != NULL){
        for(int i=n-1;i>=0;i--){
            for(int j=i-1;j>=0;j--){
                double curr = A[j][i]/A[i][i];
                A[j][i] -= A[i][i]*curr;
                A[j][n] -= A[i][n]*curr;
            }
        }

        for(int i=n-1;i>=0;i--){
            x[i] = A[i][n]/A[i][i];
        }
    }

    if(swaps%2==0){
        return det;
    } else {
        return det*(-1);
    }
}

// 4. Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps) {
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(i==j){
                A[i][j+n] = 1;
            } else {
                A[i][j+n] = 0;
            }
        }
    }
//    for(int i=0;i<n;i++){
//        for(int j=0;j<n*2;j++){
//            printf("%lf|",A[i][j]);
//        }
//        printf("\n");
//    }
    int swaps = 0;
    for(int i=0;i<n;i++){
        for(int j=i;j<n*2;j++){
            if(A[i][j] != 0){
                for(int p=i+1;p<n;p++){
                    double curr = A[p][j]/A[i][j];
                    for(int q=0;q<n*2;q++){
                        A[p][q] -= A[i][q]*curr;
                    }
                }
                break;
            } else {
                int index_to_swap = -1;
                for(int k=i+1;k<n;k++){
                    if(A[i][k] != 0){
                        index_to_swap = k;
                        break;
                    }
                }
                if(index_to_swap != -1){
                    swaps += 1;
                    for(int k=0;k<n*2;k++){
                        double temp = A[i][k];
                        A[i][k] = A[index_to_swap][k];
                        A[index_to_swap][k] = temp;
                    }
                    for(int p=i+1;p<n;p++){
                        double curr = A[p][j]/A[i][j];
                        for(int q=0;q<n*2;q++){
                            A[p][q] -= A[i][q]*curr;
                        }
                    }
                    if(fabs(A[i][i]) < eps){
                        return 0;
                    }
                    break;
                } else {
                    continue;
                }
            }
        }
    }
//    for(int i=0;i<n;i++){
//        for(int j=0;j<n*2;j++){
//            printf("%lf|",A[i][j]);
//        }
//        printf("\n");
//    }

    double det = 1;
    for(int i=0;i<n;i++){
        det *= A[i][i];
    }

    if (det != 0){
        for(int i=n-1;i>=0;i--){
            for(int j=i-1;j>=0;j--){
                double curr = A[j][i]/A[i][i];
                for(int x=i;x<n*2;x++){
                    A[j][x] -= A[i][x]*curr;
                }
            }
        }

        for(int i=0;i<n;i++){
            if(A[i][i] != 1){
                double curr = A[i][i];
                for(int j=i;j<n*2;j++){
                    A[i][j] /= curr;

//                    if(A[i][j]==0){
//                        A[i][j]=0;
//                    }
                }
            }
        }
    }

//    for(int i=0;i<n;i++){
//        for(int j=0;j<n*2;j++){
//            printf("%lf|",A[i][j]);
//        }
//        printf("\n");
//    }

    int x = 0;
    int y = 0;
    for(int i=0;i<n;i++){
        for(int j=n;j<n*2;j++){
            B[x][y] = A[i][j];
            y++;
        }
        x++;
        y = 0;
    }

    if(swaps%2==0){
        return det;
    } else {
        return det*(-1);
    }
}

int main(void) {

	double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
	double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m, n, p;

//    freopen("mat5.txt","r",stdin); //plik z ktorego czyta program

	scanf ("%d", &to_do);

	switch (to_do) {
		case 1:
			scanf("%d %d %d", &m, &p, &n);
			read_mat(A, m, p);
			read_mat(B, p, n);
			mat_product(A, B, C, m, p, n);
			print_mat(C, m, n);
			break;
		case 2:
			scanf("%d", &n);
			read_mat(A, n, n);
			printf("%.4f\n", gauss_simplified(A, n));
			break;
		case 3:
			scanf("%d", &n);
			read_mat(A,n, n);
			read_vector(b, n);
			det = gauss(A, b, x, n, eps);
			printf("%.4f\n", det);
			if (det) print_vector(x, n);
			break;
		case 4:
			scanf("%d", &n);
			read_mat(A, n, n);
			det = matrix_inv(A, B, n, eps);
			printf("%.4f\n", det);
			if (det) print_mat(B, n, n);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

