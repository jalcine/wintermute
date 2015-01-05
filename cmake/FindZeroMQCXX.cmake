# TODO: Find libzmq via pkg-config.
# TODO: Find libzmqpp via pkg-config.

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(ZMQ REQUIRED
  libzmq)

set(ZEROMQCXX_LIBRARIES ${ZMQ_LIBRARIES})
set(ZEROMQCXX_INCLUDE_DIR /usr/include/zmqpp)
