#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_STORES 5
#define NUM_CUSTOMERS 3
#define INITIAL_STOCK 10000
#define CUSTOMER_NEED_MAX 100000
#define LOADER_ADD 5000

typedef struct {
    int stock[NUM_STORES]; // Запасы в магазинах
    pthread_mutex_t mutexes[NUM_STORES]; // Мьютексы для каждого магазина
    pthread_cond_t conds[NUM_STORES]; // Условия для каждого магазина
} Store;

typedef struct {
    int id; // Идентификатор покупателя
    int need; // Потребность покупателя
    int initial_need; // Начальная потребность покупателя
    Store *store; // Указатель на структуру магазина
} Customer;

typedef struct {
    Store *store; // Указатель на структуру магазина
} Loader;

void *customer_thread(void *arg);
void *loader_thread(void *arg);

int main() {
    srand(time(NULL)); // Инициализация генератора случайных чисел

    Store store;
    for (int i = 0; i < NUM_STORES; i++) {
        store.stock[i] = rand() % INITIAL_STOCK; // Инициализация запасов в магазинах
        store.mutexes[i] = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER; // Инициализация мьютексов
        store.conds[i] = (pthread_cond_t)PTHREAD_COND_INITIALIZER; // Инициализация условий
    }

    pthread_t customers[NUM_CUSTOMERS];
    Customer customer_data[NUM_CUSTOMERS];
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        customer_data[i].id = i + 1; // Установка идентификатора покупателя
        customer_data[i].need = rand() % CUSTOMER_NEED_MAX + 1; // Установка потребности покупателя
        customer_data[i].initial_need = customer_data[i].need; // Сохранение начальной потребности покупателя
        customer_data[i].store = &store; // Указатель на структуру магазина
        pthread_create(&customers[i], NULL, customer_thread, &customer_data[i]); // Создание потока покупателя
    }

    pthread_t loader;
    Loader loader_data = { .store = &store }; // Указатель на структуру магазина
    pthread_create(&loader, NULL, loader_thread, &loader_data); // Создание потока погрузчика

    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_join(customers[i], NULL); // Ожидание завершения потоков покупателей
    }

    pthread_cancel(loader); // Отмена потока погрузчика
    pthread_join(loader, NULL); // Ожидание завершения потока погрузчика

    for (int i = 0; i < NUM_STORES; i++) {
        pthread_mutex_destroy(&store.mutexes[i]); // Уничтожение мьютексов
        pthread_cond_destroy(&store.conds[i]); // Уничтожение условий
    }

    printf("___________________________________________________\n");

    // Вывод информации о начальной и конечной потребности покупателей
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        printf("Покупатель %d: начальная потребность = %d, конечная потребность = %d\n",
               customer_data[i].id, customer_data[i].initial_need, customer_data[i].need);
    }

    // Вывод информации о запасах в магазинах
    for (int i = 0; i < NUM_STORES; i++) {
        printf("Магазин %d: конечный запас = %d\n", i + 1, store.stock[i]);
    }

    return 0;
}

void *customer_thread(void *arg) {
    Customer *customer = (Customer *)arg;
    Store *store = customer->store;

    while (customer->need > 0) {
        int found_store = 0;
        for (int i = 0; i < NUM_STORES; i++) {
            pthread_mutex_lock(&store->mutexes[i]); // Захват мьютекса

            if (store->stock[i] > 0) {
                int taken = store->stock[i]; // Забираем весь товар
                store->stock[i] = 0; // Обнуляем запасы в магазине
                customer->need -= taken; // Уменьшаем потребность покупателя

                printf("Я поток покупатель %d, у меня потребность %d, я зашёл в магазин %d, там было %d, моя потребность стала %d. Всё, я уснул.\n",
                       customer->id, customer->need + taken, i + 1, taken, customer->need);

                pthread_cond_signal(&store->conds[i]); // Сигнализируем погрузчику
                pthread_mutex_unlock(&store->mutexes[i]); // Освобождаем мьютекс

                sleep(2); // Засыпаем на 2 секунды
                found_store = 1;
                break;
            }

            pthread_mutex_unlock(&store->mutexes[i]); // Освобождаем мьютекс
        }

        if (!found_store) {
            printf("Покупатель %d: все магазины заняты или имеют 0 баланс. Подожду 2 секундочки\n", customer->id);
            sleep(2); 
        }
    }

    printf("Покупатель %d насытился и завершился.\n", customer->id);
    return NULL;
}

void *loader_thread(void *arg) {
    Loader *loader = (Loader *)arg;
    Store *store = loader->store;

    while (1) {
        for (int i = 0; i < NUM_STORES; i++) {
            pthread_mutex_lock(&store->mutexes[i]); // Захват мьютекса

            store->stock[i] += LOADER_ADD; // Добавляем товар

            printf("Я в магазине %d, там было %d, я положил %d, теперь там %d, заснул.\n", i + 1, store->stock[i] - LOADER_ADD, LOADER_ADD, store->stock[i]);

            pthread_cond_signal(&store->conds[i]); // Сигнализируем покупателям
            pthread_mutex_unlock(&store->mutexes[i]); // Освобождаем мьютекс

            sleep(1); // Засыпаем на 1 секунду
        }
    }

    return NULL;
}