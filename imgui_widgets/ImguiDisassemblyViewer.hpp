#pragma once
#include <vector>
#include <string>
#include <memory>
#include "imgui.h"
#include "../src/util/Disassembler.hpp"

struct DisassemblyViewer {
    std::vector<std::string> disassemblyList;
    std::unordered_map<uint16_t, int32_t> indexMap;

    DisassemblyViewer() = delete;
    DisassemblyViewer(std::pair<std::vector<std::string>, std::unordered_map<uint16_t, int32_t>> d) 
        : disassemblyList(std::move(d.first)), indexMap(std::move(d.second)) {}

    void drawWindow(CPU& cpu) {
        int32_t min = std::max(0, indexMap[cpu.getPC() ] - 10000);
        int32_t max = std::min(static_cast<int32_t>(disassemblyList.size()), indexMap[cpu.getPC()] + 10000);
        static bool trackCurrentInstruction = true;
        static bool showInstructionJumpWindow = false;
        static bool step = false;
        static bool noClose = false;
        static bool reload = false;
        static int32_t targetPC = 0;
        static int32_t clicked = 0;

        ImGui::Begin("Disassembly", 0, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Options")) {
                ImGui::MenuItem("Track current instruction", nullptr, &trackCurrentInstruction);
                ImGui::MenuItem("Show instruction jump window", nullptr, &showInstructionJumpWindow);
                ImGui::MenuItem("Step", nullptr, &step);
                ImGui::MenuItem("Reload disassembly", nullptr, &reload);
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        if (showInstructionJumpWindow) {
            if (!ImGui::Begin("Instruction Jump", &showInstructionJumpWindow)) {
                ImGui::End();
            }
            else {
                ImGui::Text("Run until:");
                ImGui::InputInt("", &targetPC, 1, 100, ImGuiInputTextFlags_CharsHexadecimal);

                if (ImGui::Button("Jump")) {
                    ++clicked;
                }

                if (clicked & 1 && targetPC >= 0 && targetPC < cpu.getMemory().size()) {
                    cpu.tick();
                    while (cpu.getPC() != static_cast<uint16_t>(targetPC)) {
                        if (cpu.getPC() == cpu.getPCPrev()) {
                            std::cerr << "Reached end of program before reaching target!" << std::endl;
                            break;
                        }
                        cpu.tick();
                    }
                    clicked = 0;
                }

                ImGui::End();
            }
        }

        ImGui::Separator();

        if (step) {
            cpu.tick();
            step = false;
        }

        if (reload) {
            auto [d, i] = util::disassemble(cpu.getMemory());
            disassemblyList = d;
            indexMap = i;
            reload = false;
        }

        for (int32_t i = 0; i < disassemblyList.size(); ++i) {
            if (i == indexMap[cpu.getPC()] && trackCurrentInstruction) {
                ImGui::TextColored(ImVec4(0.03f, 0.94f, 0.0f, 1.0f), disassemblyList[i].c_str());
                ImGui::SetScrollHereY(0.5f);
            }
            else {
                ImGui::Text(disassemblyList[i].c_str());
            }
            ImGui::Separator();
        }

        ImGui::End();
    }
};