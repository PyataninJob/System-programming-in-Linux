#include <stdio.h>

int main() {
    int num;
    printf("Введите целое отрицательное число: ");
    scanf("%d", &num);

    printf("Двоичное представление числа %d: ", num);
    //for (int i = sizeof(num) * 8 - 1; i >= 0; i--) // если нужно не выводить знаковый бит
    for (int i = sizeof(num) * 8; i >= 0; i--) // если нужно выводить знаковый бит
    {
        printf("%d", (num >> i) & 1);
    }
    printf("\n");

    return 0;
}