# ============================================================================
# 依赖管理 — 在此添加 find_package 调用
# ============================================================================

# nlohmann-json（用于 JSON 解析）
find_package(nlohmann_json CONFIG REQUIRED)

# 添加新依赖时：
#   1. vcpkg.json 中添加包名
#   2. 此处添加 find_package()
#   3. CMakeLists.txt 中添加 target_link_libraries()
