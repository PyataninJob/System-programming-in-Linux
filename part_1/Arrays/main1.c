#include <stdio.h>

int main() {
    int N;
    printf("Введите размер матрицы N: ");
    scanf("%d", &N);

    int matrix[N][N];
    int value = 1;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = value++;
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    return 0;
}