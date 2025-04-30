cmake_minimum_required(VERSION 3.23)
project(tcmalloc LANGUAGES C CXX ASM)

find_package(absl)

include(GNUInstallDirs)

add_library(tcmalloc)
target_sources(tcmalloc
  PRIVATE
    tcmalloc/allocation_sample.cc
    tcmalloc/allocation_sampling.cc
    tcmalloc/arena.cc
    tcmalloc/background.cc
    tcmalloc/central_freelist.cc
    tcmalloc/common.cc
    tcmalloc/cpu_cache.cc
    tcmalloc/deallocation_profiler.cc
    tcmalloc/error_reporting.cc
    tcmalloc/experimental_pow2_size_class.cc
    tcmalloc/experiment.cc
    tcmalloc/global_stats.cc
    tcmalloc/guarded_page_allocator.cc
    tcmalloc/huge_address_map.cc
    tcmalloc/huge_allocator.cc
    tcmalloc/huge_cache.cc
    tcmalloc/huge_page_aware_allocator.cc
    tcmalloc/legacy_size_classes.cc
    tcmalloc/malloc_extension.cc
    tcmalloc/malloc_hook.cc
    tcmalloc/malloc_tracing_extension.cc
    tcmalloc/page_allocator.cc
    tcmalloc/page_allocator_interface.cc
    tcmalloc/pagemap.cc
    tcmalloc/parameters.cc
    tcmalloc/peak_heap_tracker.cc
    tcmalloc/reuse_size_classes.cc
    tcmalloc/sampler.cc
    tcmalloc/segv_handler.cc
    tcmalloc/size_classes.cc
    tcmalloc/sizemap.cc
    tcmalloc/span.cc
    tcmalloc/stack_trace_table.cc
    tcmalloc/static_vars.cc
    tcmalloc/stats.cc
    tcmalloc/system-alloc.cc
    tcmalloc/tcmalloc.cc
    tcmalloc/thread_cache.cc
    tcmalloc/transfer_cache.cc

    # Internal files
    tcmalloc/internal/cache_topology.cc
    tcmalloc/internal/environment.cc
    tcmalloc/internal/hook_list.cc
    tcmalloc/internal/logging.cc
    tcmalloc/internal/memory_stats.cc
    tcmalloc/internal/memory_tag.cc
    tcmalloc/internal/mincore.cc
    tcmalloc/internal/numa.cc
    tcmalloc/internal/page_size.cc
    tcmalloc/internal/pageflags.cc
    tcmalloc/internal/proc_maps.cc
    tcmalloc/internal/sysinfo.cc
    tcmalloc/internal/util.cc

    tcmalloc/internal/percpu.cc
    tcmalloc/internal/percpu_rseq_asm.S
    tcmalloc/internal/percpu_rseq_unsupported.cc

  PUBLIC
  FILE_SET tcmallocHeaders
  TYPE HEADERS
  FILES
    tcmalloc/malloc_extension.h
    tcmalloc/malloc_hook.h
    tcmalloc/tcmalloc.h

    tcmalloc/internal/config.h
    tcmalloc/internal/declarations.h
)

# tcmalloc requires c++17 (but does not require gnu extensions)
target_compile_features(tcmalloc PUBLIC cxx_std_17)
set_target_properties(tcmalloc PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(tcmalloc PUBLIC
  absl::base
  absl::flat_hash_map
  absl::debugging
  absl::memory
  absl::span
  absl::status
  absl::statusor
  absl::str_format
  absl::time)
target_include_directories(tcmalloc PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
add_library(tcmalloc::tcmalloc ALIAS tcmalloc)

# As a sanity check, link tcmalloc into a simple hello world executable
# Since the executable doesn't reference any tcmalloc symbol, this uses
# the linker's --whole-archive to force it to include that library.
add_executable(hello_world
  tcmalloc/hello_world.cc
)
target_link_libraries(hello_world PRIVATE
  "-Wl,--whole-archive" tcmalloc "-Wl,--no-whole-archive")

install(TARGETS tcmalloc EXPORT tcmallocTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/tcmalloc
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILE_SET tcmallocHeaders)
install(EXPORT tcmallocTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tcmalloc
  NAMESPACE tcmalloc::)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${PROJECT_NAME}Config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tcmallocConfig.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tcmalloc)
