package test.ecoserv.plugins;

import clserv.shared.I_Employee;
import clserv.shared.I_EmployeeManager;

public class Mock_EmployeeManager implements I_EmployeeManager, I_Employee {

	@Override
	public I_Employee findeMitarbeiterMitLogin(String login) {
		return this;
	}

	@Override
	public I_Employee findeMitarbeiterMitNamen(String vorname, String nachname) {
		return null;
	}

	@Override
	public String getFirstName() {
		return "firstname";
	}

	@Override
	public String getName() {
		return "name";
	}

}
