#include <stdio.h>

int main() {
    unsigned int num;
    printf("Введите целое положительное число: ");
    scanf("%u", &num);

    printf("Двоичное представление числа %u: ", num);
    //for (int i = sizeof(num) * 8 - 1; i >= 0; i--) // если нужно не выводить знаковый бит
    for (int i = sizeof(num) * 8; i >= 0; i--) // если нужно выводить знаковый бит
    {
        printf("%u", (num >> i) & 1);
    }
    printf("\n");

    return 0;
}