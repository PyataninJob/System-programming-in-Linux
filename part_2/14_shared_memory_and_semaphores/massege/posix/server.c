#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>

#define SHM_NAME "/posix_shm"  // Имя разделяемой памяти
#define SEM_NAME1 "/posix_sem1" // Имя первого семафора
#define SEM_NAME2 "/posix_sem2" // Имя второго семафора
#define SHM_SIZE 1024           // Размер разделяемой памяти

void check_error(int result, const char *msg) {
    if (result == -1) {
        perror(msg);
        exit(1);
    }
}

void print_sem_value(sem_t *sem, const char *sem_name) {
    int value;
    check_error(sem_getvalue(sem, &value), "sem_getvalue");
    printf("Значение семафора %s: %d\n", sem_name, value);
}

int main() {
    int shm_fd;
    char *shm_ptr;
    sem_t *sem1, *sem2;

    // Создание и инициализация первого семафора
    sem1 = sem_open(SEM_NAME1, O_CREAT, 0666, 0);
    if (sem1 == SEM_FAILED) {
        perror("sem_open sem1");
        exit(1);
    }

    // Создание и инициализация второго семафора
    sem2 = sem_open(SEM_NAME2, O_CREAT, 0666, 0);
    if (sem2 == SEM_FAILED) {
        perror("sem_open sem2");
        exit(1);
    }

    // Создание и настройка разделяемой памяти
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    check_error(shm_fd, "shm_open");

    check_error(ftruncate(shm_fd, SHM_SIZE), "ftruncate");

    shm_ptr = mmap(0, SHM_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // Запись сообщения в разделяемую память
    strcpy(shm_ptr, "Hi!");
    printf("Отправили свое сообщение\n");

    // Сигнал клиенту, что сообщение отправлено
    check_error(sem_post(sem1), "sem_post sem1");
    printf("Сигнал клиенту через семафор sem1\n");
    print_sem_value(sem1, SEM_NAME1);

    // Ожидание ответа от клиента
    printf("Ожидание ответа от клиента...\n");
    check_error(sem_wait(sem2), "sem_wait sem2");
    printf("Получен сигнал от клиента через семафор sem2\n");
    print_sem_value(sem2, SEM_NAME2);

    // Вывод ответа клиента
    printf("Ответ клиента: %s\n", shm_ptr);

    // Очистка ресурсов
    check_error(munmap(shm_ptr, SHM_SIZE), "munmap");
    check_error(shm_unlink(SHM_NAME), "shm_unlink");
    check_error(sem_close(sem1), "sem_close sem1");
    check_error(sem_close(sem2), "sem_close sem2");
    check_error(sem_unlink(SEM_NAME1), "sem_unlink sem1");
    check_error(sem_unlink(SEM_NAME2), "sem_unlink sem2");

    return 0;
}