

set(APP_SRCS
  main.cpp
  manipulate.cpp
  )

set(APP_HDRS
  manipulate.h
  )

add_executable(cellcla ${APP_SRCS} ${APP_HDRS})
target_link_libraries(cellcla PUBLIC cellml)
target_compile_features(cellcla PUBLIC cxx_std_11)
