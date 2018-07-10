using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using WebAPI.Model;
using WebAPI.Services;

namespace WebAPI.Controllers
{
    [Produces("application/json")]
    [Route("api/Accounts")]
    public class KontoController : Controller
    {
	    private readonly IKontoService KontoService;

	    public KontoController(IKontoService kontoService)
	    {
		    KontoService = kontoService;
	    }

	    [HttpGet]
	    public IEnumerable<Account> Get()
	    {
		    return KontoService.GetAccounts();
	    }

	    [HttpGet("{id}")]
	    public Account Get(int id)
	    {
		    return KontoService.GetAccount(id);
	    }

    }
}