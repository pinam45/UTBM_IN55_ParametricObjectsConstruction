//
// Created by Julien Barbier on 17/04/2018.
//

#ifndef PARAMETRICOBJECTSCONSTRUCTION_PARAMETRICOBJECT_HPP
#define PARAMETRICOBJECTSCONSTRUCTION_PARAMETRICOBJECT_HPP


#include <vector>

struct LayerConfig{
    unsigned int nbPoint;
    float radiusFromCenter;
    float distances_with_layer;
    float rotation;

    LayerConfig(unsigned int nbPoint, float radiusFromCenter, float distances_with_layer, float rotation) noexcept;
};

class ParametricObject {
public:

    ParametricObject() = delete;
    ParametricObject(const ParametricObject& parametricObject) = default;

    explicit ParametricObject(const std::vector<LayerConfig>& layerConfigs);
    ParametricObject(ParametricObject&&) noexcept = default;

    void configure(const std::vector<LayerConfig>& layerConfigs);

    const std::vector<float>& get_vertices() const;

    const std::vector<unsigned int>& get_indexes() const;

    float* computeVertices();
    unsigned int* computeIndexes();
    unsigned int getNbPoint() const noexcept;
    unsigned long long int getNbIndexes() const noexcept;

private:

    constexpr static unsigned short m_float_vertex = 6;
    bool m_is_changed;

    //attributes for the creation of the parametric object
    std::vector<LayerConfig> m_configs;

    //attributes compute by the object
    unsigned long long int m_nb_layout;
    float m_heigth;
    std::vector<float> m_vertices_object;
    std::vector<unsigned int> m_index_object;
    std::vector<float> m_heigth_progressiv;
    std::vector<unsigned int> m_cumulativ_nb_point;

    void computeVerticesForOneLayout(unsigned int index);
    bool computeIndexesForLayer(unsigned int index);
    void linksLayer(unsigned int index);
    void linkslayerDifferentNumber(unsigned int index);
    unsigned int findShortestPointFrom(unsigned int index, unsigned int index_layer_other);
};

#endif //PARAMETRICOBJECTSCONSTRUCTION_PARAMETRICOBJECT_HPP
