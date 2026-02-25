using KK.Utilities.Utilities;

namespace Tests;

public class SimpleLoggerTest
{
    [Fact]
    public void TestSimpleLogger()
    {
        var logger = new SimpleConsoleLogger();
        logger.Log("Hello World!");
        logger.Log("Hello World!");
        logger.Log("Hello World!");
        logger.Log("Hello World!");
        // TODO Assert what?
    }    

    [Fact]
    public async Task TestSimpleFileLogger()
    {
        var logFile = Path.GetTempFileName();
        _ = new SimpleConsoleLogger();
        SimpleLogger.Instance.Log("Hello World!");  // console
        _ = new SimpleFileLogger(logFile);
        SimpleLogger.Instance.Log("Hello World1!");  // file
        SimpleLogger.Instance.Log("Hello World2!");  // file
        SimpleLogger.Instance.Log("Hello World3!");  // file
        SimpleLogger.Instance.Log("Hello World4!");  // file
        
        Assert.True(File.Exists(logFile));
        await Task.Delay(TimeSpan.FromSeconds(1));
        var content = await File.ReadAllTextAsync(logFile);
        File.Delete(logFile);
        Assert.Equal(196, content.Length);
    }    
}