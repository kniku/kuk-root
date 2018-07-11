using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using WebAPI.Model;

namespace WebAPI.Services
{
	public interface IService
	{
		string Name { get; }
	}

    public interface IKontoService : IService
    {
	    IEnumerable<Account> GetAccounts();
	    Account GetAccount(int id);
    }
}
