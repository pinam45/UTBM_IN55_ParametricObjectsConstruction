#ifndef PARAMETRICOBJECTSCONSTRUCTION_SPHERICALCAMERA_HPP
#define PARAMETRICOBJECTSCONSTRUCTION_SPHERICALCAMERA_HPP


#include <glm/vec3.hpp>
#include <Camera.hpp>

class SphericalCamera : private Camera {

public:

	using Camera::setFOV;
	using Camera::setWidth;
	using Camera::setHeight;
	using Camera::setNear;
	using Camera::setFar;

	using Camera::getPosition;
	using Camera::getViewMatrix;
	using Camera::getProjectionMatrix;
	using Camera::getMatrix;

	using Camera::update;

	SphericalCamera(
	  float FOV,
	  float width,
	  float height,
	  float near,
	  float far,
	  glm::vec3 center,
	  glm::vec3 initial_position
	);

	void setPosition(glm::vec3 position) override;
	void moveForward(float offset) override;
	void moveRight(float angle) override;
	void moveUp(float angle) override;

private:

	using Camera::move;
	void move(float angle, const glm::vec3& axis);

	glm::vec3 m_center;

};


#endif //PARAMETRICOBJECTSCONSTRUCTION_SPHERICALCAMERA_HPP
