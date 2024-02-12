#pragma once
#include "../src/CPU/CPU.hpp"
#include "../src/PPU/Display.hpp"
#include "imgui.h"

struct RegistersWindow {
  RegistersWindow() = default;

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

  void drawWindow(CPU& cpu) {
    static bool baseIsDec = false;
    static BASE base = BASE::HEX;

    ImGui::Begin("Registers State", 0,
                 ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::SetNextWindowSize(ImVec2(Display::REGISTERS_STATE_WINDOW_W,
                                    Display::REGISTERS_STATE_WINDOW_H));
    ImGui::SetNextWindowPos(ImVec2(Display::REGISTERS_STATE_WINDOW_X,
                                   Display::REGISTERS_STATE_WINDOW_Y));
    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("Options")) {
        ImGui::MenuItem("View values as decimal", nullptr, &baseIsDec);
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    if (baseIsDec) {
      base = BASE::DEC;
    } else {
      base = BASE::HEX;
    }

    ImGui::Text("Cycle count: %d", cpu.getCurrentCycleCount());
    ImGui::Separator();

    ImGui::Columns(3, "header");

    ImGui::Text("Last I: %X", cpu.getCurrInstruction());
    ImGui::NextColumn();
    ImGui::Text("IME: %d", Globals::IME);
    ImGui::NextColumn();
    ImGui::Text("Halted: %d", cpu.getHalted());
    ImGui::NextColumn();
    ImGui::Separator();
    ImGui::Separator();
    ImGui::Text("Register");
    ImGui::NextColumn();
    ImGui::Text("Value");
    ImGui::NextColumn();
    ImGui::Text("Prev Value");
    ImGui::NextColumn();

    /*ImGui::Text("A"); ImGui::NextColumn();
    formatBase(cpu.getA(), base); ImGui::NextColumn();
    formatBase(cpu.getAPrev(), base); ImGui::NextColumn();

    ImGui::Text("F"); ImGui::NextColumn(); ImGui::Separator();
    formatBase(cpu.getF(), base); ImGui::NextColumn();
    formatBase(cpu.getFPrev(), base); ImGui::NextColumn();

    ImGui::Text("B"); ImGui::NextColumn(); ImGui::Separator();
    formatBase(cpu.getB(), base);; ImGui::NextColumn();
    formatBase(cpu.getBPrev(), base); ImGui::NextColumn();

    ImGui::Text("C"); ImGui::NextColumn(); ImGui::Separator();
    formatBase(cpu.getC(), base); ImGui::NextColumn();
    formatBase(cpu.getCPrev(), base); ImGui::NextColumn();

    ImGui::Text("D"); ImGui::NextColumn(); ImGui::Separator();
    formatBase(cpu.getD(), base); ImGui::NextColumn();
    formatBase(cpu.getDPrev(), base); ImGui::NextColumn();

    ImGui::Text("E"); ImGui::NextColumn(); ImGui::Separator();
    formatBase(cpu.getE(), base); ImGui::NextColumn();
    formatBase(cpu.getEPrev(), base); ImGui::NextColumn();

    ImGui::Text("H"); ImGui::NextColumn(); ImGui::Separator();
    formatBase(cpu.getH(), base); ImGui::NextColumn();
    formatBase(cpu.getHPrev(), base); ImGui::NextColumn();

    ImGui::Text("L"); ImGui::NextColumn(); ImGui::Separator();
    formatBase(cpu.getL(), base); ImGui::NextColumn();
    formatBase(cpu.getLPrev(), base); ImGui::NextColumn(); */

    ImGui::Text("PC");
    ImGui::NextColumn();
    ImGui::Separator();
    formatBase(cpu.getPC(), base);
    ImGui::NextColumn();
    formatBase(cpu.getPCPrev(), base);
    ImGui::NextColumn();

    ImGui::Text("SP");
    ImGui::NextColumn();
    ImGui::Separator();
    formatBase(cpu.getSP(), base);
    ImGui::NextColumn();
    formatBase(cpu.getSPPrev(), base);
    ImGui::NextColumn();

    ImGui::Text("AF");
    ImGui::NextColumn();
    ImGui::Separator();
    formatBase(cpu.getAF(), base);
    ImGui::NextColumn();
    formatBase(cpu.getAFPrev(), base);
    ImGui::NextColumn();

    ImGui::Text("BC");
    ImGui::NextColumn();
    ImGui::Separator();
    formatBase(cpu.getBC(), base);
    ImGui::NextColumn();
    formatBase(cpu.getBCPrev(), base);
    ImGui::NextColumn();

    ImGui::Text("DE");
    ImGui::NextColumn();
    ImGui::Separator();
    formatBase(cpu.getDE(), base);
    ImGui::NextColumn();
    formatBase(cpu.getDEPrev(), base);
    ImGui::NextColumn();

    ImGui::Text("HL");
    ImGui::NextColumn();
    ImGui::Separator();
    formatBase(cpu.getHL(), base);
    ImGui::NextColumn();
    formatBase(cpu.getHLPrev(), base);
    ImGui::NextColumn();

    ImGui::Text("IF");
    ImGui::NextColumn();
    ImGui::Separator();
    formatBase(cpu.getIF(), base);
    ImGui::NextColumn();
    formatBase(cpu.getIFPrev(), base);
    ImGui::NextColumn();

    ImGui::Text("IE");
    ImGui::NextColumn();
    ImGui::Separator();
    formatBase(cpu.getIE(), base);
    ImGui::NextColumn();
    formatBase(cpu.getIEPrev(), base);
    ImGui::NextColumn();

    ImGui::Separator();
    ImGui::End();
  }
};