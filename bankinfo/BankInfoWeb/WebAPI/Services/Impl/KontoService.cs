using System;
using System.Collections.Generic;
using System.Data.Common;
using System.Data.SqlClient;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using WebAPI.Model;

namespace WebAPI.Services.Impl
{
    public class KontoService : IKontoService
    {
	    private ILogger logger = AppGlobal.Logging.CreateLogger<KontoService>();

	    string IService.Name => "KontoService";

		IEnumerable<Account> IKontoService.GetAccounts()
		{
			List<Account> r = new List<Account>();
			try
			{
				logger.LogTrace("IKontoService.GetAccounts()");
				using (var cmd = AppGlobal.DatabaseAccess.GetCommand("select id,blz,ktonr,curcode,ktoname,ktodescr from konten"))
				{
					DbDataReader reader = cmd.ExecuteReader();

					if (reader.HasRows)
					{
						while (reader.Read())
						{
							r.Add(new Account(reader.GetInt32(0)) { Nr = reader["ktonr"].ToString(), Currency = reader["curcode"].ToString(), Description = reader["ktodescr"].ToString(), Name =  reader["ktoname"].ToString(), BIC = reader["blz"].ToString()});
						}
					}
					reader.Close();
				}
				logger.LogTrace($"accounts: {r.Count}");
			}
			catch (Exception e)
			{
				logger.LogError(e, e.Message);
			}
			return r.ToArray();
		}

		Account IKontoService.GetAccount(int id)
		{
			return new Account(id);
		}
	}
}
