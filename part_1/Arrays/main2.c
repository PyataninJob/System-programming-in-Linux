#include <stdio.h>

int main() {
    int N;
    printf("Введите размер массива N: ");
    scanf("%d", &N);

    int array[N];
    printf("Введите элементы массива: ");
    for (int i = 0; i < N; i++) {
        scanf("%d", &array[i]);
    }

    printf("Массив в обратном порядке: ");
    for (int i = N - 1; i >= 0; i--) {
        printf("%d ", array[i]);
    }
    printf("\n");

    return 0;
}