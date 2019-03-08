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

add_executable(reactor_test03 Reactor_test03.cc)
target_link_libraries(reactor_test03 muduo_net)

add_executable(reactor_test04 Reactor_test04.cc)
target_link_libraries(reactor_test04 muduo_net)

add_executable(reactor_test05 Reactor_test05.cc)
target_link_libraries(reactor_test05 muduo_net)

add_executable(reactor_test06 Reactor_test06.cc)
target_link_libraries(reactor_test06 muduo_net)

add_executable(reactor_test07 Reactor_test07.cc)
target_link_libraries(reactor_test07 muduo_net)

add_executable(reactor_test08 Reactor_test08.cc)
target_link_libraries(reactor_test08 muduo_net)

add_executable(reactor_test09 Reactor_test09.cc)
target_link_libraries(reactor_test09 muduo_net)