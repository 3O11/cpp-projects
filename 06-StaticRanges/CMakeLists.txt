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

add_executable(sr_std
    "static_for_test_std.cpp"
)

add_executable(sr_copy
    "static_for_test_copy.cpp"
)

add_executable(sr_iota_view
    "static_for_test_iota_view.cpp"
)

add_executable(sr_static_iota_view
    "static_for_test_static_iota_view.cpp"
)

add_executable(sr_to
    "static_for_test_to.cpp"
)

add_executable(sr_transform
    "static_for_test_transform.cpp"
)

add_executable(sr_transform_view
    "static_for_test_transform_view.cpp"
)

set_property(TARGET StaticRanges PROPERTY CXX_STANDARD 20)
set_property(TARGET sr_all_view PROPERTY CXX_STANDARD 20)
set_property(TARGET sr_concepts PROPERTY CXX_STANDARD 20)
set_property(TARGET sr_convenience PROPERTY CXX_STANDARD 20)
set_property(TARGET sr_for_each PROPERTY CXX_STANDARD 20)
set_property(TARGET sr_std PROPERTY CXX_STANDARD 20)
set_property(TARGET sr_copy PROPERTY CXX_STANDARD 20)
set_property(TARGET sr_iota_view PROPERTY CXX_STANDARD 20)
set_property(TARGET sr_static_iota_view PROPERTY CXX_STANDARD 20)
set_property(TARGET sr_to PROPERTY CXX_STANDARD 20)
set_property(TARGET sr_transform PROPERTY CXX_STANDARD 20)
set_property(TARGET sr_transform_view PROPERTY CXX_STANDARD 20)
