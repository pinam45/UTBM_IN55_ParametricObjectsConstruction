#include <algorithm>

#include <imgui.h>

#include "gui/FPSOverlay.hpp"

void poc::FPSOverlay::draw() {
    const float DISTANCE = 10.0f;

    ImVec2 window_pos = ImVec2((m_corner & 1) ? ImGui::GetIO().DisplaySize.x - DISTANCE : DISTANCE,
                               (m_corner & 2) ? ImGui::GetIO().DisplaySize.y - DISTANCE : DISTANCE);
    ImVec2 window_pos_pivot = ImVec2((m_corner & 1) ? 1.0f : 0.0f, (m_corner & 2) ? 1.0f : 0.0f);

    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    ImGui::SetNextWindowBgAlpha(0.7f); // Transparent background

    if (isOpen() && ImGui::Begin("FPS Overlay", &m_open,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                     ImGuiWindowFlags_NoNav)) {

        ImGui::Text("%.3f ms/frame\n(%.1f FPS)",
                    1000. / static_cast<double>(ImGui::GetIO().Framerate),
                    static_cast<double>(ImGui::GetIO().Framerate));


        if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::MenuItem("Top-left", nullptr, m_corner == 0)) {
                m_corner = 0;
            }
            if (ImGui::MenuItem("Top-right", nullptr, m_corner == 1)) {
                m_corner = 1;
            }
            if (ImGui::MenuItem("Bottom-left", nullptr, m_corner == 2)) {
                m_corner = 2;
            }
            if (ImGui::MenuItem("Bottom-right", nullptr, m_corner == 3)) {
                m_corner = 3;
            }
            if (ImGui::MenuItem("Close")) {
                close();
            }

            ImGui::EndPopup();
        }

        ImGui::End();
    }
}

bool poc::FPSOverlay::isOpen() const {
    return m_open;
}

void poc::FPSOverlay::open() {
    m_open = true;
}

void poc::FPSOverlay::close() {
	m_open = false;
}

void poc::FPSOverlay::setOpen(bool open) {
	m_open = open;
}
