package controller;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.util.Vector;

import javax.sql.DataSource;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.zkoss.util.media.Media;
import org.zkoss.zk.ui.Sessions;
import org.zkoss.zk.ui.event.Event;
import org.zkoss.zk.ui.event.UploadEvent;
import org.zkoss.zk.ui.util.GenericForwardComposer;
import org.zkoss.zul.Button;
import org.zkoss.zul.Combobox;
import org.zkoss.zul.Listbox;
import org.zkoss.zul.Listcell;
import org.zkoss.zul.Listhead;
import org.zkoss.zul.Listheader;
import org.zkoss.zul.Listitem;
import org.zkoss.zul.Messagebox;
import org.zkoss.zul.Textbox;
import org.zkoss.zul.Toolbarbutton;
import org.zkoss.zul.Window;

import shared.AppConstants;
import shared.AppContext;
import shared.DBMSHelper;
import bo.Translation;
import bo.trados.TradosFileImporter;

public class TradosUpload extends GenericForwardComposer {
	private static final long serialVersionUID = 1L;
	Log log = LogFactory.getLog(Login.class);
	private Textbox ef_text;	// auto wired
	private Textbox ef_trans;	// auto wired
	private Textbox ef_status;	// auto wired
	private Combobox ef_language;	// auto wired
	private Button but_upload;	// auto wired
	private Button but_import;	// auto wired
	private Toolbarbutton but_select_text;	// auto wired
	private Toolbarbutton but_select_trans;	// auto wired
	private Listbox lb_transcheck;	// auto wired
	private Media m1 = null;
	private Media m2 = null;
	private String localfile_text = null;
	private String localfile_trans = null;

	private Vector<MyContainer> arr_Translations = null;
	
	class MyContainer {
		Translation T = null;
		boolean isTranslated = false;
		boolean isTranslationEqual = false;
		boolean isMD5Equal = false;
		String existingTranslation = null;
	}
	
	private void cleanupLocalTempFiles() {
		File f;
		if (localfile_text != null) {
			f = new File(localfile_text);
			f.delete();
		}
		if (localfile_trans != null) {
			f = new File(localfile_trans);
			f.delete();
		}
		localfile_text = localfile_trans = null;
	}
	
	public void onClose(Event event) throws Exception {
		log.debug("TradosUpload:onClose");
		cleanupLocalTempFiles();
//		if (m1 != null) m1.getStreamData().close();
//		if (m2 != null) m2.getStreamData().close();
	}
	
//	private String upload_media(Media iMedia) throws IOException {
//		byte [] x = iMedia.getByteData();
//		File f = File.createTempFile("mlm_", iMedia.getName());
//		BufferedOutputStream os = new BufferedOutputStream(new FileOutputStream(f));
//		os.write(x);
//		os.close();
//		return f.getAbsolutePath();
//	}
	private String upload_media(Media iMedia) throws IOException {
		String s = iMedia.getStringData();
		File f = File.createTempFile("mlm_", iMedia.getName());
		BufferedOutputStream os = new BufferedOutputStream(new FileOutputStream(f));
		os.write(s.getBytes("utf-8"));
		os.close();
		return f.getAbsolutePath();
	}
	
	public void onUpload$but_select_text(UploadEvent event) throws Exception  {
		log.debug("onUpload$but_select_text");
		ef_text.setValue(event.getMedia().getName());
		m1 = event.getMedia();
		if (!ef_text.getValue().equals("") && !ef_trans.getValue().equals("")) {
			if (ef_text.getValue().equals(ef_trans.getValue())) {
				Messagebox.show("Zweimal dieselbe Datei ausgewählt!", "Fehler", Messagebox.OK, Messagebox.ERROR);
				ef_text.setValue(null);
			} else {
				but_upload.setDisabled(false);
			}
		}
	}
	public void onUpload$but_select_trans(UploadEvent event) throws Exception {
		log.debug("onUpload$but_select_trans");
		ef_trans.setValue(event.getMedia().getName());
		m2 = event.getMedia();
		if (!ef_text.getValue().equals("") && !ef_trans.getValue().equals("")) {
			if (ef_text.getValue().equals(ef_trans.getValue())) {
				Messagebox.show("Zweimal dieselbe Datei ausgewählt!", "Fehler", Messagebox.OK, Messagebox.ERROR);
				ef_trans.setValue(null);
			} else {
				but_upload.setDisabled(false);
			}
		}
	}
	
