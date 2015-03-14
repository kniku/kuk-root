package plugins.THOLDM;

import java.util.Vector;

public class Game {
	//private enum HAND_STATE {None, Flop, Turn, River};
	// static data
	private int m_ID;								// unique game ID
	private String m_GameName = null;				// game description
	private int m_InitialBlindBet = 0;				// Big Blind Bet for the game
	private int m_InitialPlayerCapital = 0;			// starting capital of each player (fix for all?)
	// statistics
	private int m_Hands = 0;						// already played hands
	// certain container
	private Vector<Player> m_PlayerList = new Vector<Player>();	// players of the game
	private Deck m_Deck = new Deck();		// the card deck of the game
	private Vector<Card> m_PublicCards = new Vector<Card>(5);	// public cards of current game
	// game/hand states
	private boolean m_GameHandIsInProgress = false;	// hand in progress?
	private int m_CurrentDealerIndex = 0;			// index of current dealer
	private int m_CurrentPlayerIndex = 1;			// index of current player
	private int m_Pot = 0;							// current pot
	private int m_CurrentHandPhase = 0;				// current phase (0=none, 1=flop, 2=turn, 3=river
	private int m_CurrentHandMaxBet = 0;			// current max. bet of all the users
	private int m_CurrentBetRound_Bets = 0;			// number of bets in current round
	
	public Game(int iID, int iInitialPlayerCapital, int iBlindBet) {
		m_InitialPlayerCapital = iInitialPlayerCapital;
		m_InitialBlindBet = iBlindBet;
		m_ID = iID;
	}
	
	public void setGameName(String iName) {
		m_GameName = iName;
	}
	
	public boolean addPlayer(String iPlayer) {
		boolean r = false;
		if (getPlayerIndexByName(iPlayer) < 0) {
			Player p = new Player(iPlayer, m_InitialPlayerCapital);
			m_PlayerList.addElement(p);
			r = true;
		}
		return r;
	}

	public boolean player_pass(String iPlayer) {
		boolean r = false;
		int idx = getPlayerIndexByName(iPlayer);
		if (idx >= 0 && m_GameHandIsInProgress) {
			m_PlayerList.get(idx).setPlaying(false);
			if (idx == m_CurrentDealerIndex) m_CurrentDealerIndex = nextPlayerIndex(m_CurrentDealerIndex);
			if (idx == m_CurrentPlayerIndex) m_CurrentPlayerIndex = nextPlayerIndex(m_CurrentPlayerIndex);
			r = true;
		}
		return r;
	}
	
	public boolean removePlayer(String iPlayer) {
		boolean r = false;
		if (m_GameHandIsInProgress) {
			r = player_pass(iPlayer);
		} else {
			int idx = getPlayerIndexByName(iPlayer);
			if (idx >= 0) {
				m_PlayerList.remove(idx);
				if (m_CurrentDealerIndex > idx) m_CurrentDealerIndex -= 1;
				if (m_CurrentPlayerIndex > idx) m_CurrentPlayerIndex -= 1;
				r = true;
			}
		}
		return r;
	}
	
	public boolean startGame() {
		return startNewHand();
	}
	
	private int getCurrentBetRound_ActivePlayers() {
		// TODO: determine ACTIVE players of current bet round
		return m_PlayerList.size();
	}
	
	private boolean betsAreEven() {
		boolean r = true;
		if (m_CurrentBetRound_Bets < getCurrentBetRound_ActivePlayers()) return false;
		Player aPlayer;
		int idx = 0;
		while ((aPlayer = getPlayerByIndex(idx++)) != null) {
			if (aPlayer.getBet() != m_CurrentHandMaxBet) {
				r = false;
				break;
			}
		}
		return r;
	}
	
	public boolean handleBet(String iPlayer, int iBet) {
		// check if iPlayer is the current player
		if (getIndexOfCurrentPlayer() != getPlayerIndexByName(iPlayer)) {
			return false;
		}
//		Player aPlayer = getPlayerByIndex(getIndexOfCurrentPlayer());
		// check if bet is high enough
		if (getMinBetOfCurrentPlayer() > iBet) {
			return false;
		}
		// TODO: check if aPlayer has enough money for this bet
		// make the bet
		makeBet(getPlayerByName(iPlayer), iBet);
		
		// check if a new phase is reached
		if (betsAreEven()) {
			startNewPhase();
		}
		// move the player token...
		m_CurrentPlayerIndex = nextPlayerIndex(m_CurrentPlayerIndex);
		
		return true;
	}
	
