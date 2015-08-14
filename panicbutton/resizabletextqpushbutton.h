#ifndef RESIZABLETEXTQPUSHBUTTON_H
#define RESIZABLETEXTQPUSHBUTTON_H

#include <QPushButton>
#include <QResizeEvent>

class ResizableTextQPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ResizableTextQPushButton(QWidget *parent = 0);

signals:

public slots:

protected:
    void resizeEvent(QResizeEvent *resizeEvent);
};

#endif // RESIZABLETEXTQPUSHBUTTON_H
