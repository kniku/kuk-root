package controller;

import java.io.InputStream;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Iterator;

import javax.naming.Context;
import javax.naming.InitialContext;
import javax.sql.DataSource;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.zkoss.util.media.Media;
import org.zkoss.zk.ui.Executions;
import org.zkoss.zk.ui.Session;
import org.zkoss.zk.ui.Sessions;
import org.zkoss.zk.ui.event.Event;
import org.zkoss.zk.ui.event.UploadEvent;
import org.zkoss.zk.ui.util.GenericForwardComposer;
import org.zkoss.zul.Button;
import org.zkoss.zul.Checkbox;
import org.zkoss.zul.Combobox;
import org.zkoss.zul.Comboitem;
import org.zkoss.zul.ComboitemRenderer;
import org.zkoss.zul.Intbox;
import org.zkoss.zul.Label;
import org.zkoss.zul.ListModelList;
import org.zkoss.zul.Listbox;
import org.zkoss.zul.Listhead;
import org.zkoss.zul.Listheader;
import org.zkoss.zul.Messagebox;
import org.zkoss.zul.Radiogroup;
import org.zkoss.zul.Textbox;
import org.zkoss.zul.Window;

import shared.AppConstants;
import shared.AppContext;
import shared.DBMSHelper;
import bo.Language;
import bo.ListOfLanguages;
import bo.ListOfTranslationFilter;
import bo.ListOfTranslations;
import bo.Translation;

public class Main extends GenericForwardComposer {
	private static final Log log = LogFactory.getLog(Main.class);
	private static final long serialVersionUID = 1L;
	private Listbox lb_translations;		// auto wired
	private Combobox ef_filter_language;	// auto wired
	private Combobox ef_filter_version;		// auto wired
	private Textbox ef_filter_text;			// auto wired
	private Textbox ef_filter_translation;	// auto wired
	private Radiogroup radio_filter_source;	// auto wired
	private Intbox ef_filter_count;			// auto wired
	private Checkbox chk_filter_instr;		// auto wired
	private Checkbox chk_filter_case;		// auto wired
	private Textbox ef_text;				// auto wired
	private Textbox ef_structure;			// auto wired
	private Label lab_translation;			// auto wired
	private Label lab_database;				// auto wired
	private Textbox ef_translation;			// auto wired
	private Button but_delete;				// auto wired
	private Button but_save;				// auto wired
//	private ListModel ModelListOfTranslations;
	private ListModelTranslations ModelListOfTranslations;
	
//	static boolean isSelectable(Translation iTrans) {
//		boolean r = true;
//		if (iTrans != null) {
//			String l = iTrans.getTr_look();
//			if (l != null && !l.isEmpty() && !l.equals("*")) {
//				r = false;
//			}
//		}
//		return r;
//	}
//
//	class RendererListOfTranslations implements ListitemRenderer{
//		@Override
//		public void render(Listitem item, Object data) throws Exception{
//			Translation T = (Translation )data;
//			if (!isSelectable(T)) item.setStyle("color:#999999;");
//			new Listcell(T.getText()).setParent(item);
//            new Listcell(T.getTranslation()).setParent(item);
//            new Listcell(T.getVersion()).setParent(item);
//            new Listcell(T.getTr_look()).setParent(item);
//		}
//	}
	class RendererListOfLanguages implements ComboitemRenderer {
		@Override
		public void render(Comboitem arg0, Object arg1) throws Exception {
			Language L = (Language )arg1;
			arg0.setLabel(L.getCombo());
		}
	}
	
	public Main() throws Exception {
		super();
		Session s = Sessions.getCurrent();
		log.info("Main:Main - session:"+s.hashCode()+" from "+s.getRemoteAddr());
		Context initContext = new InitialContext();
		Context envContext  = (Context)initContext.lookup("java:/comp/env");
		DataSource ds = (DataSource)envContext.lookup("jdbc/itsmlm");
		AppContext ac = new AppContext();
		ac.setDataSource(ds);
		s.setAttribute(AppConstants.ATTR_APPCONTEXT, ac);

	}
	
