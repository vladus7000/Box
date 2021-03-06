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
			m_up = up;
			m_aspect = aspect;
			m_fov = fov;
			m_forward = target - m_position;
			m_forward.normalize();

			m_right = m_up.cross(m_forward);
			m_right.normalize();

			m_up = m_forward.cross(m_right);
			m_up.normalize();
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
				Vector3D tmp = m_position + m_forward;
				D3DXMatrixLookAtLH(&m_viewMatrix, &m_position, &tmp, &m_up);
			}
		}

		const std::string& getName() const { return m_name; }
		const Matrix4D& getViewMatrix() const { return m_viewMatrix; }
		const Matrix4D& getProjectionMatrix() const { return m_projectionMatrix; }
		const Vector3D& getPosition() const { return m_position; }
		void setPosition(const Vector3D& pos) { m_position = pos; m_viewDirty = true; }
		const Vector3D& getTarget() const { return m_position + m_forward; }
		void setTarget(const Vector3D& target) { m_forward = target - m_position; m_viewDirty = true; }
		const Frustum& getFrustum() const { return m_frustum; }

		void setFov(F32 fov) { m_fov = fov; m_projectionDirty = true; }
		void setZNear(F32 zNear) { m_near = zNear; m_projectionDirty = true; }
		void setZFar(F32 zFar) { m_far = zFar; m_projectionDirty = true; }

		void moveForward(F32 step) {
			m_viewDirty = true;
			m_position += m_forward * step;
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
		}

		void roll(F32 val)
		{
			/*
			rightVector = (rightVector * cos(roll)) + (upVector * sin(roll))
			upVector    = forwardVector x rightVector
			*/
			m_viewDirty = true;

			m_right = m_right * cosf(val) + m_up * sinf(val);
			m_right.normalize();
			m_up = m_forward.cross(m_right);
			m_up.normalize();
		}
		void pitch(F32 val)
		{
			/*
			forwardVector = (forwardVector * cos(pitch)) + (upVector * sin(pitch))
			upVector      = forwardVector x rightVector
			*/
			m_viewDirty = true;

			m_forward = m_forward * cosf(val) + m_up * sinf(val);
			m_forward.normalize();
			m_up = m_forward.cross(m_right);
			m_up.normalize();
		}
		void yaw(F32 val)
		{
			/*
			forwardVector = (forwardVector * cos(yaw)) - (rightVector * sin(yaw))
			rightVector   = forwardVector x upVector
			*/
			m_viewDirty = true;

			m_forward = m_forward * cosf(val) - m_right * sinf(val);
			m_forward.normalize();
			m_right = m_up.cross(m_forward);
			m_right.normalize();
		}

		int getSizeForXML() const
		{

		}

		tinyxml2::XMLNode* serializeToXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc) const
		{
			tinyxml2::XMLElement* root = doc.NewElement("Camera");

			{
				tinyxml2::XMLElement* element = doc.NewElement("Position");
				m_position.serializeToXMLElement(element);
				root->InsertEndChild(element);
			}
			{
				tinyxml2::XMLElement* element = doc.NewElement("Forward");
				m_forward.serializeToXMLElement(element);
				root->InsertEndChild(element);
			}
			{
				tinyxml2::XMLElement* element = doc.NewElement("Right");
				m_right.serializeToXMLElement(element);
				root->InsertEndChild(element);
			}
			{
				tinyxml2::XMLElement* element = doc.NewElement("Up");
				m_up.serializeToXMLElement(element);
				root->InsertEndChild(element);
			}
			root->SetAttribute("Near", m_near);
			root->SetAttribute("Far", m_far);
			root->SetAttribute("Aspect", m_aspect);
			root->SetAttribute("Fov", m_fov);
			root->SetAttribute("Name", m_name.c_str());

			if (node)
			{
				node->InsertEndChild(root);
			}

			return root;
		}

		bool loadFromXML(tinyxml2::XMLNode* node)
		{
			bool ok = false;
			if (auto root = node->ToElement())
			{
				if (strcmp(root->Name(), "Camera") == 0)
				{
					ok = true;
					m_viewDirty = m_projectionDirty = true;
					if (auto element = root->FirstChildElement("Position"))
					{
						m_position.loadFromXMLElement(element);
					}
					if (auto element = root->FirstChildElement("Forward"))
					{
						m_forward.loadFromXMLElement(element);
					}
					if (auto element = root->FirstChildElement("Right"))
					{
						m_right.loadFromXMLElement(element);
					}
					if (auto element = root->FirstChildElement("Up"))
					{
						m_up.loadFromXMLElement(element);
					}

					root->QueryFloatAttribute("Near", &m_near);
					root->QueryFloatAttribute("Far", &m_far);
					root->QueryFloatAttribute("Aspect", &m_aspect);
					root->QueryFloatAttribute("Fov", &m_fov);
					if (const char* name = root->Attribute("Name"))
					{
						m_name = name;
					}
				}
			}
			return ok;
		}

	private:
		bool m_projectionDirty;
		bool m_viewDirty;
		Frustum m_frustum;
		Matrix4D m_viewMatrix;
		Matrix4D m_projectionMatrix;
		Vector3D m_position;
		Vector3D m_forward;
		Vector3D m_right;
		Vector3D m_up;
		F32 m_near;
		F32 m_far;
		F32 m_aspect;
		F32 m_fov;
		std::string m_name;
	};
}
