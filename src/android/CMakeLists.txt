
add_library(mltmobile SHARED
  mlt_mobile_register.c
)

target_sources(mltmobile PRIVATE mlt_mobile_register.h)

target_link_libraries(mltmobile PRIVATE mlt m mltcore)

if (MOD_AVFORMAT)
  target_link_libraries(mltmobile PRIVATE mltavformat)
  target_compile_definitions(mltmobile PRIVATE OPTION_FFMPEG)
endif()

target_include_directories(mltmobile PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../framework>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/mlt-${MLT_VERSION_MAJOR}>
)

message("song song $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> ")

set_target_properties(mltmobile PROPERTIES
  VERSION ${MLT_VERSION}
  SOVERSION ${MLT_VERSION_MAJOR}
#  OUTPUT_NAME mlt-${MLT_VERSION_MAJOR}
  PUBLIC_HEADER "mlt_mobile_register.h"
)

install(TARGETS mltmobile
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

