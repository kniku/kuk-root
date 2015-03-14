package plugins.THOLDM;

import java.util.Vector;

public class GameManager {
	// SINGLETON
	static private GameManager m_Instance = null;
	static Vector<Game> m_Games = new Vector<Game>(10);
	private GameManager() {}
	public static GameManager getInstance() {
		if (m_Instance == null) m_Instance = new GameManager();
		return m_Instance;
	}
	
	public int createNewGame(int iInitialPlayerCapital, int iBlindBet) {
		int r = -1;
		int idx;
		int max = m_Games.size();//capacity();
		for (idx = 0; idx < max; idx++) {
			if (m_Games.get(idx) == null) {
				m_Games.setElementAt(new Game(idx, iInitialPlayerCapital, iBlindBet), idx);
				r = idx;
				break;
			}
		}
		if (r < 0) {
			m_Games.addElement(new Game(max, iInitialPlayerCapital, iBlindBet));
			r = max;
		}
		return r;
	}
	
	public boolean deleteGame(int id) {
		boolean r = false;
		if (id >= 0 && id < m_Games.size()) {
			m_Games.set(id, null);
			r = true;
		}
		return r;
	}
	
	public Game getGame(int game_id) {
		Game r = null;
		if (game_id >= 0 && game_id < m_Games.size()) {
			r = m_Games.get(game_id);
		}
		return r;
	}
	
	public Game iterateNextGame(Game iGame) {
		Game r = null, last_game = null, aGame;
		int max = m_Games.size();
		for (int idx = 0; idx < max; idx++) {
			aGame = m_Games.get(idx);
			if (iGame == last_game && aGame != null) {
				r = aGame; 
				break;
			}
			if (aGame != null) last_game = aGame;
		}
		return r;
	}
}
