package plugins.THOLDM;

import java.util.Vector;

public class Player {
	// static data
	private String m_Login;
	private String m_Name = "";
	private int m_Capital = 0;
	// playing data
	private Vector<Card> m_Cards = new Vector<Card>(2);
	private boolean m_isPlaying = true;
	private int m_Bet = 0;

	Player(String iPlayer, int iInitialPlayerCapital) {
		m_Login = iPlayer;
		m_Capital = iInitialPlayerCapital;
	}
	
	void setBet(int iBet) {
		m_Bet += iBet;
		m_Capital -= iBet;
	}

	public String getLogin() {
		return m_Login;
	}

	public boolean isPlaying() {
		return m_isPlaying;
	}
	public void setPlaying(boolean iActive) {
		m_isPlaying = iActive;
	}

	public int getBet() {
		return m_Bet;
	}

	public int getCapital() {
		return m_Capital;
	}
	
	public void initForNewHand() {
		m_Cards.clear();
		m_Bet = 0;
		m_isPlaying = true;
	}
	
	public void giveCard(Card iCard) {
		m_Cards.addElement(iCard);
	}

	public String getName() {
		return m_Name;
	}

	public void setName(String name) {
		m_Name = name;
	}
	
	public Card getCardByIndex(int iIndex) {
		Card r = null;
		if (iIndex >= 0 && iIndex < m_Cards.size()) {
			r = m_Cards.get(iIndex);
		}
		return r;
	}
	
	public String getLastActionName() {
		String r = null;
		return r;
	}
	
	/**
	 * @param iPlayer The other player 
	 * @return >0 ... my hand is better
	 *         =0 ... equal
	 * 		   <0 ... my hand is worse	
	 */
	public int compareMyHandWithPlayer(Player iPlayer, Vector<Card> iPublicCards) {
		Vector<Card> hand1 = getBest5OutOf7Cards(this, iPublicCards);
		Vector<Card> hand2 = getBest5OutOf7Cards(iPlayer, iPublicCards);
		return compareCardSets(hand1, hand2);
	}
	
	/**
	 * @param iCards1 Card set 1 (five cards)
	 * @param iCards2 Card set 2 (five cards)
	 * @return >0 ... card set 1 is better
	 *         =0 ... card sets are equal
	 *         <0 ... card set 2 is better
	 */
	private static int compareCardSets(Vector<Card> iCards1, Vector<Card> iCards2) {
		int r = 0;
		// TODO: compare card sets...
		return r;
	}
	
	/**
	 * @param iPlayer The player with his two cards
	 * @param iPublicCards The set of five public cards
	 * @return Best five card combination out of these seven cards
	 */
	private static Vector<Card> getBest5OutOf7Cards(Player iPlayer, Vector<Card> iPublicCards) {
		Vector<Card> r = null;
		if (iPublicCards == null || iPublicCards.size() != 5 || iPlayer.m_Cards.size() != 2) {
			return null;
		}
		// TODO: find the best 5 out of 7 cards
		return r;
	}
}
