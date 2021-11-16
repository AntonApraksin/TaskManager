Find_Package(GTest)
IF (${GTEST_FOUND})
  MESSAGE(STATUS "GTest is found locally")
  set(TASK_MANAGER_LINK_GTEST 
    GTest::gtest GTest::gtest_main)
ELSE (${GTEST_FOUND})
  MESSAGE(STATUS "GTest is not found locally")
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
  )

  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
  include(GoogleTest)
  set(TASK_MANAGER_LINK_GTEST 
    gtest_main)
ENDIF (${GTEST_FOUND})


add_subdirectory(TaskManager)
