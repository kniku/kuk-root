package clserv.clienthandler;

import java.io.IOException;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.text.DateFormat;

import org.apache.log4j.Logger;

import clserv.model.Mitarbeiter;
import clserv.model.dao.DaoES_LOGS;
import clserv.tools.AppContext;


public abstract class AClientHandler_qmi_base extends AClientHandler implements IStatemachine {
	
	private final Logger logger = Logger.getLogger(this.getClass());

	enum ParameterFlag {Intern, Kuerzel, Firma}
	
	protected class Parameter {
		ParameterFlag mFlag = null;
		String mPattern = null;
		String mFirma = null;
	}

	enum MitarbeiterTyp {Intern, Extern}
	
	@Override
	public void handle(String iCommand) throws IOException {
		Parameter myParams = (Parameter )getParameter(iCommand);
		if (myParams == null) return;
		
		String Pattern_upcase = myParams.mPattern == null ? null : myParams.mPattern.toUpperCase();
		String Firma_upcase = myParams.mFirma == null ? null : myParams.mFirma.toUpperCase();
		
		handle_START(iCommand);
		
		if (myParams.mFlag == ParameterFlag.Kuerzel) {
			Mitarbeiter mitarbeiter = (Mitarbeiter )AppContext.getInstance().findeMitarbeiterMitLogin(myParams.mPattern);
			if (mitarbeiter != null) {
				iCommand = createMsg(mitarbeiter, MitarbeiterTyp.Intern,
							mitarbeiter.getName(), mitarbeiter.getFirstName(),
							mitarbeiter.getDurchwahl(), mitarbeiter.getTelprivat(),
							mitarbeiter.getTelfirma(), mitarbeiter.getTelprivat2(),
							mitarbeiter.getHandyprivat(), mitarbeiter.getLogin(),
							mitarbeiter.getGenderAsString(), mitarbeiter.getHandyfirma(),
							mitarbeiter.getAutonummer());
				handle_ENTRY(iCommand, null);				
			}
		} else {
			Connection c = null;
			try {
				c = AppContext.getInstance().getDatabase().getConnection();
				String Stmt = null;
				PreparedStatement StmtPrep = null;
				ResultSet rs = null;
				int count = 0;
				
				if (myParams.mFlag == ParameterFlag.Firma || myParams.mFlag == null) {
	
					Stmt =	"select a.QMINDNNAME,a.QMINDVNAME,a.QMINDtel01,a.QMINDtel02,a.QMINDtelhy,"+
						 	"a.QMINDemail,a.QMINDfaxnr,a.QMINDgesch";
					if (myParams.mFlag == ParameterFlag.Firma) {
						Stmt += ",b.QMKONBZEIC";
					} else {
						Stmt += ",null";
					}
					
					Stmt += " from qmdb53 a ";
					if (myParams.mFlag == ParameterFlag.Firma) {
						Stmt += ",qmdb52 b ";
					}
					Stmt += "where (UPPER(a.qmindnname) like '%";
					Stmt += Pattern_upcase + "%' or UPPER(a.qmindvname) like '%";
					Stmt += Pattern_upcase + "%') ";
					if (myParams.mFlag == ParameterFlag.Firma) {
						Stmt += "and a.QMMANNUMMR=b.QMMANNUMMR and a.QMKONNUMMR=b.QMKONNUMMR and upper(b.QMKONBZEIC) like '%";
						Stmt += Firma_upcase + "%' ";
					}
					Stmt += "order by a.QMINDNNAME,a.QMINDVNAME";
					StmtPrep = c.prepareStatement(Stmt);
					logger.trace("Statement to execute=[" + Stmt + "]");
					StmtPrep.execute();
					rs = StmtPrep.getResultSet();
					for (; rs.next() ;count++) {
						iCommand = createMsg(null, MitarbeiterTyp.Extern, rs.getString(1), rs.getString(2), "", rs.getString(3), rs.getString(4),
											 rs.getString(5), rs.getString(6), rs.getString(7), rs.getString(8),
											 "", "");
						handle_ENTRY(iCommand, null);
					}
					StmtPrep.close();
					rs.close();
				}			
				if (myParams.mFlag == ParameterFlag.Intern || myParams.mFlag == null) {
	
					Stmt =	"select qmpernname,qmpervname,qmperteldw,QMPERTELPR,QMPERTELFA,QMPERTELP2," + 
						 	"QMPERTHYPR,qmperkubez,qmpergesch,qmperthyfa,QMPERPGRPR from qmdb10 where (";
					if (myParams.mFlag == ParameterFlag.Intern || myParams.mFlag == null) {
						Stmt += "UPPER(qmpernname) like '%";
						Stmt += Pattern_upcase + "%' or UPPER(qmpervname) like '%";
						Stmt += Pattern_upcase + "%' or ";
					}
					Stmt += "UPPER(qmperkubez) like '%";
					Stmt += Pattern_upcase + "%') order by QMperNNAME,QMperVNAME";
					StmtPrep = c.prepareStatement(Stmt);
					logger.trace("Statement to execute=[" + Stmt + "]");
					StmtPrep.execute();
					rs = StmtPrep.getResultSet();
					for (; rs.next() ;count++) {
						iCommand = createMsg(null, MitarbeiterTyp.Intern, rs.getString(1), rs.getString(2), rs.getString(3), rs.getString(4),
											 rs.getString(5), rs.getString(6), rs.getString(7), rs.getString(8),
											 rs.getString(9), rs.getString(10), rs.getString(11));
						handle_ENTRY(iCommand, null);
					}
					StmtPrep.close();
					rs.close();
				}
				
				c.rollback();
				AppContext.getInstance().getDatabase().releaseConnection(c);
				handle_FINISH(iCommand);
			} catch (Exception e) {
				logger.error(null, e);
				clserv.shared.Database.closeAllConnections();
			}
		}
	}

	private String createMsg(Mitarbeiter iMa, MitarbeiterTyp iType, String iName, String iVorname, String iDurchwahl,
			String iTelPrivat, String iTelFirma, String iTelPrivat2, String iHandyPrivat,
			String iKuerzel, String iSex, String iHandyFirma, String iKfz) {
		final String TOK_SEP = "\02";
		String inout = null;
		String info = null;
		String ts = null;
		
		if (iType == MitarbeiterTyp.Intern) {
			Mitarbeiter ma = iMa == null ? (Mitarbeiter )AppContext.getInstance().findeMitarbeiterMitLogin(iKuerzel) : iMa;
			if (ma != null) {
				DaoES_LOGS es = ma.getLog(-1);
				if (es != null) {
					inout = es.getEntry_type().equals(clserv.model.tables.Columns.StrConstrENTRY_TYPE_In) ? "IN" : "OU";
					info = es.getInfo();
					ts = DateFormat.getDateTimeInstance().format(es.getEntry_ts());
				}
			}
		}
		
		String r = (iType==MitarbeiterTyp.Intern?"I":"E")+
					TOK_SEP+iVorname+TOK_SEP+iName+TOK_SEP+iTelFirma+TOK_SEP+iDurchwahl+
					TOK_SEP+iTelPrivat+TOK_SEP+iTelPrivat2+TOK_SEP+iHandyPrivat+TOK_SEP+iHandyFirma+
					TOK_SEP+iSex+TOK_SEP+iKuerzel+TOK_SEP+iKfz+TOK_SEP+
					(inout==null?"?":inout)+TOK_SEP+(ts==null?"":ts)+TOK_SEP+(info==null?"":info)+"\n";
		return r;
	}

}
