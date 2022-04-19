add_executable(Units
    "static_vector.h"
    "units.h"
    "main.cpp"
)

add_executable(Test1
    "static_vector.h"
    "units.h"
    "test1.cpp"
)

add_executable(Test2
    "static_vector.h"
    "units.h"
    "test2.cpp"
)

add_executable(Test3
    "static_vector.h"
    "units.h"
    "test3.cpp"
)

add_executable(Test4
    "static_vector.h"
    "units.h"
    "test4.cpp"
)


set_property(TARGET Units PROPERTY CXX_STANDARD 20)
set_property(TARGET Test1 PROPERTY CXX_STANDARD 20)
set_property(TARGET Test2 PROPERTY CXX_STANDARD 20)
set_property(TARGET Test3 PROPERTY CXX_STANDARD 20)
set_property(TARGET Test4 PROPERTY CXX_STANDARD 20)