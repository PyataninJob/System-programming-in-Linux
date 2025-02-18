#include <stdio.h>

void print_binary(unsigned int num)
{
    for (int i = sizeof(num) * 8 - 1; i >= 0; i--)
    {
        printf("%u", (num >> i) & 1);
    }
    printf("\n");
}

int main()
{
    int num;
    unsigned int new_byte;

    printf("Введите целое положительное число: ");
    scanf("%d", &num);

    if (num <= 0)
    {
        printf("Ошибка: число должно быть положительным.\n");
        return 1;
    }

    print_binary((unsigned int)num);

    printf("Введите новое значение для третьего байта (0-255): ");
    scanf("%u", &new_byte);

    if (new_byte > 255)
    {
        printf("Ошибка: значение для третьего байта должно быть в диапазоне 0-255.\n");
        return 1;
    }

    unsigned char *ptr = (unsigned char *)&num; // Мы как бы интерпретируем число как масив байтов
    ptr[2] = new_byte;                          // обращаемся к 3 байту и земеняем его

    printf("Новое число: %d\n", num);
    printf("Двоичное представление нового числа:\n");
    print_binary((unsigned int)num);

    return 0;
}