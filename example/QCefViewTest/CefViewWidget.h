#ifndef CUSTOMCEFVIEW_H
#define CUSTOMCEFVIEW_H

#include <QScreen>

#include <QCefView.h>

/// <summary>
/// Represents the customized QCefView
/// </summary>
class CefViewWidget : public QCefView
{
  Q_OBJECT

public:
  CefViewWidget(const QString url, const QCefSetting* setting, QWidget* parent = 0);

  ~CefViewWidget();

  void dragEnterEvent(QDragEnterEvent* event) override;

  void dragLeaveEvent(QDragLeaveEvent* event) override;

  void dragMoveEvent(QDragMoveEvent* event) override;

  void dropEvent(QDropEvent* event) override;

protected slots:
  void onScreenChanged(QScreen* screen);

private:

  virtual void onBrowserWindowCreated(QWindow* win);

  virtual void resizeEvent(QResizeEvent* event);

  private:
  QWindow* m_cefWindow = nullptr;

  int m_cornerRadius = 50;

  QRegion m_draggableRegion;

  QRegion m_nonDraggableRegion;
};

#endif // CUSTOMCEFVIEW_H
