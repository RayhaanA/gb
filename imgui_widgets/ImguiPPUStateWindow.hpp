#pragma once
#include "../src/PPU/Display.hpp"
#include "../src/PPU/PPU.hpp"
#include "imgui.h"

struct PPUStateWindow {
  PPUStateWindow() = default;

  enum class BASE { DEC, BIN, HEX };

  template <typename T>
  void formatBase(T value, BASE base) {
    if (base == BASE::DEC) {
      ImGui::Text("%d", value);
    } else if (base == BASE::BIN) {
      // maybe not worth it
    } else {
      ImGui::Text("%X", value);
    }
  }

  void drawWindow(PPU& ppu) {
    static bool baseIsDec = false;
    static BASE base = BASE::HEX;

    ImGui::Begin("PPU State", 0,
                 ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::SetNextWindowSize(
        ImVec2(Display::PPU_STATE_WINDOW_W, Display::PPU_STATE_WINDOW_H));
    ImGui::SetNextWindowPos(
        ImVec2(Display::PPU_STATE_WINDOW_X, Display::PPU_STATE_WINDOW_Y));

    ImGui::Columns(2, "states");
    ImGui::Separator();
    ImGui::Text("Register");
    ImGui::NextColumn();
    ImGui::Text("Value");
    ImGui::NextColumn();

    ImGui::Text("Mode");
    ImGui::NextColumn();
    ImGui::Separator();
    ImGui::Text("%d", static_cast<uint16_t>(ppu.getMode()));
    ImGui::NextColumn();

    ImGui::Text("Mode cycles");
    ImGui::NextColumn();
    ImGui::Separator();
    ImGui::Text("%d", ppu.getModeCycles());
    ImGui::NextColumn();

    ImGui::Text("LCDC");
    ImGui::NextColumn();
    ImGui::Separator();
    ImGui::Text("%X", ppu.getLCDC());
    ImGui::NextColumn();

    ImGui::Text("STAT");
    ImGui::NextColumn();
    ImGui::Separator();
    ImGui::Text("%X", ppu.getSTAT());
    ImGui::NextColumn();

    ImGui::Text("LY");
    ImGui::NextColumn();
    ImGui::Separator();
    ImGui::Text("%d", ppu.getLY());
    ImGui::NextColumn();

    ImGui::Text("LYC");
    ImGui::NextColumn();
    ImGui::Separator();
    ImGui::Text("%d", ppu.getLYC());
    ImGui::NextColumn();

    ImGui::Text("WX");
    ImGui::NextColumn();
    ImGui::Separator();
    ImGui::Text("%d", ppu.getWX());
    ImGui::NextColumn();

    ImGui::Text("WY");
    ImGui::NextColumn();
    ImGui::Separator();
    ImGui::Text("%d", ppu.getWY());
    ImGui::NextColumn();

    ImGui::Text("SCY");
    ImGui::NextColumn();
    ImGui::Separator();
    ImGui::Text("%d", ppu.getSCY());
    ImGui::NextColumn();

    ImGui::Text("SCX");
    ImGui::NextColumn();
    ImGui::Separator();
    ImGui::Text("%d", ppu.getSCX());
    ImGui::NextColumn();

    ImGui::Separator();
    ImGui::End();
  }
};
