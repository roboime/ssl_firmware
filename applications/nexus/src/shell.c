/* Zephyr includes */
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>

LOG_MODULE_REGISTER(shell, LOG_LEVEL_DBG)

/* clang-format off */

SHELL_STATIC_SUBCMD_SET_CREATE(
	sub_app_config,

	SHELL_SUBCMD_SET_END
);

SHELL_STATIC_SUBCMD_SET_CREATE(
	sub_app,

	SHELL_SUBCMD_SET_END
);


/* clang-format on */

