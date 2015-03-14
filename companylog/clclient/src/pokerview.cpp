#include "pokerview.h"

PokerView::PokerView(QWidget *parent) : QWidget(parent)
{
    //Options *options=getGlobalOptionsObj();
    pixmapRequester=new PixmapRequester();
    pixmapRequester->setXScaling(30);
    connect(pixmapRequester,SIGNAL(pixmapFound (QString,QPixmap*)),this,SLOT(pixmapFound (QString,QPixmap*)));

    pokerManager=new PokerManager();
    pixmapCacheCards=new PixmapCache();
    pixmapCacheCards->setPixmapSubpath("poker/cards");
    pixmapCacheTable=new PixmapCache();
    pixmapCacheTable->setPixmapSubpath("poker/table");
    //QVBoxLayout *layoutV = new QVBoxLayout(this);
//    this->setBackgroundRole(QPalette::ButtonText);
//    this->setForegroundRole(QPalette::ButtonText);
//    this->setGeometry(10,10,100,100);
//    this->show();
    qGraphicsScene=new QGraphicsScene(0,0,870,598);
    qGraphicsView=new QGraphicsView(qGraphicsScene,this);
    qGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    qGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //qGraphicsView->viewport()->setGeometry(qGraphicsScene->sceneRect().x(),qGraphicsScene->sceneRect().y(),qGraphicsScene->sceneRect().right()+50,qGraphicsScene->sceneRect().bottom()+50);
    //qGraphicsView->viewport()->setBackgroundRole(QPalette::ButtonText);
    //layoutV->addWidget (qGraphicsView->viewport());

    player_rect_brush=new QBrush();
    player_rect_color = new QColor(255,200,255,30);
    player_rect_brush->setColor(*player_rect_color);
    player_rect_brush->setStyle(Qt::SolidPattern);

    player_rect_pen = new QPen();
    player_rect_pen->setStyle(Qt::SolidLine);
    player_rect_pen->setWidth(3);
    player_rect_pen->setBrush(QColor(20,20,20,20));
    player_rect_pen->setJoinStyle(Qt::RoundJoin);
    player_rect_pen->setCapStyle(Qt::RoundCap);

    next_player_rect_pen = new QPen();
    next_player_rect_pen->setStyle(Qt::SolidLine);
    next_player_rect_pen->setWidth(5);
    next_player_rect_pen->setBrush(QColor(20,255,20,100));
    next_player_rect_pen->setJoinStyle(Qt::RoundJoin);
    next_player_rect_pen->setCapStyle(Qt::RoundCap);

}

PokerView::~PokerView() {
    if (pokerManager) delete pokerManager;
    if (pixmapCacheCards) delete pixmapCacheCards;
    if (pixmapCacheTable) delete pixmapCacheTable;
    if (pixmapRequester) delete pixmapRequester;
    if (player_rect_brush) delete player_rect_brush;
    if (player_rect_color) delete player_rect_color;
    if (player_rect_pen) delete player_rect_pen;
    if (next_player_rect_pen) delete next_player_rect_pen;
    waitForPicturehash.clear();
    if (qGraphicsScene) {
        if (qGraphicsView) {
            qGraphicsView->setScene(0);
        }
        delete qGraphicsScene;
    }
}

void PokerView::setGameId(QString game_id) {
    if (pokerManager) {
        pokerManager->setGameId(game_id);
    }
}

QString PokerView::gameId() {
    if (pokerManager) {
        pokerManager->gameId();
    }
    return "";
}

bool PokerView::updateGamestatus(QString message)
{
    bool handled=pokerManager->updateGamestatus(message);
    if (handled==true) {
        updateViewX();
    }
    return handled;
}