	public void onCreate(Event event) throws Exception {
		log.debug("Main:onCreate");
//		ModelListOfTranslations = new ListModelList();
		ModelListOfTranslations = new ListModelTranslations();
		ModelListOfTranslations.initListbox(lb_translations);
//		Listhead hh = new Listhead();
//		hh.setSizable(true);
//		Listheader h = new Listheader("Originaltext");
//		h.setSort("auto");
//		hh.appendChild(h);
//		hh.appendChild(new Listheader("Übersetzung"));
//		h = new Listheader("Version");
//		h.setWidth("65px");
//		hh.appendChild(h);
//		h = new Listheader("L&F");
//		h.setWidth("35px");
//		hh.appendChild(h);
//		lb_translations.appendChild(hh);
//		lb_translations.setModel(ModelListOfTranslations);
//		lb_translations.setItemRenderer(new RendererListOfTranslations());
		
		log.info("loading languages...");
		ListModelList xx = new ListModelList();
		ListOfLanguages ll = new ListOfLanguages();
		AppContext ac = (AppContext )Sessions.getCurrent().getAttribute(AppConstants.ATTR_APPCONTEXT);
		Connection conn = DBMSHelper.getNewConnection((DataSource )ac.getDataSource());
//		conn.setAutoCommit(false);
		ll.reload(conn);
		conn.rollback();
		for (Language L : ll.getList()) {
			xx.add(L);			
		}
		ef_filter_language.setModel(xx);
		ef_filter_language.setItemRenderer(new RendererListOfLanguages());
		if (ll.getList().size() > 1) {
			if (!ll.getList().get(0).getCombo().contains("(de)")) ef_filter_language.setValue(ll.getList().get(0).getCombo());
			else ef_filter_language.setValue(ll.getList().get(1).getCombo());
		}
		
		log.info("loading product versions...");
		PreparedStatement stmt = conn.prepareStatement(
			"select version from mlproduct order by version"
		);
		xx = new ListModelList();
		stmt.execute();
		ResultSet rs = stmt.getResultSet();
		while (rs.next()) {
			xx.add(rs.getString(1));			
		}
		stmt.close();
		ef_filter_version.setModel(xx);

		log.info("checking for mltrans history...");
		try {
			stmt = conn.prepareStatement(
				"select null from mltrans_history"
			);
			stmt.execute();
			ac.setMltransHistoryable(true);
		} catch (Exception e) {
			// just catch it...
		} finally {
			if (stmt != null) stmt.close();
		}
		log.info("mltrans history=" + (ac.isMltransHistoryable() ? "ENABLED" : "DISABLED"));
		
		lab_database.setValue("Datenbank: " + conn.getMetaData().getURL() + " - " + conn.getMetaData().getUserName());
		conn.close();
		
	}
	
	private void disableActionButtons(boolean iDisable) {
		but_delete.setDisabled(iDisable);			
		but_save.setDisabled(iDisable);					
	}
	
	public void onSelect$lb_translations(Event event) {
		log.debug("Main:onSelect$lb_translations");
		Translation T;
		T = (Translation)lb_translations.getModel().getElementAt(lb_translations.getSelectedIndex());
		ef_text.setText(T.getText());
		ef_translation.setText(T.getTranslation());
		ef_structure.setText(
				"MD5:\t\t["+T.getMd5()+"]\n"+
				"Textstruktur:\tModul: ["+T.getModul()+"], Dialog: ["+T.getDialog()+"], Beschreibung: ["+T.getDialogdescr()+"], Element: ["+T.getDialogelement()+"],\n"+
				"Übersetzung:\tmcode: ["+T.getTr_mcode()+"], look: ["+T.getTr_look()+"], App: ["+T.getTr_app()+"], Modul: ["+T.getTr_modul()+"], Dialog: ["+T.getTr_dialog()+"], active: ["+T.getTr_isactive()+"]");
		disableActionButtons(!ListModelTranslations.isSelectable(T));
	}
	
