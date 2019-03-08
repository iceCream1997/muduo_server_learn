add_executable(atomic_unittest Atomic_unittest.cc)
#target_link_libraries(atomic_unittest muduo_base)

add_executable(exception_test Exception_test.cc)
target_link_libraries(exception_test muduo_base)

add_executable(mutex_test Mutex_test.cc)
target_link_libraries(mutex_test muduo_base)

add_executable(thread_test Thread_test.cc)
target_link_libraries(thread_test muduo_base)

add_executable(timestamp_unittest Timestamp_unittest.cc)
target_link_libraries(timestamp_unittest muduo_base)

