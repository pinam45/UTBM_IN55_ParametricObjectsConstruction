//
// Created by Julien Barbier on 17/04/2018.
//
#include <ParametricObject.hpp>

#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>

#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>

poc::ParametricObject::ParametricObject(const std::vector<LayerConfig>& layerConfigs)
		: m_is_changed(true)
		, m_configs(layerConfigs)
		, m_height()
		, m_vertices_object()
		, m_index_object()
		, m_height_progressive()
		, m_cumulative_nb_point() {
	unsigned int nb_point = 0;
	for(unsigned int i = 0; i < layerConfigs.size(); ++i) {

		nb_point += layerConfigs[i].nbPoint;
		if(i == 0) {
			m_cumulative_nb_point.push_back(0);
			m_height_progressive.push_back(0);
			m_height = 0.0f;
		}
		else {
			m_cumulative_nb_point.push_back(m_cumulative_nb_point[i - 1] + layerConfigs[i - 1].nbPoint);
			m_height_progressive.push_back(m_height_progressive[i - 1] + layerConfigs[i].distances_with_layer);
			m_height += layerConfigs[i].distances_with_layer;
		}
	}
	m_vertices_object.resize(nb_point * m_float_vertex);
    computeVertices();
    computeIndexes();
}

//Compute vertices
float* poc::ParametricObject::computeVertices() {
	if(m_is_changed) {
		for(unsigned int i = 0; i < m_configs.size(); ++i) {
			computeVerticesForOneLayer(i);
		}
	}
	//std::copy(m_vertices_object.begin(), m_vertices_object.end(), std::ostream_iterator<float>(std::cout, "\n"));
	//std::cout << std::endl;
	return m_vertices_object.data();
}

void poc::ParametricObject::computeVerticesForOneLayer(unsigned int index) {
	float z = m_height / 2 - m_height_progressive[index];
	unsigned int nb_point = m_configs[index].nbPoint;
	double r = m_configs[index].radiusFromCenter;

	const unsigned int index_tmp = m_cumulative_nb_point[index] * m_float_vertex;

	if(nb_point == 1) {
		m_vertices_object[index_tmp] = 0.0f;
		m_vertices_object[index_tmp + 1] = 0.0f;
		m_vertices_object[index_tmp + 2] = z;

		m_vertices_object[index_tmp + 3] = m_configs[index].color[0];
		m_vertices_object[index_tmp + 4] = m_configs[index].color[1];
		m_vertices_object[index_tmp + 5] = m_configs[index].color[2];
	}
	else {
		const double angle = 2 * M_PI / nb_point;
		const double rotation = m_configs[index].rotation;

		for(unsigned int i = 0; i < nb_point; ++i) {
			const unsigned int tmp = i * m_float_vertex;
			m_vertices_object[index_tmp + tmp] = static_cast<float>(cos(i * angle + rotation) * r);
			m_vertices_object[index_tmp + tmp + 1] = static_cast<float>(sin(i * angle + rotation) * r);
			m_vertices_object[index_tmp + tmp + 2] = z;

			m_vertices_object[index_tmp + tmp + 3] = m_configs[index].color[0];
			m_vertices_object[index_tmp + tmp + 4] = m_configs[index].color[1];
			m_vertices_object[index_tmp + tmp + 5] = m_configs[index].color[2];
		}
	}
}

//Compute indices
unsigned int* poc::ParametricObject::computeIndexes() {
	for(unsigned int i = 0; i < m_configs.size(); ++i) {
		computeIndexesForLayer(i);
		linksLayer(i);
	}
	return m_index_object.data();
}

bool poc::ParametricObject::computeIndexesForLayer(unsigned int index) {
	const unsigned int nb_point = m_configs[index].nbPoint;
	const unsigned int j = m_cumulative_nb_point[index];
	unsigned int end = 0;
	if(nb_point < 3) {
		return false;
	}
	else {
		if(index == m_cumulative_nb_point.size() - 1) {
			end = static_cast<unsigned int>(m_vertices_object.size() / m_float_vertex);
		}
		else {
			end = m_cumulative_nb_point[index + 1];
		}

		for(unsigned int i = 0; i + 2 <= nb_point; i += 2) {
			m_index_object.push_back(i + j);
			m_index_object.push_back(i + j + 1);
			m_index_object.push_back((i + 2) % nb_point + j);

			//std::cout << i+j << " " <<i+j+1 << " " << (i+2)%nb_point+j << std::endl;
		}
		if(nb_point % 2) {
			for(unsigned int i = end - 3; i > j; i -= 2) {
				m_index_object.push_back(i);
				m_index_object.push_back(i + 2);
				m_index_object.push_back(j);

				//std::cout << i << " " <<i+2 << " " << j << std::endl;
			}
		}
		else {
			for(unsigned int i = end - 4; i > j; i -= 2) {
				m_index_object.push_back(i);
				m_index_object.push_back(i + 2);
				m_index_object.push_back(j);

				//std::cout << i << " " <<i+2 << " " << j << std::endl;
			}
		}
		//std::cout << std::endl;
		return true;
	}
}

