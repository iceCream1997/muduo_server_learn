add_executable(bsa bsa.cc)

add_executable(deadlock_test Deadlock_test.cc)
target_link_libraries(deadlock_test pthread)

add_executable(deadlock_test2 Deadlock_test2.cc)
target_link_libraries(deadlock_test2 pthread)

add_executable(pthread_atfork_test Pthread_atfork_test.cc)
target_link_libraries(pthread_atfork_test pthread)
