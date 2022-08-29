using System;

using Eos;

namespace Sandbox
{
	public class Player : Entity
	{
		private Rigidbody2DComponent m_Rigidbody;

		public float Speed = 0.01f;
		public float Time = 0.0f;

		void OnCreate()
		{
			Console.WriteLine($"Player.OnCreate - {ID}");

			m_Rigidbody = GetComponent<Rigidbody2DComponent>();
		}

		void OnUpdate(float ts)
		{
			Time += ts;

			float speed = Speed;
			Vector3 velocity = Vector3.Zero;

			if (Input.IsKeyDown(KeyCode.W))
				velocity.Y = 1.0f;
			else if (Input.IsKeyDown(KeyCode.S))
				velocity.Y = -1.0f;

			if (Input.IsKeyDown(KeyCode.A))
				velocity.X = -1.0f;
			else if (Input.IsKeyDown(KeyCode.D))
				velocity.X = 1.0f;

			velocity *= speed;

			m_Rigidbody.ApplyLinearImpulse(velocity.XY, true);
		}

	}
}
