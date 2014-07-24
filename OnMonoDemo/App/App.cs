using System;

namespace App
{
	public class App : Lib.Lib
	{
		protected void OnInput (string input)
		{
			Console.WriteLine ("App got:" + input);
		}
	}
}

