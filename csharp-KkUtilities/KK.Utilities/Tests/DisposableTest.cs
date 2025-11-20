using KK.Utilities;

namespace Tests;

public class DisposableTest
{
    [Fact]
    public void DisposingMethod_ShouldBeExecuted()
    {
        int testValue = 10;
        var myDisposable = new Disposable(() => testValue = 0);

        Assert.False(myDisposable.IsDisposed);
        
        using (myDisposable)
        {
            testValue++;
        }

        Assert.True(myDisposable.IsDisposed);
        Assert.Equal(0, testValue);
    }

    [Fact]
    public void DisposingMethod_ExceptionShouldBeHandled()
    {
        int testValue = 10;
        using (new Disposable(() =>
               {
                   testValue = 0;
                   var x = 10 / testValue;  // force exception
               }))
        {
            testValue++;
        }
        Assert.Equal(0, testValue);
    }
}