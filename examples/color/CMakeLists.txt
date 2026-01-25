cmake_minimum_required(VERSION 3.15)

# Color library tests
add_executable(ColorConversionTest ColorConversionTest.cpp)
add_executable(ColorQuantizeTest ColorQuantizeTest.cpp)
add_executable(ColorDitheringTest ColorDitheringTest.cpp)
add_executable(ColorAdvancedTest ColorAdvancedTest.cpp)

# Link against the necessary libraries
foreach(test ColorConversionTest ColorQuantizeTest ColorDitheringTest ColorAdvancedTest)
    target_link_libraries(${test} PRIVATE hgl-math hgl-color)
    target_include_directories(${test} PRIVATE 
        ${CMAKE_SOURCE_DIR}/CMCoreType/inc
        ${CMAKE_SOURCE_DIR}/CMMath/inc
    )
endforeach()

# Set output directory
set_target_properties(
    ColorConversionTest 
    ColorQuantizeTest 
    ColorDitheringTest 
    ColorAdvancedTest
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples/color
)
