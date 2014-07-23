using System;
using System.Runtime.CompilerServices;

public class Lib
{
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    protected extern void NativeFoo();

    public virtual void OnStart()
    {
        Console.WriteLine("Lib Start");
    }

    public virtual void OnInput(string input)
    {
        Console.WriteLine("Lib Got: " + input);
    }

}