	private void makeBet(Player iPlayer, int iAmount) {
		iPlayer.setBet(iAmount);
		m_CurrentHandMaxBet = iPlayer.getBet();
		m_Pot += iAmount;
		m_CurrentBetRound_Bets++;
	}
	
	private boolean startNewPhase() {
		// check
		if (m_CurrentHandPhase > 3) {
			return false;
		}
		// TODO: who is the next player?
		
		// new open card(s)
		m_PublicCards.add(m_Deck.takeCard());
		if (m_CurrentHandPhase == 0) {
			// -> flop, three new open cards
			m_PublicCards.add(m_Deck.takeCard());
			m_PublicCards.add(m_Deck.takeCard());
		}
		// reset bet counter
		m_CurrentBetRound_Bets = 0;
		// new phase...
		m_CurrentHandPhase++;
		return true;
	}
	
	private boolean startNewHand() {
		
		if (m_PlayerList.size() < 2 || m_GameHandIsInProgress) {
			return false;
		}
		
		m_GameHandIsInProgress = true;
		
		if (m_Hands > 0) {
			m_CurrentDealerIndex = nextPlayerIndex(m_CurrentDealerIndex);
		}
		m_Hands++;
		
		m_Deck.riffleCards();
		
		// two cards for each player
		for (int i = 0; i < m_PlayerList.size(); i++) {
			Player p = m_PlayerList.get(i);
			p.initForNewHand();
			p.giveCard(m_Deck.takeCard());
			p.giveCard(m_Deck.takeCard());
		}
		// clean open cards
		m_PublicCards.clear();

		// small blind
		m_CurrentPlayerIndex = nextPlayerIndex(m_CurrentDealerIndex);
		makeBet(m_PlayerList.get(m_CurrentPlayerIndex), m_InitialBlindBet / 2);
		
		// big blind
		m_CurrentPlayerIndex = nextPlayerIndex(m_CurrentPlayerIndex);
		makeBet(m_PlayerList.get(m_CurrentPlayerIndex), m_InitialBlindBet);
		
		// first player
		m_CurrentPlayerIndex = nextPlayerIndex(m_CurrentPlayerIndex);
		
		// hand phase reset
		m_CurrentHandPhase = 0;
		
		return true;
	}

	private int nextPlayerIndex(int iIndex) {
		if (iIndex < m_PlayerList.size() - 1) {
			iIndex++;
		} else iIndex = 0;
		return iIndex;
	}
	
	private int getPlayerIndexByName(String iPlayer) {
		int r = -1;
		for (int i = 0; i < m_PlayerList.size(); i++) {
			Player p = m_PlayerList.get(i);
			if (p != null && p.getLogin().equals(iPlayer)) {
				r = i;
				break;
			}
		}
		
		return r;
	}
	
	public Player getPlayerByName(String iPlayer) {
		return getPlayerByIndex(getPlayerIndexByName(iPlayer));
	}
	
	public Player getPlayerByIndex(int iIndex) {
		Player r = null;
		if (iIndex >= 0 && iIndex < m_PlayerList.size()) r = m_PlayerList.get(iIndex);
		return r;
	}
	
	public int getIndexOfCurrentDealer() {
		return m_CurrentDealerIndex;
	}
	
	public int getIndexOfCurrentSmallBlind() {
		return nextPlayerIndex(m_CurrentDealerIndex);
	}
	public int getIndexOfCurrentBigBlind() {
		return nextPlayerIndex(getIndexOfCurrentSmallBlind());
	}

	public int getIndexOfCurrentPlayer() {
		return m_CurrentPlayerIndex;
	}
	
	public int getMinBetOfCurrentPlayer() {
		Player aPlayer = getPlayerByIndex(m_CurrentPlayerIndex);
		return m_CurrentHandMaxBet - aPlayer.getBet();
	}
	
	public Card getPublicCardByIndex(int iIndex) {
		Card r = null;
		if (iIndex >= 0 && iIndex < m_PublicCards.size()) r = m_PublicCards.get(iIndex);
		return r;
	}
	
	public String getGameStatus() {
		String r = "RUNNING";
		if (!m_GameHandIsInProgress) r = "PENDING";
		if (m_CurrentHandPhase > 3) r = "FINISHED";
		return r;
	}
	// simple getter
	
	public int getHands() {
		return m_Hands;
	}
	public int getID() {
		return m_ID;
	}
	public String getGameName() {
		return m_GameName;
	}
	public int getPot() {
		return m_Pot;
	}
	public int getCurrentHandPhase() {
		return m_CurrentHandPhase;
	}
	
}
