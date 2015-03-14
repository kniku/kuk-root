#include "pokerplayer.h"

PokerPlayer::PokerPlayer()
{
    il="";
    iin_game="";
    imoney="";
    iact_bet="";
    icard_1="";
    icard_2="";
}


QString PokerPlayer::user() {return il;}
bool PokerPlayer::in_game() {
    if (iin_game=="TRUE") return true;
    if (iin_game=="true") return true;
    if (iin_game=="1") return true;
    return false;
}
double PokerPlayer::money() {return imoney.toDouble();}
double PokerPlayer::act_bet() {return iact_bet.toDouble();}
QString PokerPlayer::card_1() {return icard_1;}
QString PokerPlayer::card_2() {return icard_2;}
QString PokerPlayer::last_action() {return ilast_action;}

void PokerPlayer::setUser(QString l) {
    il=l;
}
void PokerPlayer::setIn_game(QString in_game) {
    iin_game=in_game;
}
void PokerPlayer::setMoney(QString money) {
    imoney=money;
}
void PokerPlayer::setAct_bet(QString act_bet) {
    iact_bet=act_bet;
}
void PokerPlayer::setCard_1(QString card_1) {
    icard_1=card_1;
}
void PokerPlayer::setCard_2(QString card_2) {
    icard_2=card_2;
}
void PokerPlayer::setLast_Action(QString last_action) {
    ilast_action=last_action;
}
