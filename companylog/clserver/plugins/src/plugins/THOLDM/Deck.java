package plugins.THOLDM;

import java.util.Random;

public class Deck {
	final int DECK_SIZE = 52;
	Card m_Deck[] = new Card[DECK_SIZE];
	int m_CurrentCard;
	
	Deck() {
		for(int v = 0; v < DECK_SIZE; v++) {
			m_Deck[v] = new Card(v);
		}				
	}
	
	void riffleCards() {
		int i;
		Random rnd = new Random();
		for (i = 0; i < 1000; i++) {
			int p1 = rnd.nextInt(DECK_SIZE);
			int p2 = rnd.nextInt(DECK_SIZE);
			if (p1 != p2) {
				Card tmp = m_Deck[p1]; 
				m_Deck[p1] = m_Deck[p2];
				m_Deck[p2] = tmp;
			}
		}
		m_CurrentCard = 0;
	}
	Card takeCard() {
		Card r = null;
		if (m_CurrentCard < DECK_SIZE) {
			r = m_Deck[m_CurrentCard]; 
		}
		m_CurrentCard++;
		return r;
	}

}
