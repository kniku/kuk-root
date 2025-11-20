using System.Collections.Concurrent;
using System.Diagnostics;

namespace KK.Utilities.Multithreading;

public class SimpleConcurrentWorkQueue<T>(Action<T> action, int boundedCapacity = -1, long delayMilliseconds = 0)
{
    private readonly Stopwatch _stopwatch = Stopwatch.StartNew();
    private BlockingCollection<T> bc = new(boundedCapacity > 0 ? boundedCapacity : -1); // => uses ConcurrentQueue<T>
    private CancellationTokenSource? cancellationTokenSource;
    private Task? taskConsumer;
    private int isRunning;
    private readonly long _delayMilliseconds = delayMilliseconds > 0 ?  delayMilliseconds : 0;

    public void Start()
    {
        Stop();

        cancellationTokenSource =  new CancellationTokenSource();

        taskConsumer = Task.Run(async () =>
        {
            while(!bc.IsCompleted)
            {
                var item = bc.Take(cancellationTokenSource.Token);

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
        }, cancellationTokenSource.Token);

        Interlocked.Exchange(ref isRunning, 1);
    }

    public void Stop()
    {
        Interlocked.Exchange(ref isRunning, 0);
        _stopwatch.Stop();
        cancellationTokenSource?.Cancel();
        cancellationTokenSource?.Dispose();
        taskConsumer = null;
    }
    
    public bool Enqueue(T value)
    {
        var r = false;
        if (Interlocked.CompareExchange(ref isRunning, 1, 1) == 1)
            r = bc.TryAdd(value);

        return r;
    }
    // public Task<TOut> EnqueueTask<TIn, TOut>(Func<TIn, TOut> func)
    // {
    //     bc.Add(func);
    // }
}