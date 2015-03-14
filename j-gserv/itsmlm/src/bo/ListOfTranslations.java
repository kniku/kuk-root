package bo;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import shared.StringTools;

public class ListOfTranslations {
	private static final Log log = LogFactory.getLog(ListOfTranslations.class);
	private ArrayList<Translation> list = new ArrayList<Translation>();
	
	public void reload(Connection iConn, ListOfTranslationFilter iFilter, boolean iHistoryable) throws SQLException {
		list.clear();
		
		log.debug("ListOfTranslations::reload: language=["+iFilter.language+"]");
		PreparedStatement stmt = null;

		int pos = 1;
		String likePartText = null;
		String likePartTrans = null;
		String s;
		
		switch (iFilter.source) {
		default:
		case 0: // Text
			if (StringTools.isEmptyStrg(iFilter.text)) {
				likePartText = "";
			} else if (iFilter.useInstr) {
				if (iFilter.useCase) {
					likePartText = "and instr(tx.text,?)>0";
				} else {
					likePartText = "and instr(upper(tx.text),upper(?))>0";					
				}
			} else {
				if (iFilter.useCase) {
					likePartText = "and tx.text like ?";
				} else {
					likePartText = "and upper(tx.text) like upper(?)";					
				}
			}
			s = "select tr.lang,tx.TEXT,tr.TRANSLATION,tx.textmd5,prd.version,tx.modul,tx.dialog,tx.dialogdescr,tx.dialogelement,null,null,null,null,null,null"+
				" from mltext tx"+
				" join MLPRODUCT prd on prd.ID=tx.PRODUCTID"+
				" join MLAPP app on app.ID=prd.APPID"+
				" left join mltrans tr on tx.TEXT=tr.TEXTKEY and tr.lang=?"+
				" where app.name='ITSWEB' "+
				(StringTools.isEmptyStrg(iFilter.version) ? "" : "and prd.version like ? ")+
				likePartText;
			if (log.isDebugEnabled()) log.debug("stmt:["+s+"]");
			stmt = iConn.prepareStatement(s);
			stmt.setString(pos++, iFilter.language);
			if (!StringTools.isEmptyStrg(iFilter.version)) stmt.setString(pos++, iFilter.version);
			if (!StringTools.isEmptyStrg(iFilter.text)) stmt.setString(pos++, iFilter.text);
			break;
		case 1: // not translated
			if (StringTools.isEmptyStrg(iFilter.text)) {
				likePartText = "";
			} else if (iFilter.useInstr) {
				if (iFilter.useCase) {
					likePartText = "and instr(tx.text,?)>0";
				} else {
					likePartText = "and instr(upper(tx.text),upper(?))>0";					
				}
			} else {
				if (iFilter.useCase) {
					likePartText = "and tx.text like ?";
				} else {
					likePartText = "and upper(tx.text) like upper(?)";					
				}
			}
			s = "select null,tx.TEXT,null,tx.textmd5,prd.version,tx.modul,tx.dialog,tx.dialogdescr,tx.dialogelement,null,null,null,null,null,null"+
				" from mltext tx"+
				" join MLPRODUCT prd on prd.ID=tx.PRODUCTID"+
				" join MLAPP app on app.ID=prd.APPID"+
				" where app.name='ITSWEB' "+
				(StringTools.isEmptyStrg(iFilter.version) ? "" : "and prd.version like ? ")+
				likePartText+
				" and not exists (select null from mltrans tr where tr.lang=? and tr.textkey=tx.text)";
			if (log.isDebugEnabled()) log.debug("stmt:["+s+"]");
			stmt = iConn.prepareStatement(s);
			if (!StringTools.isEmptyStrg(iFilter.version)) stmt.setString(pos++, iFilter.version);
			if (!StringTools.isEmptyStrg(iFilter.text)) stmt.setString(pos++, iFilter.text);
			stmt.setString(pos++, iFilter.language);
			break;
		case 2:	// Translation
			if (StringTools.isEmptyStrg(iFilter.text)) {
				likePartText = "";
			}
			if (StringTools.isEmptyStrg(iFilter.translation)) {
				likePartTrans = "";
			}
			if (iFilter.useInstr) {
				if (iFilter.useCase) {
					if (likePartText == null) likePartText = "and instr(textkey,?)>0";
					if (likePartTrans == null) likePartTrans = "and instr(translation,?)>0";
				} else {
					if (likePartText == null) likePartText = "and instr(upper(textkey),upper(?))>0";					
					if (likePartTrans == null) likePartTrans = "and instr(upper(translation),upper(?))>0";					
				}
			} else {
				if (iFilter.useCase) {
					if (likePartText == null) likePartText = "and textkey like ?";
					if (likePartTrans == null) likePartTrans = "and translation like ?";
				} else {
					if (likePartText == null) likePartText = "and upper(textkey) like upper(?)";					
					if (likePartTrans == null) likePartTrans = "and upper(translation) like upper(?)";					
				}
			}
			s = "select lang,textkey,translation,textmd5,null,null,null,null,null,mcode,look,app,modul,dialog,isactive"+
				" from mltrans where lang=? "+likePartText+" "+likePartTrans;
			if (log.isDebugEnabled()) log.debug("stmt:["+s+"]");
			stmt = iConn.prepareStatement(s);
			stmt.setString(pos++, iFilter.language);
			if (!StringTools.isEmptyStrg(iFilter.text)) stmt.setString(pos++, iFilter.text);
			if (!StringTools.isEmptyStrg(iFilter.translation)) stmt.setString(pos++, iFilter.translation);
			break;
		}
		
		stmt.execute();
		ResultSet rs = stmt.getResultSet();
		while (rs.next()) {
			Translation T = new Translation(rs.getString(1),rs.getString(2),rs.getString(3),rs.getString(4), null, iHistoryable);
			T.setVersion(rs.getString(5));
			T.setModul(rs.getString(6));
			T.setDialog(rs.getString(7));
			T.setDialogdescr(rs.getString(8));
			T.setDialogelement(rs.getString(9));
			T.setTr_mcode(rs.getString(10));
			T.setTr_look(rs.getString(11));
			T.setTr_app(rs.getString(12));
			T.setTr_modul(rs.getString(13));
			T.setTr_dialog(rs.getString(14));
			T.setTr_isactive(rs.getString(15));
			list.add(T);
		}
		stmt.close();
	}
	
	public ArrayList<Translation> getList() {
		return list;
	}
}
