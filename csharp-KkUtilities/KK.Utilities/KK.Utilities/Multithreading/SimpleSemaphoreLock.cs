namespace KK.Utilities.Multithreading;

public class SimpleSemaphoreLock
{
    private readonly SemaphoreSlim _lock = new SemaphoreSlim(1);
    private int _threadId = -1;

    public IDisposable EnterLock()
    {
        lock (_lock)
        {
            var ti = Environment.CurrentManagedThreadId; 
            if (ti == _threadId)
                return new Disposable(() => { });

            _lock.Wait();
            _threadId = ti;
        }
        return new Disposable(() =>
        {
            _threadId = -1;
            _lock.Release();
        });
    }

    public async Task<Disposable> EnterLockAsync()
    {
        await _lock.WaitAsync();
        return new Disposable(() => _lock.Release());
    }
}
