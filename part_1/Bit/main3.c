#include <stdio.h>

int main() {
    unsigned int num;
    printf("Введите целое положительное число: ");
    scanf("%u", &num);

    int count = 0;
    while (num) {
        count += num & 1;
        num >>= 1;
    }

    printf("Количество единиц в двоичном представлении: %d\n", count);

    return 0;
}