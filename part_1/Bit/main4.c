#include <stdio.h>

int main() {
    unsigned int num;
    unsigned char new_byte;
    printf("Введите целое положительное число: ");
    scanf("%u", &num);
    printf("Введите новое значение для третьего байта (0-255): ");
    scanf("%hhu", &new_byte);

    // Обнуляем третий байт
    num &= ~(0xFF << 16);
    // Устанавливаем новое значение для третьего байта
    num |= (new_byte << 16);

    printf("Новое число: %u\n", num);

    return 0;
}