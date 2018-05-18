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

#ifndef IMGUI_EASY_THEMING_HPP
#define IMGUI_EASY_THEMING_HPP


#include <imgui.h>

struct ImVec3
{
	float x;
	float y;
	float z;

	explicit ImVec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) noexcept;
};

struct ImGuiColorTheme
{
	ImVec3 text;
	ImVec3 head;
	ImVec3 area;
	ImVec3 body;
	ImVec3 pops;

	ImGuiColorTheme(ImVec3 _text, ImVec3 head, ImVec3 area, ImVec3 body, ImVec3 pops) noexcept;

	// Arc Dark: https://github.com/horst3180/arc-theme
	static const ImGuiColorTheme ArcDark;

	// Flat UI by yorick.penninks: https://color.adobe.com/Flat-UI-color-theme-2469224/
	static const ImGuiColorTheme FlatUI;

	// Mint-Y-Dark: https://github.com/linuxmint/mint-y-theme
	static const ImGuiColorTheme MintYDark;
};

void ImGuiEasyTheming(ImGuiColorTheme theme);


#endif //IMGUI_EASY_THEMING_HPP
