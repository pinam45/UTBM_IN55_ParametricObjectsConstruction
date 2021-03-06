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

#include <ParametricObject.hpp>

#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <cmath>

#include <cassert>

namespace {
	template<class T>
	constexpr T pi = T(3.1415926535897932385L);
}

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

		nb_point += layerConfigs[i].nb_point;
		if(i == 0) {
			m_cumulative_nb_point.push_back(0);
			m_height_progressive.push_back(0);
			m_height = 0.0f;
		} else {
			m_cumulative_nb_point.push_back(m_cumulative_nb_point[i - 1] + layerConfigs[i - 1].nb_point);
			m_height_progressive.push_back(m_height_progressive[i - 1] + layerConfigs[i].distances_with_layer);
			m_height += layerConfigs[i].distances_with_layer;
		}
	}
	m_vertices_object.resize(nb_point * m_float_vertex);
	computeVertices();
	computeIndexes();
}

float* poc::ParametricObject::computeVertices() {
	if(m_is_changed) {
		for(unsigned int i = 0; i < m_configs.size(); ++i) {
			computeVerticesForOneLayer(i);
		}
	}
	return m_vertices_object.data();
}

void poc::ParametricObject::computeVerticesForOneLayer(unsigned int index) {
	float z = m_height / 2 - m_height_progressive[index];
	unsigned int nb_point = m_configs[index].nb_point;
	double r = m_configs[index].radius_from_center;

	const unsigned int index_tmp = m_cumulative_nb_point[index] * m_float_vertex;

	if(nb_point == 1) {
		m_vertices_object[index_tmp] = 0.0f;
		m_vertices_object[index_tmp + 1] = 0.0f;
		m_vertices_object[index_tmp + 2] = z;

		m_vertices_object[index_tmp + 3] = m_configs[index].color[0];
		m_vertices_object[index_tmp + 4] = m_configs[index].color[1];
		m_vertices_object[index_tmp + 5] = m_configs[index].color[2];
	} else {
		const double angle = 2 * pi<double> / nb_point;
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

unsigned int* poc::ParametricObject::computeIndexes() {
	for(unsigned int i = 0; i < m_configs.size(); ++i) {
		computeIndexesForLayer(i);
		linksLayer(i);
	}
	return m_index_object.data();
}

bool poc::ParametricObject::computeIndexesForLayer(unsigned int index) {
	const unsigned int nb_point = m_configs[index].nb_point;
	const unsigned int j = m_cumulative_nb_point[index];
	unsigned int end = 0;
	if(nb_point < 3) {
		return false;
	} else {
		if(index == m_cumulative_nb_point.size() - 1) {
			end = static_cast<unsigned int>(m_vertices_object.size() / m_float_vertex);
		} else {
			end = m_cumulative_nb_point[index + 1];
		}

		for(unsigned int i = 0; i + 2 <= nb_point; i += 2) {
			m_index_object.push_back(i + j);
			m_index_object.push_back(i + j + 1);
			m_index_object.push_back((i + 2) % nb_point + j);
		}
		if(nb_point % 2) {
			for(unsigned int i = end - 3; i > j; i -= 2) {
				m_index_object.push_back(i);
				m_index_object.push_back(i + 2);
				m_index_object.push_back(j);
			}
		} else {
			for(unsigned int i = end - 4; i > j; i -= 2) {
				m_index_object.push_back(i);
				m_index_object.push_back(i + 2);
				m_index_object.push_back(j);
			}
		}
		return true;
	}
}

void poc::ParametricObject::linksLayer(unsigned int index) {
	const unsigned int nb_point = m_configs[index].nb_point;
	// If there is only one point
	if(nb_point == 1) {
		// If it is the first layer and there is more than one layer
		if(index == 0 && m_configs.size() > 1) {
			for(unsigned int t = 1; t + 1 < m_cumulative_nb_point[index + 1] + m_configs[index + 1].nb_point; ++t) {
				m_index_object.push_back(0);
				m_index_object.push_back(t);
				m_index_object.push_back(t + 1);
			}
			m_index_object.push_back(0);
			m_index_object.push_back(m_cumulative_nb_point[index + 1] + m_configs[index + 1].nb_point - 1);
			m_index_object.push_back(1);
		} else {
			// Else the layer is linked with the previous layer
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
	} else {
		// If the previous layer have only one point and is not the first layer
		if(index != 0 && index - 1 != 0 && m_configs[index - 1].nb_point == 1) {
			const unsigned int index_layer = m_cumulative_nb_point[index];
			const unsigned int index_layer_before = m_cumulative_nb_point[index - 1];
			for(unsigned int t = index_layer; t + 1 < m_cumulative_nb_point[index] + m_configs[index].nb_point; ++t) {
				m_index_object.push_back(index_layer_before);
				m_index_object.push_back(t);
				m_index_object.push_back(t + 1);
			}
			m_index_object.push_back(m_cumulative_nb_point[index] + m_configs[index].nb_point - 1);
			m_index_object.push_back(index_layer_before);
			m_index_object.push_back(m_cumulative_nb_point[index]);
		}
		// Link with the next layer
		linksLayerDifferentNumber(index);
	}
}

void poc::ParametricObject::linksLayerDifferentNumber(unsigned int index) {
	if(static_cast<int>(index - 1) >= 0 && m_configs[index - 1].nb_point != 1) {
		unsigned int nb_point_first = m_configs[index - 1].nb_point;
		unsigned int nb_point_second = m_configs[index].nb_point;
		unsigned int index_first_layer = m_cumulative_nb_point[index - 1];
		unsigned int index_second_layer = m_cumulative_nb_point[index];


		unsigned int lastPoint = index_first_layer;

		std::vector<unsigned int> tmp;


		if(nb_point_first > nb_point_second) {
			nb_point_first = m_configs[index].nb_point;
			nb_point_second = m_configs[index - 1].nb_point;

			index_first_layer = m_cumulative_nb_point[index];
			index_second_layer = m_cumulative_nb_point[index - 1];
			lastPoint = index_first_layer;

			++index;
		}


		unsigned int nearestPointForFirstPoint = findShortestPointFrom(index_second_layer, index - 1);

		for(unsigned int i = 0; i < nb_point_second; ++i) {
			unsigned int nearestPointFrom = findShortestPointFrom(i + index_second_layer, index - 1);
			if(nearestPointFrom != lastPoint) {
				if(nearestPointFrom - lastPoint == 2) {
					m_index_object.push_back((lastPoint + nb_point_first) % nb_point_first + index_first_layer);
					m_index_object.push_back((lastPoint + 1 + nb_point_first) % nb_point_first + index_first_layer);
					m_index_object.push_back(index_second_layer + (i - 1 + nb_point_second) % nb_point_second);

				} else {
					if(nearestPointFrom == (lastPoint + 2 + nb_point_first) % nb_point_first + index_first_layer) {
						m_index_object.push_back((lastPoint + nb_point_first) % nb_point_first + index_first_layer);
						m_index_object.push_back((lastPoint + 1 + nb_point_first) % nb_point_first + index_first_layer);
						m_index_object.push_back(index_second_layer + (i - 1 + nb_point_second) % nb_point_second);
					}
				}

				if(computeDistanceBetween((nearestPointFrom - 1 - index_first_layer + nb_point_first) % nb_point_first + index_first_layer, index_second_layer + i)
				   < computeDistanceBetween(nearestPointFrom, index_second_layer + (i - 1 + nb_point_second) % nb_point_second)) {

					m_index_object.push_back((nearestPointFrom - 1 - index_first_layer + nb_point_first) % nb_point_first + index_first_layer);
					m_index_object.push_back(index_second_layer + (i - 1 + nb_point_second) % nb_point_second);
					m_index_object.push_back(index_second_layer + i);
					m_index_object.push_back((nearestPointFrom - 1 - index_first_layer + nb_point_first) % nb_point_first + index_first_layer);
					m_index_object.push_back(index_second_layer + i);
					m_index_object.push_back(nearestPointFrom);
				} else {
					m_index_object.push_back(index_second_layer + (i - 1 + nb_point_second) % nb_point_second);
					m_index_object.push_back((nearestPointFrom - 1 - index_first_layer + nb_point_first) % nb_point_first + index_first_layer);
					m_index_object.push_back(nearestPointFrom);
					m_index_object.push_back(nearestPointFrom);
					m_index_object.push_back(index_second_layer + (i - 1 + nb_point_second) % nb_point_second);
					m_index_object.push_back(index_second_layer + i);
				}

				lastPoint = nearestPointFrom;
				tmp.clear();
			}
			if(tmp.empty()) {
				tmp.push_back(nearestPointFrom);
				tmp.push_back(i + index_second_layer);
			} else {
				m_index_object.push_back(tmp[0]);
				m_index_object.push_back(tmp[1]);
				m_index_object.push_back(index_second_layer + i);

				tmp.clear();

				tmp.push_back(nearestPointFrom);
				tmp.push_back(i + index_second_layer);
			}
		}
		if(nearestPointForFirstPoint != lastPoint) {
			if(computeDistanceBetween((nearestPointForFirstPoint - 1 - index_first_layer + nb_point_first) % nb_point_first + index_first_layer, index_second_layer)
			   < computeDistanceBetween(nearestPointForFirstPoint, index_second_layer + (nb_point_second - 1) % nb_point_second)) {

				m_index_object.push_back((nearestPointForFirstPoint - 1 - index_first_layer + nb_point_first) % nb_point_first + index_first_layer);
				m_index_object.push_back(index_second_layer + (nb_point_second - 1) % nb_point_second);
				m_index_object.push_back(index_second_layer);
				m_index_object.push_back((nearestPointForFirstPoint - 1 - index_first_layer + nb_point_first) % nb_point_first + index_first_layer);
				m_index_object.push_back(index_second_layer);
				m_index_object.push_back(nearestPointForFirstPoint);
			} else {
				m_index_object.push_back(index_second_layer + (nb_point_second - 1) % nb_point_second);
				m_index_object.push_back((nearestPointForFirstPoint - 1 - index_first_layer + nb_point_first) % nb_point_first + index_first_layer);
				m_index_object.push_back(nearestPointForFirstPoint);
				m_index_object.push_back(nearestPointForFirstPoint);
				m_index_object.push_back(index_second_layer + (nb_point_second - 1) % nb_point_second);
				m_index_object.push_back(index_second_layer);
			}
		} else {
			if(!tmp.empty()) {
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

	if(layerConfigs.empty()) {
		assert(false);
	}

	m_configs = layerConfigs;
	m_cumulative_nb_point.resize(m_configs.size());
	m_height_progressive.resize(m_configs.size());

	m_height = 0;
	nb_point = m_configs[0].nb_point;
	m_cumulative_nb_point[0] = 0;
	m_height_progressive[0] = 0;

	for(unsigned int i = 1; i < m_configs.size(); ++i) {
		nb_point += m_configs[i].nb_point;
		m_height_progressive[i] = m_height_progressive[i - 1] + m_configs[i].distances_with_layer;
		m_cumulative_nb_point[i] = m_cumulative_nb_point[i - 1] + m_configs[i - 1].nb_point;
		m_height += m_configs[i].distances_with_layer;
	}
	m_vertices_object.resize(nb_point * m_float_vertex);
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
	unsigned int nb_point = m_configs[index_layer_other].nb_point;
	unsigned int index_begin_layer = m_cumulative_nb_point[index_layer_other];
	unsigned int min_index = 0;
	double min_distance = std::numeric_limits<double>::max();
	for(unsigned int i = index_begin_layer; i < index_begin_layer + nb_point; ++i) {
		const double tmp_distance =
		  pow(static_cast<double>((m_vertices_object[i * m_float_vertex] - m_vertices_object[index * m_float_vertex])), 2.0) +
		  pow(static_cast<double>((m_vertices_object[i * m_float_vertex + 1] - m_vertices_object[index * m_float_vertex + 1])), 2.0) +
		  pow(static_cast<double>((m_vertices_object[i * m_float_vertex + 2] - m_vertices_object[index * m_float_vertex + 2])), 2.0);
		if(tmp_distance <= min_distance) {
			min_distance = tmp_distance;
			min_index = i;
		}
	}
	return min_index;
}

double poc::ParametricObject::computeDistanceBetween(unsigned int i, unsigned int j) {
	return sqrt(
	  pow(static_cast<double>((m_vertices_object[i * m_float_vertex] - m_vertices_object[j * m_float_vertex])), 2.0) +
	  pow(static_cast<double>((m_vertices_object[i * m_float_vertex + 1] - m_vertices_object[j * m_float_vertex + 1])), 2.0) +
	  pow(static_cast<double>((m_vertices_object[i * m_float_vertex + 2] - m_vertices_object[j * m_float_vertex + 2])), 2.0));
}
