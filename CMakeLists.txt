cmake_minimum_required(VERSION 3.9)
project(Progetto_SO)

set(CMAKE_CXX_STANDARD 11)

add_executable(Progetto_SO
        init/init.c
        init/init.h
        process/process.c
        process/process.h
        timer/timer.c
        timer/timer.h
        utils/utils.c
        utils/utils.h
        2017so_prj.pdf
        config.cfg
        LICENSE
        Makefile
        manager.c
        manager.h
        README.md
        ripcs.sh)
