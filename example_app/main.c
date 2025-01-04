/*
 * Copyright (c) 2025 Jakub Zimnol
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <string.h>

#include <hardware/gpio.h>
#include <pico/stdlib.h>

#include <FreeRTOS.h>
#include <task.h>

#include <pico_freertos_shell/init.h>

#include "common.h"

#define MAIN_APP_STACK_SIZE (1000U)
static StackType_t main_app_task_stack[MAIN_APP_STACK_SIZE];
static StaticTask_t main_app_task_buffer;

#define DELAY_MS (2000U)

static void main_app_task(__unused void *params) {
    int x = 0;
    while (1) {
        LOG(main_app, INF, "This is a message using printf: %d\n", x++);
        vTaskDelay(DELAY_MS / portTICK_PERIOD_MS);
        LOG(main_app, INF, "This is a message using puts\n");
        vTaskDelay(DELAY_MS / portTICK_PERIOD_MS);
    }
}

int main(void) {
    stdio_init_all();

    if (example_app_register_commands()) {
        LOG(main, ERR, "Failed to register commands\n");
    }

    pfs_init();

    xTaskCreateStatic(main_app_task, "MainAppTask", MAIN_APP_STACK_SIZE, NULL,
                      tskIDLE_PRIORITY + 2UL, main_app_task_stack,
                      &main_app_task_buffer);

    vTaskStartScheduler();
}
