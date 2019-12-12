

set(APP_SRCS
  datastore.cpp
  main.cpp
  manipulate.cpp
  test.cpp
  utils.cpp
  xmldoc.cpp
  )

set(APP_HDRS
  datastore.h
  manipulate.h
  utils.h
  xmldoc.h
  )

add_executable(cellcla ${APP_SRCS} ${APP_HDRS})
target_link_libraries(cellcla PUBLIC cellml b64 ${LIBXSLT_LIBRARIES})
target_compile_features(cellcla PUBLIC cxx_std_11)
