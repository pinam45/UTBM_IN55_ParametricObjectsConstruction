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
