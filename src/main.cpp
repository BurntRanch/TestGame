#include "camera.hpp"
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_scancode.h>
#include <chrono>
#include <engine.hpp>
#include <glm/ext/matrix_projection.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/matrix.hpp>
#include <memory>
#include <new>
#include <settings.hpp>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <stdexcept>
#include <string>
#include "camera.hpp"
#include "common.hpp"
#include "model.hpp"
#include "steamnetworkingtypes.h"
#include "isteamnetworkingutils.h"
#include "ui/arrows.hpp"
#include "ui/button.hpp"
#include "ui/label.hpp"
#include "ui/panel.hpp"
#include "util.hpp"
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#include "tinyfiledialogs.h"

Camera cam(glm::vec3(0.0f, 0.0f, 0.0f));
Settings settings("settings.toml");
std::unique_ptr<Engine> engine;

namespace State {
}

float lastX, lastY;
Uint32 lastMouseState;

void Update() {
    //fmt::println("Hi!");
}

void FixedUpdate(const std::array<bool, 322> &keyMap) {

}

void onButtonClick(std::string id) {
    if (id == "connectButton") {
        SteamNetworkingIPAddr ipAddr;
        ipAddr.Clear();
        ipAddr.ParseString("127.0.0.1:9582");

        engine->ConnectToGameServer(ipAddr);
    }
}

int main() {
    cam.MouseSensitivity = settings.MouseSensitivity;
    cam.FOV = settings.FieldOfView;
    cam.MovementSpeed = settings.Velocity;

    engine = std::make_unique<Engine>();
    
    engine->InitRenderer(settings, &cam);
    engine->InitNetworking();

    engine->RegisterUIButtonListener(onButtonClick);

    engine->LoadUIFile("ui.xml");

    engine->StartRenderer();

    return 0;
}
