namespace KK.Utilities.Multithreading.ConcurrentWorkQueue;

public interface IConcurrentWorkQueue<in T>
{
    void Start();
    void Stop();
    bool Enqueue(T value);
}