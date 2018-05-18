//
// Created by Julien Barbier on 17/04/2018.
//
#define _USE_MATH_DEFINES

#include <vector>
#include <ParametricObject.hpp>

#include <cmath>
#include <iostream>
#include <iterator>
#include <algorithm>


poc::ParametricObject::ParametricObject(const std::vector<LayerConfig>& layerConfigs)
        : m_is_changed(true)
        , m_configs(layerConfigs)
        , m_nb_layout(layerConfigs.size())
        , m_heigth()
        , m_vertices_object()
        , m_index_object()
        , m_heigth_progressiv()
        , m_cumulativ_nb_point() {
    unsigned int nb_point = 0;
    for (unsigned int i = 0; i < layerConfigs.size(); ++i){

        nb_point += layerConfigs[i].nbPoint;
        if(i == 0){
            m_cumulativ_nb_point.push_back(0);
            m_heigth_progressiv.push_back(0);
            m_heigth = 0.0f;
        } else {
            m_cumulativ_nb_point.push_back(m_cumulativ_nb_point[i-1] + layerConfigs[i-1].nbPoint);
            m_heigth_progressiv.push_back(m_heigth_progressiv[i-1]+layerConfigs[i].distances_with_layer);
            m_heigth += layerConfigs[i].distances_with_layer;
        }
    }
    m_vertices_object = std::vector<float>(nb_point*m_float_vertex);
}

//Compute vertices
float* poc::ParametricObject::computeVertices() {
    if(m_is_changed){
        for(unsigned int i = 0; i < m_configs.size(); ++i){
            computeVerticesForOneLayout(i);
        }
    }
    //std::copy(m_vertices_object.begin(), m_vertices_object.end(), std::ostream_iterator<float>(std::cout, "\n"));
    //std::cout << std::endl;
    return m_vertices_object.data();
}

void poc::ParametricObject::computeVerticesForOneLayout(const unsigned int index) {
    double angle;

    float z = m_heigth/2 - m_heigth_progressiv[index];
    unsigned int nb_point = m_configs[index].nbPoint;
    double r = m_configs[index].radiusFromCenter;

    unsigned int index_tmp = m_cumulativ_nb_point[index]*m_float_vertex;

    if (nb_point == 1){
        m_vertices_object[index_tmp] = 0.0f;
        m_vertices_object[index_tmp + 1] = 0.0f;
        m_vertices_object[index_tmp + 2] = z;

        m_vertices_object[index_tmp + 3] = static_cast<float>(index%3);
        m_vertices_object[index_tmp + 4] = static_cast<float>((index+1)%3);
        m_vertices_object[index_tmp + 5] = static_cast<float>((index+2)%3);
    } else {
        angle = 2*M_PI/nb_point;
        double rotation = m_configs[index].rotation;

        for (unsigned int i = 0; i < nb_point; ++i){
            unsigned int tmp = i * m_float_vertex;
            m_vertices_object[index_tmp + tmp] = static_cast<float>(cos(i*angle + rotation) * r);
            m_vertices_object[index_tmp + tmp+1] = static_cast<float>(sin(i*angle + rotation) * r);
            m_vertices_object[index_tmp + tmp +2] = z;

            m_vertices_object[index_tmp + tmp+3] = static_cast<float>(index%3);
            m_vertices_object[index_tmp + tmp+4] = static_cast<float>((index+1)%3);
            m_vertices_object[index_tmp + tmp+5] = static_cast<float>((index+2)%3);
        }
    }
}

//Compute indices
unsigned int* poc::ParametricObject::computeIndexes(){
    for(unsigned int i = 0; i < m_nb_layout; ++i){
        computeIndexesForLayer(i);
        linksLayer(i);
        //if(i !=0)
         //   linksLayer(i);
    }
    //std::copy(m_index_object.begin(), m_index_object.end(), std::ostream_iterator<unsigned int>(std::cout, "\n"));
    //std::cout << std::endl;
    return m_index_object.data();
}

bool poc::ParametricObject::computeIndexesForLayer(unsigned int index){
    unsigned int nb_point = m_configs[index].nbPoint;
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

        for (unsigned int i = 0; i + 2 <= nb_point; i += 2){
            m_index_object.push_back(i+j);
            m_index_object.push_back(i+j+1);
            m_index_object.push_back((i+2)%nb_point+j);

            //std::cout << i+j << " " <<i+j+1 << " " << (i+2)%nb_point+j << std::endl;
        }
        if(nb_point%2){
            for(unsigned int i = end - 3; i > j; i -= 2){
                m_index_object.push_back(i);
                m_index_object.push_back(i+2);
                m_index_object.push_back(j);

                //std::cout << i << " " <<i+2 << " " << j << std::endl;
            }
        } else {
            for(unsigned int i = end - 4; i > j; i -= 2){
                m_index_object.push_back(i);
                m_index_object.push_back(i+2);
                m_index_object.push_back(j);

                //std::cout << i << " " <<i+2 << " " << j << std::endl;
            }
        }
        //std::cout << std::endl;
        return true;
    }
}

