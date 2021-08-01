#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

typedef struct Matrix {
    uint16_t rows;
    uint16_t cols;
    uint8_t** data; // pointer to pointer of list of values
} Matrix;

/*
 @brief Initialize a matrix (of size `[n_rows, n_cols]`) with zeros.

 @param n_rows Number of rows.
 @param n_cols Number of columns.
 @returns Matrix of zeros.
 */
Matrix* zeros(uint16_t n_rows, uint16_t n_cols) {
    Matrix* matrix = (Matrix*)malloc(sizeof(Matrix));
    matrix->rows = n_rows;
    matrix->cols = n_cols;
    // Allocate memory for the data, and fill with zeros.
    // matrix->data = (double*) calloc(n_cols*n_rows, sizeof(double));
    uint8_t** data = (uint8_t**)malloc(sizeof(uint8_t*) * n_rows);
    for (uint16_t x = 0; x < n_rows; x++) {
        data[x] = (uint8_t*)calloc(n_cols, sizeof(uint8_t));
    }
    matrix->data = data;
    return matrix;
}

/*
 @brief Create a matrix (2D tensor) with data provided.

 @param data Data to be inserted in row-major order.
 @param n_rows Number of rows.
 @param n_cols Number of columns.
 @returns Matrix of size [n,m] with values inserted.
 */
Matrix* fill(uint8_t* data, uint16_t n_rows, uint16_t n_cols) {
    Matrix* matrix = zeros(n_rows, n_cols);
    for (uint16_t x = 0; x < n_rows; x++) {
        for (uint16_t y = 0; y < n_cols; y++) {
            matrix->data[x][y] = data[n_cols * x + y];
        }
    }
    return matrix;
}

// @brief Free an allocated matrix. 
void free_matrix(Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        free(m->data[i]);
    }
    free(m->data);
    free(m);
}

// @brief Print matrix.
void print_matrix(Matrix* m) {
    for (uint16_t x = 0; x < m->rows; x++) {
        for (uint16_t y = 0; y < m->cols; y++) {
            // printf("%.2f\t", m->data[m->cols*x+y]);
            printf("%d\t", m->data[x][y]);
        }
        printf("\n");
    }
    printf("\n");
}

/*
In-place NxN matrix transpose. Unfortunately, due to time constraints and the
non-trivial nature of in-place O(1) NxM matrix transpose, we leave this
for future work.
We assume that NxM matrix is stored in row-major order with zero-based
indexing. This means that the (n,m) element, for `n = [0,n-1]` and `m = [0,
m-1]`, is stored at the memory address a = Mn+m (plus some offset, which we
ignore). In the transposed MxN matrix, the corresponding (m,n) element is
stored at the address a' = Nm+n.
@param arr Array to be transposed.
*/
void transpose(Matrix* arr) {
    // Switch rows/cols (to ensure matrix is properly printed) 
    int tmp_rows = arr->rows;
    arr->rows = arr->cols;
    arr->cols = tmp_rows;

    for (uint16_t i = 0; i < arr->rows; i++) {
        for (uint16_t j = 0; j < arr->cols; j++) {
            // Original memory location before permutation: a = Mn + m 
            // uint8_t* orig_addr = (arr->data[arr->cols * i + j]);
            uint8_t tmp = arr->data[i][j];
            arr->data[i][j] = arr->data[j][i];
            arr->data[j][i] = tmp;
        }
    }

    // Use only as many threads as are available
// #ifdef _OPENMP
//     omp_set_num_threads(omp_get_num_procs());
// #endif

// #pragma omp parallel for
//     for (uint16_t i = 1; i < arr->rows; i++) {
//         for (uint16_t j = 0; j < i; j++) {
//             uint8_t tmp = arr->data[i][j];
//             arr->data[i][j] = arr->data[j][i];
//             arr->data[j][i] = tmp;
//         }
//     }
}

int main(int argc, char* argv[]) {
    uint16_t N = atoi(argv[1]);
    uint8_t a[N * N];
    for (uint32_t i = 0; i < N * N; i++) {
        a[i] = (rand() % (255 - 0 + 1)) + 0; // generate random number between [0,255]
    }
    Matrix* orig = fill(a, N, N);
    print_matrix(orig);
    transpose(orig);
    print_matrix(orig);
    free_matrix(orig);
}
