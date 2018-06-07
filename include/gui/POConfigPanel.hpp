#ifndef PARAMETRICOBJECTSCONSTRUCTION_POCONFIGPANEL_HPP
#define PARAMETRICOBJECTSCONSTRUCTION_POCONFIGPANEL_HPP


#include <imgui.h>

#include "ParametricObject.hpp"
#include "imgui_easy_theming.hpp"

namespace poc{

	class POConfigPanel {

	public:

		POConfigPanel(float x, float y, float width, float height);

		void setX(float x);
		void setY(float y);
		void setWidth(float width);
		void setHeight(float height);

		const std::vector<poc::LayerConfig>& getLayers() const;

		bool draw();

	private:

		struct ThemeHolder
		{
			const char* name;
			const ImGuiColorTheme& colorTheme;
			bool enabled;
		};
		std::vector<ThemeHolder> themeHolders;

		ImVec2 m_position;
		ImVec2 m_size;

		std::vector<poc::LayerConfig> layers;
	};
}


#endif //PARAMETRICOBJECTSCONSTRUCTION_POCONFIGPANEL_HPP
