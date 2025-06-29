cmake_minimum_required(VERSION 3.16)
project(textunpack)

add_executable(textunpack
  "./external/sokol_time/sokol_time.c"
  "./external/sokol_time/sokol_time.h"
  "./src/ix/ix.cpp"
  "./src/ix/ix.hpp"
  # "./src/ix/ix_AhoCorasick.cpp"
  # "./src/ix/ix_AhoCorasick.hpp"
  "./src/ix/ix_Array.cpp"
  "./src/ix/ix_Array.hpp"
  # "./src/ix/ix_BTree.cpp"
  # "./src/ix/ix_BTree.hpp"
  # "./src/ix/ix_BinaryRelation.cpp"
  # "./src/ix/ix_BinaryRelation.hpp"
  # "./src/ix/ix_BitVector.cpp"
  # "./src/ix/ix_BitVector.hpp"
  "./src/ix/ix_Buffer.cpp"
  "./src/ix/ix_Buffer.hpp"
  "./src/ix/ix_CmdArgsEater.cpp"
  "./src/ix/ix_CmdArgsEater.hpp"
  # "./src/ix/ix_ConcurrentHashSet.cpp"
  # "./src/ix/ix_ConcurrentHashSet.hpp"
  # "./src/ix/ix_ConditionVariable.cpp"
  # "./src/ix/ix_ConditionVariable.hpp"
  # "./src/ix/ix_ContextMenu.cpp"
  # "./src/ix/ix_ContextMenu.hpp"
  # "./src/ix/ix_DoubleArrayAhoCorasick.cpp"
  # "./src/ix/ix_DoubleArrayAhoCorasick.hpp"
  "./src/ix/ix_DumbInt.cpp"
  "./src/ix/ix_DumbInt.hpp"
  "./src/ix/ix_DumbString.cpp"
  "./src/ix/ix_DumbString.hpp"
  # "./src/ix/ix_FixedLengthBitVector.cpp"
  # "./src/ix/ix_FixedLengthBitVector.hpp"
  "./src/ix/ix_Function.cpp"
  "./src/ix/ix_Function.hpp"
  # "./src/ix/ix_Handle.cpp"
  # "./src/ix/ix_Handle.hpp"
  # "./src/ix/ix_HashMapDoubleArray.cpp"
  # "./src/ix/ix_HashMapDoubleArray.hpp"
  "./src/ix/ix_HashMapSingleArray.cpp"
  "./src/ix/ix_HashMapSingleArray.hpp"
  # "./src/ix/ix_HashMultiMap.cpp"
  # "./src/ix/ix_HashMultiMap.hpp"
  "./src/ix/ix_HashSet.cpp"
  "./src/ix/ix_HashSet.hpp"
  "./src/ix/ix_HollowValue.cpp"
  "./src/ix/ix_HollowValue.hpp"
  # "./src/ix/ix_Json.cpp"
  # "./src/ix/ix_Json.hpp"
  # "./src/ix/ix_LinkedList.cpp"
  # "./src/ix/ix_LinkedList.hpp"
  "./src/ix/ix_Logger.cpp"
  "./src/ix/ix_Logger.hpp"
  "./src/ix/ix_Mutex.cpp"
  "./src/ix/ix_Mutex.hpp"
  "./src/ix/ix_Option.cpp"
  "./src/ix/ix_Option.hpp"
  "./src/ix/ix_Pair.hpp"
  # "./src/ix/ix_PersistentPool.cpp"
  # "./src/ix/ix_PersistentPool.hpp"
  # "./src/ix/ix_Pool.cpp"
  # "./src/ix/ix_Pool.hpp"
  # "./src/ix/ix_Process.cpp"
  # "./src/ix/ix_Process.hpp"
  "./src/ix/ix_Result.cpp"
  "./src/ix/ix_Result.hpp"
  "./src/ix/ix_Span.cpp"
  "./src/ix/ix_Span.hpp"
  # "./src/ix/ix_SpanPool.cpp"
  # "./src/ix/ix_SpanPool.hpp"
  "./src/ix/ix_StackVector.cpp"
  "./src/ix/ix_StackVector.hpp"
  # "./src/ix/ix_StackVectorMap.cpp"
  # "./src/ix/ix_StackVectorMap.hpp"
  # "./src/ix/ix_StringArena.cpp"
  # "./src/ix/ix_StringArena.hpp"
  "./src/ix/ix_StringView.cpp"
  "./src/ix/ix_StringView.hpp"
  "./src/ix/ix_SystemManager.cpp"
  "./src/ix/ix_SystemManager.hpp"
  # "./src/ix/ix_SystemTray.cpp"
  # "./src/ix/ix_SystemTray.hpp"
  # "./src/ix/ix_TagArena.cpp"
  # "./src/ix/ix_TagArena.hpp"
  "./src/ix/ix_TempFile.cpp"
  "./src/ix/ix_TempFile.hpp"
  "./src/ix/ix_Thread.cpp"
  "./src/ix/ix_Thread.hpp"
  "./src/ix/ix_ThreadPool.cpp"
  "./src/ix/ix_ThreadPool.hpp"
  "./src/ix/ix_UniquePointer.cpp"
  "./src/ix/ix_UniquePointer.hpp"
  # "./src/ix/ix_Vec2.cpp"
  # "./src/ix/ix_Vec2.hpp"
  # "./src/ix/ix_Vec2i.cpp"
  # "./src/ix/ix_Vec2i.hpp"
  "./src/ix/ix_Vector.cpp"
  "./src/ix/ix_Vector.hpp"
  # "./src/ix/ix_VectorMap.cpp"
  # "./src/ix/ix_VectorMap.hpp"
  "./src/ix/ix_Vector_util.cpp"
  "./src/ix/ix_Vector_util.hpp"
  "./src/ix/ix_Vector_util_fwd.hpp"
  "./src/ix/ix_Windows.hpp"
  "./src/ix/ix_Writer.cpp"
  "./src/ix/ix_Writer.hpp"
  # "./src/ix/ix_algorithm.cpp"
  # "./src/ix/ix_algorithm.hpp"
  "./src/ix/ix_assert.cpp"
  "./src/ix/ix_assert.hpp"
  "./src/ix/ix_atomic.cpp"
  "./src/ix/ix_atomic.hpp"
  # "./src/ix/ix_benchmark.cpp"
  # "./src/ix/ix_benchmark.hpp"
  "./src/ix/ix_bit.cpp"
  "./src/ix/ix_bit.hpp"
  "./src/ix/ix_bulk.hpp"
  # "./src/ix/ix_c.h"
  # "./src/ix/ix_collision.cpp"
  # "./src/ix/ix_collision.hpp"
  # "./src/ix/ix_color.cpp"
  # "./src/ix/ix_color.hpp"
  "./src/ix/ix_combinatorics.cpp"
  "./src/ix/ix_combinatorics.hpp"
  "./src/ix/ix_compare.hpp"
  "./src/ix/ix_defer.cpp"
  "./src/ix/ix_defer.hpp"
  "./src/ix/ix_environment.cpp"
  "./src/ix/ix_environment.hpp"
  # "./src/ix/ix_even_odd.cpp"
  # "./src/ix/ix_even_odd.hpp"
  "./src/ix/ix_file.cpp"
  "./src/ix/ix_file.hpp"
  "./src/ix/ix_filepath.cpp"
  "./src/ix/ix_filepath.hpp"
  "./src/ix/ix_floating_point.cpp"
  "./src/ix/ix_floating_point.hpp"
  "./src/ix/ix_hash.cpp"
  "./src/ix/ix_hash.hpp"
  "./src/ix/ix_hash_container_detail.cpp"
  "./src/ix/ix_hash_container_detail.hpp"
  "./src/ix/ix_initializer_list.hpp"
  # "./src/ix/ix_input.cpp"
  # "./src/ix/ix_input.hpp"
  # "./src/ix/ix_leb128.cpp"
  # "./src/ix/ix_leb128.hpp"
  "./src/ix/ix_limits.cpp"
  "./src/ix/ix_limits.hpp"
  "./src/ix/ix_math.cpp"
  "./src/ix/ix_math.hpp"
  "./src/ix/ix_memory.cpp"
  "./src/ix/ix_memory.hpp"
  "./src/ix/ix_min_max.cpp"
  "./src/ix/ix_min_max.hpp"
  "./src/ix/ix_new.hpp"
  "./src/ix/ix_new_delete.hpp"
  "./src/ix/ix_polyfill.cpp"
  "./src/ix/ix_polyfill.hpp"
  "./src/ix/ix_print.cpp"
  "./src/ix/ix_print.hpp"
  "./src/ix/ix_printf.cpp"
  "./src/ix/ix_printf.hpp"
  "./src/ix/ix_random.cpp"
  "./src/ix/ix_random.hpp"
  # "./src/ix/ix_report_env.hpp"
  "./src/ix/ix_scanf.cpp"
  "./src/ix/ix_scanf.hpp"
  # "./src/ix/ix_slide.cpp"
  # "./src/ix/ix_slide.hpp"
  # "./src/ix/ix_software_prefetch.hpp"
  "./src/ix/ix_sort.cpp"
  "./src/ix/ix_sort.hpp"
  "./src/ix/ix_string.cpp"
  "./src/ix/ix_string.hpp"
  "./src/ix/ix_test.cpp"
  "./src/ix/ix_test.hpp"
  "./src/ix/ix_test_HashMap.hpp"
  "./src/ix/ix_test_Map.hpp"
  "./src/ix/ix_test_Vector.hpp"
  # "./src/ix/ix_test_main.cpp"
  "./src/ix/ix_time.cpp"
  "./src/ix/ix_time.hpp"
  # "./src/ix/ix_type.cpp"
  # "./src/ix/ix_type.hpp"
  "./src/ix/ix_type_traits.cpp"
  "./src/ix/ix_type_traits.hpp"
  "./src/ix/ix_util_win.cpp"
  "./src/ix/ix_util_win.hpp"
  "./src/ix/ix_utility.cpp"
  "./src/ix/ix_utility.hpp"
  "./src/ix/ix_wrap.cpp"
  "./src/ix/ix_wrap.hpp"
  "./src/textunpack/textunpack.cpp"
  "./src/textunpack/textunpack.hpp"
  "./src/textunpack/textunpack_cli.cpp"
)

set_property(TARGET textunpack PROPERTY CXX_STANDARD 17)
target_compile_definitions(textunpack PRIVATE UNICODE)
target_compile_definitions(textunpack PRIVATE _UNICODE)
target_compile_definitions(textunpack PRIVATE ix_DO_TEST=1)
target_include_directories(textunpack PRIVATE "./src/ix/")
target_include_directories(textunpack PRIVATE "./src/textunpack/")
target_include_directories(textunpack PRIVATE "./external/sokol_time/")
if (MSVC)
  target_compile_options(textunpack PRIVATE /utf-8)
  set_directory_properties(PROPERTY VS_STARTUP_PROJECT textunpack)
endif ()
