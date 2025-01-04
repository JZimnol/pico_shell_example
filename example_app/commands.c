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

#include <hardware/watchdog.h>
#include <pico/stdlib.h>

#include <stdio.h>
#include <stdlib.h>

#include <FreeRTOS.h>
#include <task.h>

#include <pico_freertos_shell/commands.h>

#include "common.h"

static int m_some_variable;

static void variable_set_cmd_handler(int argc, char **argv) {
    if (argc != 1) {
        LOG(variable_set, CMD_HANDLER, "Invalid number of arguments\n");
        return;
    }

    m_some_variable = atoi(argv[0]);
    LOG(variable_set, CMD_HANDLER, "Set variable to %d\n", m_some_variable);
}

static void variable_read_cmd_handler(int argc, char **argv) {
    if (argc != 0) {
        LOG(variable_read, CMD_HANDLER, "Invalid number of arguments\n");
        return;
    }

    LOG(variable_get, CMD_HANDLER, "Variable is %d\n", m_some_variable);
}

static void helloworld_cmd_handler(int argc, char **argv) {
    LOG(helloworld, CMD_HANDLER, "Hello World!\n");
    if (argc > 0) {
        LOG(helloworld, CMD_HANDLER, "My arguments:\n");
        for (size_t i = 0; i < argc; i++) {
            LOG(helloworld, CMD_HANDLER, "argv[%d] = %s\n", i, argv[i]);
        }
    }
}

static void reboot_cmd_handler(int argc, char **argv) {
    if (argc != 1) {
        LOG(reboot, CMD_HANDLER, "Invalid number of arguments\n");
        return;
    }

    int delay_ms = atoi(argv[0]);
    if (delay_ms < 0) {
        LOG(reboot, CMD_HANDLER, "Invalid delay: %d\n", delay_ms);
        return;
    }

    LOG(reboot, CMD_HANDLER, "Rebooting in %d milliseconds\n", delay_ms);
    vTaskDelay(delay_ms / portTICK_PERIOD_MS);
    watchdog_enable(1, 1);
    while (1)
        ;
}

static const pfs_command_t VARIABLE_SUBCOMMANDS[] = {
        PFS_COMMAND_INITIALIZER(set,
                                "set the variable to a value",
                                PFS_COMMAND_HANDLER(variable_set_cmd_handler)),
        PFS_COMMAND_INITIALIZER(read,
                                "read the value of the variable",
                                PFS_COMMAND_HANDLER(variable_read_cmd_handler)),
};

static const pfs_command_t SHELL_COMMANDS[] = {
        PFS_COMMAND_INITIALIZER(
                variable,
                "set or read a variable",
                PFS_SUBCOMMANDS(VARIABLE_SUBCOMMANDS,
                                ARRAY_SIZE(VARIABLE_SUBCOMMANDS))),
        PFS_COMMAND_INITIALIZER(helloworld,
                                "prints 'Hello World!' and a list of arguments",
                                PFS_COMMAND_HANDLER(helloworld_cmd_handler)),
        PFS_COMMAND_INITIALIZER(reboot,
                                "reboot the device after a delay",
                                PFS_COMMAND_HANDLER(reboot_cmd_handler)),
};

int example_app_register_commands(void) {
    return pfs_commands_register(SHELL_COMMANDS, ARRAY_SIZE(SHELL_COMMANDS));
}
