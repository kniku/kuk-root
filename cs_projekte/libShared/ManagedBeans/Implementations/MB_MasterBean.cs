using System;
using System.Collections;
using System.Linq;
using System.Text;

namespace knk.shared.ManagedBeans.Implementations
{
	/// <summary>
	/// Master Bean: Dient nur dazu, alle Beans im Container zyklisch aufzurufen,
	/// sofern sie nicht über einen eigenen Thread verfügen.
	/// </summary>
	class MB_MasterBean : AbstractManagedThreadedBean
	{
		ArrayList listBeans = new ArrayList();	// Liste der Beans ohne eigenen Thread

		public override void init()
		{
			base.init();
			foreach (IManagedBean aBean in BeanManager.instance.Beans.Values)
			{
				if (!(aBean is AbstractManagedThreadedBean))
				{
					listBeans.Add(aBean);
				}
			}

		}

		protected override int getTimeoutMS()
		{
			return 100;
		}

		public override string getName()
		{
			return "Master Bean";
		}

		public override string getDescription()
		{
			return "Globale Bean für alle Beans ohne eigenen Thread";
		}

		public override void beanRun()
		{
			// Wenn nicht zumindest ein Bean in der Liste, die eigene
			// Verarbeitung stoppen, da unnötig...
			if (listBeans.Count < 1)
			{
				logger.pushPrefix(getName()).LogDebug(": nothing to do...").popPrefix();
				stop();
			}

			// Alle Beans zyklisch aufrufen...
			foreach (IManagedBean aBean in listBeans)
			{
				aBean.beanRun();
			}
		}
	}
}
