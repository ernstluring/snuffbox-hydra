INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}")

ADD_SUBDIRECTORY("foundation")
ADD_SUBDIRECTORY("graphics")

IF (NOT SNUFF_NSCRIPTING)
  ADD_SUBDIRECTORY("scripting")
  SET_SOLUTION_FOLDER("snuffbox-hydra"
    snuffbox-scripting
  )
ENDIF (NOT SNUFF_NSCRIPTING)

ADD_SUBDIRECTORY("engine")

SET_SOLUTION_FOLDER("snuffbox-hydra"
  snuffbox-foundation
  snuffbox-engine
  snuffbox-graphics
)

IF (SNUFF_BUILD_TEST)
  ADD_SUBDIRECTORY("test")
  
  SET_SOLUTION_FOLDER("snuffbox-hydra"
    snuffbox-test
  )
ENDIF (SNUFF_BUILD_TEST)

ADD_SUBDIRECTORY("tools")
