package plugins.THOLDM;

public class Card {

/*
 *  0 .. 12 Karo  2 .. Ass
 * 13 .. 25 Herz  2 .. Ass
 * 26 .. 38 Pik   2 .. Ass
 * 39 .. 51 Kreuz 2 .. Ass
 */
	int m_Value;

	Card(int iValue) {
		m_Value = iValue;
	}

	public int getValue() {
		return m_Value;
	}
	
}
