#include <stdio.h>
#include "main.h"

int app_main() {
    printf("app Main!!!\n");
    while (1) {
        HAL_GPIO_TogglePin(USER_LED1_GPIO_Port, USER_LED2_Pin);
        HAL_GPIO_TogglePin(USER_LED2_GPIO_Port, USER_LED1_Pin);
        HAL_Delay(500);
    }
    return 0;
}