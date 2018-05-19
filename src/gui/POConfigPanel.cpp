#include <gui/POConfigPanel.hpp>

#include <string>

poc::POConfigPanel::POConfigPanel(float x, float y, float width, float height)
  : themeHolders{
	  POConfigPanel::ThemeHolder{"Arc Dark", ImGuiColorTheme::ArcDark, true},
	  POConfigPanel::ThemeHolder{"Flat UI", ImGuiColorTheme::FlatUI, false},
	  POConfigPanel::ThemeHolder{"Mint-Y-Dark", ImGuiColorTheme::MintYDark, false},
    }
    , m_position(x, y)
    , m_size(width, height) {

	for(ThemeHolder& themeHolder : themeHolders) {
		if(themeHolder.enabled) {
			ImGuiEasyTheming(themeHolder.colorTheme);
		}
	}
}

void poc::POConfigPanel::setX(float x) {
	m_position.x = x;
}

void poc::POConfigPanel::setY(float y) {
	m_position.y = y;
}

void poc::POConfigPanel::setWidth(float width) {
	m_size.x = width;
}

void poc::POConfigPanel::setHeight(float height) {
	m_size.y = height;
}

bool poc::POConfigPanel::draw(std::vector<LayerConfig>& layers) {
	bool modification = false;

	ImGui::SetNextWindowPos(m_position);
	ImGui::SetNextWindowSize(m_size);
	ImGui::Begin("Panel",
	             nullptr,
	             ImGuiWindowFlags_NoTitleBar
	             | ImGuiWindowFlags_NoResize
	             | ImGuiWindowFlags_NoMove
	             | ImGuiWindowFlags_NoCollapse
	             | ImGuiWindowFlags_NoSavedSettings
	             | ImGuiWindowFlags_MenuBar);

	if(ImGui::BeginMenuBar()) {
		if(ImGui::BeginMenu("Options")) {
			if(ImGui::BeginMenu("Color theme", true)) {
				for(ThemeHolder& themeHolder : themeHolders) {
					if(ImGui::MenuItem(themeHolder.name, nullptr, themeHolder.enabled)) {
						ImGuiEasyTheming(themeHolder.colorTheme);
						for(ThemeHolder& themeHolderBis : themeHolders) {
							themeHolderBis.enabled = false;
						}
						themeHolder.enabled = true;
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	unsigned int layerNumber = 0;
	for(LayerConfig& layer : layers) {
		const std::string layer_name = std::string("Layer ") + std::to_string(++layerNumber);
		ImGui::PushID(&layer);
		ImGui::CollapsingHeader(layer_name.c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf);

		int nb_points = static_cast<int>(layer.nbPoint);
		ImGui::InputInt("points", &nb_points);
		if(nb_points != static_cast<int>(layer.nbPoint) && nb_points > 0) {
			layer.nbPoint = static_cast<unsigned int>(nb_points);
			modification = true;
		}

		float radius_from_center = layer.radiusFromCenter;
		ImGui::SliderFloat("radius", &radius_from_center, 0.0f, 5.0f);
		if(std::abs(radius_from_center - layer.radiusFromCenter) > std::numeric_limits<float>::epsilon() && radius_from_center > 0) {
			layer.radiusFromCenter = radius_from_center;
			modification = true;
		}

		float rotation = layer.rotation;
		ImGui::SliderAngle("rotation", &rotation);
		if(std::abs(rotation - layer.radiusFromCenter) > std::numeric_limits<float>::epsilon() ) {
			layer.rotation = rotation;
			modification = true;
		}

		if(layerNumber > 1) {
			float distances_with_layer = layer.distances_with_layer;
			ImGui::SliderFloat("distance", &distances_with_layer, 0.0f, 5.0f);
			if(std::abs(distances_with_layer - layer.distances_with_layer) > std::numeric_limits<float>::epsilon() && distances_with_layer > 0) {
				layer.distances_with_layer = distances_with_layer;
				modification = true;
			}
		}

		std::array<float,3> color = layer.color;
		ImGui::ColorEdit3("color", color.data());
		if(color != layer.color){
			layer.color = color;
			modification = true;
		}

		ImGui::PopID();
	}

	ImGui::Separator();
	if(ImGui::Button("Add", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 20))) {
		layers.emplace_back(3, 1, 1, 0, std::array<float,3>{{0,0,0}});
		modification = true;
	}
	if(layers.size() > 2) {
		ImGui::SameLine();
		if(ImGui::Button("Remove", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 20))) {
			if(!layers.empty()) {
				layers.pop_back();
				modification = true;
			}
		}
	}

	ImGui::End();

	return modification;
}
