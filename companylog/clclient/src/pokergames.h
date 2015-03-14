/********************************************************************************************************
 * PROGRAM      : companylog
 * DATE - TIME  : Sunday 06 Apr 2009 - 10h15
 * AUTHOR       : Martin Paar ( martin.paar@gmx.at )
 * FILENAME     : pokergames.h
 * LICENSE      :
 * COMMENTARY   :
 ********************************************************************************************************/

#ifndef POKERGAMES_H
#define POKERGAMES_H

#include <QDockWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTreeView>
#include <QStandardItemModel>
#include "Options.h"
#include <QDomElement>
#include <QDomDocument>
#include "EcoMiniReg.h"

class PokerGames : public QDockWidget
{
    Q_OBJECT
public:
    PokerGames(QWidget *parent);
    ~PokerGames();
    bool /* handled */ updateGameList(QString message);
private:
    void addGame(QString name, QString gameid);
    void addPlayer(QString shortName, QString longName, QString gameid);
    void reloadGameList();
    void registerNewGame();
    bool initGamelist();
    bool appendPlayer(QString id, QString player_name, QString player_shortname);
    bool appendGame(QString id, QString game_name);


    QStandardItemModel *model;
    QWidget     *panel;
    QTreeView	*qTreeView;
    QLabel 		*qLabelNewGameName;
    QLineEdit	*qLineEditGameName;
    QPushButton *qPushButtonNewGame;
    QPushButton *qPushButtonJoinGame;
    QPushButton *qPushButtonReloadGameList;
    QHBoxLayout *layout;
    QHBoxLayout *layout2;
    QVBoxLayout *layoutV;
    QString     selectedID;

public slots:
    void qPushButtonJoinGame_Clicked(bool checked);
    void qLineEditGameName_ReturnPressed();
    void qPushButtonNewGame_Clicked(bool checked);
    void qPushButtonReloadGameList_Clicked(bool checked);
    void onLineClicked(const QModelIndex & index);
#if 0
signals:
    void onPersonAdded(QString shortName);
#endif

};

#endif // POKERGAMES_H
