using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using WebAPI.Model;

namespace WebAPI.Services.Impl
{
    public class KontoService : IKontoService
    {
	    public IEnumerable<Account> GetAccounts()
	    {
		    return new[] {new Account(11), new Account(22)};
	    }

	    public Account GetAccount(int id)
	    {
		    return new Account(id);
	    }
    }
}
