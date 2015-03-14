
namespace DCS.Configurator
{
	public interface IConfigurator
	{
		IConfigurator loadConfig();

		bool getParameter(string iParamName, bool iDefault);
		int getParameter(string iParamName, int iDefault);
		double getParameter(string iParamName, double iDefault);
		string getParameter(string iParamName, string iDefault);
	}
}
