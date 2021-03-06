SET(RootSources
  "script_type_traits.h"
  "script_state.h"
  "script_value.h"
  "script_value.cc"
  "script_args.h"
  "script_args.cc"
  "script_callback.h"
  "script_class.h"
  "script_class.cc"
  "script_register.h"
  "scripting.h"
)

SET(DefinitionsSources
  "definitions/script_types.h"
)

SET(NativeSources "")

IF (SNUFF_DUKTAPE)
  MESSAGE(STATUS "Building the duktape version of the native scripting library")
  SET(DuktapeSources
    "duk/duk_definitions.h"
    "duk/duk_allocator.h"
    "duk/duk_allocator.cc"
    "duk/duk_state.h"
    "duk/duk_state.cc"
    "duk/duk_wrapper.h"
    "duk/duk_wrapper.cc"
    "duk/duk_function.h"
    "duk/duk_callback.h"
    "duk/duk_register.h"
    "duk/duk_register.cc"
  )
  
  SOURCE_GROUP("duk" FILES ${DuktapeSources})
  SET(NativeSources ${NativeSources} ${DuktapeSources})
ENDIF (SNUFF_DUKTAPE)

SOURCE_GROUP("\\"             FILES ${RootSources})
SOURCE_GROUP("definitions"    FILES ${DefinitionsSources})

SET(ScriptingSources
  ${RootSources}
  ${DefinitionsSources}
  ${NativeSources}
)

ADD_LIBRARY(snuffbox-scripting ${ScriptingSources})

IF (SNUFF_DUKTAPE)
  TARGET_LINK_LIBRARIES(snuffbox-scripting snuffbox-foundation duktape)
ENDIF (SNUFF_DUKTAPE)
