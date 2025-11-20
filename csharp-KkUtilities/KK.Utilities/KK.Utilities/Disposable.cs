using KK.Utilities.Extensions;

namespace KK.Utilities;

/// <summary>
/// A disposable object that invokes a <see cref="T:System.Action" /> within.
/// </summary>
public class Disposable : IDisposable
{
    private bool _disposed;
    private Action? _method;

    /// <summary>
    /// Initializes a new instance of the <see cref="T:AX.Service.WebBuilder.Utils.Disposable" /> class.
    /// </summary>
    /// <param name="method">The method to be invokes on <see cref="M:AX.Service.WebBuilder.Utils.Disposable.Dispose" />.</param>
    public Disposable(Action? method) => _method = method;

    /// <summary>
    /// Gets a value indicating whether this instance is disposed.
    /// </summary>
    /// <value>
    /// 	<c>true</c> if this instance is disposed; otherwise, <c>false</c>.
    /// </value>
    public bool IsDisposed => _disposed;

    /// <summary>
    /// Releases unmanaged resources and performs other cleanup operations before the
    /// <see cref="T:AX.Service.WebBuilder.Utils.Disposable" /> is reclaimed by garbage collection.
    /// </summary>
    ~Disposable() => Dispose(false);

    /// <summary>Do nothing</summary>
    public void Dispose()
    {
        Dispose(true);
        GC.SuppressFinalize(this);
    }

    /// <summary>
    /// Releases unmanaged and - optionally - managed resources
    /// </summary>
    /// <param name="disposing"><c>true</c> to release both managed and unmanaged resources; <c>false</c> to release only unmanaged resources.</param>
    protected virtual void Dispose(bool disposing)
    {
        _disposed = true;
        if (disposing && _method != null)
        {
            _method.InvokeSave();
        }

        _method = null;
    }
}