	private void reload(Connection iConn) throws Exception {
		Connection conn = iConn;
		AppContext ac = (AppContext )Sessions.getCurrent().getAttribute(AppConstants.ATTR_APPCONTEXT);
		if (conn == null) {
			conn = DBMSHelper.getNewConnection((DataSource )ac.getDataSource());
		}
		ListOfTranslations xlist = new ListOfTranslations();
		xlist.reload(conn,
				new ListOfTranslationFilter(
						Language.getLanguageFromCombo(ef_filter_language.getValue()),
						ef_filter_version.getValue(),
						ef_filter_text.getValue(),
						ef_filter_translation.getValue(),
						radio_filter_source.getSelectedIndex(),
						chk_filter_case.isChecked(),
						chk_filter_instr.isChecked()
				), ac.isMltransHistoryable()
		);
		ListModelList x = (ListModelList)lb_translations.getModel();
		x.clear();
		int count = 0;
		for (Translation T : xlist.getList()) {
			if (count++ >= ef_filter_count.getValue()) break;
			x.add(T);			
		}
		lb_translations.invalidate();
		if (iConn == null) {
			conn.rollback();
			conn.close();
		}
		lab_translation.setValue("Übersetzung ("+Language.getDescrFromCombo(ef_filter_language.getValue()) + ")");
		for (Iterator<?> iterator = lb_translations.getHeads().iterator(); iterator.hasNext();) {
			Listhead object = (Listhead )iterator.next();
			Listheader h = (Listheader)object.getFirstChild();
			h = (Listheader)h.getNextSibling();
			h.setImage("images/flag_"+Language.getLanguageFromCombo(ef_filter_language.getValue())+".png");
		} 
		disableActionButtons(false);
		ef_text.setText(null);
		ef_translation.setText(null);
	}
	public void onClick$but_refresh(Event event) throws Exception { 
		log.debug("Main:onClick$but_refresh");
		reload(null);
		lb_translations.setSizedByContent(true);
	}

	public void onClick$but_delete(Event event) throws Exception { 
		log.debug("Main:onClick$but_delete");
		if (ef_text.getText().isEmpty()) {
			Messagebox.show("Kein Text selektiert oder das Feld \"Originaltext\" ist leer.", "Info", Messagebox.OK, Messagebox.INFORMATION);
			return;
		}
		int answer = Messagebox.show("Diese Übersetzung wirklich löschen?\n\nKann nicht mehr rückgängig gemacht werden!", "Achtung", Messagebox.OK | Messagebox.CANCEL, Messagebox.QUESTION);
		if (answer == 1) {
			log.debug("Main:onClick$but_deleteOKOK");
			AppContext ac = (AppContext )Sessions.getCurrent().getAttribute(AppConstants.ATTR_APPCONTEXT);
			Translation trans = new Translation(Language.getLanguageFromCombo(ef_filter_language.getText()), ef_text.getText(), ef_translation.getText(), null, Sessions.getCurrent().getRemoteAddr(), ac.isMltransHistoryable());
			Connection conn = DBMSHelper.getNewConnection((DataSource )ac.getDataSource());
			trans.delete(conn);
			reload(conn);
			conn.commit();
			conn.close();
		}
	}
	
