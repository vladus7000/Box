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
			: m_dirty(true)
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
		}

		virtual void update(F32 delta)
		{
			if (m_dirty)
			{
				m_dirty = false;
				D3DXMatrixLookAtLH(&m_viewMatrix, &m_position, &m_target, &m_up);
				D3DXMatrixIdentity(&m_projectionMatrix);
				D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, m_fov, m_aspect, m_near, m_far);
			}
		}

		const std::string& getName() const { return m_name; }
		const Matrix4D& getViewMatrix() const { return m_viewMatrix; }
		const Matrix4D& getProjectionMatrix() const { return m_projectionMatrix; }
		const Vector3D& getPosition() const { return m_position; }
		const Vector3D& getTarget() const { return m_target; }
		const Frustum& getFrustum() const { return m_frustum; }

	private:
		bool m_dirty;
		Frustum m_frustum;
		Matrix4D m_viewMatrix;
		Matrix4D m_projectionMatrix;
		Vector3D m_position;
		Vector3D m_target;
		Vector3D m_up;
		F32 m_near;
		F32 m_far;
		F32 m_aspect;
		F32 m_fov;
		std::string m_name;
	};
}
