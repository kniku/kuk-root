using KK.Utilities.Multithreading;

namespace KK.Utilities.Utilities;

public interface ISimpleLogger
{
    void Log(string message);
}

public abstract class SimpleLogger: ISimpleLogger
{
    private readonly SimpleConcurrentWorkQueue<string> _logQueue;

    public static ISimpleLogger Instance { get; private set; } = null!;

    protected SimpleLogger()
    {
        _logQueue = new SimpleConcurrentWorkQueue<string>(LogWriter);
        _logQueue.Start();
        Instance = this;
    }

    protected virtual void LogWriter(string line)
    {
    }

    public void Log(string message)
    {
        _logQueue.Enqueue(message);
    }
}

public class SimpleConsoleLogger : SimpleLogger
{
    protected override void LogWriter(string line)
    {
        try
        {
            Console.WriteLine(line);
        }
        catch
        {
            // TODO ???
        }
    }
}

public class SimpleFileLogger(string fileName) : SimpleLogger
{
    protected override void LogWriter(string line)
    {
        try
        {
            line = $"{DateTime.Now:O} {line}";
            File.AppendAllLines(fileName, [line]);
        }
        catch
        {
            // TODO ???
        }
    }
}