void poc::ParametricObject::linksLayer(unsigned int index){
    unsigned int nb_point = m_configs[index].nbPoint;
    //Si on a 1 seul point
    if(nb_point == 1){
        //Si on est sur le premier layer et qu'on a plus de 1 layer
        if(index == 0 && m_configs.size() > 1){
            for(unsigned int t = 1; t+1 < m_cumulativ_nb_point[index+1]+m_configs[index+1].nbPoint; ++t){
                m_index_object.push_back(0);
                m_index_object.push_back(t);
                m_index_object.push_back(t+1);
            }
            m_index_object.push_back(0);
            m_index_object.push_back(m_cumulativ_nb_point[index+1]+m_configs[index+1].nbPoint-1);
            m_index_object.push_back(1);
        } else {
            //Sinon on prend le layer d'avant et on raccorde
            unsigned int index_before_layer = m_cumulativ_nb_point[index-1];
            for(unsigned int t = index_before_layer; t+1 <= m_cumulativ_nb_point[index]; ++t){
                m_index_object.push_back(t);
                m_index_object.push_back(t+1);
                m_index_object.push_back(m_cumulativ_nb_point[index]);
            }
            m_index_object.push_back(m_cumulativ_nb_point[index] - 1);
            m_index_object.push_back(index_before_layer);
            m_index_object.push_back(m_cumulativ_nb_point[index]);
        }
    } else {
        //Si le layer d'avant a un seul point et il ne s'agit pas du premier layer (déjà raccorder)
        if(index != 0 && index-1 != 0 && m_configs[index-1].nbPoint == 1){
            unsigned int index_layer = m_cumulativ_nb_point[index];
            unsigned int index_layer_before = m_cumulativ_nb_point[index-1];
            for(unsigned int t = index_layer; t+1 < m_cumulativ_nb_point[index] + m_configs[index].nbPoint; ++t){
                m_index_object.push_back(index_layer_before);
                m_index_object.push_back(t);
                m_index_object.push_back(t+1);
            }
            m_index_object.push_back(m_cumulativ_nb_point[index] + m_configs[index].nbPoint - 1);
            m_index_object.push_back(index_layer_before);
            m_index_object.push_back(m_cumulativ_nb_point[index]);
        }
        //On rattache avec le layer d'après
        linkslayerDifferentNumber(index);
    }
}

