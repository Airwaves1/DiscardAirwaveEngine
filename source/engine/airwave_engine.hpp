#pragma once


//----------window----------
#include "window/aw_window.hpp"

//----------utils----------
#include "utils/log.hpp"
#include "utils/common.hpp"
#include "utils/file_utils.hpp"

//----------event----------
#include "event/event.hpp"
#include "event/application_event.hpp"
#include "event/key_event.hpp"
#include "event/mouse_event.hpp"
#include "event/event_dispatcher.hpp"
#include "event/event_observer.hpp"

//----------camera----------
#include "camera/camera.hpp"
#include "camera/controller/trackball_controller.hpp"

//----------renderer----------
#include "render/renderer/renderer.hpp"
#include "render/renderer/render_command.hpp"
#include "render/renderer/renderer_api.hpp"

//----------shader----------
#include "render/shader/shader.hpp"
#include "render/shader/shader_library.hpp"

//----------buffer----------
#include "render/buffer/vertex_buffer.hpp"
#include "render/buffer/vertex_array.hpp"
#include "render/buffer/framebuffer.hpp"

//----------texture----------
#include "render/texture/texture.hpp"
#include "render/texture/texture_2d.hpp"
#include "render/texture/texture_manager.hpp"
#include "render/texture/cube_texture.hpp"

//----------geometry----------
#include "render/geometry/geometry.hpp"
#include "render/geometry/geometry_utils.hpp"

//----------material----------
#include "render/material/material.hpp"
#include "render/material/basic_material.hpp"

//----------object----------
#include "render/object/mesh.hpp"
#include "render/object/fullscreen_quad.hpp"

//----------model----------
#include "render/model/model_loader.hpp"


//----------ECS----------
#include "ecs/uuid.hpp"
#include "ecs/aw_entity.hpp"
#include "ecs/scene.hpp"
#include "ecs/node.hpp"
#include "ecs/system/forward_render_system.hpp"
#include "ecs/system/light_system.hpp"
#include "ecs/system/camera_system.hpp"
#include "ecs/system/background_system.hpp"

//----------Component----------
#include "ecs/component/material/material_component.hpp"
#include "ecs/component/geometry/geometry_component.hpp"
#include "ecs/component/light/light_component.hpp"
#include "ecs/component/camera/camera_component.hpp"
#include "ecs/component/camera/camera_controller_component.hpp"
#include "ecs/component/transform/transform_component.hpp"
#include "ecs/component/object/mesh_component.hpp"
#include "ecs/component/object/model_component.hpp"

//----------imgui----------
#include <imgui.h>

//----------application----------
#include "application/application.hpp"
#include "application/entry_point.hpp"
