#include "pokergames.h"

#define GAMENAME_INDEX              0
#define PLAYERNAME_INDEX            1
#define SHORTNAME_INDEX             2
#define GAMEID_INDEX                3

#define COLUMN_COUNT 4

PokerGames::~PokerGames() {
    if (model) {
        delete model;
    }
#if 0
    if (layout) {
        delete layout;
    }
    if (layout2) {
        delete layout2;
    }
    if (layoutV) {
        delete layoutV;
    }
#endif
}

PokerGames::PokerGames(QWidget *parent) : QDockWidget(parent) {
    Options *options=getGlobalOptionsObj();
    panel=new QWidget(this);

    layout = new QHBoxLayout;
    layout2 = new QHBoxLayout;
    layoutV = new QVBoxLayout;
    qLabelNewGameName=new QLabel("Neues Spiel");
    qLabelNewGameName->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));

    qLineEditGameName=new QLineEdit();
    qLineEditGameName->setMaxLength(20);
    qLineEditGameName->setMaximumWidth(200);
    qLineEditGameName->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
    connect(qLineEditGameName,SIGNAL(returnPressed()),this,SLOT(qLineEditGameName_ReturnPressed()));

    qPushButtonNewGame = new QPushButton(*options->pixmapCacheIcons()->getIcon("filenew.png"),"");
    qPushButtonNewGame->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
    qPushButtonNewGame->setAutoDefault(true);
    qPushButtonNewGame->setToolTip(tr("neues Spiel eröffnen"));
    connect(qPushButtonNewGame,SIGNAL(clicked (bool)),this,SLOT(qPushButtonNewGame_Clicked(bool)));

    qPushButtonJoinGame = new QPushButton(*options->pixmapCacheIcons()->getIcon("in.png"),"Spiel beitreten");
    qPushButtonJoinGame->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
    qPushButtonJoinGame->setAutoDefault(false);
    qPushButtonJoinGame->setToolTip(tr("Spiel beitreten"));
    connect(qPushButtonJoinGame,SIGNAL(clicked (bool)),this,SLOT(qPushButtonJoinGame_Clicked(bool)));

    qPushButtonReloadGameList = new QPushButton(*options->pixmapCacheIcons()->getIcon("reload.png"),"");
    qPushButtonReloadGameList->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
    qPushButtonReloadGameList->setAutoDefault(false);
    qPushButtonReloadGameList->setToolTip(tr("Spiel beitreten"));
    connect(qPushButtonReloadGameList,SIGNAL(clicked (bool)),this,SLOT(qPushButtonReloadGameList_Clicked(bool)));

    model=new QStandardItemModel(0,COLUMN_COUNT);
    initGamelist();

    qTreeView=new QTreeView();
    qTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    qTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    qTreeView->setModel(model);
    connect(qTreeView, SIGNAL(pressed(QModelIndex)), this, SLOT(onLineClicked(QModelIndex)));


    layout->insertStretch(-1);
    layout->addWidget(qLabelNewGameName);
    layout->addWidget(qLineEditGameName);
    layout->addWidget(qPushButtonNewGame);
    layout2->insertStretch(-1);
    layout2->addWidget(qPushButtonJoinGame);
    layout2->addWidget(qPushButtonReloadGameList);

    layoutV->addLayout(layout);
    layoutV->addLayout(layout2);
    layoutV->addWidget(qTreeView);
    panel->setLayout(layoutV);

    setWidget(panel);
}


bool PokerGames::initGamelist() {
    model->clear();
    model->setColumnCount(COLUMN_COUNT);
    model->setHeaderData(GAMENAME_INDEX,    Qt::Horizontal, tr("Spiel"));
    model->setHeaderData(PLAYERNAME_INDEX,  Qt::Horizontal, tr("Mitspieler"));
    model->setHeaderData(SHORTNAME_INDEX,   Qt::Horizontal, tr(""));
    model->setHeaderData(GAMEID_INDEX,      Qt::Horizontal, tr("ID"));
    return true;
}


bool PokerGames::appendPlayer(QString id, QString player_name, QString player_shortname) {
    bool rv=false;
    if (id.size()>0 && (player_name.size()>0 || player_shortname.size()>0) && model->rowCount()>0) {
        rv=true;
        QModelIndex parent_modelindex=model->index(model->rowCount()-1,GAMENAME_INDEX);
        model->insertRow (0, parent_modelindex);
        model->setData(model->index(0,PLAYERNAME_INDEX,parent_modelindex),player_name);
        model->setData(model->index(0,SHORTNAME_INDEX,parent_modelindex),player_shortname);
        model->setData(model->index(0,GAMEID_INDEX,parent_modelindex),id);
    }
    return rv;
}

bool PokerGames::appendGame(QString id, QString game_name) {
    bool rv=false;
    if (id.size()>0 && game_name.size()>0) {
        rv=true;
        model->insertRows (model->rowCount(), 1);
        model->insertColumns (0, COLUMN_COUNT, model->index(model->rowCount()-1,GAMENAME_INDEX));
        model->setData(model->index(model->rowCount()-1,GAMENAME_INDEX),game_name/*,Qt::DisplayRole*/);
        model->setData(model->index(model->rowCount()-1,GAMEID_INDEX),id/*,Qt::DisplayRole*/);
    }
    return rv;
}

