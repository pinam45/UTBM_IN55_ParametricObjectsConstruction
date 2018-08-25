/*****************************************************************************************
 *                                                                                       *
 * MIT License                                                                           *
 *                                                                                       *
 * Copyright (c) 2017 Julien Barbier & Jérôme Boulmier & Maxime Pinard                   *
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

#ifndef PARAMETRICOBJECTSCONSTRUCTION_POCONFIGPANEL_HPP
#define PARAMETRICOBJECTSCONSTRUCTION_POCONFIGPANEL_HPP


#include <imgui.h>
#include <random>

#include "ParametricObject.hpp"
#include "imgui_easy_theming.hpp"
#include "Drawable.hpp"

namespace poc{

	class POConfigPanel : public Drawable {

	public:

		POConfigPanel(float x, float y, float width, float height);

		void setX(float x);
		void setY(float y);
		void setWidth(float width);
		void setHeight(float height);

		const std::vector<poc::LayerConfig>& getLayers() const;

		void draw() override;

		bool need_recompute();

	private:

		poc::LayerConfig randomLayer();

		struct ThemeHolder
		{
			const char* name;
			const ImGuiColorTheme& colorTheme;
			bool enabled;
		};
		std::vector<ThemeHolder> themeHolders;

		ImVec2 m_position;
		ImVec2 m_size;

		std::random_device rd;
		std::vector<poc::LayerConfig> layers;

		bool m_modified;
	};
}


#endif //PARAMETRICOBJECTSCONSTRUCTION_POCONFIGPANEL_HPP
