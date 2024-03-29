#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    char c = 0;
    char str[100];
    memset(str,0,sizeof(str));

    while(c != '\n') {
        c = getchar();
        // Para bloquear o terminal e esperar por ele, precisamos cuidar do buffer
        if (c != 0xff) {
            str[strlen(str)] = c;
            printf("%c",c);
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    printf("Voce digitou: %s", str);
}
