using System;

public class App : Lib
{
    public override void OnInput(string input)
    {
        this.NativeFoo();
        Console.WriteLine();
        Console.WriteLine("App Got: " + input);
    }
}