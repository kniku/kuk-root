using KK.Utilities.Utilities;

namespace Tests;

public class SimpleCommandlineParserTest
{
    [Fact]
    void Test()
    {
        var p = new SimpleCommandlineParser(["a=10", "b"]);
        Assert.Equal(-5, p.GetArgument("x", -5));
        Assert.Equal(10, p.GetArgument("a", 0));
        Assert.Equal(10, p.GetArgument<int>("a"));
        Assert.True(p.GetFlag("b"));
        
        Assert.False(p.GetFlag("x"));
        Assert.Null(p.GetArgument("x"));
    }
}