#ifndef POKERPLAYER_H
#define POKERPLAYER_H

#include <QObject>

class PokerPlayer : public QObject
{
    Q_OBJECT
private:
    QString il;
    QString iin_game;
    QString imoney;
    QString iact_bet;
    QString icard_1;
    QString icard_2;
    QString ilast_action;
public:
    PokerPlayer();
    QString user();
    bool in_game();
    double money();
    double act_bet();
    QString card_1();
    QString card_2();
    QString last_action();

    void setUser(QString l);
    void setIn_game(QString in_game);
    void setMoney(QString money);
    void setAct_bet(QString act_bet);
    void setCard_1(QString card_1);
    void setCard_2(QString card_2);
    void setLast_Action(QString last_action);
};

#endif // POKERPLAYER_H