	public void onClick$but_upload(Event event) throws Exception  {
		log.debug("onClick$but_upload");
		localfile_text = upload_media(m1);
		log.debug("uploaded "+m1.getName()+" into "+localfile_text);
//		m1.getStreamData().close();
		ef_text.setValue(ef_text.getValue()+": Upload Ok");
//		ef_text.setDisabled(true);
		localfile_trans = upload_media(m2);
		log.debug("uploaded "+m2.getName()+" into "+localfile_trans);
//		m2.getStreamData().close();
		ef_trans.setValue(ef_trans.getValue()+": Upload Ok");
//		ef_trans.setDisabled(true);
		
		Vector<String> arr_texts = TradosFileImporter.loadTexts(localfile_text);
		log.debug("reading: " + localfile_text + ", found: " + arr_texts.size() + " texts");
		Vector<String> arr_trans = TradosFileImporter.loadTexts(localfile_trans);
		log.debug("reading: " + localfile_trans + ", found: " + arr_trans.size() + " translations");
		if (arr_texts.size() != arr_trans.size()) {
			cleanupLocalTempFiles();
			Messagebox.show("Anzahl der Texte und der Übersetzungen stimmt nicht überein!\n" +
					m1.getName()+": "+arr_texts.size()+" Texte\n"+
					m2.getName()+": "+arr_trans.size()+" Übersetzungen", "Fehler", Messagebox.OK, Messagebox.ERROR);
			return;
		}

		Listhead hh = new Listhead();
		hh.setSizable(true);
		Listheader h = new Listheader("Text");
		h.setSort("auto");
		h.setHflex("1");
		hh.appendChild(h);
		h = new Listheader("Übersetzung");
		h.setHflex("1");
		hh.appendChild(h);
		h = new Listheader("Bereits übersetzt?");
		h.setHflex("min");
		hh.appendChild(h);
		h = new Listheader("MD5 ident?");
		h.setHflex("min");
		hh.appendChild(h);
		h = new Listheader("Übersetzung ident?");
		h.setHflex("min");
		hh.appendChild(h);
		lb_transcheck.appendChild(hh);

		but_upload.setDisabled(true);
		but_import.setDisabled(false);
		but_select_text.setVisible(false);
		but_select_trans.setVisible(false);
		
		arr_Translations = new Vector<MyContainer>();

		AppContext ac = (AppContext )Sessions.getCurrent().getAttribute(AppConstants.ATTR_APPCONTEXT);
		Connection conn = DBMSHelper.getNewConnection((DataSource )ac.getDataSource());
		String origin = Sessions.getCurrent().getRemoteAddr();
		PreparedStatement s;
		ResultSet rs;
		int cnt_MD5Warnings = 0, cnt_NewTrans = 0, cnt_ModTrans = 0;
		s = conn.prepareStatement("select textmd5,translation from mltrans where mcode='*' and look='*' and lang='"+ef_language.getValue()+"' and app='ITSWEB' and modul='*' and dialog='*' and textkey=?");
		
		for (int i = 0; i < arr_texts.size(); i++) {
            MyContainer c = new MyContainer();
            c.T = new Translation("en", arr_texts.get(i), arr_trans.get(i), null, origin, ac.isMltransHistoryable());
            arr_Translations.add(c);
    		Listitem item = new Listitem();
    		Listcell cell = new Listcell(c.T.getText());
    		cell.setParent(item);
    		cell = new Listcell(c.T.getTranslation());
    		cell.setParent(item);
    		if (c.T.getText().equals(c.T.getTranslation())) {
    			item.setStyle("color:#0000ff !important;");
    		}
			s.setString(1, arr_Translations.get(i).T.getText());
    		s.execute();
    		rs = s.getResultSet();
    		if (rs.next()) {
    			c.isTranslated = true;
    			if (c.T.getMd5().compareToIgnoreCase(rs.getString(1)) == 0) {
    				c.isMD5Equal = true;
    			} else {
        			cnt_MD5Warnings++;
    			}
    			if (c.T.getTranslation().equals(rs.getString(2))) {
    				c.isTranslationEqual = true;
        			item.setStyle("color:#0fff0f !important;");
    			} else {
        			cnt_ModTrans++;
    			}
    		} else {
    			cnt_NewTrans++;
    		}
    		rs.close();
       		cell = new Listcell(c.isTranslated ? "Ja" : "Nein");
    		cell.setParent(item);
    		if (c.isTranslated) {
	       		cell = new Listcell(c.isMD5Equal ? "Ja" : "Nein");
	    		cell.setParent(item);
	       		cell = new Listcell(c.isTranslationEqual ? "Ja" : "Nein");
	    		cell.setParent(item);
    		}
       	 	lb_transcheck.appendChild(item);
		}
		s.close();
		conn.rollback();
		conn.close();

		cleanupLocalTempFiles();

		Window wnd = (Window )ef_status.getRoot();
		wnd.setMaximized(true);

		ef_status.setValue("Anzahl: " + arr_Translations.size() +
				", Neu: " + cnt_NewTrans +
				", Geändert: " + cnt_ModTrans +
				", Duplikate (werden ignoriert): " + (arr_Translations.size() - cnt_NewTrans - cnt_ModTrans));
		ef_status.setVisible(true);
		ef_status.setWidth("100%");
		lb_transcheck.setVisible(true);
	}

