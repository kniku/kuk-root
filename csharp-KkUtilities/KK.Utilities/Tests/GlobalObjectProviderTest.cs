using KK.Utilities.Utilities;

namespace Tests;

public class GlobalObjectProviderTest
{
    [Fact]
    public void Test()
    {
        var x = new SimpleConsoleLogger();
        GlobalObjectProvider.AddSingletonObject(x);
        var y = new SimpleFileLogger("xxx");
        GlobalObjectProvider.AddSingletonObject(y);
        GlobalObjectProvider.AddSingletonObject<ISimpleLogger>(y);
        GlobalObjectProvider.AddSingletonObject("test", 1);
        GlobalObjectProvider.AddSingletonObject("test", 2);
        
        var xx = GlobalObjectProvider.GetSingletonObject<SimpleFileLogger>();
        Assert.IsType<SimpleFileLogger>(xx);
        var yy = GlobalObjectProvider.GetSingletonObject<SimpleConsoleLogger>();
        Assert.IsType<SimpleConsoleLogger>(yy);
        var zz = GlobalObjectProvider.GetSingletonObject<ISimpleLogger>();
        Assert.IsType<SimpleFileLogger>(zz);
        var zzz = GlobalObjectProvider.GetSingletonObject("test");
        Assert.IsType<int>(zzz);
        Assert.Equal(zzz, 2);
    }
}