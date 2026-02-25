using System.Collections.Concurrent;

namespace KK.Utilities.Utilities;

public static class GlobalObjectProvider
{
    private static readonly ConcurrentDictionary<string, object> DictObjects = new ConcurrentDictionary<string, object>();

    private static string GetKey<T>()
    {
        return typeof(T).FullName ?? typeof(T).Name;
    }

    private static string GetKey(string name)
    {
        return $"$${name}";
    }

    public static void AddSingletonObject<T>(T objectToRegister) where T : class
    {
        if (objectToRegister == null)
            throw new ArgumentNullException(nameof(objectToRegister));

        DictObjects[GetKey<T>()] = objectToRegister;
    }

    public static void AddSingletonObject<TIf, T>(T objectToRegister) where T : class where TIf : class
    {
        AddSingletonObject((objectToRegister as TIf)!);
    }

    public static void AddSingletonObject(string name, object objectToRegister)
    {
        if (objectToRegister == null)
            throw new ArgumentNullException(nameof(objectToRegister));

        DictObjects[GetKey(name)] = objectToRegister;
    }
    
    public static T GetSingletonObject<T>()
    {
        return (T) DictObjects[GetKey<T>()];
    }

    public static object GetSingletonObject(string name)
    {
        return DictObjects[GetKey(name)];
    }
}