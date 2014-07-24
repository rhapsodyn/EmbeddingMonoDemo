using System;
using System.IO;
using System.Linq;
using System.Reflection;

namespace Framework
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			var appType = typeof(App.App);

			var onStartMethod = appType.GetMethod ("OnStart", BindingFlags.NonPublic | BindingFlags.Instance);
			var onInputMethod = appType.GetMethod ("OnInput", BindingFlags.NonPublic | BindingFlags.Instance);

			var appInstance = Activator.CreateInstance (appType);
			//call start
			onStartMethod.Invoke (appInstance, new object[]{ });

			string line;
			while ((line = Console.ReadLine ()) != "666") {
				//call every input
				onInputMethod.Invoke (appInstance, new object[] { line });
			}
		}
	}
}
