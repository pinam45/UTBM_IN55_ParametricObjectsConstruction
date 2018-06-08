#include <gui/POConfigPanel.hpp>

#include <string>
#include <algorithm>

namespace {
	constexpr unsigned int DEFAULT_LAYER_NB_POINTS = 3;
	constexpr float DEFAULT_LAYER_RADIUS = 1;
	constexpr float DEFAULT_LAYER_DISTANCE = 1;
	constexpr float DEFAULT_LAYER_ROTATION = 0;
	constexpr std::array<float,3> DEFAULT_LAYER_COLOR = {0, 0, 0};
	constexpr poc::LayerConfig DEFAULT_LAYER(
	  DEFAULT_LAYER_NB_POINTS,
	  DEFAULT_LAYER_RADIUS,
	  DEFAULT_LAYER_DISTANCE,
	  DEFAULT_LAYER_ROTATION,
	  DEFAULT_LAYER_COLOR
	);

	constexpr int MINIMUM_LAYER_POINTS_NUMBER = 1;
	constexpr int MAXIMUM_LAYER_POINTS_NUMBER = 50;

	constexpr float MINIMUM_LAYER_RADIUS = 0.0f;
	constexpr float MAXIMUM_LAYER_RADIUS = 5.0f;

	constexpr float MINIMUM_LAYER_DISTANCE = 0.0f;
	constexpr float MAXIMUM_LAYER_DISTANCE = 5.0f;

	constexpr size_t MINIMUM_LAYERS_NUMBER = 2;

	constexpr std::array<float,3> RED {{1,0,0}};
	constexpr std::array<float,3> GREEN {{0,1,0}};
	constexpr std::array<float,3> BLUE {{0,0,1}};
	constexpr poc::LayerConfig LAYER_1(4, 1.0f, 1.0f, 0, RED);
	constexpr poc::LayerConfig LAYER_2(10, 1.5f, 0.4f, 0.0f, BLUE);
	constexpr poc::LayerConfig LAYER_3(1, 0.5f, 1.0f, 0, GREEN);
	const std::initializer_list<poc::LayerConfig> DEFAULT_LAYERS {
		LAYER_1,
		LAYER_2,
		LAYER_3
	};

	template<class T = float>
	constexpr T pi = T(3.1415926535897932385L);
}

