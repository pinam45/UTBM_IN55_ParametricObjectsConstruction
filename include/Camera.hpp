#ifndef PARAMETRICOBJECTSCONSTRUCTION_CAMERA_HPP
#define PARAMETRICOBJECTSCONSTRUCTION_CAMERA_HPP


#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

class Camera {

public:

	Camera(float FOV, float width, float height, float near, float far);

	void setFOV(float FOV);
	void setWidth(float width);
	void setHeight(float height);
	void setNear(float near);
	void setFar(float far);

	void setPosition(glm::vec3 position);
	void move(glm::vec3 offset);
	void moveForward(float offset);
	void moveUp(float offset);
	void moveRight(float offset);

	void setDirection(glm::quat direction);
	void rotate(glm::quat rotation);
	void lookAt(glm::vec3 position);
	void rotateHorizontally(float angle);
	void rotateVertically(float angle);

	void update();

	glm::vec3 getPosition() const;

	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;
	glm::mat4 getMatrix() const;

	glm::vec3 getForwardVector() const;

	glm::vec3 getRightVector() const;

	glm::vec3 getUpVector() const;

private:

	void updateDirectionsVectors();

	float m_FOV;
	float m_width;
	float m_height;
	float m_near;
	float m_far;

	glm::vec3 m_position;
	glm::quat m_direction;
	glm::vec3 m_forward;
	glm::vec3 m_right;
	glm::vec3 m_up;

	bool m_update_view;
	bool m_update_projection;

	glm::mat4 m_view_matrix;
	glm::mat4 m_projection_matrix;
};


#endif //PARAMETRICOBJECTSCONSTRUCTION_CAMERA_HPP
