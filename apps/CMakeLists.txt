cmake_minimum_required(VERSION 2.6)

include(../common.cmake)

#add_subdirectory(c1)

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
FOREACH(subdir ${SUBDIRS})
	ADD_SUBDIRECTORY(${subdir})
	MESSAGE(STATUS "SUBDIR:         "
         ${subdir}
	)

ENDFOREACH()
