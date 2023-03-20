using System;

namespace Raptor
{
	public class Main
	{
		public float floatVar { get; set; }

		public Main()
		{
			Console.WriteLine("Main");
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