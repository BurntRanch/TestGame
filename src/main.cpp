#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_scancode.h>
#include <engine.hpp>
#include <glm/ext/matrix_projection.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/matrix.hpp>
#include <memory>
#include <settings.hpp>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <string>
#include "Node/Node.hpp"
#include "Node/Node3D/Model3D/Model3D.hpp"
#include "Node/Node3D/Node3D.hpp"
#include "SceneTree.hpp"
#include "steamnetworkingtypes.h"
#include "isteamnetworkingutils.h"
#include "util.hpp"

Settings settings("settings.toml");
std::unique_ptr<Engine> engine;

float t = 0.0f;

void Update() {
    //fmt::println("Hi!");
}

void FixedUpdate() {
    /* funny name */
    auto nodes = engine->GetSceneTree()->FindNodesByName("EpicConsole420");
    UTILASSERT(!nodes.empty() && typeid(*nodes[0]) == typeid(Model3D));

    float sinResult = glm::min(glm::max((sin(t)+1)/2, 0.1f), 0.9f);

    reinterpret_cast<Model3D *>(nodes[0])->SetScale(glm::vec3(sinResult * 5, sinResult, sinResult * 5));
    t += ENGINE_FIXED_UPDATE_DELTATIME;
}

// void onKeyPress(SDL_Event *event) {
//     if (!engine->IsConnectedToGameServer()) {
//         return;
//     }

//     State::Movement = 0;

//     if (event->key.scancode == SDL_SCANCODE_RIGHT) {
//         fmt::println("Moving right!");
//         State::Movement += 1.0f / 64.0f;
//     }

//     if (event->key.scancode == SDL_SCANCODE_RIGHT) {
//         fmt::println("Moving left");
//         State::Movement -= 1.0f / 64.0f;
//     }

//     State::Packet packet{};
//     packet.type = State::PACKET_MOVEMENT_REQUEST;
//     packet.movementDirection = State::Movement;

//     if (packet.movementDirection != State::lastPacket.movementDirection) {
//         SendPacketToServer(packet);

//         State::lastPacket = packet;
//     }
// }

// void onButtonClick(std::string id) {
//     if (id == "connectButton") {
//         SteamNetworkingIPAddr ipAddr;
//         ipAddr.Clear();

//         if (!State::ServerAddress.empty()) {
//             ipAddr.ParseString(State::ServerAddress.c_str());
            
//             if (ipAddr.m_port == 0) {
//                 ipAddr.m_port = 9582;
//             }
//         } else {
//            ipAddr.ParseString("127.0.0.1:9582");
//         }

//         engine->ConnectToGameServer(ipAddr);
//     }
// }

int main(int argc, char *argv[]) {
    engine = std::make_unique<Engine>();
    engine->InitRenderer(settings);
    engine->RegisterFixedUpdateFunction(FixedUpdate);

    SceneTree *tree = engine->GetSceneTree();
    tree->ImportFromGLTF2("resources/scene.glb");

    engine->Start();

    return 0;
}
