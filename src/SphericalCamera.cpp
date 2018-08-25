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

#include <glm/gtc/matrix_transform.hpp>

#include "SphericalCamera.hpp"

poc::SphericalCamera::SphericalCamera(
  float FOV,
  float width,
  float height,
  float near,
  float far,
  glm::vec3 center,
  glm::vec3 initial_position)
  : Camera(FOV, width, height, near, far)
  , m_center(center) {

	Camera::setPosition(initial_position);
	update();
}

void poc::SphericalCamera::setPosition(glm::vec3 position) {
	Camera::setPosition(position);
	lookAt(m_center);
}

void poc::SphericalCamera::moveForward(float offset) {
	if(glm::dot(m_center - (m_position + offset * m_forward), m_center - m_position) > 0) {
		Camera::moveForward(offset);
	}
}

void poc::SphericalCamera::moveRight(float angle) {
	move(angle, m_up);
}

void poc::SphericalCamera::moveUp(float angle) {
	move(angle, m_right);
}

void poc::SphericalCamera::move(float angle, const glm::vec3& axis) {
	glm::quat rot = glm::angleAxis(angle, axis);
	m_position -= m_center;
	m_position = rot * m_position;
	m_position += m_center;

	lookAt(m_center);
	updateDirectionsVectors();
	m_update_view = true;
}
