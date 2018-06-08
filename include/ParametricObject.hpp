//
// Created by Julien Barbier on 17/04/2018.
//

#ifndef PARAMETRICOBJECTSCONSTRUCTION_PARAMETRICOBJECT_HPP
#define PARAMETRICOBJECTSCONSTRUCTION_PARAMETRICOBJECT_HPP


#include <vector>
#include <array>

namespace poc {
    struct LayerConfig{
        unsigned int nb_point;
        float radius_from_center;
        float distances_with_layer;
        float rotation;
        std::array<float,3> color;

        constexpr LayerConfig(unsigned int nbPoint_, float radiusFromCenter_, float distances_with_layer_, float rotation_, std::array<float,3> color_) noexcept;
    };

    class ParametricObject {
    public:

        ParametricObject() = delete;
        ParametricObject(const ParametricObject& parametricObject) = default;

        explicit ParametricObject(const std::vector<LayerConfig>& layerConfigs);
        ParametricObject(ParametricObject&&) noexcept = default;

        void configure(const std::vector<LayerConfig>& layerConfigs);

        const std::vector<float>& getVertices() const;

        const std::vector<unsigned int>& getIndexes() const;

        unsigned int getNbPoint() const noexcept;
        unsigned long long int getNbIndexes() const noexcept;

    private:

        constexpr static unsigned short m_float_vertex = 6;
        bool m_is_changed;

        //attributes for the creation of the parametric object
        std::vector<LayerConfig> m_configs;

        //attributes compute by the object
        float m_height;
        std::vector<float> m_vertices_object;
        std::vector<unsigned int> m_index_object;
        std::vector<float> m_height_progressive;
        std::vector<unsigned int> m_cumulative_nb_point;

        float* computeVertices();
        unsigned int* computeIndexes();
        void computeVerticesForOneLayer(unsigned int index);
        bool computeIndexesForLayer(unsigned int index);
        void linksLayer(unsigned int index);
        void linksLayerDifferentNumber(unsigned int index);
        unsigned int findShortestPointFrom(unsigned int index, unsigned int index_layer_other);
        double computeDistanceBetween(unsigned int i, unsigned int j);
    };
}

constexpr poc::LayerConfig::LayerConfig(unsigned int nbPoint_, float radiusFromCenter_, float distances_with_layer_, float rotation_, std::array<float,3> color_) noexcept
		: nb_point(nbPoint_)
		, radius_from_center(radiusFromCenter_)
		, distances_with_layer(distances_with_layer_)
		, rotation(rotation_)
		, color(color_) {

}


#endif //PARAMETRICOBJECTSCONSTRUCTION_PARAMETRICOBJECT_HPP
