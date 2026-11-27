add_library(catch2 STATIC ${PROJECT_SOURCE_DIR}/submodules/catch2/catch_main.cpp)
target_include_directories(catch2 PUBLIC ${PROJECT_SOURCE_DIR}/submodules)
target_compile_features(catch2 PUBLIC cxx_std_20)
target_compile_definitions(catch2 PUBLIC CATCH_CONFIG_ENABLE_BENCHMARKING)
