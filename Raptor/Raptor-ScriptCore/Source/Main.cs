using System;
using System.Runtime.CompilerServices;

namespace Raptor
{
	public struct Vector3
	{
		public float X, Y, Z;

		public Vector3(float x, float y, float z)
		{
			X = x;
			Y = y;
			Z = z;
		}
	}

	public static class InrenalCalls
	{
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void NativeLog(string text, int parameter);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void NativeLog_Vector(ref Vector3 parameter, out Vector3 result);
	}

	public class Entity 
	{ 
		public float floatVar { get; set; }

		public Entity()
		{
			Console.WriteLine("Main");
			InrenalCalls.NativeLog("message",34554);

			Vector3 pos = new Vector3(5,2.5f,1);
			InrenalCalls.NativeLog_Vector(ref pos,out Vector3 result);
			Console.WriteLine(result.X);
		}

		public void PrintMessage()
		{
			Console.WriteLine("Hello world from C#");
		}

		public void PrintInt(int value)
		{
			Console.WriteLine($"{value}");

		}

		public void PrintInts(int value1,int value2)
		{
			Console.WriteLine($"{value1} and {value2}");
		}

		public void PrintCustomMessage(string message)
		{
			Console.WriteLine($"{message}");
		}

	

	}
}