void poc::ParametricObject::linkslayerDifferentNumber(unsigned int index) {
    if(static_cast<int>(index-1) >= 0 && m_configs[index-1].nbPoint != 1) {
        unsigned int nb_point_first = m_configs[index-1].nbPoint;
        unsigned int nb_point_second = m_configs[index].nbPoint;
        unsigned int index_first_layer = m_cumulativ_nb_point[index-1];
        unsigned int index_second_layer = m_cumulativ_nb_point[index];

        unsigned int nb_links_by_point = nb_point_second/nb_point_first;
        unsigned int nb_point_with_one_more_links = 0;

        bool is_More = false;

        if(nb_point_first > nb_point_second){
            nb_point_first = m_configs[index].nbPoint;
            nb_point_second = m_configs[index-1].nbPoint;

            index_first_layer = m_cumulativ_nb_point[index];
            index_second_layer = m_cumulativ_nb_point[index-1];

            nb_links_by_point = nb_point_second/nb_point_first;
        }

        if(nb_point_second%nb_point_first){
            is_More = 1.0*nb_point_second/nb_point_first - nb_point_second/nb_point_first > 0.5;
            if(!is_More){
                while((nb_point_second - (nb_point_first - nb_point_with_one_more_links++)*nb_links_by_point) % (nb_links_by_point+1));
                --nb_point_with_one_more_links;
            } else {
                if(nb_links_by_point == 1){
                    while(nb_point_second - ((nb_point_first-nb_point_with_one_more_links)*(nb_links_by_point+1) + nb_point_with_one_more_links*nb_links_by_point)){
                        ++nb_point_with_one_more_links;
                    }
                } else {
                    while((nb_point_second - (nb_point_first-nb_point_with_one_more_links++)*(nb_links_by_point+1)) % (nb_links_by_point));
                    --nb_point_with_one_more_links;
                }
            }
        }
        unsigned int save_nb_point_with_one_more_link = nb_point_with_one_more_links;

        unsigned int nb_links = 0;
        unsigned int shortest_point_index = findShortestPointFrom(index_first_layer, index);
        for(unsigned int i = 0; i < nb_point_first; ++i){
            unsigned int nb_links_for_point = is_More?nb_links_by_point+1:nb_links_by_point;
            unsigned int t = is_More?i+1:i;
            if(!(t%2) && nb_point_with_one_more_links){
                is_More?--nb_links_for_point:++nb_links_for_point;
                --nb_point_with_one_more_links;
            }
            unsigned int index_begin = (shortest_point_index - index_second_layer + nb_point_second - nb_links_for_point/2)%nb_point_second + nb_links;
            if(nb_links_for_point > 1){
                for(unsigned int j = 0; j < nb_links_for_point-1; ++j){
                    m_index_object.push_back(i+index_first_layer);
                    m_index_object.push_back(index_second_layer+(j+index_begin)%nb_point_second);
                    m_index_object.push_back(index_second_layer+(j+1+index_begin)%nb_point_second);

                    //std::cout << i + index_first_layer << " " <<j+index_second_layer+nb_links << " " << j+index_second_layer+1+nb_links << std::endl;
                }
            }
            nb_links += nb_links_for_point;
        }

        nb_links = 0;
        for(unsigned int i = 0; i < nb_point_first; ++i){
            unsigned int tmp = is_More?nb_links_by_point+1:nb_links_by_point;
            unsigned int t = is_More?i+1:i;
            if(!(t%2) && save_nb_point_with_one_more_link){
                is_More?--tmp:++tmp;
                --save_nb_point_with_one_more_link;
            }
            if(!i){
                nb_links = (shortest_point_index - index_second_layer
                            - tmp/2 +nb_point_second)%nb_point_second;
            }
            nb_links += tmp;
            m_index_object.push_back(i+index_first_layer);
            m_index_object.push_back(index_second_layer + (nb_links)%nb_point_second);
            m_index_object.push_back(index_second_layer + (nb_links-1)%nb_point_second);

            m_index_object.push_back(i+index_first_layer);
            m_index_object.push_back(index_first_layer+(i+1)%nb_point_first);
            m_index_object.push_back(index_second_layer + (nb_links)%nb_point_second);

            /*std::cout << i + index_first_layer
                      << " " <<index_second_layer + (nb_links-1)%nb_point_second
                      << " " << index_second_layer + (nb_links)%nb_point_second
                      << " " << i+index_first_layer
                      << " " << index_first_layer+(i+1)%nb_point_first
                      << " " << index_second_layer + (nb_links)%nb_point_second
                      << std::endl;*/
        }
    }
    //std::cout << std::endl;
}

//Getter
unsigned long long int poc::ParametricObject::getNbIndexes() const noexcept {
    return m_index_object.size();
}

unsigned int poc::ParametricObject::getNbPoint() const noexcept {
    return static_cast<unsigned int>(m_vertices_object.size()/m_float_vertex);
}

void poc::ParametricObject::configure(const std::vector<LayerConfig>& layerConfigs) {
    m_configs = layerConfigs;
    computeVertices();
    computeIndexes();
}

const std::vector<float>& poc::ParametricObject::get_vertices() const {
    return m_vertices_object;
}

const std::vector<unsigned int>& poc::ParametricObject::get_indexes() const {
    return m_index_object;
}

unsigned int poc::ParametricObject::findShortestPointFrom(unsigned int index, unsigned int index_layer_other) {
    unsigned int nb_point = m_configs[index_layer_other].nbPoint;
    unsigned int index_begin_layer = m_cumulativ_nb_point[index_layer_other];
    unsigned int min_index = 0;
    double min_distance = std::numeric_limits<float>::max();
    for (unsigned int  i = index_begin_layer; i < index_begin_layer +nb_point; ++i){
        const double tmp_distance = pow(static_cast<double>((m_vertices_object[i*m_float_vertex] - m_vertices_object[index*m_float_vertex])),2.0)
                            + pow(static_cast<double>((m_vertices_object[i*m_float_vertex+1] - m_vertices_object[index*m_float_vertex+1])),2.0)
                            + pow(static_cast<double>((m_vertices_object[i*m_float_vertex+2] - m_vertices_object[index*m_float_vertex+2])),2.0);
        if(tmp_distance < min_distance){
            min_distance = tmp_distance;
            min_index = i;
        }
    }
    return min_index;
}


poc::LayerConfig::LayerConfig(unsigned int nbPoint_, float radiusFromCenter_, float distances_with_layer_, float rotation_) noexcept
        : nbPoint(nbPoint_), radiusFromCenter(radiusFromCenter_), distances_with_layer(distances_with_layer_),
          rotation(rotation_) {}
