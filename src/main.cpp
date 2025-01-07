#include "camera.hpp"
#include <SDL3/SDL_events.h>
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

Settings settings("settings.toml");
std::unique_ptr<Engine> engine;

namespace State {
    /* The direction to move along the X axis. */
    float Movement;

    std::string ServerAddress;

    enum packetType {
        PACKET_MOVEMENT_REQUEST,
    };

    struct Packet {
        packetType type;

        /* if packetType == PACKET_MOVEMENT_REQUEST*/
        float movementDirection;
    };

    Packet lastPacket;
}

float lastX, lastY;
Uint32 lastMouseState;

void SendPacketToServer(State::Packet &packet) {
    UTILASSERT(engine->IsConnectedToGameServer());

    std::vector<std::byte> data;
    Serialize(static_cast<int>(packet.type), data);
    Serialize(packet.movementDirection, data);

    engine->SendRequestToServer(data);
}

void Update() {
    //fmt::println("Hi!");
}

void FixedUpdate(const std::array<bool, 322> &keyMap) {
    if (!engine->IsConnectedToGameServer()) {
        return;
    }

    State::Movement = 0;

    if (keyMap[SDL_SCANCODE_RIGHT]) {
        fmt::println("Moving right!");
        State::Movement += 1.0f / 64.0f;
    }

    if (keyMap[SDL_SCANCODE_LEFT]) {
        fmt::println("Moving left");
        State::Movement -= 1.0f / 64.0f;
    }

    State::Packet packet{};
    packet.type = State::PACKET_MOVEMENT_REQUEST;
    packet.movementDirection = State::Movement;

    if (packet.movementDirection != State::lastPacket.movementDirection) {
        SendPacketToServer(packet);

        State::lastPacket = packet;
    }
}

void onButtonClick(std::string id) {
    if (id == "connectButton") {
        SteamNetworkingIPAddr ipAddr;
        ipAddr.Clear();

        if (!State::ServerAddress.empty()) {
            ipAddr.ParseString(State::ServerAddress.c_str());
            
            if (ipAddr.m_port == 0) {
                ipAddr.m_port = 9582;
            }
        } else {
           ipAddr.ParseString("127.0.0.1:9582");
        }

        engine->ConnectToGameServer(ipAddr);
    }
}

int main(int argc, char *argv[]) {
    engine = std::make_unique<Engine>();
    
    engine->InitRenderer(settings, nullptr);
    engine->InitNetworking();

    engine->GetRenderer()->RegisterFixedUpdateFunction(FixedUpdate);

    engine->RegisterUIButtonListener(onButtonClick);

    engine->LoadUIFile("ui.xml");

    if (argc >= 2) {
        State::ServerAddress = argv[1];
    }

    engine->StartRenderer();

    return 0;
}
