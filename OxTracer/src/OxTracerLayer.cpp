#include "OxTracerLayer.h"

#include <imgui.h>
#include <Scene/SceneSerializer.h>

#include <Render/RendererConfig.h>

#include "Core/Application.h"

#include "Systems/FreeCamera.h"

#include "UI/UI.h"

namespace Oxrena {
using namespace Oxylus;
OxTracerLayer* OxTracerLayer::s_instance = nullptr;

OxTracerLayer::OxTracerLayer() : Layer("Game Layer") {
  s_instance = this;
}

void OxTracerLayer::on_attach(EventDispatcher& dispatcher) {
  UI::init();

  auto& style = ImGui::GetStyle();
  style.WindowMenuButtonPosition = ImGuiDir_Left;

  runtime_console.render_menu_bar = false;
  runtime_console.visible = false;
  runtime_console.set_focus_to_keyboard_always = true;
  runtime_console.register_command("free_camera", fmt::format("free_camera {0}", free_camera), &free_camera);

  dispatcher.sink<ReloadSceneEvent>().connect<&OxTracerLayer::on_scene_reload>(*this);
  load_scene();
}

void OxTracerLayer::on_detach() { }

void OxTracerLayer::on_update(const Timestep& delta_time) {
  m_scene->on_runtime_update(delta_time);
}

void OxTracerLayer::on_imgui_render() {
  m_scene->on_imgui_render(Application::get_timestep());

  UI::draw_runtime_console(runtime_console, &blocking_player_input);
  UI::draw_fps_counter();
}

void OxTracerLayer::load_scene() {
  m_scene = create_ref<Scene>();
  const SceneSerializer serializer(m_scene);
  serializer.deserialize(get_assets_path("Scenes/Main.oxscene"));

  m_scene->on_runtime_start();

  m_scene->add_system<FreeCamera>(&free_camera);
}

bool OxTracerLayer::on_scene_reload(ReloadSceneEvent&) {
  load_scene();
  OX_CORE_INFO("Scene reloaded.");
  return true;
}
}
