# subsys/CMakeLists.txt
zephyr_library()

# Conditionally add source files if CONFIG_ROBO_LED is set
zephyr_library_sources_ifdef(CONFIG_ROBO_LED robo_led.c)
