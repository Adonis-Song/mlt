add_subdirectory(framework)
add_subdirectory(modules)

if (NOT ANDROID)
  add_subdirectory(melt)
  add_subdirectory(mlt++)
endif()

if(SWIG_FOUND)
  add_subdirectory(swig)
endif()

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
