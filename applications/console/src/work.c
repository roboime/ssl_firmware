#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#include "work.h"

LOG_MODULE_REGISTER(work, LOG_LEVEL_DBG);

#define STACK_SIZE 1024
#define THREAD_PRIORITY 5

K_THREAD_STACK_DEFINE(motor_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(communication_stack, STACK_SIZE);

struct k_thread motor_thread;
struct k_thread communication_thread;

struct k_work_delayable motor_work;
struct k_work_delayable communication_work;

static void motor_work_handler(struct k_work *work)
{
    // Handle motor control
    LOG_INF("Motor control");
    k_work_schedule(&motor_work, K_MSEC(1000));
}

static void communication_work_handler(struct k_work *work)
{
    // Handle SX1280 communication
    LOG_INF("SX1280 communication");
    k_work_schedule(&communication_work, K_MSEC(5000));
}

static void motor_thread_entry(void *p1, void *p2, void *p3)
{
    k_work_init_delayable(&motor_work, motor_work_handler);
    k_work_schedule(&motor_work, K_NO_WAIT);

    while (1) {
        k_sleep(K_FOREVER);
    }
}

static void communication_thread_entry(void *p1, void *p2, void *p3)
{
    k_work_init_delayable(&communication_work, communication_work_handler);
    k_work_schedule(&communication_work, K_NO_WAIT);

    while (1) {
        k_sleep(K_FOREVER);
    }
}

void work_init(void)
{
    LOG_DBG("Debug message test");
    k_thread_create(&motor_thread, motor_stack, STACK_SIZE,
                    motor_thread_entry, NULL, NULL, NULL,
                    THREAD_PRIORITY, 0, K_NO_WAIT);

    k_thread_create(&communication_thread, communication_stack, STACK_SIZE,
                    communication_thread_entry, NULL, NULL, NULL,
                    THREAD_PRIORITY, 0, K_NO_WAIT);
}