void poc::ParametricObject::linksLayer(unsigned int index) {
	const unsigned int nb_point = m_configs[index].nbPoint;
	//Si on a 1 seul point
	if(nb_point == 1) {
		//Si on est sur le premier layer et qu'on a plus de 1 layer
		if(index == 0 && m_configs.size() > 1) {
			for(unsigned int t = 1; t + 1 < m_cumulative_nb_point[index + 1] + m_configs[index + 1].nbPoint; ++t) {
				m_index_object.push_back(0);
				m_index_object.push_back(t);
				m_index_object.push_back(t + 1);
			}
			m_index_object.push_back(0);
			m_index_object.push_back(m_cumulative_nb_point[index + 1] + m_configs[index + 1].nbPoint - 1);
			m_index_object.push_back(1);
		}
		else {
			//Sinon on prend le layer d'avant et on raccorde
			const unsigned int index_before_layer = m_cumulative_nb_point[index - 1];
			for(unsigned int t = index_before_layer; t + 1 <= m_cumulative_nb_point[index]; ++t) {
				m_index_object.push_back(t);
				m_index_object.push_back(t + 1);
				m_index_object.push_back(m_cumulative_nb_point[index]);
			}
			m_index_object.push_back(m_cumulative_nb_point[index] - 1);
			m_index_object.push_back(index_before_layer);
			m_index_object.push_back(m_cumulative_nb_point[index]);
		}
	}
	else {
		//Si le layer d'avant a un seul point et il ne s'agit pas du premier layer (déjà raccorder)
		if(index != 0 && index - 1 != 0 && m_configs[index - 1].nbPoint == 1) {
			const unsigned int index_layer = m_cumulative_nb_point[index];
			const unsigned int index_layer_before = m_cumulative_nb_point[index - 1];
			for(unsigned int t = index_layer; t + 1 < m_cumulative_nb_point[index] + m_configs[index].nbPoint; ++t) {
				m_index_object.push_back(index_layer_before);
				m_index_object.push_back(t);
				m_index_object.push_back(t + 1);
			}
			m_index_object.push_back(m_cumulative_nb_point[index] + m_configs[index].nbPoint - 1);
			m_index_object.push_back(index_layer_before);
			m_index_object.push_back(m_cumulative_nb_point[index]);
		}
		//On rattache avec le layer d'après
		linksLayerDifferentNumber(index);
	}
}

