using System.Collections.Concurrent;
using System.Diagnostics;

namespace KK.Utilities.Multithreading.ConcurrentWorkQueue;

public class SimpleConcurrentWorkQueue<T>(Action<T> action, int boundedCapacity = -1, long delayMilliseconds = 0)
    : IConcurrentWorkQueue<T>
{
    private readonly Stopwatch _stopwatch = Stopwatch.StartNew();

    private readonly BlockingCollection<T> _bc = boundedCapacity > 0
        ? new BlockingCollection<T>(boundedCapacity)
        : new BlockingCollection<T>(); // => uses ConcurrentQueue<T>
    private CancellationTokenSource? _cancellationTokenSource;
    private int _isRunning;
    private readonly long _delayMilliseconds = delayMilliseconds > 0 ?  delayMilliseconds : 0;

    public void Start()
    {
        Stop();

        _cancellationTokenSource =  new CancellationTokenSource();

        Task.Run(async () =>
        {
            while(!_bc.IsCompleted)
            {
                var item = _bc.Take(_cancellationTokenSource.Token);

                if (_delayMilliseconds - _stopwatch.ElapsedMilliseconds is var delay and > 0)
                {
                    await Task.Delay(TimeSpan.FromMilliseconds(delay));
                }
                _stopwatch.Restart();

                try
                {
                    action.Invoke(item);
                }
                catch (OperationCanceledException)
                {
                    /* just catch it*/
                }
                catch
                {
                    /* just catch it*/
                }
            }
        }, _cancellationTokenSource.Token);

        Interlocked.Exchange(ref _isRunning, 1);
    }

    public void Stop()
    {
        Interlocked.Exchange(ref _isRunning, 0);
        _stopwatch.Stop();
        _cancellationTokenSource?.Cancel();
        _cancellationTokenSource?.Dispose();
    }
    
    public bool Enqueue(T value)
    {
        var r = false;
        if (Interlocked.CompareExchange(ref _isRunning, 1, 1) == 1)
            r = _bc.TryAdd(value);

        return r;
    }
}