#include <stdio.h>
#include <pthread.h>

#define N 5

void *thread_calc(void *args)
{
    int *i = (int *) args;
    printf("thread num %d\n", *i);
    return NULL;
}

int main(void)
{
    int i, i1;
    int *s;
    int v[N];
    pthread_t thread[N];

    for (i = 0; i < N; i ++) {
        v[i] = i; // отправляем указатель на отдельные i
        pthread_create(&thread[i], NULL, thread_calc, (void *)&v[i]);
    }

    for (i1 = 0; i1 < N; i1 ++) { // Другая переменная и все лучше
        pthread_join(thread[i1], (void **) &s);
    }

    return 0;
}
