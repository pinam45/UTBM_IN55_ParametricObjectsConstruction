//
// Created by Julien Barbier on 17/04/2018.
//
#define _USE_MATH_DEFINES

#include <vector>
#include <ParametricObject.hpp>

#include <cmath>
#include <iostream>
#include <iterator>

ParametricObject::ParametricObject(std::vector<float> radius_from_center, std::vector<unsigned int> nb_point_layout,
                                   std::vector<float> distances_between_layout, std::vector<float> rotation_layout)
        : m_is_changed(true)
        , m_radius_from_center(radius_from_center)
        , m_nb_point_layout(nb_point_layout)
        , m_distances_between_layout(distances_between_layout)
        , m_rotation_layout(rotation_layout)
        , m_nb_layout()
        , m_heigth()
        , m_vertices_object()
        , m_heigth_progressiv(radius_from_center.size())
        , m_cumulativ_nb_point(radius_from_center.size())
{
    unsigned long long int size = radius_from_center.size();
    m_nb_layout = nb_point_layout.size();
    m_heigth = 0.0f;
    unsigned int nb_points = m_nb_point_layout[0];

    m_cumulativ_nb_point[0] = 0;
    m_heigth_progressiv[0] = 0.0f;
    for(unsigned int i = 1; i < size; ++i){
        m_heigth += m_distances_between_layout[i-1];
        m_heigth_progressiv[i] = m_heigth;

        m_cumulativ_nb_point[i] += m_cumulativ_nb_point[i-1] + m_nb_point_layout[i-1];
        nb_points += m_nb_point_layout[i];
    }
    m_vertices_object = std::vector<float>(nb_points*m_float_vertex);
}

ParametricObject::ParametricObject(const ParametricObject &parametricObject):
        m_is_changed(parametricObject.m_is_changed)
        , m_radius_from_center(parametricObject.m_radius_from_center)
        , m_nb_point_layout(parametricObject.m_nb_point_layout)
        , m_distances_between_layout(parametricObject.m_distances_between_layout)
        , m_rotation_layout(parametricObject.m_rotation_layout)
        , m_nb_layout(parametricObject.m_nb_layout)
        , m_heigth(parametricObject.m_heigth)
        , m_vertices_object(parametricObject.m_vertices_object)
        , m_heigth_progressiv(parametricObject.m_heigth_progressiv)
        , m_cumulativ_nb_point(parametricObject.m_cumulativ_nb_point){}

float* ParametricObject::computeVertices() {
    if(m_is_changed){
        for(unsigned int i = 0; i < m_nb_point_layout.size(); ++i){
            computeVerticesForOneLayout(i);
        }
    }
    std::copy(m_vertices_object.begin(), m_vertices_object.end(), std::ostream_iterator<float>(std::cout, "\n"));
    std::cout << std::endl;
    return m_vertices_object.data();
}

void ParametricObject::computeVerticesForOneLayout(const unsigned int index) {
    double angle;

    float z = m_heigth/2 - m_heigth_progressiv[index];
    unsigned int nb_point = m_nb_point_layout[index];
    double r = m_radius_from_center[index];

    unsigned int index_tmp = m_cumulativ_nb_point[index]*m_float_vertex;

    if (nb_point == 1){
        m_vertices_object[index_tmp] = 0.0f;
        m_vertices_object[index_tmp + 1] = 0.0f;
        m_vertices_object[index_tmp + 2] = z;

        m_vertices_object[index_tmp + 3] = index%3;
        m_vertices_object[index_tmp + 4] = (index+1)%3;
        m_vertices_object[index_tmp + 5] = (index+2)%3;
    } else {
        angle = 2*M_PI/nb_point + static_cast<double >(m_rotation_layout[index]);

        for (unsigned int i = 0; i < nb_point; ++i){
            unsigned int tmp = i * m_float_vertex;
            m_vertices_object[index_tmp + tmp] = static_cast<float>(cos(i*angle) * r);
            m_vertices_object[index_tmp + tmp+1] = static_cast<float>(sin(i*angle) * r);
            m_vertices_object[index_tmp + tmp +2] = z;

            m_vertices_object[index_tmp + tmp+3] = index%3;
            m_vertices_object[index_tmp + tmp+4] = (index+1)%3;
            m_vertices_object[index_tmp + tmp+5] = (index+2)%3;
        }
    }
}

unsigned int* ParametricObject::computeIndexes(){
    for(unsigned int i = 0; i < m_nb_layout; ++i){
        bool is_computed = computeIndexesForLayer(i);
        if(!is_computed){
            if(i == 0){
                for(unsigned int j = 1; j+1 < m_cumulativ_nb_point[i+1]; ++j){
                    m_index_object.push_back(0);
                    m_index_object.push_back(j);
                    m_index_object.push_back(j+1);
                }
            } else {
                unsigned int index_before_layer = m_cumulativ_nb_point[i-1];
                for(unsigned int j = index_before_layer; j+1 <= m_cumulativ_nb_point[i]; ++j){
                    m_index_object.push_back(j);
                    m_index_object.push_back(j+1);
                    m_index_object.push_back(m_cumulativ_nb_point[i]);
                }
                m_index_object.push_back(m_cumulativ_nb_point[i] - 1);
                m_index_object.push_back(index_before_layer);
                m_index_object.push_back(m_cumulativ_nb_point[i]);
            }
        }
    }
    std::copy(m_index_object.begin(), m_index_object.end(), std::ostream_iterator<float>(std::cout, "\n"));
    std::cout << std::endl;
    return m_index_object.data();
}

unsigned int ParametricObject::getNbPoint() {
    return static_cast<unsigned int>(m_vertices_object.size()/m_float_vertex);
}

bool ParametricObject::computeIndexesForLayer(unsigned int index) {
    int nb_point = m_nb_point_layout[index];
    unsigned int j = m_cumulativ_nb_point[index];
    unsigned int end = 0;
    if(nb_point == 1){
        return false;
    } else {
        if(index == m_cumulativ_nb_point.size()-1){
            end = static_cast<unsigned int>(m_vertices_object.size()/m_float_vertex);
        } else {
            end = m_cumulativ_nb_point[index+1];
        }
        for (unsigned int i = j; i + 2 <= end; i += 2){
            m_index_object.push_back(i);
            m_index_object.push_back(i+1);
            m_index_object.push_back((i+2)%nb_point+j);
        }
        if(nb_point%2){
            for(int i = end - 3; i > j; i -= 2){
                m_index_object.push_back(i);
                m_index_object.push_back(i+2);
                m_index_object.push_back(j);
            }
        }
        return true;
    }
}

unsigned long long int ParametricObject::getNbIndexes() {
    return m_index_object.size();
}
