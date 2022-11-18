add_executable(test
        test.c
)


target_link_libraries(test PRIVATE mlt m Threads::Threads PkgConfig::sdl2)

