package plugins;

import java.sql.Connection;

import plugins.THOLDM.Card;
import plugins.THOLDM.Game;
import plugins.THOLDM.GameManager;
import plugins.THOLDM.Player;
import clserv.shared.I_ConnectionHandler;
import clserv.shared.I_Employee;
import clserv.shared.I_Plugin;

public final class THOLDM_v1 implements I_Plugin {

	@Override
	public String getDescription() {
		return "Texas Holdem";
	}

	@Override
	public String getVersion() {
		return "0.0.2";
	}

	@Override
	public void handle(I_ConnectionHandler iClientConnection, Connection iDatabaseConnection, String iCommand) {
		boolean success = false;
		Player aPlayer = null;
		Game aGame = null;
		
		plugins.THOLDM.Parameter p = new plugins.THOLDM.Parameter();
		p.parseCommand(iCommand);

		if (p.m_PlayerName != null) {
			if (p.m_RequestType.equals("new_game")) {
				int id = GameManager.getInstance().createNewGame(100, 2);
				aGame = GameManager.getInstance().getGame(id);
				if (aGame != null) {
					aGame.setGameName(p.m_GameName);
					if (p.m_PlayerName != null) aGame.addPlayer(p.m_PlayerName);
					setPlayerName(iClientConnection, aGame, p.m_PlayerName);
					iClientConnection.sendMessageToClientImmediately(iClientConnection.createResponse_ok(null));					
					iClientConnection.sendMessageToUserImmediately(p.m_PlayerName, iClientConnection.envelopClientMessage("THOLDM", "t=\"new_game\" game_id=\""+Integer.toString(id)+"\"", null));
//					informAllPlayersGameStatus(iClientConnection, aGame, p.m_PlayerName);
				} else {
					iClientConnection.sendMessageToClientImmediately(iClientConnection.createResponse_error(null));					
				}
				success = true;
			} else if (p.m_RequestType.equals("game_list")) {
				aGame = null;
				String msg = "";
				while ((aGame = GameManager.getInstance().iterateNextGame(aGame)) != null) {
					msg += "<game game_id=\""+Integer.toString(aGame.getID())+"\"";
					if (aGame.getGameName() != null) {
						msg += " game_name=\""+aGame.getGameName()+"\">";
					} else msg += ">";
					for (int i = 0; ; i++) {
						aPlayer = aGame.getPlayerByIndex(i);
						if (aPlayer == null) break;
						msg += "<player l=\""+aPlayer.getLogin()+"\" n=\""+aPlayer.getName()+"\"/>";
					}
					msg += "</game>";					
				}
				iClientConnection.sendMessageToClientImmediately(iClientConnection.createResponse_ok(null));					
				iClientConnection.sendMessageToUserImmediately(p.m_PlayerName, iClientConnection.envelopClientMessage("THOLDM", "t=\"game_list\"", msg));
				success = true;
			} else if (p.m_GameId >= 0 && (aGame = GameManager.getInstance().getGame(p.m_GameId)) != null) {
				if (p.m_RequestType.equals("bet")) {
					if (aGame.handleBet(p.m_PlayerName, p.m_Amount)) {
						iClientConnection.sendMessageToClientImmediately(iClientConnection.createResponse_ok(null));
						informAllPlayersGameStatus(iClientConnection, aGame, p.m_PlayerName);
					} else {
						iClientConnection.sendMessageToClientImmediately(iClientConnection.createResponse_error(null));					
					}
					success = true;
				} else if (p.m_RequestType.equals("join_game")) {
					if (aGame.addPlayer(p.m_PlayerName)) {
						setPlayerName(iClientConnection, aGame, p.m_PlayerName);
						iClientConnection.sendMessageToClientImmediately(iClientConnection.createResponse_ok(null));
						informAllPlayersGameStatus(iClientConnection, aGame, p.m_PlayerName);
					} else {
						iClientConnection.sendMessageToClientImmediately(iClientConnection.createResponse_error(null));					
					}
					success = true;
				} else if (p.m_RequestType.equals("leave_game")) {
					if (aGame.removePlayer(p.m_PlayerName)) {
						iClientConnection.sendMessageToClientImmediately(iClientConnection.createResponse_ok(null));
						informAllPlayersGameStatus(iClientConnection, aGame, p.m_PlayerName);
					} else {
						iClientConnection.sendMessageToClientImmediately(iClientConnection.createResponse_error(null));					
					}
					success = true;
				} else if (p.m_RequestType.equals("deal")) {
					if (aGame.startGame()) {
						iClientConnection.sendMessageToClientImmediately(iClientConnection.createResponse_ok(null));
						informAllPlayersGameStatus(iClientConnection, aGame, p.m_PlayerName);
					} else {
						iClientConnection.sendMessageToClientImmediately(iClientConnection.createResponse_error(null));					
					}
					success = true;
				} else if (p.m_RequestType.equals("delete_game") && p.m_GameId >= 0) {
					if (GameManager.getInstance().deleteGame(p.m_GameId)) {
						iClientConnection.sendMessageToClientImmediately(iClientConnection.createResponse_ok(null));
					} else {
						iClientConnection.sendMessageToClientImmediately(iClientConnection.createResponse_error(null));					
					}
					success = true;
				}
			}
		}
		if (!success) {
			iClientConnection.sendMessageToClientImmediately(iClientConnection.createResponse_error("invalid request"));
		}
	}

