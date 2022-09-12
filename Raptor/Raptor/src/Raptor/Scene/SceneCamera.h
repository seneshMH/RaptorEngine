#pragma once

#include "Raptor/Renderer/Camera.h"

namespace Raptor {
	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t heigh);

		float GetOrthographicsize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; ReCalculateProjection(); }
	private:
		void ReCalculateProjection();
	private:
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		float m_AscpectRatio = 0.0f;
	};
}
