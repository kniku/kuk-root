package bitools.xml2db;


import java.sql.Connection;
import java.text.SimpleDateFormat;
import java.util.Date;

import org.apache.log4j.Logger;

import shared.bo.Auszugsposition;
import shared.bo.Konto;


public class KontoinfoDBWriter implements IKontoinfoHandler {
	private static Logger logger = Logger.getLogger(KontoinfoParser.class);

	private Konto konto = null;
	private Auszugsposition position = null;
	long cntKonten = 0;
	long cntPositionen = 0;
	
	private Connection conn = null;

	private void fullStop() {
		logger.error("FullStop request: rollback and exit!");
		if (conn != null) {
			try {
				conn = bitools.database.ConnectionHelper.releaseDBConnection(conn);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		System.exit(-1);		
	}
	
	@Override
	public void KontoEnde() {
		logger.debug("KontoEnde");
	}

	@Override
	public void KontoStart() {
		logger.debug("KontoStart");
		cntKonten++;
	}

	@Override
	public void PositionEnde() {
		logger.debug("PositionEnde");
		if (position.save(conn) == false) {
			fullStop();			
		}
		position.reset();
	}

	@Override
	public void PositionStart() {
		logger.debug("PositionStart");
		/*if (position == null) */position = new Auszugsposition(konto.getId());
		position.reset();
		cntPositionen++;
	}

	@Override
	public void SetKontoNummer(String iNummer) {
		logger.debug("SetKontoNummer:"+iNummer);
		konto = new Konto();
		konto.setKontonummer(iNummer);
		if (konto.loadWithAccountNumber(conn) == false) {
			logger.error("Ungueltige Kontonummer oder Konto noch nicht in der Datenbank:"+iNummer);
			fullStop();
		}
		logger.debug("Konto aus Datenbank geladen.");
	}

	@Override
	public void SetPosBetrag(Double iBetrag) {
		logger.debug("SetPosBetrag:"+iBetrag);
		position.setBetrag(iBetrag);
	}

	@Override
	public void SetPosBuchungsdatum(Date iDatum) {
		if (logger.isDebugEnabled()) {
			logger.debug("SetPosBuchungsdatum:"+new SimpleDateFormat("dd.MM.yyyy").format(iDatum));
		}
		position.setBuchungsdatum(iDatum);
	}

	@Override
	public void SetPosDatum(Date iDatum) {
		if (logger.isDebugEnabled()) {
			logger.debug("SetPosDatum:"+new SimpleDateFormat("dd.MM.yyyy").format(iDatum));
		}
		position.setDatum(iDatum);
	}

	@Override
	public void SetPosInfo(String iInfo) {
		logger.debug("SetPosInfo:"+iInfo);
		position.setInfo(iInfo);
	}

	@Override
	public void SetPosWaehrung(String iWaehrung) {
		logger.debug("SetPosWaehrung:"+iWaehrung);
		position.setWaehrung(iWaehrung);
	}

	@Override
	public void ParserEnde() throws Exception {
		logger.debug("ParserEnde");
		String isCommit = bitools.shared.AppContext.getInstance("xml2db").getProperty("commit");
		if (isCommit != null && isCommit.equals("1")) {
			logger.debug("commit=1; Datenbanktransaktion wird abgeschlossen");
			conn.commit();
		} else {
			logger.debug("commit=0; Datenbanktransaktion wird zurueckgenommen");
			conn.rollback();
		}
		conn = bitools.database.ConnectionHelper.releaseDBConnection(conn);
		logger.info("Konten    : " + cntKonten);
		logger.info("Positionen: " + cntPositionen);
	}

	@Override
	public void ParserStart() throws Exception {
		logger.debug("ParserStart");
		conn = bitools.database.ConnectionHelper.getDBConnection(bitools.shared.AppContext.getInstance("xml2db"));
	}

}