/*
  <ecm t="THOLDM">
        <msg t="game_list">
                <game id="$GAMEID" game_name="$GAMENAME">
                        <player l="$USER" n="$USER_LONGNAME"/>
                        <player l="$USER" n="$USER_LONGNAME"/>
                        <player l="$USER" n="$USER_LONGNAME"/>
                        <player l="$USER" n="$USER_LONGNAME"/>
                </game>
                <game id="$GAMEID" game_name="$GAMENAME">
                        <player l="$USER" n="$USER_LONGNAME"/>
                        <player l="$USER" n="$USER_LONGNAME"/>
                        <player l="$USER" n="$USER_LONGNAME"/>
                        <player l="$USER" n="$USER_LONGNAME"/>
                </game>
        </msg>
</ecm>
*/
bool PokerGames::updateGameList(QString message) {
    bool handled=false;

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
                if (msg_t=="game_list") {
                    handled=true;
                    initGamelist();
                    QDomNodeList games=msg0.elementsByTagName("game");

                    for (int game_index=0;game_index<games.count();game_index++) {
                        QDomNode game_node=games.item(game_index);
                        if (game_node.isElement()) {
                            QDomElement game = game_node.toElement();
                            QString id = game.attribute( "game_id", "" );
                            QString game_name = game.attribute( "game_name", "" );

                            if (appendGame(id, game_name)) {
                                QDomNodeList players=game.elementsByTagName("player");
                                for (int player_index=0;player_index<players.count();player_index++) {
                                    QDomNode player_node=players.item(player_index);
                                    if (player_node.isElement()) {
                                        QDomElement player = player_node.toElement();
                                        QString player_shortname = player.attribute( "l", "" );
                                        QString player_name = player.attribute( "n", "" );
                                        appendPlayer(id, player_name, player_shortname);
                                    }
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

void PokerGames::qLineEditGameName_ReturnPressed() {
    registerNewGame();
}
void PokerGames::qPushButtonNewGame_Clicked(bool checked) {
    if (checked) {}
    registerNewGame();
}
void PokerGames::qPushButtonReloadGameList_Clicked(bool checked) {
    if (checked) {}
    reloadGameList();
}

void PokerGames::onLineClicked( const QModelIndex & index) {
    int row=index.row();
    if (row>=0) {
        QVariant var=model->data(model->index(row,GAMEID_INDEX)/*,Qt::DisplayRole*/);
        selectedID=var.toString();
    }
}


/*
<esm t="THOLDM" l="$USER">
        <msg t="game_list"/>
</esm>
*/
void PokerGames::reloadGameList() {
    Options *options=getGlobalOptionsObj();
    EcoMiniReg* ecoMiniReg=getGlobalEcoMiniReg();

    if (options && ecoMiniReg) {
        fprintf(stderr,"Request Poker Gamelist\n");
        QDomDocument doc;
        QDomElement esm=doc.createElement("esm");
        QDomElement msg=doc.createElement("msg");

        esm.setAttribute("t","THOLDM");
        msg.setAttribute("t","game_list");
        msg.setAttribute("l",options->getUsername());

        doc.appendChild(esm);
        esm.appendChild(msg);

        QString qmessage=doc.toString();

        fprintf(stderr,"Request Poker Gamelist: XML-String:\n%s\n",qmessage.toAscii().data());

       ecoMiniReg->sendCommandToServer(qmessage,options->serverPort()+1);
    }
}
/*
    <esm t="THOLDM" l="$USER">
        <msg t="new_game" game_name="$GAMENAME"/>
    </esm>
*/
void PokerGames::registerNewGame() {
    Options *options=getGlobalOptionsObj();
    EcoMiniReg* ecoMiniReg=getGlobalEcoMiniReg();

    if (options && ecoMiniReg && qLineEditGameName->text().size()>0) {
        fprintf(stderr,"Register new Poker Game:%s\n",qLineEditGameName->text().toAscii().data());
        QDomDocument doc;
        QDomElement esm=doc.createElement("esm");
        QDomElement msg=doc.createElement("msg");

        esm.setAttribute("t","THOLDM");

        msg.setAttribute("t","new_game");
        msg.setAttribute("game_name",qLineEditGameName->text().replace(">","SP9PS-SP8PS"));
        msg.setAttribute("l",options->getUsername());

        doc.appendChild(esm);
        esm.appendChild(msg);

        QString qmessage=doc.toString();
        qmessage=qmessage.replace("SP9PS-SP8PS","&gt;");

        fprintf(stderr,"Register new Game: XML-String:\n%s\n",qmessage.toAscii().data());

        ecoMiniReg->sendCommandToServer(qmessage,options->serverPort()+1);
        qLineEditGameName->setText("");
    }
}

/*
<esm t="THOLDM" l="$USER">
        <msg t="join_game"/>
</esm>
*/
void PokerGames::qPushButtonJoinGame_Clicked(bool checked) {
    if (checked) {}
    Options *options=getGlobalOptionsObj();
    EcoMiniReg* ecoMiniReg=getGlobalEcoMiniReg();

    if (options && ecoMiniReg && selectedID.size()>0) {
        fprintf(stderr,"Join new Poker Game:%s\n",selectedID.toAscii().data());
        QDomDocument doc;
        QDomElement esm=doc.createElement("esm");
        QDomElement msg=doc.createElement("msg");

        esm.setAttribute("t","THOLDM");

        msg.setAttribute("t","join_game");
        msg.setAttribute("game_id",selectedID);
        msg.setAttribute("l",options->getUsername());

        doc.appendChild(esm);
        esm.appendChild(msg);

        QString qmessage=doc.toString();
        fprintf(stderr,"Join new Game: XML-String:\n%s\n",qmessage.toAscii().data());

        ecoMiniReg->sendCommandToServer(qmessage,options->serverPort()+1);
        qLineEditGameName->setText("");
    }
}


