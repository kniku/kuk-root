using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace WebAPI.Model
{
    public class Account
    {
	    public int Id { get; }
	    public string Nr { get; set; }
	    public string Currency { get; set; }
	    public string Description { get; set; }

	    public Account(int id)
	    {
		    Id = id;
	    }
    }
}
