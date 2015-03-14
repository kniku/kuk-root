package controller;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.zkoss.zk.ui.Session;
import org.zkoss.zk.ui.Sessions;
import org.zkoss.zk.ui.event.Event;
import org.zkoss.zk.ui.util.GenericForwardComposer;
import org.zkoss.zul.Include;

public class Wartung extends GenericForwardComposer {
	private static final long serialVersionUID = 1L;
	private static final Log log = LogFactory.getLog(Wartung.class);
	Include inc_center;	// auto wired

	public void onCreate(Event event) {
		Session s = Sessions.getCurrent();
		log.debug("Session "+s.hashCode()+": onCreate");
		if (s.getAttribute("LOGIN") == null) {
			this.execution.sendRedirect("login.zul");
		}
	}
	public void onClick$mi_chkMD5(Event event) {
		inc_center.setSrc("zul/CenterMD5.zul");		
	}

	public void onClick$mi_chkBLANKS(Event event) {
		//inc_center.setSrc("zul/CenterBLANKS.zul");
//		Window wnd = (Window)Executions.createComponents("index.zul", null, null);
//        try {
//			wnd.doModal();
//		} catch (Exception e) {
//		}
	}
	
	public void onClick$mi_logout(Event event) throws Exception {
		Session s = Sessions.getCurrent();
		log.debug("Session "+s.hashCode()+": onClick$mi_logout");
		s.setAttribute("LOGIN", null);
		
		if (true) {
			Cookie cookie = new Cookie("login", "");
				cookie.setMaxAge(0);//delete
				String cp = this.execution.getContextPath();
				cookie.setPath(cp);
				((HttpServletResponse)this.execution.getNativeResponse()).addCookie(cookie);
		}
		
		this.execution.sendRedirect("login.zul");
	}

}
