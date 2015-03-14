package controller;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.zkoss.zk.ui.Execution;
import org.zkoss.zk.ui.Session;
import org.zkoss.zk.ui.Sessions;
import org.zkoss.zk.ui.event.Event;
import org.zkoss.zk.ui.util.GenericForwardComposer;
import org.zkoss.zul.Textbox;

public class Login extends GenericForwardComposer {
	Log log = LogFactory.getLog(Login.class);
	private static final long serialVersionUID = 1L;
	private Textbox ef_login;	// auto wired


	public static String getLoginCookie(Execution exe) {
		Cookie[] cookies = ((HttpServletRequest)exe.getNativeRequest()).getCookies();
		if(cookies!=null){
			for(int i=0;i<cookies.length;i++){
				if("login".equals(cookies[i].getName())){
					String fs = cookies[i].getValue();
					return fs;
				}
			}
		}
		return null;
	}

	public void onCreate(Event event) {
		Session s = Sessions.getCurrent();
		log.debug("Session "+s.hashCode()+": onCreate");
		if (s.getAttribute("LOGIN") != null) {
			this.execution.sendRedirect("wartung.zul");
		}
		String l = getLoginCookie(this.execution);
		if (l != null) ef_login.setValue(l);
//		else ef_login.setValue(s.getRemoteAddr());
		ef_login.select();
	}
	
	public void onEvent(Event event) throws Exception {
		Session s = Sessions.getCurrent();
		log.debug("Session "+s.hashCode()+": onEvent:"+event.getName());
		super.onEvent(event);
	}
	
	public void onOK(Event event) {
		onClick$but_ok(null);
	}
	
	public void onClick$but_ok(Event event)  {
		Session s = Sessions.getCurrent();
		log.debug("Session "+s.hashCode()+": onClick$but_ok");
		if (ef_login.getText().isEmpty()) ef_login.setValue(s.getRemoteAddr());
		if (!ef_login.getText().isEmpty()) {
			s.setAttribute("LOGIN", ef_login.getText());
			
			Cookie cookie = new Cookie("login", ef_login.getText());
			cookie.setMaxAge(60*60*24*30);//store 30 days
			String cp = this.execution.getContextPath();
			cookie.setPath(cp);
			((HttpServletResponse)this.execution.getNativeResponse()).addCookie(cookie);

			this.execution.sendRedirect("wartung.zul");

		}
	}
}
