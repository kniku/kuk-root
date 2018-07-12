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
			logger.LogTrace("Get()");
			return KontoService.GetAccounts();
		}

		[HttpGet("{id}")]
	    public Account Get(int id)
	    {
		    logger.LogTrace($"Get({id})");
		    return KontoService.GetAccount(id);
	    }

    }
}