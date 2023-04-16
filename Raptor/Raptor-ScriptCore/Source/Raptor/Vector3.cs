namespace Raptor
{
	public struct Vector3
	{
		public float X, Y, Z;

		public static Vector3 Zero => new Vector3(0.0f);
		public Vector3(float scaler)
		{
			X = scaler;
			Y = scaler;
			Z = scaler;
		}

		public Vector3(float x, float y, float z)
		{
			X = x;
			Y = y;
			Z = z;
		}

		public Vector3(Vector2 xy, float z)
		{
			X = xy.X;
			Y = xy.Y;
			Z = z;
		}

		public Vector2 XY
		{
			get => new Vector2(X, Y);
			set
			{
				X = value.X;
				Y = value.Y;
			}
		}

		public static Vector3 operator +(Vector3 a, Vector3 b)
		{
			return new Vector3(a.X + b.X, a.Y + b.Y, a.Z + b.Z); 
		}

		public static Vector3 operator *(Vector3 vector, float scaler)
		{
			return new Vector3(vector.X * scaler, vector.Y * scaler, vector.Z * scaler); 
		}
	}
}
