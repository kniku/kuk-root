using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using WebAPI.Model;

namespace WebAPI.Services.Impl
{
    public class KontoService : IKontoService
    {
		string IService.Name => "KontoService";

		IEnumerable<Account> IKontoService.GetAccounts()
		{
			return new[] {new Account(11) {Currency = "EUR", Nr = "x0001", Description = "Hauptkonto"}, new Account(22){Currency = "CHF", Nr = "x0002", Description = "Kreditverrechnungskonto"}};
		}

		Account IKontoService.GetAccount(int id)
		{
			return new Account(id);
		}
	}
}
