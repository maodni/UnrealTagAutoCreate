# ============================================================================
# UnrealTagAutoCreate - UE标签自动创建工具
# ============================================================================
cmake_minimum_required(VERSION 4.1)
project(UnrealTagAutoCreate
        VERSION 1.0.0
        DESCRIPTION "Unreal Engine Tag Auto Create Tool"
        LANGUAGES CXX
)

# ---------------------------------------------------------------------------
# C++ Standard
# ---------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------------------------------------------
# 依赖管理 (vcpkg manifest mode)
# ---------------------------------------------------------------------------
include(cmake/Dependencies.cmake)

# ---------------------------------------------------------------------------
# 编译器警告
# ---------------------------------------------------------------------------
include(cmake/CompilerWarnings.cmake)

# ---------------------------------------------------------------------------
# 源文件
# ---------------------------------------------------------------------------
set(SOURCES
        main.cpp
)

set(HEADERS
        source/tag_read_print.cpp
        source/tag_read_print.h
)
add_compile_options("/utf-8")
# ---------------------------------------------------------------------------
# 目标
# ---------------------------------------------------------------------------
add_executable(${PROJECT_NAME}
        ${SOURCES}
        ${HEADERS}
)
target_compile_options(${PROJECT_NAME} PRIVATE "/utf-8")
# ---------------------------------------------------------------------------
# 包含目录
# ---------------------------------------------------------------------------
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# ---------------------------------------------------------------------------
# 链接库
# ---------------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE
        nlohmann_json::nlohmann_json
)

# ---------------------------------------------------------------------------
# 平台相关设置
# ---------------------------------------------------------------------------
if (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            WIN32_EXECUTABLE FALSE
    )
endif ()

# ---------------------------------------------------------------------------
# 输出命名
# ---------------------------------------------------------------------------
set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "UnrealTagAutoCreate"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
