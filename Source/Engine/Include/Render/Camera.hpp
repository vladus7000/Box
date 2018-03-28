#pragma once

#include <string>
#include <memory>
#include "Math/Frustum.hpp"
#include "Math/Vector3D.hpp"
#include "Math/Matrix4D.hpp"

namespace box
{
	class Camera
	{
	public:
		using CameraStrongPtr = std::shared_ptr<Camera>;
		using CameraWeakPtr = std::weak_ptr<Camera>;

		Camera(const std::string& name = "default")
			: m_projectionDirty(true)
			, m_viewDirty(true)
			, m_name(name)
		{}
		virtual ~Camera() = default;

		void initialize(F32 zNear, F32 zFar, F32 aspect, F32 fov, const Vector3D& pos, const Vector3D& target, const Vector3D& up)
		{
			m_near = zNear;
			m_far = zFar;
			m_position = pos;
			m_target = target;
			m_up = up;
			m_aspect = aspect;
			m_fov = fov;
			m_forvard = m_target - m_position;
			m_forvard.normalize();

			m_right = m_up.cross(m_forvard);
			m_right.normalize();
		}

		virtual void update(F32 delta)
		{
			if (m_projectionDirty)
			{
				m_projectionDirty = false;
				D3DXMatrixIdentity(&m_projectionMatrix);
				D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, m_fov, m_aspect, m_near, m_far);
			}
			if (m_viewDirty)
			{
				m_viewDirty = false;
				D3DXMatrixLookAtLH(&m_viewMatrix, &m_position, &m_target, &m_up);
			}
		}

		const std::string& getName() const { return m_name; }
		const Matrix4D& getViewMatrix() const { return m_viewMatrix; }
		const Matrix4D& getProjectionMatrix() const { return m_projectionMatrix; }
		const Vector3D& getPosition() const { return m_position; }
		void setPosition(const Vector3D& pos) { m_position = pos; m_viewDirty = true; }
		const Vector3D& getTarget() const { return m_target; }
		void setTarget(const Vector3D& target) { m_target = target; m_viewDirty = true; }
		const Frustum& getFrustum() const { return m_frustum; }

		void setFov(F32 fov) { m_fov = fov; m_projectionDirty = true; }
		void setZNear(F32 zNear) { m_near = zNear; m_projectionDirty = true; }
		void setZFar(F32 zFar) { m_far = zFar; m_projectionDirty = true; }

		void moveForward(F32 step) {
			m_viewDirty = true;
			m_position += m_forvard * step;
			m_target += m_forvard * step;
		};
		void moveBackward(F32 step) { moveForward(-step); };

		void moveLeft(F32 step)
		{
			moveRight(-step);
		}

		void moveRight(F32 step)
		{
			m_viewDirty = true;
			m_position += m_right * step;
			m_target += m_right * step;
		}

	private:
		bool m_projectionDirty;
		bool m_viewDirty;
		Frustum m_frustum;
		Matrix4D m_viewMatrix;
		Matrix4D m_projectionMatrix;
		Vector3D m_position;
		Vector3D m_target;
		Vector3D m_forvard;
		Vector3D m_right;
		Vector3D m_up;
		F32 m_near;
		F32 m_far;
		F32 m_aspect;
		F32 m_fov;
		std::string m_name;
	};
}
