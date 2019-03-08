set(net_SRCS
  Channel.cc
  EventLoop.cc
  EventLoopThread.cc
  InetAddress.cc
  Poller.cc
  poller/DefaultPoller.cc
  poller/EPollPoller.cc
  poller/PollPoller.cc
  Socket.cc
  SocketsOps.cc
  Timer.cc
  TimerQueue.cc
  )

add_library(muduo_net ${net_SRCS})
target_link_libraries(muduo_net muduo_base)

install(TARGETS muduo_net DESTINATION lib)
set(HEADERS
  Channel.h
  Endian.h
  EventLoop.h
  EventLoopThread.h
  InetAddress.h
  TimerId.h
  )
install(FILES ${HEADERS} DESTINATION include/muduo/net)

#add_subdirectory(http)
#add_subdirectory(inspect)
#
if(NOT CMAKE_BUILD_NO_EXAMPLES)
  add_subdirectory(tests)
endif()
#
#if(PROTOBUF_FOUND)
#  add_subdirectory(protorpc)
#else()
#  add_subdirectory(protorpc EXCLUDE_FROM_ALL)
#endif()
