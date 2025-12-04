/* ------------------------------------------------------------
 * Автор: Сокун Михаил
 * Назначение программы:
 *     Имитация работы производственной линии: производство
 *     деталей A, B, C, сборка модуля (A+B) и сборка изделия
 *     (модуль + C). Добавлен механизм корректного завершения
 *     программы, обработка ошибок и освобождение ресурсов.
 * Файл: production_line.c
 * ------------------------------------------------------------
 */

 #include <stdio.h>
 #include <pthread.h>
 #include <unistd.h>
 #include <semaphore.h>
 #include <signal.h>
 #include <stdlib.h>
 
 // Семафоры
 sem_t sem_A, sem_B, sem_C, sem_module;
 
 // Потоки
 pthread_t tA, tB, tC, tModule, tProduct;
 
 // Обработчик Ctrl+C
 void handle_sigint(int sig) {
     printf("\nПoлyчeн SIGINT - немедленное завершение потоков...\n");
 
     // Немедленное уничтожение потоков
     pthread_cancel(tA);
     pthread_cancel(tB);
     pthread_cancel(tC);
     pthread_cancel(tModule);
     pthread_cancel(tProduct);
 }
 
 // Производство детали A
 void* produce_A(void* arg) {
     pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
     pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
 
     while (1) {
         sleep(1);
         sem_post(&sem_A);
         printf("Изготовлена деталь A\n");
     }
 }
 
 // Производство детали B
 void* produce_B(void* arg) {
     pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
     pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
 
     while (1) {
         sleep(2);
         sem_post(&sem_B);
         printf("Изготовлена деталь B\n");
     }
 }
 
 // Производство детали C
 void* produce_C(void* arg) {
     pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
     pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
 
     while (1) {
         sleep(3);
         sem_post(&sem_C);
         printf("Изготовлена деталь C\n");
     }
 }
 
 // Сборка модуля из A и B
 void* assemble_module(void* arg) {
     pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
     pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
 
     while (1) {
         sem_wait(&sem_A);
         sem_wait(&sem_B);
         printf("Собран модуль (A + B)\n");
         sem_post(&sem_module);
     }
 }
 
 // Сборка изделия из модуля и C
 void* assemble_product(void* arg) {
     pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
     pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
 
     while (1) {
         sem_wait(&sem_module);
         sem_wait(&sem_C);
         printf("➡️  Собрано готовое изделие (модуль + C)\n\n");
     }
 }
 
 int main() {
 
     // Установка обработчика Ctrl+C
     signal(SIGINT, handle_sigint);
 
     // Инициализация семафоров
     sem_init(&sem_A, 0, 0);
     sem_init(&sem_B, 0, 0);
     sem_init(&sem_C, 0, 0);
     sem_init(&sem_module, 0, 0);
 
     // Создание потоков с проверкой ошибок
     if (pthread_create(&tA, NULL, produce_A, NULL) ||
         pthread_create(&tB, NULL, produce_B, NULL) ||
         pthread_create(&tC, NULL, produce_C, NULL) ||
         pthread_create(&tModule, NULL, assemble_module, NULL) ||
         pthread_create(&tProduct, NULL, assemble_product, NULL)) {
         perror("Ошибка создания потока");
         exit(1);
     }
 
     // Ожидание завершения потоков
     pthread_join(tA, NULL);
     pthread_join(tB, NULL);
     pthread_join(tC, NULL);
     pthread_join(tModule, NULL);
     pthread_join(tProduct, NULL);
 
     // Уничтожение семафоров
     sem_destroy(&sem_A);
     sem_destroy(&sem_B);
     sem_destroy(&sem_C);
     sem_destroy(&sem_module);
 
     printf("Bce потоки остановлены. Программа завершена.\n");
     return 0;
 }

