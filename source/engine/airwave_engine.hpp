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


//----------application----------
#include "application/application.hpp"
#include "application/entry_point.hpp"

//----------geometry----------
#include "render/geometry/geometry_utils.hpp"