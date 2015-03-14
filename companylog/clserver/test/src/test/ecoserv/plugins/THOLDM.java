package test.ecoserv.plugins;

import junit.framework.Assert;

import mock.I_Mock_Server;
import mock.Mock_ConnectionHandler;

import org.junit.Test;

import clserv.clienthandler.NewClientHandlerFactory;
import clserv.shared.I_ClientHandler;
import clserv.shared.I_ConnectionHandler;

public class THOLDM implements I_Mock_Server {
	private boolean ok = false;
	private String m_Step_direct = "";
	private String m_Step_chat = "";
	private I_ConnectionHandler h_conn = new Mock_ConnectionHandler(this);
	
	private String delete_game_cmd(String iGameId) {
		return "<esm t=\"THOLDM_v1\"><msg t=\"delete_game\" l=\"kuk\" game_id=\""+iGameId+"\"/></esm>";
	}
	/*
	 * Anlegen und loeschen von Games
	 * MUSS: Ergebnis: Spielliste: Game 1,3; Im Game 1 spielen ret, in Game 3 spielen kuk,pam
	 */
	@Test
	public void CreateAndDeleteGame() {
		String cmd_create = "<esm t=\"THOLDM_v1\"><msg t=\"new_game\" l=\"kuk\"/></esm>";
		I_ClientHandler h = NewClientHandlerFactory.getHandler(h_conn, cmd_create);
		Assert.assertNotNull(h);
		if (h!= null) {
			try {
				// create game 0
				m_Step_chat = m_Step_direct = "ignore";
				h.handle(h_conn, cmd_create);
				// create game 1
				m_Step_chat = m_Step_direct = "new_game_1";
				h.handle(h_conn, cmd_create);
				// create game 2
				m_Step_chat = m_Step_direct = "ignore";
				h.handle(h_conn, cmd_create);
				// delete game 1
				m_Step_chat = m_Step_direct = "delete_game_1";
				h.handle(h_conn, delete_game_cmd("1"));
				// re-create game 1
				m_Step_chat = m_Step_direct = "new_game_1";
				h.handle(h_conn, cmd_create);
				m_Step_chat = "join_1_1"; m_Step_direct = "ok";
				h.handle(h_conn, "<esm t=\"THOLDM_v1\"><msg t=\"join_game\" game_id=\"1\" l=\"ret\"/></esm>");
				// create game 3
				m_Step_chat = m_Step_direct = "ignore";
				h.handle(h_conn, "<esm t=\"THOLDM_v1\"><msg t=\"new_game\" l=\"kuk\" game_name=\"testgame\"/></esm>");
				m_Step_chat = "join_3_1"; m_Step_direct = "ok";
				h.handle(h_conn, "<esm t=\"THOLDM_v1\"><msg t=\"join_game\" game_id=\"3\" l=\"pam\"/></esm>");
				m_Step_chat = "leave_1_1"; m_Step_direct = "ok";
				h.handle(h_conn, "<esm t=\"THOLDM_v1\"><msg t=\"leave_game\" game_id=\"1\" l=\"kuk\"/></esm>");
				m_Step_chat = m_Step_direct = "error";
				h.handle(h_conn, "<esm t=\"THOLDM_v1\"><msg t=\"join_game\" game_id=\"3\" l=\"pam\"/></esm>");
				// delete game 0
				m_Step_chat = m_Step_direct = "ignore";
				h.handle(h_conn, delete_game_cmd("0"));
				h.handle(h_conn, delete_game_cmd("2"));
				// delete game 0 again
				m_Step_chat = m_Step_direct = "error";
				h.handle(h_conn, delete_game_cmd("0"));
				ok = true;
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		Assert.assertTrue(ok);
	}

	@Test
	public void GameList() {
		String cmd = "<esm t=\"THOLDM_v1\"><msg t=\"game_list\" l=\"kuk\"/></esm>";
		I_ClientHandler h = NewClientHandlerFactory.getHandler(h_conn, cmd);
		Assert.assertNotNull(h);
		if (h!= null) {
			try {
				m_Step_chat = m_Step_direct = "game_list";
				h.handle(h_conn, cmd);
				ok = true;
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		Assert.assertTrue(ok);
	}

	@Test
	public void StartGame() {
		String command = "<esm t=\"THOLDM_v1\"><msg t=\"deal\" l=\"kuk\" game_id=\"3\"/></esm>";
		I_ClientHandler h = NewClientHandlerFactory.getHandler(h_conn, command);
		Assert.assertNotNull(h);
		if (h!= null) {
			try {
				m_Step_chat = m_Step_direct = "start_game";
				h.handle(h_conn, command);
				ok = true;
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		Assert.assertTrue(ok);
	}

	@Test
	public void PlayGame3() {
		String command = "<esm t=\"THOLDM_v1\"><msg t=\"bet\" l=\"pam\" game_id=\"3\" amount=\"10\"/></esm>";
		I_ClientHandler h = NewClientHandlerFactory.getHandler(h_conn, command);
		Assert.assertNotNull(h);
		if (h!= null) {
			try {
				m_Step_chat = m_Step_direct = "error";
				h.handle(h_conn, "<esm t=\"THOLDM_v1\"><msg t=\"bet\" l=\"kuk\" game_id=\"3\" amount=\"10\"/></esm>");
				m_Step_direct = "ok";
				m_Step_chat = "bet_1";
				h.handle(h_conn, "<esm t=\"THOLDM_v1\"><msg t=\"bet\" l=\"pam\" game_id=\"3\" amount=\"10\"/></esm>");
				m_Step_direct = "ok";
				m_Step_chat = "bet_2";
				h.handle(h_conn, "<esm t=\"THOLDM_v1\"><msg t=\"bet\" l=\"kuk\" game_id=\"3\" amount=\"9\"/></esm>");
				m_Step_direct = "ok";
				m_Step_chat = "bet_3";
				h.handle(h_conn, "<esm t=\"THOLDM_v1\"><msg t=\"bet\" l=\"pam\" game_id=\"3\" amount=\"0\"/></esm>");
				m_Step_direct = "ok";
				m_Step_chat = "bet_4";
				h.handle(h_conn, "<esm t=\"THOLDM_v1\"><msg t=\"bet\" l=\"kuk\" game_id=\"3\" amount=\"0\"/></esm>");
				m_Step_direct = "ok";
				m_Step_chat = "bet_5";
				h.handle(h_conn, "<esm t=\"THOLDM_v1\"><msg t=\"bet\" l=\"pam\" game_id=\"3\" amount=\"0\"/></esm>");
				m_Step_direct = "ok";
				m_Step_chat = "bet_6";
				h.handle(h_conn, "<esm t=\"THOLDM_v1\"><msg t=\"bet\" l=\"kuk\" game_id=\"3\" amount=\"0\"/></esm>");
				m_Step_direct = "ok";
				m_Step_chat = "bet_7";
				h.handle(h_conn, "<esm t=\"THOLDM_v1\"><msg t=\"bet\" l=\"pam\" game_id=\"3\" amount=\"0\"/></esm>");
				m_Step_direct = "ok";
				m_Step_chat = "bet_8";
				h.handle(h_conn, "<esm t=\"THOLDM_v1\"><msg t=\"bet\" l=\"kuk\" game_id=\"3\" amount=\"0\"/></esm>");
				ok = true;
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		Assert.assertTrue(ok);
	}
	
	@Override
	public void directFromServer(String iMessage) {
		if (m_Step_direct.equals("new_game_1")) {
			Assert.assertEquals("<ecm t=\"ok\"/>", iMessage);				
		} else  if (m_Step_direct.equals("start_game")) {
			Assert.assertEquals("<ecm t=\"ok\"/>", iMessage);				
		} else if (m_Step_direct.equals("delete_game_1")) {
			Assert.assertEquals("<ecm t=\"ok\"/>", iMessage);				
		} else if (m_Step_direct.equals("game_list")) {
			Assert.assertEquals("<ecm t=\"ok\"/>", iMessage);				
		} else if (m_Step_direct.equals("ok")) {
			Assert.assertEquals("<ecm t=\"ok\"/>", iMessage);			
		} else if (m_Step_direct.equals("error")) {
			Assert.assertEquals("<ecm t=\"error\"", iMessage.substring(0, 14));			
		} else if (m_Step_direct.equals("ignore")) {
			// ignore server response ...
		} else {
			Assert.fail("unhandled step/server directFromServer response");
		}
	}
	
	@Override
	public boolean chatFromServer(String iUser, String iMessage) {
		String message_wanted;
		if (m_Step_chat.equals("new_game_1")) {
			Assert.assertEquals("<ecm t=\"THOLDM\"><msg t=\"new_game\" game_id=\"1\"/></ecm>", iMessage);			
		} else if (m_Step_chat.equals("delete_game_1")) {
			Assert.assertEquals("<ecm t=\"ok\"/>", iMessage);			
		} else if (m_Step_chat.equals("game_list")) {
			Assert.assertEquals("<ecm t=\"THOLDM\">"+
					"<msg t=\"game_list\">"+
					"<game game_id=\"1\">"+
					"<player l=\"ret\" n=\"firstname name\"/>"+
					"</game>"+
					"<game game_id=\"3\" game_name=\"testgame\">"+
					"<player l=\"kuk\" n=\"firstname name\"/>"+
					"<player l=\"pam\" n=\"firstname name\"/>"+
					"</game>"+
					"</msg></ecm>", iMessage);			
		} else if (m_Step_chat.equals("join_1_1")) {
			message_wanted = fixCardValues("<ecm t=\"THOLDM\">"+
					"<msg t=\"game_state\" game_id=\"1\" count=\"0\" state=\"PENDING\" pot=\"0\" round=\"0\"" +
					" dealer=\"kuk\" smallblind=\"ret\" bigblind=\"kuk\" player=\"ret\""+
					" can_deal=\"?\" can_bet=\"?\" can_call=\"?\" can_raise=\"?\" can_check=\"?\" can_fold=\"?\""+
					" min_bet=\"0\">"+
					"<player l=\"kuk\" n=\"firstname name\" last_action=\"null\" money=\"100\" bet=\"0\"/>"+
					"<player l=\"ret\" n=\"firstname name\" last_action=\"null\" money=\"100\" bet=\"0\"/>"+
					"</msg></ecm>", iMessage);
			Assert.assertEquals(message_wanted, iMessage);
		} else if (m_Step_chat.equals("leave_1_1")) {
			message_wanted = fixCardValues("<ecm t=\"THOLDM\">"+
					"<msg t=\"game_state\" game_id=\"1\" count=\"0\" state=\"PENDING\" pot=\"0\" round=\"0\"" +
					" dealer=\"ret\" smallblind=\"ret\" bigblind=\"ret\" player=\"ret\""+
					" can_deal=\"?\" can_bet=\"?\" can_call=\"?\" can_raise=\"?\" can_check=\"?\" can_fold=\"?\""+
					" min_bet=\"0\">"+
					"<player l=\"ret\" n=\"firstname name\" last_action=\"null\" money=\"100\" bet=\"0\"/>"+
					"</msg></ecm>", iMessage);
			Assert.assertEquals(message_wanted, iMessage);
		} else if (m_Step_chat.equals("join_3_1")) {
			message_wanted = fixCardValues("<ecm t=\"THOLDM\">"+
					"<msg t=\"game_state\" game_id=\"3\" game_name=\"testgame\" count=\"0\" state=\"PENDING\" pot=\"0\" round=\"0\"" +
					" dealer=\"kuk\" smallblind=\"pam\" bigblind=\"kuk\" player=\"pam\""+
					" can_deal=\"?\" can_bet=\"?\" can_call=\"?\" can_raise=\"?\" can_check=\"?\" can_fold=\"?\""+
					" min_bet=\"0\">"+
					"<player l=\"kuk\" n=\"firstname name\" last_action=\"null\" money=\"100\" bet=\"0\"/>"+
					"<player l=\"pam\" n=\"firstname name\" last_action=\"null\" money=\"100\" bet=\"0\"/>"+
					"</msg></ecm>", iMessage);
			Assert.assertEquals(message_wanted, iMessage);
		} else if (m_Step_chat.equals("start_game")) {
			message_wanted = fixCardValues("<ecm t=\"THOLDM\">"+
					"<msg t=\"game_state\" game_id=\"3\" game_name=\"testgame\" count=\"1\" state=\"RUNNING\" pot=\"3\" round=\"0\"" +
					" dealer=\"kuk\" smallblind=\"pam\" bigblind=\"kuk\" player=\"pam\""+
					" can_deal=\"?\" can_bet=\"?\" can_call=\"?\" can_raise=\"?\" can_check=\"?\" can_fold=\"?\""+
					" min_bet=\"1\">"+
					"<player l=\"kuk\" n=\"firstname name\" last_action=\"null\" money=\"98\" bet=\"2\" card1=\"?\" card2=\"?\"/>"+
					"<player l=\"pam\" n=\"firstname name\" last_action=\"null\" money=\"99\" bet=\"1\" card1=\"?\" card2=\"?\"/>"+
					"</msg></ecm>", iMessage);
			Assert.assertEquals(message_wanted, iMessage);
		} else if (m_Step_chat.equals("bet_1")) {
			message_wanted = fixCardValues("<ecm t=\"THOLDM\">"+
					"<msg t=\"game_state\" game_id=\"3\" game_name=\"testgame\" count=\"1\" state=\"RUNNING\" pot=\"13\" round=\"0\"" +
					" dealer=\"kuk\" smallblind=\"pam\" bigblind=\"kuk\" player=\"kuk\""+
					" can_deal=\"?\" can_bet=\"?\" can_call=\"?\" can_raise=\"?\" can_check=\"?\" can_fold=\"?\""+
					" min_bet=\"9\">"+
					"<player l=\"kuk\" n=\"firstname name\" last_action=\"null\" money=\"98\" bet=\"2\" card1=\"?\" card2=\"?\"/>"+
					"<player l=\"pam\" n=\"firstname name\" last_action=\"null\" money=\"89\" bet=\"11\" card1=\"?\" card2=\"?\"/>"+
					"</msg></ecm>", iMessage);
			Assert.assertEquals(message_wanted, iMessage);
		} else if (m_Step_chat.equals("bet_2")) {
			message_wanted = fixCardValues("<ecm t=\"THOLDM\">"+
					"<msg t=\"game_state\" game_id=\"3\" game_name=\"testgame\" count=\"1\" state=\"RUNNING\" pot=\"22\" round=\"1\"" +
					" dealer=\"kuk\" smallblind=\"pam\" bigblind=\"kuk\" player=\"pam\""+
					" can_deal=\"?\" can_bet=\"?\" can_call=\"?\" can_raise=\"?\" can_check=\"?\" can_fold=\"?\""+
					" min_bet=\"0\" card1=\"?\" card2=\"?\" card3=\"?\">"+
					"<player l=\"kuk\" n=\"firstname name\" last_action=\"null\" money=\"89\" bet=\"11\" card1=\"?\" card2=\"?\"/>"+
					"<player l=\"pam\" n=\"firstname name\" last_action=\"null\" money=\"89\" bet=\"11\" card1=\"?\" card2=\"?\"/>"+
					"</msg></ecm>", iMessage);
			Assert.assertEquals(message_wanted, iMessage);
		} else if (m_Step_chat.equals("bet_3")) {
			message_wanted = fixCardValues("<ecm t=\"THOLDM\">"+
					"<msg t=\"game_state\" game_id=\"3\" game_name=\"testgame\" count=\"1\" state=\"RUNNING\" pot=\"22\" round=\"1\"" +
					" dealer=\"kuk\" smallblind=\"pam\" bigblind=\"kuk\" player=\"kuk\""+
					" can_deal=\"?\" can_bet=\"?\" can_call=\"?\" can_raise=\"?\" can_check=\"?\" can_fold=\"?\""+
					" min_bet=\"0\" card1=\"?\" card2=\"?\" card3=\"?\">"+
					"<player l=\"kuk\" n=\"firstname name\" last_action=\"null\" money=\"89\" bet=\"11\" card1=\"?\" card2=\"?\"/>"+
					"<player l=\"pam\" n=\"firstname name\" last_action=\"null\" money=\"89\" bet=\"11\" card1=\"?\" card2=\"?\"/>"+
					"</msg></ecm>", iMessage);
			Assert.assertEquals(message_wanted, iMessage);
		} else if (m_Step_chat.equals("bet_4")) {
			message_wanted = fixCardValues("<ecm t=\"THOLDM\">"+
					"<msg t=\"game_state\" game_id=\"3\" game_name=\"testgame\" count=\"1\" state=\"RUNNING\" pot=\"22\" round=\"2\"" +
					" dealer=\"kuk\" smallblind=\"pam\" bigblind=\"kuk\" player=\"pam\""+
					" can_deal=\"?\" can_bet=\"?\" can_call=\"?\" can_raise=\"?\" can_check=\"?\" can_fold=\"?\""+
					" min_bet=\"0\" card1=\"?\" card2=\"?\" card3=\"?\" card4=\"?\">"+
					"<player l=\"kuk\" n=\"firstname name\" last_action=\"null\" money=\"89\" bet=\"11\" card1=\"?\" card2=\"?\"/>"+
					"<player l=\"pam\" n=\"firstname name\" last_action=\"null\" money=\"89\" bet=\"11\" card1=\"?\" card2=\"?\"/>"+
					"</msg></ecm>", iMessage);
			Assert.assertEquals(message_wanted, iMessage);
		} else if (m_Step_chat.equals("bet_5")) {
			message_wanted = fixCardValues("<ecm t=\"THOLDM\">"+
					"<msg t=\"game_state\" game_id=\"3\" game_name=\"testgame\" count=\"1\" state=\"RUNNING\" pot=\"22\" round=\"2\"" +
					" dealer=\"kuk\" smallblind=\"pam\" bigblind=\"kuk\" player=\"kuk\""+
					" can_deal=\"?\" can_bet=\"?\" can_call=\"?\" can_raise=\"?\" can_check=\"?\" can_fold=\"?\""+
					" min_bet=\"0\" card1=\"?\" card2=\"?\" card3=\"?\" card4=\"?\">"+
					"<player l=\"kuk\" n=\"firstname name\" last_action=\"null\" money=\"89\" bet=\"11\" card1=\"?\" card2=\"?\"/>"+
					"<player l=\"pam\" n=\"firstname name\" last_action=\"null\" money=\"89\" bet=\"11\" card1=\"?\" card2=\"?\"/>"+
					"</msg></ecm>", iMessage);
			Assert.assertEquals(message_wanted, iMessage);
		} else if (m_Step_chat.equals("bet_6")) {
			message_wanted = fixCardValues("<ecm t=\"THOLDM\">"+
					"<msg t=\"game_state\" game_id=\"3\" game_name=\"testgame\" count=\"1\" state=\"RUNNING\" pot=\"22\" round=\"3\"" +
					" dealer=\"kuk\" smallblind=\"pam\" bigblind=\"kuk\" player=\"pam\""+
					" can_deal=\"?\" can_bet=\"?\" can_call=\"?\" can_raise=\"?\" can_check=\"?\" can_fold=\"?\""+
					" min_bet=\"0\" card1=\"?\" card2=\"?\" card3=\"?\" card4=\"?\" card5=\"?\">"+
					"<player l=\"kuk\" n=\"firstname name\" last_action=\"null\" money=\"89\" bet=\"11\" card1=\"?\" card2=\"?\"/>"+
					"<player l=\"pam\" n=\"firstname name\" last_action=\"null\" money=\"89\" bet=\"11\" card1=\"?\" card2=\"?\"/>"+
					"</msg></ecm>", iMessage);
			Assert.assertEquals(message_wanted, iMessage);
		} else if (m_Step_chat.equals("bet_7")) {
			message_wanted = fixCardValues("<ecm t=\"THOLDM\">"+
					"<msg t=\"game_state\" game_id=\"3\" game_name=\"testgame\" count=\"1\" state=\"RUNNING\" pot=\"22\" round=\"3\"" +
					" dealer=\"kuk\" smallblind=\"pam\" bigblind=\"kuk\" player=\"kuk\""+
					" can_deal=\"?\" can_bet=\"?\" can_call=\"?\" can_raise=\"?\" can_check=\"?\" can_fold=\"?\""+
					" min_bet=\"0\" card1=\"?\" card2=\"?\" card3=\"?\" card4=\"?\" card5=\"?\">"+
					"<player l=\"kuk\" n=\"firstname name\" last_action=\"null\" money=\"89\" bet=\"11\" card1=\"?\" card2=\"?\"/>"+
					"<player l=\"pam\" n=\"firstname name\" last_action=\"null\" money=\"89\" bet=\"11\" card1=\"?\" card2=\"?\"/>"+
					"</msg></ecm>", iMessage);
			Assert.assertEquals(message_wanted, iMessage);
		} else if (m_Step_chat.equals("bet_8")) {
			message_wanted = fixCardValues("<ecm t=\"THOLDM\">"+
					"<msg t=\"game_state\" game_id=\"3\" game_name=\"testgame\" count=\"1\" state=\"FINISHED\" pot=\"22\" round=\"4\"" +
					" dealer=\"kuk\" smallblind=\"pam\" bigblind=\"kuk\" player=\"pam\""+
					" can_deal=\"?\" can_bet=\"?\" can_call=\"?\" can_raise=\"?\" can_check=\"?\" can_fold=\"?\""+
					" min_bet=\"0\" card1=\"?\" card2=\"?\" card3=\"?\" card4=\"?\" card5=\"?\">"+
					"<player l=\"kuk\" n=\"firstname name\" last_action=\"null\" money=\"89\" bet=\"11\" card1=\"?\" card2=\"?\"/>"+
					"<player l=\"pam\" n=\"firstname name\" last_action=\"null\" money=\"89\" bet=\"11\" card1=\"?\" card2=\"?\"/>"+
					"</msg></ecm>", iMessage);
			Assert.assertEquals(message_wanted, iMessage);
		} else if (m_Step_chat.equals("ignore")) {
			// ignore server response ...
		} else {
			Assert.fail("unhandled step/server chatFromServer response");
		}
		return false;
	}
	
	private String fixCardValues(String iMessageWanted, String iMessage) {
		int i1_1 = 0, i1_2 = 0, i2_1 = 0, i2_2 = 0;
		do {
			i1_1 = iMessageWanted.indexOf("card", i1_2);
			if (i1_1 >= 0) i1_1 = iMessageWanted.indexOf("\"", i1_1);
			i2_1 = iMessage.indexOf("card", i2_2);
			if (i2_1 >= 0) i2_1 = iMessage.indexOf("\"", i2_1);
			if (i1_1 >= 0 && i2_1 >= 0) {
				i1_2 = iMessageWanted.indexOf("\"", i1_1 + 1);
				i2_2 = iMessage.indexOf("\"", i2_1 + 1);
				iMessageWanted = iMessageWanted.substring(0, i1_1)+iMessage.substring(i2_1, i2_2 + 1)+iMessageWanted.substring(i1_2+1);
			} else break;
		} while (true);
		return iMessageWanted;
	}
}
