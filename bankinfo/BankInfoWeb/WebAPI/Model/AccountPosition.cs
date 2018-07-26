using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace WebAPI.Model
{
    public class AccountPosition
    {
	    public Guid Id { get; set; }
	    public double Amount { get; set; }
	    public DateTime Valuta { get; set; }
	    public DateTime Posted { get; set; }
	    public string Currency { get; set; }
	    public string Memo { get; set; }
	    public string Voucher { get; set; }
    }
}
