#ifndef POKERVIEW_H
#define POKERVIEW_H

#include <QWidget>
#include "pokermanager.h"
#include "Options.h"
#include "pixmapcache.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "PixmapRequester.h"
#include <QHash>
#include <QPushButton>
#include <QGraphicsProxyWidget>
class PokerView : public QWidget
{
    Q_OBJECT
public:
    PokerView(QWidget *parent=0);
    ~PokerView();
    bool updateGamestatus(QString message);
    void setGameId(QString game_id);
    QString gameId();
private:
    PokerManager *pokerManager;
    PixmapCache *pixmapCacheCards;
    PixmapCache *pixmapCacheTable;
    PixmapRequester *pixmapRequester;
    QGraphicsScene *qGraphicsScene;
    QGraphicsView *qGraphicsView;
    bool updateViewX();
    QHash<QString, QGraphicsPixmapItem*>      waitForPicturehash;

    QBrush       *player_rect_brush;
    QColor       *player_rect_color;
    QPen         *player_rect_pen;
    QPen         *next_player_rect_pen;

    void sendCommandToServer(QString command, QString *key2=0, QString *value2=0, QString *key3=0, QString *value3=0);

public slots:
    void pixmapFound(QString name, QPixmap *pixmapx);
    void on_raise_ClickedX();
    void on_bet_ClickedX();
    void on_check_ClickedX();
    void on_fold_ClickedX();
    void on_call_ClickedX();
    void on_deal_ClickedX();
};

#endif // POKERVIEW_H
