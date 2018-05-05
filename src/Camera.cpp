#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"

namespace {
	constexpr glm::quat DEFAULT_DIRECTION(1.0f, 0.0f, 0.0f, 0.0f);
	constexpr glm::vec3 DEFAULT_FORWARD(0.0f, 0.0f, 1.0f);
	constexpr glm::vec3 DEFAULT_RIGHT(1.0f, 0.0f, 0.0f);
	constexpr glm::vec3 DEFAULT_UP(0.0f, 1.0f, 0.0f);
}

poc::Camera::Camera(float FOV, float width, float height, float near, float far)
  : m_FOV(FOV)
    , m_width(width)
    , m_height(height)
    , m_near(near)
    , m_far(far)
    , m_position()
    , m_direction(DEFAULT_DIRECTION)
    , m_forward(DEFAULT_FORWARD)
    , m_right(DEFAULT_RIGHT)
    , m_up(DEFAULT_UP)
    , m_update_view(true)
    , m_update_projection(true)
    , m_view_matrix()
    , m_projection_matrix() {

	update();
}

void poc::Camera::setFOV(float FOV) {
	m_FOV = FOV;
	m_update_projection = true;
}

void poc::Camera::setWidth(float width) {
	m_width = width;
	m_update_projection = true;
}

void poc::Camera::setHeight(float height) {
	m_height = height;
	m_update_projection = true;
}

void poc::Camera::setNear(float near) {
	m_near = near;
	m_update_projection = true;
}

void poc::Camera::setFar(float far) {
	m_far = far;
	m_update_projection = true;
}

void poc::Camera::setPosition(glm::vec3 position) {
	m_position = position;
	m_update_view = true;
}

void poc::Camera::move(glm::vec3 offset) {
	m_position += offset;
	m_update_view = true;
}

void poc::Camera::moveForward(float offset) {
	m_position += offset * m_forward;
	m_update_view = true;
}

void poc::Camera::moveUp(float offset) {
	m_position += offset * m_up;
	m_update_view = true;
}

void poc::Camera::moveRight(float offset) {
	m_position += offset * m_right;
	m_update_view = true;
}

void poc::Camera::setDirection(glm::quat direction) {
	m_direction = direction;
	updateDirectionsVectors();
	m_update_view = true;
}


void poc::Camera::rotate(glm::quat rotation) {
	m_direction *= rotation;
	updateDirectionsVectors();
	m_update_view = true;
}

void poc::Camera::lookAt(glm::vec3 position) {
	const glm::vec3 new_forward(glm::normalize(position - m_position));

	const float dot = glm::dot(DEFAULT_FORWARD, new_forward);

	// Same direction
	if(std::abs(dot - 1.0f) < 0.000001f) {
		setDirection(DEFAULT_DIRECTION);
		return;
	}

	// Opposite direction
	if(std::abs(dot + 1.0f) < 0.000001f) {
		setDirection(glm::angleAxis(glm::pi<float>(), DEFAULT_UP));
		return;
	}

	// General case
	const float angle = -std::acos(dot);
	const glm::vec3 axis = glm::normalize(glm::cross(DEFAULT_FORWARD, new_forward));
	setDirection(glm::angleAxis(angle, axis));
}

void poc::Camera::rotateHorizontally(float angle) {
	rotate(glm::angleAxis(angle, m_up));
}

void poc::Camera::rotateVertically(float angle) {
	rotate(glm::angleAxis(angle, m_right));
}

void poc::Camera::update() {
	if(m_update_view) {
		const glm::mat4 translate = glm::translate(glm::mat4(1.0f), m_position);
		const glm::mat4 rotate = glm::mat4_cast(m_direction);
		m_view_matrix = rotate * translate;

		m_update_view = false;
	}

	if(m_update_projection) {
		m_projection_matrix = glm::perspective(
		  m_FOV,
		  m_width / m_height,
		  m_near,
		  m_far
		);

		m_update_projection = false;
	}
}

glm::vec3 poc::Camera::getPosition() const {
	return m_position;
}

glm::mat4 poc::Camera::getViewMatrix() const {
	return m_view_matrix;
}

glm::mat4 poc::Camera::getProjectionMatrix() const {
	return m_projection_matrix;
}

glm::mat4 poc::Camera::getMatrix() const {
	return m_projection_matrix * m_view_matrix;
}

glm::vec3 poc::Camera::getForwardVector() const {
	return m_forward;
}

glm::vec3 poc::Camera::getRightVector() const {
	return m_right;
}

glm::vec3 poc::Camera::getUpVector() const {
	return m_up;
}

void poc::Camera::updateDirectionsVectors() {
	m_forward = glm::normalize(DEFAULT_FORWARD * m_direction);
	m_right = glm::normalize(DEFAULT_RIGHT * m_direction);
	m_up = glm::normalize(DEFAULT_UP * m_direction);
}
