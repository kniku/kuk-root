using System;
using System.Collections.Generic;
using System.Data;
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
xx				throw e;
			}
			return r.ToArray();
		}

		Account IKontoService.GetAccount(int id)
		{
			return new Account(id);
		}

		IEnumerable<AccountPosition> IKontoService.GetPositions(int accountId)
		{
			List<AccountPosition> r = new List<AccountPosition>();
			try
			{
				logger.LogTrace("IKontoService.GetPositions()");
				using (var cmd = AppGlobal.DatabaseAccess.GetCommand("select id, effdt, amt, memo, curcode, posteddt, voucher from positionen where ktoid=:ktoid"))
				{
					DbParameter p = cmd.CreateParameter();
					p.ParameterName = "ktoid";
					p.Direction = ParameterDirection.Input;
					p.Value = accountId;
					cmd.Parameters.Add(p);

					DbDataReader reader = cmd.ExecuteReader();

					if (reader.HasRows)
					{
						while (reader.Read())
						{
							r.Add(new AccountPosition() { Id = new Guid(reader.GetString(0)), Valuta = reader.GetDateTime(1), Amount = reader.GetDouble(2), Memo = reader.GetString(3), Currency = reader.GetString(4), Posted = reader.GetDateTime(5), Voucher = reader.IsDBNull(6) ? null: reader.GetString(6)});
						}
					}
					reader.Close();
				}
				logger.LogTrace($"account positions: {r.Count}");
			}
			catch (Exception e)
			{
				logger.LogError(e, e.Message);
			}
			return r.ToArray();
		}
	}
}