void poc::ParametricObject::linksLayerDifferentNumber(unsigned int index) {
	if(static_cast<int>(index - 1) >= 0 && m_configs[index - 1].nbPoint != 1) {
        unsigned int nb_point_first = m_configs[index - 1].nbPoint;
        unsigned int nb_point_second = m_configs[index].nbPoint;
        unsigned int index_first_layer = m_cumulative_nb_point[index - 1];
        unsigned int index_second_layer = m_cumulative_nb_point[index];


        unsigned int lastPoint = index_first_layer;

        std::vector<unsigned int> tmp;


        if (nb_point_first > nb_point_second) {
            nb_point_first = m_configs[index].nbPoint;
            nb_point_second = m_configs[index - 1].nbPoint;

            index_first_layer = m_cumulative_nb_point[index];
            index_second_layer = m_cumulative_nb_point[index - 1];
			lastPoint = index_first_layer;

			++index;
        }


        unsigned int nearestPointForFirstPoint = findShortestPointFrom(index_second_layer, index - 1);

        for (unsigned int  i = 0; i < nb_point_second; ++i){
            unsigned int nearestPointFrom = findShortestPointFrom(i+index_second_layer, index - 1);
            if(nearestPointFrom != lastPoint){
                m_index_object.push_back((nearestPointFrom -1 - index_first_layer + nb_point_first)%nb_point_first + index_first_layer);
                m_index_object.push_back(index_second_layer + (i -1 + nb_point_second)%nb_point_second);
                m_index_object.push_back(index_second_layer + i);
                m_index_object.push_back((nearestPointFrom -1 - index_first_layer  + nb_point_first)%nb_point_first + index_first_layer);
                m_index_object.push_back(index_second_layer + i);
                m_index_object.push_back(nearestPointFrom);

                lastPoint = nearestPointFrom;
                tmp.clear();
            }
            if(tmp.size() == 0){
                tmp.push_back(nearestPointFrom);
                tmp.push_back(i + index_second_layer);
            } else {
                m_index_object.push_back(tmp[0]);
                m_index_object.push_back(tmp[1]);
                m_index_object.push_back(index_second_layer+i);

				tmp.clear();

                tmp.push_back(nearestPointFrom);
                tmp.push_back(i + index_second_layer);
            }
        }
		if(nearestPointForFirstPoint != lastPoint){
            m_index_object.push_back((nearestPointForFirstPoint -1 - index_first_layer + nb_point_first)%nb_point_first + index_first_layer);
            m_index_object.push_back(index_second_layer + (nb_point_second - 1)%nb_point_second);
            m_index_object.push_back(index_second_layer);
            m_index_object.push_back((nearestPointForFirstPoint -1 - index_first_layer  + nb_point_first)%nb_point_first + index_first_layer);
            m_index_object.push_back(index_second_layer );
            m_index_object.push_back(nearestPointForFirstPoint);
		} else {
			if(tmp.size()){
				m_index_object.push_back(tmp[0]);
				m_index_object.push_back(tmp[1]);
				m_index_object.push_back(index_second_layer);
			}
		}
    }
}

//Getter
unsigned long long int poc::ParametricObject::getNbIndexes() const noexcept {
	return m_index_object.size();
}

unsigned int poc::ParametricObject::getNbPoint() const noexcept {
	return static_cast<unsigned int>(m_vertices_object.size() / m_float_vertex);
}

void poc::ParametricObject::configure(const std::vector<LayerConfig>& layerConfigs) {
	unsigned int nb_point = 0;

	if(!layerConfigs.size()){
		assert(false);
	}

	m_configs = layerConfigs;
	m_cumulative_nb_point.resize(m_configs.size());
	m_height_progressive.resize(m_configs.size());

	m_height = 0;
	nb_point = m_configs[0].nbPoint;
	m_cumulative_nb_point[0] = 0;
	m_height_progressive[0] = 0;

	for(unsigned int i = 1; i < m_configs.size(); ++i){
		nb_point += m_configs[i].nbPoint;
		m_height_progressive[i] = m_height_progressive[i-1] + m_configs[i].distances_with_layer;
		m_cumulative_nb_point[i] = m_cumulative_nb_point[i-1] + m_configs[i-1].nbPoint;
		m_height += m_configs[i].distances_with_layer;
	}
    m_vertices_object.resize(nb_point*m_float_vertex);
	m_index_object.clear();
	computeVertices();
	computeIndexes();
}

const std::vector<float>& poc::ParametricObject::getVertices() const {
	return m_vertices_object;
}

const std::vector<unsigned int>& poc::ParametricObject::getIndexes() const {
	return m_index_object;
}

unsigned int poc::ParametricObject::findShortestPointFrom(unsigned int index, unsigned int index_layer_other) {
	unsigned int nb_point = m_configs[index_layer_other].nbPoint;
	unsigned int index_begin_layer = m_cumulative_nb_point[index_layer_other];
	unsigned int min_index = 0;
	double min_distance = std::numeric_limits<float>::max();
	for(unsigned int i = index_begin_layer; i < index_begin_layer + nb_point; ++i) {
		const double tmp_distance = pow(static_cast<double>((m_vertices_object[i * m_float_vertex] - m_vertices_object[index * m_float_vertex])), 2.0)
		                            + pow(static_cast<double>((m_vertices_object[i * m_float_vertex + 1] - m_vertices_object[index * m_float_vertex + 1])), 2.0)
		                            + pow(static_cast<double>((m_vertices_object[i * m_float_vertex + 2] - m_vertices_object[index * m_float_vertex + 2])), 2.0);
		if(tmp_distance < min_distance) {
			min_distance = tmp_distance;
			min_index = i;
		}
	}
	return min_index;
}
