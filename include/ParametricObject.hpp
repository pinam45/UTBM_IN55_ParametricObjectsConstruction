//
// Created by Julien Barbier on 17/04/2018.
//

#ifndef PARAMETRICOBJECTSCONSTRUCTION_PARAMETRICOBJECT_HPP
#define PARAMETRICOBJECTSCONSTRUCTION_PARAMETRICOBJECT_HPP


#include <vector>

class ParametricObject {
public:

    ParametricObject() = delete;
    ParametricObject(std::vector<float> radius_from_center
            , std::vector<unsigned int> nb_point_layout, std::vector<float> distances_between_layout, std::vector<float> rotation_layout);
    ParametricObject(const ParametricObject& parametricObject);

    float* computeVertices();
    unsigned int* computeIndexes();
    unsigned int getNbPoint();
    unsigned long long int getNbIndexes();

private:

    constexpr static unsigned short m_float_vertex = 6;
    bool m_is_changed;

    //attributes for the creation of the parametric object
    std::vector<float> m_radius_from_center;
    std::vector<unsigned int> m_nb_point_layout;
    std::vector<float> m_distances_between_layout;
    std::vector<float> m_rotation_layout;

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
};


#endif //PARAMETRICOBJECTSCONSTRUCTION_PARAMETRICOBJECT_HPP
