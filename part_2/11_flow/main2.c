#include <stdio.h>
#include <pthread.h>

#define N 18

long a = 0;
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;

void *thread_calc(void *args)
{
    int i, tmp;
    for (i = 0; i < 10000000; i++) {
        pthread_mutex_lock(&m1); // залочили, может умереть поток и все будет плохо,
        a = a + 1;
        pthread_mutex_unlock(&m1); // отлочили, только тот поток что залочил может отлочить
        tmp = a;
        tmp++;
        a = tmp;
    }
    return NULL;
}


int main(void)
{
    int i, i1;
    int *s;
    int v[N];
    pthread_t thread[N];

    for (i = 0; i < N; i ++) {
        v[i] = i;
        pthread_create(&thread[i], NULL, thread_calc, (void *)&v[i]);
    }

    for (i1 = 0; i1 < N; i1 ++) {
        pthread_join(thread[i1], (void **) &s);
    }

    return 0;
}
