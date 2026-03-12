namespace KK.Utilities.Multithreading.ConcurrentWorkQueue;

public class ConcurrentWorkQueueFactory
{
    public static IConcurrentWorkQueue<T> Create<T>(Action<T> action, int boundedCapacity = -1, long delayMilliseconds = 0)
    {
        return new SimpleConcurrentWorkQueue<T>(action, boundedCapacity, delayMilliseconds);        
    }
}