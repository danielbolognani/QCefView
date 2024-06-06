#include "CefViewWidget.h"

#if defined(Q_OS_WIN)
#include <windows.h>
#elif defined(Q_OS_MAC)
#elif defined(Q_OS_LINUX)
#else
#endif

#include <QColor>
#include <QRandomGenerator>
#include <QResizeEvent>

#include <QLabel>
#include <QDrag>
#include <QMimeData>
#include <QPainter>

CefViewWidget::CefViewWidget(const QString url, const QCefSetting* setting, QWidget* parent /* = 0*/)
  : QCefView(url, setting, parent)
{
  setAcceptDrops(true);
}

CefViewWidget::~CefViewWidget() {}


void
CefViewWidget::dragEnterEvent(QDragEnterEvent* event)
{
  if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
          
    event->acceptProposedAction();

  } else {
    event->ignore();
  }

  this->onDragEnter(event);
   
}

void
CefViewWidget::dragLeaveEvent(QDragLeaveEvent* event)
{
   this->onDragLeave(event);
}

void
CefViewWidget::dragMoveEvent(QDragMoveEvent* event)
{
  this->onDragMove(event);
 }

void
CefViewWidget::dropEvent(QDropEvent* event)
{
  this->onDrop(event);
}