	private void setPlayerName(I_ConnectionHandler iClientConnection, Game iGame, String iPlayerName) {
		Player aPlayer = iGame.getPlayerByName(iPlayerName);
		String name;
		String firstname;
		I_Employee aEmployee = iClientConnection.getEmployeeManager().findeMitarbeiterMitLogin(iPlayerName);
		if (aPlayer != null && aEmployee != null) {
			name = aEmployee.getName();
			firstname = aEmployee.getFirstName();
			if (name != null && firstname != null) aPlayer.setName(firstname+" "+name);
		}
	}

	private void informAllPlayersGameStatus(I_ConnectionHandler iClientConnection, Game iGame, String iPlayerName) {
		for (int i = 0; ; i++) {
			Player aPlayer = iGame.getPlayerByIndex(i);
			if (aPlayer == null) break;
			String attr = attr_gameStatus(iGame, iPlayerName);
			String msg = msg_gameStatus(iGame, iPlayerName);
			iClientConnection.sendMessageToUserImmediately(aPlayer.getLogin(), iClientConnection.envelopClientMessage("THOLDM", attr, msg));
		}
	}

	private String attr_gameStatus(Game iGame, String iPlayerName) {
		String attr = "";
		attr = "t=\"game_state\" game_id=\""+Integer.toString(iGame.getID())+"\""+
			(iGame.getGameName() != null ? " game_name=\""+iGame.getGameName()+"\"" : "")+
			" count=\""+iGame.getHands()+"\" state=\""+iGame.getGameStatus()+"\" pot=\""+iGame.getPot()+
			"\" round=\""+iGame.getCurrentHandPhase()+"\""+
			" dealer=\""+iGame.getPlayerByIndex(iGame.getIndexOfCurrentDealer()).getLogin()+
			"\" smallblind=\""+iGame.getPlayerByIndex(iGame.getIndexOfCurrentSmallBlind()).getLogin()+
			"\" bigblind=\""+iGame.getPlayerByIndex(iGame.getIndexOfCurrentBigBlind()).getLogin()+
			"\" player=\""+iGame.getPlayerByIndex(iGame.getIndexOfCurrentPlayer()).getLogin()+
			"\" can_deal=\"?\" can_bet=\"?\" can_call=\"?\" can_raise=\"?\" can_check=\"?\" can_fold=\"?\""+
			" min_bet=\""+iGame.getMinBetOfCurrentPlayer()+"\"";
		for (int idx = 0; idx < 5; idx++) {
			Card card = iGame.getPublicCardByIndex(idx);
			if (card != null) {
				attr += " card"+(idx+1)+"=\""+card.getValue()+"\"";
			}
		}
		return attr;
	}

	private String msg_gameStatus(Game iGame, String iPlayerName) {
		String msg = "";
		for (int i = 0; ; i++) {
			Player aPlayer = iGame.getPlayerByIndex(i);
			if (aPlayer == null) break;
			Card card0 = aPlayer.getCardByIndex(0);
			Card card1 = aPlayer.getCardByIndex(1);
			msg += "<player l=\""+aPlayer.getLogin()+"\" n=\""+aPlayer.getName()+
			"\" last_action=\""+aPlayer.getLastActionName()+
//			"\" in_game=\""+(aPlayer.isPlaying() ? "1" : "0")+
			"\" money=\""+aPlayer.getCapital()+"\" bet=\""+aPlayer.getBet()+"\"";
			if (card0 != null) msg += " card1=\""+card0.getValue()+"\"";
			if (card1 != null) msg += " card2=\""+card1.getValue()+"\"";
			msg += "/>";
		}
		return msg;
	}

	@Override
	public boolean isDatabaseNeeded() {
		return false;
	}

	@Override
	public void onNewInstance() {
	}

}
