# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

set(BOARD st_nucleo_h723zg)

project(nexus)

target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/shell.c)
target_sources(app PRIVATE src/sx1280.c)
target_sources(app PRIVATE src/work.c)
target_sources(app PRIVATE src/init.c)