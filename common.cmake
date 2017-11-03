# return directory list except 'CMakeFiles'
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF((IS_DIRECTORY ${curdir}/${child}) AND (NOT ${child} STREQUAL "CMakeFiles"))
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

MACRO(SRCLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(filelist "")
  FOREACH(child ${children})
    IF( ${child} MATCHES "(.*).cpp$")
      LIST(APPEND filelist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${filelist})
ENDMACRO()
