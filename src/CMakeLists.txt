set(SOURCES
  aux.c
  main.c)
set(HEADERS
  aux.h
  config.h.in)

configure_file(config.h.in config.h @ONLY)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

include_directories (
  ${TIMESTREAM_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR})

### Install rules
install (TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
