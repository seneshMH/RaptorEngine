namespace Raptor
{
	public struct Vector2
	{
		public float X, Y;

		public static Vector2 Zero => new Vector2(0.0f);
		public Vector2(float scaler)
		{
			X = scaler;
			Y = scaler;
		}

		public Vector2(float x, float y)
		{
			X = x;
			Y = y;
		}

		public static Vector2 operator +(Vector2 a, Vector2 b)
		{
			return new Vector2(a.X + b.X, a.Y + b.Y);
		}

		public static Vector2 operator *(Vector2 vector, float scaler)
		{
			return new Vector2(vector.X * scaler, vector.Y * scaler); 
		}
	}
}
