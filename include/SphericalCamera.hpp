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

#ifndef PARAMETRICOBJECTSCONSTRUCTION_SPHERICALCAMERA_HPP
#define PARAMETRICOBJECTSCONSTRUCTION_SPHERICALCAMERA_HPP

#include <glm/vec3.hpp>

#include "Camera.hpp"

namespace poc {
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
}

#endif //PARAMETRICOBJECTSCONSTRUCTION_SPHERICALCAMERA_HPP
