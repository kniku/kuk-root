using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using WebAPI.Model;
using WebAPI.Services;

namespace WebAPI.Controllers
{
	public class RequestErrorHandler : IDisposable
	{
		private bool disposed;
		protected virtual void Dispose(bool disposing)
		{
			if (!disposed)
			{
				disposed = true;
				if (disposing)
				{
				}
			}
		}

		public void Dispose()
		{
			Dispose(true);
			GC.SuppressFinalize(this);
		}

		public void Handle(Action callback)
		{
			callback();
		}
	}


    [Produces("application/json")]
    [Route("api/v1/Accounts")]
    public class KontoController : Controller
    {
	    private readonly IKontoService KontoService;
	    private ILogger logger = AppGlobal.Logging.CreateLogger<KontoController>();

	    public KontoController(IKontoService kontoService)
	    {
		    KontoService = kontoService;
			logger.LogTrace($"KontoController({KontoService.Name})");
	    }

	    [HttpGet]
		//public ActionResult<IEnumerable<Account>> Get()
		//{
		// logger.LogTrace("Get()");
		// return KontoService.GetAccounts() as ActionResult<IEnumerable<Account>>;
		//}
		public IEnumerable<Account> Get()
		{
			IEnumerable<Account> r = null;
			using (var x = new RequestErrorHandler())
			{
xxx				x.Handle(() =>
				{
					logger.LogTrace("Get()");
					r = KontoService.GetAccounts();
				});
			}

			return r;
		}

		[HttpGet("{id}")]
	    public Account Get(int id)
	    {
		    logger.LogTrace($"Get({id})");
		    return KontoService.GetAccount(id);
	    }

	    [HttpGet("positions/{accountId}")]
		//[Route("Buchungen")]
	    public IEnumerable<AccountPosition> GetBuchungen(int accountId)
	    {
		    logger.LogTrace("GetBuchungen()");
		    return KontoService.GetPositions(accountId);
	    }

    }
}