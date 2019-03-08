add_executable(bsa bsa.cc)

add_executable(countdownlatch_test1 CountDownLatch_test1.cc)
target_link_libraries(countdownlatch_test1 muduo_base)

add_executable(countdownlatch_test2 CountDownLatch_test2.cc)
target_link_libraries(countdownlatch_test2 muduo_base)

add_executable(deadlock_test Deadlock_test.cc)
target_link_libraries(deadlock_test pthread)

add_executable(deadlock_test2 Deadlock_test2.cc)
target_link_libraries(deadlock_test2 pthread)

add_executable(log_test1 Log_test1.cc)
target_link_libraries(log_test1 muduo_base)

add_executable(log_test2 Log_test2.cc)
target_link_libraries(log_test2 muduo_base)

add_executable(pthread_atfork_test Pthread_atfork_test.cc)
target_link_libraries(pthread_atfork_test pthread)

add_executable(reactor_test01 Reactor_test01.cc)
target_link_libraries(reactor_test01 muduo_net)

add_executable(reactor_test02 Reactor_test02.cc)
target_link_libraries(reactor_test02 muduo_net)