	public void onClick$but_save(Event event) throws Exception { 
		log.debug("Main:onClick$but_save");
		if (ef_translation.getText().isEmpty()) {
			onClick$but_delete(event);
			return;
		}
		AppContext ac = (AppContext )Sessions.getCurrent().getAttribute(AppConstants.ATTR_APPCONTEXT);
		Translation trans = new Translation(Language.getLanguageFromCombo(ef_filter_language.getText()), ef_text.getText(), ef_translation.getText(), null, Sessions.getCurrent().getRemoteAddr(), ac.isMltransHistoryable());
		if (log.isDebugEnabled()) {
			log.debug("ef_filter_language:"+ef_filter_language.getValue());
			log.debug("ef_filter_version:"+ef_filter_version.getText());
			log.debug("ef_filter_text:"+ef_filter_text.getText());
			log.debug("lang:"+trans.getLanguage());
			log.debug("text:"+trans.getText());
			log.debug("trans:"+trans.getTranslation());
			log.debug("md5:"+trans.getMd5());
		}
		
		Connection conn = null;
		boolean ok = false;
		try {
			conn = DBMSHelper.getNewConnection((DataSource )ac.getDataSource());
			trans.save(conn);
			ok = true;
		} catch (SQLException e) {
			log.error(e);
		} finally {
			if (conn != null) {
				conn.commit();
				conn.close();
			}
		}
		if (ok) {
			ListModelList x = (ListModelList)lb_translations.getModel();
			int maxidx = x.getSize();
			for (int i = 0; i < maxidx; i++) {
				Translation T = (Translation )x.get(i);
				if (T != null && T.getText().equals(trans.getText())) {
					T.setTranslation(trans.getTranslation());
					T.setMd5(trans.getMd5());
					x.set(i, T);
				}
			}
		} else {
			Messagebox.show("Fehler beim Speichern der Übersetzung! - ask kuk...", "Fehler", Messagebox.OK, Messagebox.ERROR);			
		}
	}

	/*
	public void onClick$but_tradosupload(Event event) throws Exception { 
		log.debug("Main:onClick$tradosupload");
		Window wnd = (Window)Executions.createComponents("trados_upload.zul", null, null);
		wnd.doModal();
	}
	
	public void onClick$popup_listentry_history(Event event) throws Exception {
		log.debug("Main:onClick$popup_listentry_history");
//		Translation T = (Translation )lb_translations.getModel().getElementAt(lb_translations.getSelectedIndex());
		
		log.debug("Main:onClick$popup_listentry_history:"+event.getTarget().getId());
		Window wnd = (Window)Executions.createComponents("hello.zul", null, null);
//      try {
			wnd.doModal();
//		} catch (Exception e) {
//		}
		log.debug("Main:onClick$popup_listentry_history:"+event.getTarget().getId());
	}
*/	
	public void onClick$but_tradosupload(Event event) throws Exception { 
		log.debug("Main:onClick$tradosupload");
		Window wnd = (Window)Executions.createComponents("trados_upload.zul", null, null);
		wnd.doModal();
	}
//	<menuitem label="Attach" upload="true" onUpload="do_something(event)"/>
	public void onUpload$KUKKUK(UploadEvent event) throws Exception {
		log.debug("Main:onUpload$tradosupload");
		Media m = event.getMedia();
		log.debug(m.getName());
		int cnt = 0;
		if (m.isBinary()) {
			InputStream s =m.getStreamData(); 
			while (s.read()>=0) {
				cnt+=1;
			}
			s.close();
		} else {
			String x = m.getStringData();
			cnt = x.length();
		}
		log.debug("read: "+cnt);
	}
	
	public void onClick$popup_ef_trans_history(Event event) throws Exception {
		log.debug("Main:onClick$popup_ef_trans_history");
//		Translation T = (Translation )lb_translations.getModel().getElementAt(lb_translations.getSelectedIndex());
		
		log.debug("Main:onClick$popup_ef_trans_history:"+event.getTarget().getId());
		
		HashMap<String, String> m = new HashMap<String, String>();	
		m.put("text", ef_text.getValue());
		Window wnd = (Window)Executions.createComponents("HistoryView.zul", null, m);
//      try {
			wnd.doModal();
//		} catch (Exception e) {
//		}
//		log.debug("Main:onClick$popup_ef_trans_history:"+event.getTarget().getId());

	}
/*		public void onClick$popup_ef_text_mi_history(Event event) {
			Window wnd = (Window)Executions.createComponents("index.zul", null, null);
		  try {
				wnd.doModal();
			} catch (Exception e) {
		}*/
}
