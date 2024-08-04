#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

// ANSI color codes
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// Shell command implementations
static int cmd_hello(const struct shell *shell, size_t argc, char **argv)
{
    shell_print(shell, "Hello, %s!", (argc > 1) ? argv[1] : "world");
    return 0;
}

static int cmd_menu(const struct shell *shell, size_t argc, char **argv)
{
    shell_print(shell, ANSI_COLOR_GREEN "Menu:" ANSI_COLOR_RESET);
    shell_print(shell, "1. Option 1 - Print Hello");
    shell_print(shell, "2. Option 2 - Print Goodbye");
    shell_print(shell, "3. Exit");
    return 0;
}

static int cmd_option1(const struct shell *shell, size_t argc, char **argv)
{
    shell_print(shell, "You selected Option 1");
    shell_print(shell, "Type 'hello [name]' to print a greeting.");
    return 0;
}

static int cmd_option2(const struct shell *shell, size_t argc, char **argv)
{
    shell_print(shell, "You selected Option 2");
    shell_print(shell, "Goodbye!");
    return 0;
}

static int cmd_exit(const struct shell *shell, size_t argc, char **argv)
{
    shell_print(shell, "Exiting...");
    return 0;
}

// Register commands
SHELL_CMD_REGISTER(hello, NULL, "Prints a greeting message", cmd_hello);
SHELL_CMD_REGISTER(menu, NULL, "Displays the menu", cmd_menu);
SHELL_CMD_REGISTER(option1, NULL, "Select Option 1", cmd_option1);
SHELL_CMD_REGISTER(option2, NULL, "Select Option 2", cmd_option2);
SHELL_CMD_REGISTER(exit, NULL, "Exit the application", cmd_exit);

void main(void)
{
    LOG_INF("Application started");

    // Print initial welcome message
    shell_print(shell_backend_uart_get_ptr(), ANSI_COLOR_GREEN "Welcome to the Zephyr terminal UI!" ANSI_COLOR_RESET);
    shell_print(shell_backend_uart_get_ptr(), "Type 'menu' to see options.");

    while (1) {
        LOG_INF("Alive");
        k_sleep(K_MSEC(1000));
        LOG_DBG("Debug");
        k_sleep(K_MSEC(1000));
    }
}
