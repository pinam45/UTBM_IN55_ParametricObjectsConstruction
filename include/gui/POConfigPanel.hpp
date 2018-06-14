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
