#include "resizabletextqpushbutton.h"
#include <QFont>
#include <QFontMetrics>
#include <QMessageBox>

ResizableTextQPushButton::ResizableTextQPushButton(QWidget *parent) :
    QPushButton(parent)
{
}

/* Override QWidget::resizeEvent(QResizeEvent *) in order to
  ensure that the text of ui->panicButton is the right size
  for the new window size.
*/
void ResizableTextQPushButton::resizeEvent(QResizeEvent *resizeEvent)
{
#ifdef Q_OS_SYMBIAN
    const double magicNumber = 0.06;
#elif defined(Q_WS_MAEMO_5) || defined(Q_WS_MAEMO_6)
    const double magicNumber = 0.1;
#else
    const double magicNumber = 0.1;
#endif

    QSize newSize = resizeEvent->size();

//    QMessageBox::information(this, "resizeEvent", QString("button h:%1 w:%2").arg(newSize.height()).
//			     arg(newSize.width()));
#if 1
    QFont f = font();
    f.setPointSize((int)(magicNumber * newSize.width() + 0.5));
    setFont(f);
#else
    // BUG: This makes the QPushButton forget what color its text is
    setStyleSheet(QString("font-size: %1pt;").arg((int) (magicNumber * newSize.width() + 0.5)));
#endif
    QPushButton::resizeEvent(resizeEvent);
}
