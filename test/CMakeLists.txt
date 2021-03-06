set(TEST_EXECUTABLE ${PROJECT_NAME}-test)

set(TEST_SOURCE
  discovery_test.cpp
  configuration_test.cpp
  search_pattern_test.cpp
)

find_package(Boost COMPONENTS unit_test_framework)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_executable(${TEST_EXECUTABLE} EXCLUDE_FROM_ALL
  ${TEST_SOURCE} ${SOURCES}
)

add_executable(sdtest EXCLUDE_FROM_ALL
  test.cpp ${SOURCES}
)

target_link_libraries(${TEST_EXECUTABLE} 
  ${PROJECT_NAME} ${EXTERNAL_LIBS} ${Boost_LIBRARIES}
)

target_link_libraries(sdtest ${PROJECT_NAME} ${EXTERNAL_LIBS} ${Boost_LIBRARIES})

add_custom_target(test 
  ${TEST_EXECUTABLE} --log_level=message
  DEPENDS ${TEST_EXECUTABLE}
)
