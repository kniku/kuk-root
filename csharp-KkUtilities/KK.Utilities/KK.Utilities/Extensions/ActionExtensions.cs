namespace KK.Utilities.Extensions;

public static class ActionExtensions
{
    public static void InvokeSave(this Action? action)
    {
        try
        {
            action?.Invoke();
        }
        catch { /* just catch it */ }
    }
}