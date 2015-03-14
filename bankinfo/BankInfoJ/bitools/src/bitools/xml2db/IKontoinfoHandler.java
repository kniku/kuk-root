package bitools.xml2db;

import java.util.Date;


public interface IKontoinfoHandler {
	public void ParserStart() throws Exception;
	public void ParserEnde() throws Exception;
	public void KontoStart();
	public void SetKontoNummer(String iNummer);
	public void KontoEnde();
	public void PositionStart();
	public void SetPosBetrag(Double iBetrag);
	public void SetPosWaehrung(String iWaehrung);
	public void SetPosInfo(String iInfo);
	public void SetPosDatum(Date iDatum);
	public void SetPosBuchungsdatum(Date iDatum);
	public void PositionEnde();
}
