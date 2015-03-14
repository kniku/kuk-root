#include "pokermanager.h"

PokerManager::PokerManager(QObject * parent) : QObject(parent) {

}

PokerManager::~PokerManager() {
    removeAllPlayers();
}

int PokerManager::insertPlayer(QDomNode *player_node) {
    PokerPlayer *pokerPlayer=new PokerPlayer();


    if (player_node && player_node->isElement()) {
        QDomElement player = player_node->toElement();
        pokerPlayer->setUser(player.attribute( "l", "" ));
        pokerPlayer->setIn_game(player.attribute( "in_game", "" ));
        pokerPlayer->setMoney(player.attribute( "money", "" ));
        pokerPlayer->setAct_bet(player.attribute( "bet", "" ));
        pokerPlayer->setCard_1(player.attribute( "card1", "" ));
        pokerPlayer->setCard_2(player.attribute( "card2", "" ));
        pokerPlayer->setLast_Action(player.attribute( "last_action", "" ));
        hash.insert(QString::number(playerCount()),pokerPlayer);
    }
    return playerCount();
}


void PokerManager::removeAllPlayers() {
    foreach(PokerPlayer * player_value, hash) {
        if (player_value) delete player_value;
    }
    hash.clear();
}

void  PokerManager::setGameId(QString game_id) {
    igame_id=game_id;
}

/*
<!-- ******** Spielstatus -->
<ecm t="THOLDM">
       <msg t="game_status"
                        game_id="$GAMEID"
                        count="$ANZAHL_DER_SPIELE_INSGESAMMT"
                        status="ROUND_STARTED od. NEXT_PLAYER od. ROUND_FINISHED"
                        pot="$AKTUELLE_POTHOEHE"
                        round="$ANZAHL_DER_RUNDEN_DES_AKTUELLEN_SPIELS (max. 4)"

                        min_bigblind="$MINDESTHOEHE_DES_BIGBLIND"
                        dealer="$USER"
                        smallblind="$USER"
                        bigblind="$USER"
                        nextplayer="$USER"
                        can_bet="TRUE/FALSE - bieten/mitgehen ev. mit can_call zusammenlegen"
                        can_call="TRUE/FALSE - mitgehen"
                        can_raise="TRUE/FALSE - erhoehen"
                        can_check="TRUE/FALSE - weiterschieben auf den naechsten"
                        can_fold="TRUE/FALSE - aussteigen"
                        can_deal="TRUE/FALSE - dealen moeglich"
                        min_call="$MINDESTHOEHE_ZUM_MITGEHEN (CALL)"

                        card_1="0-51" <!--flop -->
                        card_2="0-51" <!--flop -->
                        card_3="0-51" <!--flop -->
                        card_4="0-51" <!--turn -->
                        card_5="0-51" <!--river -->
                        >

                        <player
                                l="$USER"
                                in_game="TRUE/FALSE - noch im Spiel?"
                                money="$NOCH_VERFUEGBARES_KAPITAL"
                                act_bet="$AKTUELL_IN_DEN_POT_EINGEZAHNLTEN_BETRAG"
                                card_1="0-51"
                                card_2="0-51"
                        />
                        <!--... next Player -->
                </msg>
</ecm>

  */