poc::POConfigPanel::POConfigPanel(float x, float y, float width, float height)
  : themeHolders{
	  POConfigPanel::ThemeHolder{"Arc Dark", ImGuiColorTheme::ArcDark, true},
	  POConfigPanel::ThemeHolder{"Flat UI", ImGuiColorTheme::FlatUI, false},
	  POConfigPanel::ThemeHolder{"Mint-Y-Dark", ImGuiColorTheme::MintYDark, false},
    }
    , m_position(x, y)
    , m_size(width, height)
    , rd()
    , layers(DEFAULT_LAYERS) {

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

const std::vector<poc::LayerConfig>& poc::POConfigPanel::getLayers() const {
	return layers;
}

bool poc::POConfigPanel::draw() {
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

	int layerNumber = 0;
	std::vector<bool> open_layers;
	bool insert_layer = false;
	int insert_layer_pos = 0;
	open_layers.reserve(layers.size());
	int i_layer = 0;
	bool swap = false;
	int to_swap = 0;

	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2.0f/7.0f, 0.6f, 0.6f));
	if(ImGui::Button("+", ImVec2(ImGui::GetWindowContentRegionWidth(), 20))) {
		insert_layer = true;
		insert_layer_pos = -1;
	}
	ImGui::PopStyleColor();

	for(LayerConfig& layer : layers) {
		bool open = true;
		const std::string layer_name = std::string("Layer ") + std::to_string(++layerNumber);
		ImGui::PushID(layerNumber);
		if(ImGui::CollapsingHeader(layer_name.c_str(), &open, ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_DefaultOpen)){
			int nb_points = static_cast<int>(layer.nb_point);
			ImGui::InputInt("points", &nb_points);
			if(nb_points != static_cast<int>(layer.nb_point) && nb_points >= MINIMUM_LAYER_POINTS_NUMBER && nb_points <= MAXIMUM_LAYER_POINTS_NUMBER) {
				layer.nb_point = static_cast<unsigned int>(nb_points);
				modification = true;
			}

			float radius_from_center = layer.radius_from_center;
			ImGui::SliderFloat("radius", &radius_from_center, MINIMUM_LAYER_RADIUS, MAXIMUM_LAYER_RADIUS);
			if(std::abs(radius_from_center - layer.radius_from_center) > std::numeric_limits<float>::epsilon() && radius_from_center > 0) {
				layer.radius_from_center = radius_from_center;
				modification = true;
			}

			float rotation = layer.rotation;
			ImGui::SliderAngle("rotation", &rotation);
			if(std::abs(rotation - layer.rotation) > std::numeric_limits<float>::epsilon()) {
				layer.rotation = rotation;
				modification = true;
			}

			if(layerNumber > 1) {
				float distances_with_layer = layer.distances_with_layer;
				ImGui::SliderFloat("distance", &distances_with_layer, MINIMUM_LAYER_DISTANCE, MAXIMUM_LAYER_DISTANCE);
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
		}

		if(ImGui::ArrowButton("up", ImGuiDir_Up)) {
			swap = true;
			to_swap = i_layer;
		}
		ImGui::SameLine();
		if(ImGui::ArrowButton("down", ImGuiDir_Down)) {
			swap = true;
			to_swap = i_layer + 1;
		}
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f/7.0f, 0.6f, 0.6f));
		ImGui::SameLine();
		if(ImGui::Button("Randomise", ImVec2(ImGui::GetContentRegionAvailWidth(), 20))) {
			layers[i_layer] =randomLayer();
			modification = true;
		}
		ImGui::PopStyleColor();

		ImGui::Separator();
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2.0f/7.0f, 0.6f, 0.6f));
		if(ImGui::Button("+", ImVec2(ImGui::GetWindowContentRegionWidth(), 20))) {
			insert_layer = true;
			insert_layer_pos = i_layer;
		}
		ImGui::PopStyleColor();

		ImGui::PopID();
		open_layers.push_back(open);
		++i_layer;
	}

	ImGui::End();

	if(swap && to_swap > 0 && to_swap < layers.size()){
		std::swap(layers[to_swap], layers[to_swap-1]);
		modification = true;
	}

	std::vector<poc::LayerConfig> old = std::move(layers);
	layers.clear();
	if(insert_layer && insert_layer_pos < 0){
		layers.push_back(DEFAULT_LAYER);
		modification = true;
	}
	for(unsigned int i = 0; i < open_layers.size(); ++i){
		if(open_layers[i] || old.size() <= MINIMUM_LAYERS_NUMBER){
			layers.push_back(old[i]);
		}
		else{
			modification = true;
		}
		if(insert_layer && i == insert_layer_pos){
			layers.push_back(DEFAULT_LAYER);
			modification = true;
		}
	}

	return modification;
}

poc::LayerConfig poc::POConfigPanel::randomLayer(){
	std::mt19937 random(rd());
	std::uniform_int_distribution<unsigned int> rng_nb_point(MINIMUM_LAYER_POINTS_NUMBER, MAXIMUM_LAYER_POINTS_NUMBER);
	std::uniform_real_distribution<float> rng_radius_from_center(MINIMUM_LAYER_RADIUS, MAXIMUM_LAYER_DISTANCE);
	std::uniform_real_distribution<float> rng_distances_with_layer(MINIMUM_LAYER_DISTANCE, MAXIMUM_LAYER_DISTANCE);
	std::uniform_real_distribution<float> rng_rotation(-2 * pi<>, 2 * pi<>);
	std::uniform_real_distribution<float> rng_color(0, 1);

	return poc::LayerConfig(
	  rng_nb_point(random),
	  rng_radius_from_center(random),
	  rng_distances_with_layer(random),
	  rng_rotation(random),
	  std::array<float,3>{
		rng_color(random),
		rng_color(random),
		rng_color(random)
	  });
}