bool PokerView::updateViewX() {
    Options *options=getGlobalOptionsObj();
    qGraphicsScene->clear();
    waitForPicturehash.clear();

    QGraphicsPixmapItem *table_item=new QGraphicsPixmapItem(*pixmapCacheTable->getPixmap("table.png"));
    table_item->setPos(0,0);
    qGraphicsScene->addItem(table_item);
    QGraphicsPixmapItem *item=new QGraphicsPixmapItem(*pixmapCacheTable->getPixmap("handranking.png"),table_item);
    item->setPos(20,440);
    //qGraphicsScene->addItem(item);

    int base_card_x=300;
    int base_card_y=200;
    if (pokerManager->card_1().size()>0) {
        QGraphicsPixmapItem *card=new QGraphicsPixmapItem(*pixmapCacheCards->getPixmap(pokerManager->card_1()+".png"),table_item,qGraphicsScene);
        card->setPos(base_card_x,base_card_y);
        if (pokerManager->card_2().size()>0) {
            QGraphicsPixmapItem *card=new QGraphicsPixmapItem(*pixmapCacheCards->getPixmap(pokerManager->card_2()+".png"),table_item,qGraphicsScene);
            card->setPos(base_card_x+50,base_card_y);
            if (pokerManager->card_3().size()>0) {
                QGraphicsPixmapItem *card=new QGraphicsPixmapItem(*pixmapCacheCards->getPixmap(pokerManager->card_3()+".png"),table_item,qGraphicsScene);
                card->setPos(base_card_x+100,base_card_y);
                if (pokerManager->card_4().size()>0) {
                    QGraphicsPixmapItem *card=new QGraphicsPixmapItem(*pixmapCacheCards->getPixmap(pokerManager->card_4()+".png"),table_item,qGraphicsScene);
                    card->setPos(base_card_x+150,base_card_y);
                    if (pokerManager->card_5().size()>0) {
                        QGraphicsPixmapItem *card=new QGraphicsPixmapItem(*pixmapCacheCards->getPixmap(pokerManager->card_5()+".png"),table_item,qGraphicsScene);
                        card->setPos(base_card_x+200,base_card_y);
                    }
                }
            }
        }
    }
    QString pot_text="Pot: "+ QString::number(pokerManager->pot());
    QGraphicsTextItem *pot_item = new QGraphicsTextItem(pot_text,table_item,qGraphicsScene);
    pot_item->setPos(300,150);


    PokerPlayer *pokerPlayer=0;

    int player_count=pokerManager->playerCount();
    int basex=0;
    int basey=0;
    double player_rect_width=170.0;
    double player_rect_height=120.0;
    bool         is_me=false;
    for (int i=0;i<player_count;i++) {
        pokerPlayer=pokerManager->player(i);
        if (pokerPlayer->user()==options->getUsername()) {
            is_me=true;
        } else {
            is_me=false;
        }
        basex=0;
        basey=0;
        if (pokerPlayer) {
            switch(i) {
            case 0:
                is_me=true;/* FOR DEBUG ONLY!!! */
                basex=230;
                basey=300;
                break;
            case 1:
                basex=30;
                basey=230;
                break;
            case 2:
                basex=30;
                basey=80;
                break;
            case 3:
                basex=230;
                basey=10;
                break;
            case 4:
                basex=445;
                basey=10;
                break;
            case 5:
                basex=645;
                basey=80;
                break;
            case 6:
                basex=645;
                basey=230;
                break;
            case 7:
                basex=445;
                basey=300;
                break;
            default:
                break;
            }
            if (basex>0 && basey>0) {
                QGraphicsRectItem *player_rect=new QGraphicsRectItem(basex,basey,player_rect_width,player_rect_height,table_item,qGraphicsScene);
                player_rect->setBrush(*player_rect_brush);
                if (pokerManager->nextplayer()==pokerPlayer->user()) {
                    player_rect->setPen(*next_player_rect_pen);
                } else {
                    player_rect->setPen(*player_rect_pen);
                }
                QPixmap * playerPixmap=pixmapRequester->getLoadedPixmap(pokerPlayer->user());
                if (!playerPixmap) {
                    pixmapRequester->requestPixmap(pokerPlayer->user());
                    playerPixmap=pixmapRequester->getLoadedPixmap("unknown");
                    if (playerPixmap) {
                        item=new QGraphicsPixmapItem(*playerPixmap,player_rect,qGraphicsScene);
                        item->setPos(basex+10,basey+4);
                        waitForPicturehash.insert(pokerPlayer->user(),item);
                    }
                } else {
                    item=new QGraphicsPixmapItem(*playerPixmap,player_rect,qGraphicsScene);
                    item->setPos(basex+10,basey+4);
                }

                if (pokerPlayer->card_1().size()>0) {
                    QGraphicsPixmapItem *card_1;
                    if (is_me) {
                        card_1=new QGraphicsPixmapItem(*pixmapCacheCards->getPixmap(pokerPlayer->card_1()+".png"),player_rect,qGraphicsScene);
                    } else {
                        card_1=new QGraphicsPixmapItem(*pixmapCacheCards->getPixmap("flipside.png"),player_rect,qGraphicsScene);
                    }
                    card_1->setPos(basex+80,basey+5);
                    if (pokerPlayer->card_1().size()>0) {
                        QGraphicsPixmapItem *card_2;
                        if (is_me) {
                            card_2=new QGraphicsPixmapItem(*pixmapCacheCards->getPixmap(pokerPlayer->card_2()+".png"),card_1,qGraphicsScene);
                        } else {
                            card_2=new QGraphicsPixmapItem(*pixmapCacheCards->getPixmap("flipside.png"),card_1,qGraphicsScene);
                        }
                        card_2->setPos(30,5);
                    }
                }
                if (pokerManager->bigblind()==pokerPlayer->user()) {
                    QGraphicsPixmapItem *xxitem=new QGraphicsPixmapItem(*pixmapCacheTable->getPixmap("bigblindPuck.png"),player_rect,qGraphicsScene);
                    xxitem->setPos(basex-10,basey+45);
                }
                if (pokerManager->smallblind()==pokerPlayer->user()) {
                    QGraphicsPixmapItem *xxitem=new QGraphicsPixmapItem(*pixmapCacheTable->getPixmap("smallblindPuck.png"),player_rect,qGraphicsScene);
                    xxitem->setPos(basex-10,basey+45);
                }
                if (pokerManager->dealer()==pokerPlayer->user()) {
                    QGraphicsPixmapItem *xxitem=new QGraphicsPixmapItem(*pixmapCacheTable->getPixmap("dealerPuck.png"),player_rect,qGraphicsScene);
                    xxitem->setPos(basex-10,basey+45);
                }

                QString bet_text="Bet: "+ QString::number(pokerPlayer->act_bet());
                QGraphicsTextItem *bet_item = new QGraphicsTextItem(bet_text,player_rect,qGraphicsScene);
                bet_item->setDefaultTextColor(Qt::white);
                bet_item->setPos(basex+10,basey+player_rect_height-40);

                QString money_text=pokerPlayer->user()+ "´s Money: "+ QString::number(pokerPlayer->money());
                QGraphicsTextItem *money_item = new QGraphicsTextItem(money_text,player_rect,qGraphicsScene);
                money_item->setDefaultTextColor(Qt::yellow);
                money_item->setPos(basex+10,basey+player_rect_height-20);

                QString last_action=pokerPlayer->last_action();
                if (last_action.size()>0) {
                    if (last_action=="CALL") {
                        QGraphicsPixmapItem *last_action_item=new QGraphicsPixmapItem(*pixmapCacheTable->getPixmap("action_call.png"),player_rect,qGraphicsScene);
                        last_action_item->setPos(basex+2,basey+50);
                    }
                    if (last_action=="BET") {
                        QGraphicsPixmapItem *last_action_item=new QGraphicsPixmapItem(*pixmapCacheTable->getPixmap("action_bet.png"),player_rect,qGraphicsScene);
                        last_action_item->setPos(basex+2,basey+50);
                    }
                    if (last_action=="RAISE") {
                        QGraphicsPixmapItem *last_action_item=new QGraphicsPixmapItem(*pixmapCacheTable->getPixmap("action_raise.png"),player_rect,qGraphicsScene);
                        last_action_item->setPos(basex+2,basey+50);
                    }
                    if (last_action=="CHECK") {
                        QGraphicsPixmapItem *last_action_item=new QGraphicsPixmapItem(*pixmapCacheTable->getPixmap("action_check.png"),player_rect,qGraphicsScene);
                        last_action_item->setPos(basex+2,basey+50);
                    }
                    if (last_action=="FOLD") {
                        QGraphicsPixmapItem *last_action_item=new QGraphicsPixmapItem(*pixmapCacheTable->getPixmap("action_fold.png"),player_rect,qGraphicsScene);
                        last_action_item->setPos(basex+2,basey+50);
                    }
                }

                if (is_me) {
                    int button_basex=basex+30;
                    int button_basey=basey+player_rect_height+10;
                    QPushButton *button;
                    if (pokerManager->can_raise()) {
                        button=new QPushButton("RAISE"/*,qGraphicsView->viewport()*/);
                        if (button) {
                            connect(button,SIGNAL(clicked()),this,SLOT(on_raise_ClickedX()));
                            QGraphicsProxyWidget* item=qGraphicsScene->addWidget(button);
                            if (item) {
                                item->setParentItem(table_item);
                                item->setPos(button_basex,button_basey);
                            }
                            button_basex+=button->width()+5;
                        }
                    }
                    if (pokerManager->can_bet()) {
                        button=new QPushButton("BET"/*,qGraphicsView->viewport()*/);
                        if (button) {
                            connect(button,SIGNAL(clicked()),this,SLOT(on_bet_ClickedX()));
                            QGraphicsProxyWidget* item=qGraphicsScene->addWidget(button);
                            if (item) {
                                item->setParentItem(table_item);
                                item->setPos(button_basex,button_basey);
                            }
                            button_basex+=button->width()+5;
                        }
                    }
                    if (pokerManager->can_call()) {
                        button=new QPushButton("CALL"/*,qGraphicsView->viewport()*/);
                        if (button) {
                            connect(button,SIGNAL(clicked()),this,SLOT(on_call_ClickedX()));
                            QGraphicsProxyWidget* item=qGraphicsScene->addWidget(button);
                            if (item) {
                                item->setParentItem(table_item);
                                item->setPos(button_basex,button_basey);
                            }
                            button_basex+=button->width()+5;
                        }
                    }

                    if (pokerManager->can_check()) {
                        button=new QPushButton("CHECK"/*,qGraphicsView->viewport()*/);
                        if (button) {
                            connect(button,SIGNAL(clicked()),this,SLOT(on_check_ClickedX()));
                            QGraphicsProxyWidget* item=qGraphicsScene->addWidget(button);
                            if (item) {
                                item->setParentItem(table_item);
                                item->setPos(button_basex,button_basey);
                            }
                            button_basex+=button->width()+5;
                        }
                    }
                    if (pokerManager->can_fold()) {
                        button=new QPushButton("FOLD"/*,qGraphicsView->viewport()*/);
                        if (button) {
                            connect(button,SIGNAL(clicked()),this,SLOT(on_fold_ClickedX()));
                            QGraphicsProxyWidget* item=qGraphicsScene->addWidget(button);
                            if (item) {
                                item->setParentItem(table_item);
                                item->setPos(button_basex,button_basey);
                            }
                            button_basex+=button->width()+5;
                        }
                    }
                    if (pokerManager->can_deal()) {
                        button=new QPushButton("DEAL"/*,qGraphicsView->viewport()*/);
                        if (button) {
                            connect(button,SIGNAL(clicked()),this,SLOT(on_deal_ClickedX()));
                            QGraphicsProxyWidget* item=qGraphicsScene->addWidget(button);
                            if (item) {
                                item->setParentItem(table_item);
                                item->setPos(button_basex,button_basey);
                            }
                            button_basex+=button->width()+5;
                        }
                    }
                }
            }
        }
    }

    //qGraphicsScene->addPixmap(*pixmapCacheTable->getPixmap("table.png"));
    //qGraphicsScene->addText("Hello, world!");
    //qGraphicsView->scale(0.90,0.95);
    qGraphicsView->show();
    return true;
}

