
namespace DCS.ManagedBeans
{
	/// <summary>
	/// Interface einer empfangenden Instanz
	/// </summary>
	interface IReceivingBean
	{
		void handleData(ICallingBean iCaller, params object[] iArgs);
	}

	/// <summary>
	/// Interface einer rufenden Instanz
	/// </summary>
	interface ICallingBean
	{
		string getName();
		string getDescription();
		BeanState getState();
		void setLogger(DCS.Logger.ILogger iLogger);
		DCS.Logger.ILogger getLogger();
		void addReceivingBean(IReceivingBean iBean);
//		void sendDataToAllReceivers(object iData);
	}

	public enum BeanState { none, initialized, running, stopped, error };
	/// <summary>
	/// Interface einer managebaren Instanz.
	/// Ist gleichzeitig auch immer eine rufende Instanz.
	/// </summary>
	interface IManagedBean : ICallingBean
	{
		void init();
		void start();
		void stop();
		void done();

		/// <summary>
		/// Der working-Callback.
		/// AbstractManagedBean: Vom Manager-Thread zyklisch aufgerufen
		/// AbstractManagedThreadedBean: Vom eigenen Thread in definierten Intervallen aufgerufen.
		/// </summary>
		void beanRun();
	}

}
