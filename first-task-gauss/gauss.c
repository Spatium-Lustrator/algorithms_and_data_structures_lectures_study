#include <stdio.h>
#include <stdlib.h>

double calculate_the_determinant(double **matrix, int count_of_rows, int count_of_columns) {
    int k, l, m;
    double result = 1.0, cur_non_zero_value, determinant_multiplied_on=1.0;

    for (k=0; k<count_of_rows-1; k++) {
        for (l=k+1; l<count_of_rows; l++) {
            cur_non_zero_value = matrix[l][k];
            determinant_multiplied_on *= matrix[k][k];
            for (m=k; m<count_of_columns; m++) {
                matrix[l][m] = matrix[l][m]*matrix[k][k] - cur_non_zero_value*matrix[k][m];
            }
        }
    }

    for (k=0; k<count_of_rows; k++) { result *= matrix[k][k]; }
    result /= determinant_multiplied_on;


    return result;
}

int main(void) {

    double **matrix;
    int count_of_rows, count_of_columns;
    double determinant;
    int i, j;

    scanf("%d %d", &count_of_rows, &count_of_columns);

    matrix = (double**) malloc(sizeof(double*)*count_of_rows);

    for (i=0; i<count_of_rows; i++) {
        matrix[i] = (double*) malloc(sizeof(double)*count_of_columns);
        for (j=0; j<count_of_columns; j++) {
            scanf("%lf", matrix[i]+j);
        }
    }

    if (count_of_columns == count_of_rows) {
        printf("Calculated determinant: %0.2lf\n", calculate_the_determinant(matrix, count_of_rows, count_of_columns));
    } else {
        printf("Sorry, invalid matrix sizes :(");
    }


    for (i=0; i<count_of_rows; i++) {
        free(matrix[i]);
    }

    free(matrix);




    return 0;
}