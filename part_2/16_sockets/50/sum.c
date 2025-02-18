#include <stdio.h>
#include <stdint.h>

uint16_t compute_checksum(uint16_t *header, int len) {
    uint32_t sum = 0;

    // Сложение всех 16-битных слов заголовка
    for (int i = 0; i < len; i++) {
        sum += header[i];
        // Если есть перенос, добавляем его к сумме
        if (sum & 0xFFFF0000) {
            sum = (sum & 0xFFFF) + (sum >> 16);
        }
    }

    return ~sum;
}

int main() {
    // Пример IP-заголовка (без контрольной суммы)
    uint16_t ip_header[] = {
        0x4500, 0x0034, 0x1c46, 0x4000, 0x4006, 0x0000, // Первые 6 слов (контрольная сумма = 0x0000)
        0xc0a8, 0x0001, 0xc0a8, 0x0002  // Последние 4 слова (IP-адреса источника и назначения)
    };

    // Вычисление контрольной суммы
    uint16_t checksum = compute_checksum(ip_header, sizeof(ip_header) / 2);

    // Вывод контрольной суммы
    printf("Контрольная сумма IP-заголовка: 0x%04x\n", checksum);

    return 0;
}