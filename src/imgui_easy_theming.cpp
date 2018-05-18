/*****************************************************************************************
 *                                                                                       *
 * MIT License                                                                           *
 *                                                                                       *
 * Copyright (c) 2017 Maxime Pinard                                                      *
 *                                                                                       *
 * Permission is hereby granted, free of charge, to any person obtaining a copy          *
 * of this software and associated documentation files (the "Software"), to deal         *
 * in the Software without restriction, including without limitation the rights          *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell             *
 * copies of the Software, and to permit persons to whom the Software is                 *
 * furnished to do so, subject to the following conditions:                              *
 *                                                                                       *
 * The above copyright notice and this permission notice shall be included in all        *
 * copies or substantial portions of the Software.                                       *
 *                                                                                       *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR            *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,              *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE           *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER                *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,         *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE         *
 * SOFTWARE.                                                                             *
 *                                                                                       *
 *****************************************************************************************/

// Based on functions from gpulib by procedural (https://github.com/procedural/gpulib)

#include <imgui_easy_theming.hpp>

ImVec3::ImVec3(float x_, float y_, float z_) noexcept
  : x(x_), y(y_), z(z_)
{
}

ImGuiColorTheme::ImGuiColorTheme(ImVec3 text_, ImVec3 head_, ImVec3 area_, ImVec3 body_, ImVec3 pops_) noexcept
  : text(text_), head(head_), area(area_), body(body_), pops(pops_)
{

}

// Arc Dark: https://github.com/horst3180/arc-theme
const ImGuiColorTheme ImGuiColorTheme::ArcDark{
  ImVec3{211 / 255.f, 218 / 255.f, 227 / 255.f},
  ImVec3{64 / 255.f, 132 / 255.f, 214 / 255.f},
  ImVec3{47 / 255.f, 52 / 255.f, 63 / 255.f},
  ImVec3{56 / 255.f, 60 / 255.f, 74 / 255.f},
  ImVec3{28 / 255.f, 30 / 255.f, 37 / 255.f}
};

// Flat UI by yorick.penninks: https://color.adobe.com/Flat-UI-color-theme-2469224/
const ImGuiColorTheme ImGuiColorTheme::FlatUI{
  ImVec3{236 / 255.f, 240 / 255.f, 241 / 255.f},
  ImVec3{41 / 255.f, 128 / 255.f, 185 / 255.f},
  ImVec3{57 / 255.f, 79 / 255.f, 105 / 255.f},
  ImVec3{44 / 255.f, 62 / 255.f, 80 / 255.f},
  ImVec3{33 / 255.f, 46 / 255.f, 60 / 255.f}
};

// Mint-Y-Dark: https://github.com/linuxmint/mint-y-theme
const ImGuiColorTheme ImGuiColorTheme::MintYDark{
  ImVec3{211 / 255.f, 211 / 255.f, 211 / 255.f},
  ImVec3{95 / 255.f, 142 / 255.f, 85 / 255.f},
  ImVec3{47 / 255.f, 47 / 255.f, 47 / 255.f},
  ImVec3{64 / 255.f, 64 / 255.f, 64 / 255.f},
  ImVec3{30 / 255.f, 30 / 255.f, 30 / 255.f}
};

void ImGuiEasyTheming(ImGuiColorTheme theme)
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Text] = ImVec4(theme.text.x, theme.text.y, theme.text.z, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(theme.text.x, theme.text.y, theme.text.z, 0.58f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(theme.body.x, theme.body.y, theme.body.z, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(theme.area.x, theme.area.y, theme.area.z, 0.58f);
	style.Colors[ImGuiCol_Border] = ImVec4(theme.body.x, theme.body.y, theme.body.z, 0.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(theme.body.x, theme.body.y, theme.body.z, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(theme.area.x, theme.area.y, theme.area.z, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(theme.head.x, theme.head.y, theme.head.z, 0.78f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(theme.head.x, theme.head.y, theme.head.z, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(theme.area.x, theme.area.y, theme.area.z, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(theme.area.x, theme.area.y, theme.area.z, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(theme.area.x, theme.area.y, theme.area.z, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(theme.area.x, theme.area.y, theme.area.z, 0.47f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(theme.area.x, theme.area.y, theme.area.z, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(theme.head.x, theme.head.y, theme.head.z, 0.21f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(theme.head.x, theme.head.y, theme.head.z, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(theme.head.x, theme.head.y, theme.head.z, 1.00f);
	//style.Colors[ImGuiCol_ComboBg] = ImVec4(theme.area.x, theme.area.y, theme.area.z, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(theme.head.x, theme.head.y, theme.head.z, 0.80f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(theme.head.x, theme.head.y, theme.head.z, 0.50f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(theme.head.x, theme.head.y, theme.head.z, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(theme.head.x, theme.head.y, theme.head.z, 0.50f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(theme.head.x, theme.head.y, theme.head.z, 0.86f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(theme.head.x, theme.head.y, theme.head.z, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(theme.head.x, theme.head.y, theme.head.z, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(theme.head.x, theme.head.y, theme.head.z, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(theme.head.x, theme.head.y, theme.head.z, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(theme.head.x, theme.head.y, theme.head.z, 0.32f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(theme.head.x, theme.head.y, theme.head.z, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(theme.head.x, theme.head.y, theme.head.z, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(theme.head.x, theme.head.y, theme.head.z, 0.15f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(theme.head.x, theme.head.y, theme.head.z, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(theme.head.x, theme.head.y, theme.head.z, 1.00f);
	//style.Colors[ImGuiCol_CloseButton] = ImVec4(theme.text.x, theme.text.y, theme.text.z, 0.16f);
	//style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(theme.text.x, theme.text.y, theme.text.z, 0.39f);
	//style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(theme.text.x, theme.text.y, theme.text.z, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(theme.text.x, theme.text.y, theme.text.z, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(theme.head.x, theme.head.y, theme.head.z, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(theme.text.x, theme.text.y, theme.text.z, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(theme.head.x, theme.head.y, theme.head.z, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(theme.head.x, theme.head.y, theme.head.z, 0.43f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(theme.pops.x, theme.pops.y, theme.pops.z, 0.92f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(theme.area.x, theme.area.y, theme.area.z, 0.73f);
}
