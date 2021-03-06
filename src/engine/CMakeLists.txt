SET(DefinitionsSources
  "definitions/cli.h"
  "definitions/keycodes.h"
  "definitions/input.h"
  "definitions/components.h"
  "definitions/camera.h"
  "definitions/material_data.h"
)

SET(AuxiliarySources
  "auxiliary/debug.h"
  "auxiliary/debug.cc"
)

SET(ServicesSources
  "services/service.h"
  "services/service.cc"
  "services/cvar_service.h"
  "services/cvar_service.cc"
  "services/window_service.h"
  "services/window_service.cc"
  "services/input_service.h"
  "services/input_service.cc"
  "services/renderer_service.h"
  "services/renderer_service.cc"
  "services/scene_service.h"
  "services/scene_service.cc"
  "services/asset_service.h"
  "services/asset_service.cc"
)

IF (NOT SNUFF_NSCRIPTING)
  SET(ServicesSources ${ServicesSources}
    "services/script_service.h"
    "services/script_service.cc"
  )
ENDIF ()

SET(ApplicationSources
  "application/application.h"
  "application/application.cc"
  "application/window.h"
  "application/window.cc"
)

SET(CVarSources
  "cvar/cvar.h"
  "cvar/cvar.cc"
  "cvar/command_line_parser.h"
  "cvar/command_line_parser.cc"
)

SET(InputSources
  "input/input_filter.h"
  "input/input_filter.cc"
  "input/input_event.h"
  "input/input_event.cc"
  "input/input_event_handler.h"
  "input/input_event_handler.cc"
  "input/keyboard.h"
  "input/keyboard.cc"
  "input/mouse.h"
  "input/mouse.cc"
  "input/joystick.h"
  "input/joystick.cc"
)

SET(ECSSources
  "ecs/scene.h"
  "ecs/scene.cc"
  "ecs/entity.h"
  "ecs/entity.cc"
  "ecs/component.h"
  "ecs/component.cc"
)

SET(ComponentsSources
  "components/transform_component.h"
  "components/transform_component.cc"
  "components/mesh_component.h"
  "components/mesh_component.cc"
  "components/mesh_renderer_component.h"
  "components/mesh_renderer_component.cc"
  "components/camera_component.h"
  "components/camera_component.cc"
)

SET(GraphicsSources
  "graphics/material.h"
  "graphics/material.cc"
  "graphics/mesh.h"
  "graphics/mesh.cc"
)

SET(AssetsSources
  "assets/asset.h"
  "assets/asset.cc"
  "assets/script_asset.h"
  "assets/script_asset.cc"
  "assets/scene_asset.h"
  "assets/scene_asset.cc"
  "assets/shader_asset.h"
  "assets/shader_asset.cc"
  "assets/material_asset.h"
  "assets/material_asset.cc"
  "assets/model_asset.h"
  "assets/model_asset.cc"
)

IF (NOT SNUFF_NSCRIPTING)
  SET(ComponentsSources
    ${ComponentsSources}
    "components/script_component.h"
    "components/script_component.cc"
  )
ENDIF (NOT SNUFF_NSCRIPTING)

SET(Resources
  "rsc/icons/directory_icon.png"
  "rsc/icons/script_icon.png"
  "rsc/icons/scene_icon.png"
  "rsc/icons/vertex_shader_icon.png"
  "rsc/icons/pixel_shader_icon.png"
  "rsc/icons/geometry_shader_icon.png"
  "rsc/icons/material_icon.png"
  "rsc/icons/model_icon.png"
)

SOURCE_GROUP("definitions"  FILES ${DefinitionsSources})
SOURCE_GROUP("auxiliary"    FILES ${AuxiliarySources})
SOURCE_GROUP("services"     FILES ${ServicesSources})
SOURCE_GROUP("application"  FILES ${ApplicationSources})
SOURCE_GROUP("cvar"         FILES ${CVarSources})
SOURCE_GROUP("input"        FILES ${InputSources})
SOURCE_GROUP("ecs"          FILES ${ECSSources})
SOURCE_GROUP("components"   FILES ${ComponentsSources})
SOURCE_GROUP("graphics"     FILES ${GraphicsSources})
SOURCE_GROUP("assets"       FILES ${AssetsSources})
SOURCE_GROUP("rsc"          FILES ${Resources})

SET(EngineSources
  ${DefinitionsSources}
  ${AuxiliarySources}
  ${ServicesSources}
  ${ApplicationSources}
  ${CVarSources}
  ${InputSources}
  ${ECSSources}
  ${ComponentsSources}
  ${GraphicsSources}
  ${AssetsSources}
  ${Resources}
)

ADD_LIBRARY(snuffbox-engine ${EngineSources})
TARGET_LINK_LIBRARIES(snuffbox-engine snuffbox-graphics snuffbox-compilers glfw)

ADD_DEPENDENCIES(snuffbox-engine snuffbox-bin2h)

SET(WorkingDir ${PROJECT_BINARY_DIR}/generated)

FILE(MAKE_DIRECTORY ${WorkingDir})
FILE(MAKE_DIRECTORY ${WorkingDir}/resources)

SNUFF_BIN2H("${Resources}" ${WorkingDir}/resources)

TARGET_INCLUDE_DIRECTORIES(snuffbox-engine PUBLIC ${WorkingDir})

IF (NOT SNUFF_NSCRIPTING)
  TARGET_LINK_LIBRARIES(snuffbox-engine snuffbox-scripting)
  ADD_DEPENDENCIES(snuffbox-engine snuffbox-sparse)
  
  SET(ScriptingBinds
    "auxiliary/debug.h"
    "services/input_service.h"
    "services/asset_service.h"
    "definitions/keycodes.h"
    "definitions/components.h"
    "ecs/entity.h"
    "graphics/material.h"
    "graphics/mesh.h"
    "assets/asset.h"
    "components/transform_component.h"
    "components/script_component.h"
    "components/mesh_component.h"
    "components/mesh_renderer_component.h"
    "components/camera_component.h"
  )
  
  FILE(MAKE_DIRECTORY ${WorkingDir}/sparsed)
  
  CREATE_SCRIPT_BINDS("${ScriptingBinds}" ${WorkingDir}/sparsed)
ENDIF (NOT SNUFF_NSCRIPTING)
