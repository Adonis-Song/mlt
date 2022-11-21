
add_library(mlt-android SHARED
        mlt_android_register.c
)

target_link_libraries(mlt-android PRIVATE  mlt m mltcore Threads::Threads)

target_include_directories(mlt-android PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/mlt-${MLT_VERSION_MAJOR}>
)

set_target_properties(mlt-android PROPERTIES
  VERSION ${MLT_VERSION}
  SOVERSION ${MLT_VERSION_MAJOR}
#  OUTPUT_NAME mlt-${MLT_VERSION_MAJOR}
  PUBLIC_HEADER "${MLT_PUBLIC_HEADERS}"
)

install(TARGETS mlt-android
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