	public void onClick$but_import(Event event) throws Exception  {
		log.debug("onClick$but_import");
		log.debug("modifying translations in DBMS - wait ...");			
		
		AppContext ac = (AppContext )Sessions.getCurrent().getAttribute(AppConstants.ATTR_APPCONTEXT);
		Connection conn = DBMSHelper.getNewConnection((DataSource )ac.getDataSource());

		PreparedStatement stmt_insert = conn.prepareStatement(
				"insert into mltrans (mcode,look,lang,app,modul,dialog,textkey,translation,isactive,textmd5)"+
				" values ('*','*',?,'ITSWEB','*','*',?,?,'Y',?)"
			);
		PreparedStatement stmt_delete = conn.prepareStatement(
				"delete from mltrans where mcode='*' and look='*' and lang=? and app='ITSWEB' and modul='*' and dialog='*' and textkey=?"
			);
		
		int cnt_AllTrans = arr_Translations.size();
		
		for (int i = 0; i < cnt_AllTrans; i++) {
			if (arr_Translations.get(i).isTranslated) {
				if (arr_Translations.get(i).isTranslationEqual) {
					// nothing to do ...
					log.debug("skipping/equal trans: " + arr_Translations.get(i).T.getText());			
					continue;
				} else {
					// delete existing translation...					
					log.debug("deleting: " + arr_Translations.get(i).T.getText());			
					stmt_delete.setString(1, ef_language.getValue());
					stmt_delete.setString(2, arr_Translations.get(i).T.getText());
					stmt_delete.execute();
				}
			}
			// insert new translation...
			log.debug("inserting: " + arr_Translations.get(i).T.getText() + " -> " + arr_Translations.get(i).T.getTranslation());			
			stmt_insert.setString(1, ef_language.getValue());
			stmt_insert.setString(2, arr_Translations.get(i).T.getText());
			stmt_insert.setString(3, arr_Translations.get(i).T.getTranslation());
			stmt_insert.setString(4, arr_Translations.get(i).T.getMd5());
			stmt_insert.execute();
		}
		stmt_delete.close();
		stmt_insert.close();
		conn.commit();
		conn.close();
		log.debug("modifying translations - done.");
	
		Messagebox.show("Übersetzungen wurden importiert.", "Info", Messagebox.OK, Messagebox.INFORMATION);
	
		Window wnd = (Window )ef_status.getRoot();
		wnd.detach();
	}
}