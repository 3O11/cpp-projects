add_executable(StaticRanges
    "main.cpp"
)

add_executable(sr_all_view
    "static_for_test_all_view.cpp"
)

add_executable(sr_concepts
    "static_for_test_concepts.cpp"
)

add_executable(sr_convenience
    "static_for_test_convenience.cpp"
)

add_executable(sr_for_each
    "static_for_test_for_each.cpp"
)

set_property(TARGET StaticRanges PROPERTY CXX_STANDARD 20)
set_property(TARGET sr_all_view PROPERTY CXX_STANDARD 20)
set_property(TARGET sr_concepts PROPERTY CXX_STANDARD 20)
set_property(TARGET sr_convenience PROPERTY CXX_STANDARD 20)
set_property(TARGET sr_for_each PROPERTY CXX_STANDARD 20)
