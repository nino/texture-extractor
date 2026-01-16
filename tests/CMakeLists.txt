# Test CMakeLists.txt

# Helper macro to create a test
macro(add_gui_test test_name)
    add_executable(${test_name} ${test_name}.cpp)
    target_link_libraries(${test_name} PRIVATE
        texture_extractor_lib
        Qt${QT_VERSION_MAJOR}::Test
        Qt${QT_VERSION_MAJOR}::Widgets
    )
    add_test(NAME ${test_name} COMMAND ${test_name})
endmacro()

# Unit tests
add_gui_test(test_EditableRectItem)
add_gui_test(test_SourceImageView)
add_gui_test(test_ExtractedView)
add_gui_test(test_DocumentWindow)
add_gui_test(test_MainWindow)

# Integration tests
add_gui_test(test_Integration)

# Test utilities library
add_library(test_helpers STATIC test_helpers.hpp test_helpers.cpp)
target_link_libraries(test_helpers PUBLIC
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Widgets
)

# Link test helpers to all tests
target_link_libraries(test_EditableRectItem PRIVATE test_helpers)
target_link_libraries(test_SourceImageView PRIVATE test_helpers)
target_link_libraries(test_ExtractedView PRIVATE test_helpers)
target_link_libraries(test_DocumentWindow PRIVATE test_helpers)
target_link_libraries(test_MainWindow PRIVATE test_helpers)
target_link_libraries(test_Integration PRIVATE test_helpers)