void PokerView::pixmapFound(QString name, QPixmap *pixmapx) {
    /* TODO: update existing pixmap to the scene */
    if (waitForPicturehash.contains(name)) {
        QGraphicsPixmapItem *item=waitForPicturehash.value(name);
        if (item) {
            item->setPixmap(*pixmapx);
        }
        waitForPicturehash.remove(name);
    }
}

void PokerView::sendCommandToServer(QString command, QString *key2, QString *value2, QString *key3, QString *value3) {
    Options *options=getGlobalOptionsObj();
    EcoMiniReg* ecoMiniReg=getGlobalEcoMiniReg();

    if (options && ecoMiniReg && command.size()>0) {
        QDomDocument doc;
        QDomElement esm=doc.createElement("esm");
        QDomElement msg=doc.createElement("msg");

        esm.setAttribute("t","THOLDM");
        msg.setAttribute("t",command);
        msg.setAttribute("game_id",pokerManager->gameId());
        msg.setAttribute("l",options->getUsername());
        if (key2 && key2->size()>0 && value2) {
            msg.setAttribute(*key2,*value2);
        }
        if (key3 && key3->size()>0 && value3) {
            msg.setAttribute(*key3,*value3);
        }

        doc.appendChild(esm);
        esm.appendChild(msg);

        QString qmessage=doc.toString();

        fprintf(stderr,"Request XML-String:\n%s\n",qmessage.toAscii().data());

       ecoMiniReg->sendCommandToServer(qmessage,options->serverPort()+1);
    }
}

void PokerView::on_deal_ClickedX() {
    sendCommandToServer("deal");
}
void PokerView::on_raise_ClickedX() {
    QString amount=("amount");
    double amount_value=0.0;
    if (pokerManager->player(0)) {
        amount_value=pokerManager->player(0)->act_bet()+10;
    }
    QString amount_value_string=QString::number(amount_value);
    sendCommandToServer("raise",&amount,&amount_value_string);
}
void PokerView::on_bet_ClickedX() {
    QString amount=("amount");
    double amount_value=100.0;
    QString amount_value_string=QString::number(amount_value);
    sendCommandToServer("bet",&amount,&amount_value_string);
}
void PokerView::on_fold_ClickedX() {
    sendCommandToServer("fold");
}
void PokerView::on_check_ClickedX() {
    sendCommandToServer("check");
}
void PokerView::on_call_ClickedX() {
    QString amount=("amount");
    double amount_value=pokerManager->min_call();
    QString amount_value_string=QString::number(amount_value);
    sendCommandToServer("call",&amount,&amount_value_string);
}
