using System;
using Raptor;

namespace Sandbox
{
	public class Player : Entity
	{
		private TransformComponent m_Transform;
		private Rigidbody2DComponent m_RigidBody;

		public float Speed;
		public float Time = 0.0f;

		void OnCreate()
		{
			m_Transform = GetComponent<TransformComponent>(); ;
			m_RigidBody = GetComponent<Rigidbody2DComponent>();

			//Speed = 0.25f;

			Console.WriteLine($"Player OnCreate {ID}");
		}

		void OnUpdate(float ts)
		{
			Time += ts;
			Console.WriteLine($"Player OnUpdate {ts}");

			float speed = Speed;
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
