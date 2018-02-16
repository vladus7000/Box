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
			: m_name(name)
		{}
		virtual ~Camera() = default;

		void initialize(F32 near, F32 far, F32 aspect, F32 fov, const Vector3D& pos, const Vector3D& target)
		{

		}

		virtual void update(F32 delta) {}

		const std::string& getName() const { return m_name; }
		const Matrix4D& getMatrix() const { return m_cameraMatrix; }
		const Frustum& getFrustum() const { return m_frustum; }
	private:
		Frustum m_frustum;
		Matrix4D m_cameraMatrix;
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