bool PokerManager::updateGamestatus(QString message) {
    bool handled=false;
    Options *options=getGlobalOptionsObj();
    if (message.size()>0) {
        QDomDocument doc( "AdBookML" );
        doc.setContent(message);
        QDomElement root = doc.documentElement();
        QString t=root.attribute("t", "" );
        if (t=="THOLDM") {
            QDomNodeList msg=root.elementsByTagName("msg");
            if (msg.count()>0) {
                QDomNode msg0_node=msg.item(0);
                if (msg0_node.isElement()) {
                    QDomElement msg0 = msg0_node.toElement();
                    QString msg_t=msg0.attribute("t", "" );
                    if (msg_t=="game_state") {
                        QString id = msg0.attribute( "id", "" );
                        if (id.size()==0) {
                            id = msg0.attribute( "game_id", "" );
                        }
                        if (igame_id.size()==0 || id==igame_id) {
                            handled=true;
                            igame_id=id;
                            icount=msg0.attribute( "count", "" );
                            istatus=msg0.attribute( "state", "" );
                            ipot=msg0.attribute( "pot", "" );
                            iround=msg0.attribute( "round", "" );

                            imin_bigblind=msg0.attribute( "min_bigblind", "" );
                            idealer=msg0.attribute( "dealer", "" );
                            ismallblind=msg0.attribute( "smallblind", "" );
                            ibigblind=msg0.attribute( "bigblind", "" );
                            inextplayer=msg0.attribute( "player", "" );
                            ican_bet=msg0.attribute( "can_bet", "" );
                            ican_call=msg0.attribute( "can_call", "" );
                            ican_raise=msg0.attribute( "can_raise", "" );
                            ican_check=msg0.attribute( "can_check", "" );
                            ican_fold=msg0.attribute( "can_fold", "" );
                            ican_deal=msg0.attribute( "can_deal", "" );
                            imin_call=msg0.attribute( "min_call", "" );

                            icard_1=msg0.attribute( "card1", "" );
                            icard_2=msg0.attribute( "card2", "" );
                            icard_3=msg0.attribute( "card3", "" );
                            icard_4=msg0.attribute( "card4", "" );
                            icard_5=msg0.attribute( "card5", "" );

                            removeAllPlayers();
                            QDomNodeList players=msg0.elementsByTagName("player");

                            int me_index=-1;
                            int player_index;
                            QDomNode player_node;
                            for (player_index=0;player_index<players.count();player_index++) {
                                player_node=players.item(player_index);
                                if (player_node.isElement()) {
                                    QDomElement player = player_node.toElement();
                                    QString l = player.attribute( "l", "" );
                                    if (l==options->getUsername()) {
                                        me_index=player_index;
                                        break;
                                    }
                                }
                            }
                            if (me_index>=0) {
                                /* der Eigene Spieler immer als erstes hinzufuegen, die Runde beginnt somit immer mit dem eigenen Spieler */
                                player_node=players.item(me_index);
                                insertPlayer(&player_node);
                                /* danach alle Spieler nach dem eigenen Spieler */
                                for (player_index=me_index+1;player_index<players.count();player_index++) {
                                    player_node=players.item(player_index);
                                    insertPlayer(&player_node);
                                }
                                /* danach alle Spieler vor dem eigenen Spieler */
                                if (me_index>0) {
                                    for (player_index=0;player_index<me_index;player_index++) {
                                        player_node=players.item(player_index);
                                        insertPlayer(&player_node);
                                    }
                                }
                            } else {
                                /* zuschauer! */
                                for (player_index=0;player_index<players.count();player_index++) {
                                    player_node=players.item(player_index);
                                    insertPlayer(&player_node);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return handled;
}

QString PokerManager::gameId() {return igame_id;}
int PokerManager::count() {return icount.toInt();}
QString PokerManager::status() {return istatus;}
double PokerManager::pot() {return ipot.toDouble();}
int PokerManager::round() {return iround.toInt();}
double PokerManager::min_bigblind() {return imin_bigblind.toDouble();}
QString PokerManager::dealer() {return idealer;}
QString PokerManager::smallblind() {return ismallblind;}
QString PokerManager::bigblind() {return ibigblind;}
QString PokerManager::nextplayer() {return inextplayer;}
bool PokerManager::can_bet() {
    if (ican_bet=="TRUE") return true;
    if (ican_bet=="true") return true;
    if (ican_bet=="1") return true;
    return false;
}
bool PokerManager::can_call() {
    if (ican_call=="TRUE") return true;
    if (ican_call=="true") return true;
    if (ican_call=="1") return true;
    return false;
}
bool PokerManager::can_raise() {
    if (ican_raise=="TRUE") return true;
    if (ican_raise=="true") return true;
    if (ican_raise=="1") return true;
    return false;
}
bool PokerManager::can_check() {
    if (ican_check=="TRUE") return true;
    if (ican_check=="true") return true;
    if (ican_check=="1") return true;
    return false;
}
bool PokerManager::can_fold() {
    if (ican_fold=="TRUE") return true;
    if (ican_fold=="true") return true;
    if (ican_fold=="1") return true;
    return false;
}
bool PokerManager::can_deal() {
    if (ican_deal=="TRUE") return true;
    if (ican_deal=="true") return true;
    if (ican_deal=="1") return true;
    return false;
}
double PokerManager::min_call() {return imin_call.toDouble();}

QString PokerManager::card_1() {return icard_1;}
QString PokerManager::card_2() {return icard_2;}
QString PokerManager::card_3() {return icard_3;}
QString PokerManager::card_4() {return icard_4;}
QString PokerManager::card_5() {return icard_5;}


int PokerManager::playerCount() {
    return hash.count();
}

PokerPlayer* PokerManager::player(int player_index) {
    if (player_index>=0 && player_index<playerCount()) {
        if (hash.contains(QString::number(player_index))) {
            return hash.value(QString::number(player_index));
        }
    }
    return 0;
}
