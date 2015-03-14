#ifndef POKERMANAGER_H
#define POKERMANAGER_H

#include <QObject>
#include "Options.h"
#include "pokerplayer.h"
#include <QDomElement>
#include <QDomDocument>
#include "EcoMiniReg.h"

class PokerManager : public QObject
{
    Q_OBJECT
public:
    PokerManager(QObject * parent = 0);
    ~PokerManager();
    void setGameId(QString game_id);
    bool updateGamestatus(QString message);

    QString gameId();
    int count();
    QString status();
    double pot();
    int round();
    double min_bigblind();
    QString dealer();
    QString smallblind();
    QString bigblind();
    QString nextplayer();
    bool can_bet();
    bool can_call();
    bool can_raise();
    bool can_check();
    bool can_fold();
    bool can_deal();
    double min_call();

    QString card_1();
    QString card_2();
    QString card_3();
    QString card_4();
    QString card_5();
    int          playerCount();
    PokerPlayer* player(int player_index);
private:
    int insertPlayer(QDomNode *player_node);
    void removeAllPlayers();

    QString igame_id;
    QString icount;
    QString istatus;
    QString ipot;
    QString iround;

    QString imin_bigblind;
    QString idealer;
    QString ismallblind;
    QString ibigblind;
    QString inextplayer;
    QString ican_bet;
    QString ican_call;
    QString ican_raise;
    QString ican_check;
    QString ican_fold;
    QString ican_deal;
    QString imin_call;

    QString icard_1;
    QString icard_2;
    QString icard_3;
    QString icard_4;
    QString icard_5;

    QHash<QString, PokerPlayer*> hash;
};

#endif // POKERMANAGER_H
