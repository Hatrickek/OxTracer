#include <Core/EntryPoint.h>
#include <Core/Systems/HotReloadableScenes.h>

#include "OxTracerLayer.h"

#include "Core/Systems/SystemManager.h"

namespace Oxylus {
class OxTracerApp : public Application {
public:
  OxTracerApp(const AppSpec& spec) : Application(spec) { }
};

Application* create_application(const ApplicationCommandLineArgs args) {
  AppSpec spec;
  spec.name = "OxTracer";
  spec.working_directory = std::filesystem::current_path().string();
  spec.command_line_args = args;
  spec.resources_path = "Assets";

  const auto app = new OxTracerApp(spec);
  app->push_layer(new Oxrena::OxTracerLayer());
  app->get_system_manager()
     ->add_system<HotReloadableScenes>(
       Oxrena::OxTracerLayer::get_assets_path("Scenes/Main.oxscene"));

  return app;
}
}
