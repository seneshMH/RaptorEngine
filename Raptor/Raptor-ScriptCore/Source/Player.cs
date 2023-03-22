using System;
using Raptor;

namespace Sandbox
{
	public class Player : Entity
	{
		private TransformComponent m_Transform;
		private Rigidbody2DComponent m_RigidBody;

		void OnCreate()
		{
			m_Transform = GetComponent<TransformComponent>(); ;
			m_RigidBody = GetComponent<Rigidbody2DComponent>();

			Console.WriteLine($"Player OnCreate {ID}");
		}

		void OnUpdate(float ts)
		{
			Console.WriteLine($"Player OnUpdate {ts}");

			float speed = 0.1f;
			Vector3 velocity = Vector3.Zero;

			if (Input.IsKeyDown(KeyCode.W))
				velocity.Y = 1.0f;
			else if (Input.IsKeyDown(KeyCode.S))
				velocity.Y = -1.0f;
			else if (Input.IsKeyDown(KeyCode.A))
				velocity.X = -1.0f;
			else if (Input.IsKeyDown(KeyCode.D))
				velocity.X = 1.0f;

			velocity *= speed;
			m_RigidBody.ApplyLinerImpulse(velocity.XY, true);

			Vector3 translation = m_Transform.Translation;
			translation += velocity * ts;
			m_Transform.Translation = translation;
		}
	}
}
