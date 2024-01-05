#include "CCefClientDelegate.h"

#include <QApplication>
#include <QDebug>
#include <QImage>
#include <QScreen>

#include "QCefViewPrivate.h"

//#include <thread>
#include <QIODevice>
#include <QDrag>


#if defined(CEF_USE_OSR)

bool
CCefClientDelegate::getRootScreenRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
  if (!IsValidBrowser(browser)) {
    return false;
  }

  // get the screen which the view is currently residing in
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
  QScreen* currentScreen = pCefViewPrivate_->q_ptr->screen();
#else
  QWidget* ancestorWidget = pCefViewPrivate_->q_ptr->window();
  QWindow* ancestorWindow = ancestorWidget ? ancestorWidget->windowHandle() : nullptr;
  QScreen* currentScreen = ancestorWindow ? ancestorWindow->screen() : nullptr;
#endif

  if (!currentScreen) {
    // the view is not visible so we retrieve the main screen info
    currentScreen = QApplication::screens().at(0);
  }

  QRect rcScreen = currentScreen->geometry();

  // qDebug() << "CCefClientDelegate::GetRootScreenRect:" << rcScreen;

  rect.Set(rcScreen.x(), rcScreen.y(), rcScreen.width(), rcScreen.height());
  return true;
}

void
CCefClientDelegate::getViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
  if (!IsValidBrowser(browser)) {
    rect.Set(0, 0, 1, 1);
    return;
  }

  // Note: the documentation of the CefRenderHandler::GetViewRect
  // says `the rect is relative to the screen coordinates` but actually
  // it is relative to the top level window. In the source code of of the cefclient example
  // it just returns the rect retrieved from GetClinetRect which is relative to the window
  QSize rcSize = pCefViewPrivate_->q_ptr->size();
  QPoint ptWindow = pCefViewPrivate_->q_ptr->mapTo(pCefViewPrivate_->q_ptr->window(), QPoint(0, 0));

  // qDebug() << "CCefClientDelegate::GetViewRect:" << QRect(ptWindow, rcSize);

  rect.Set(ptWindow.x(), ptWindow.y(), rcSize.width() ? rcSize.width() : 1, rcSize.height() ? rcSize.height() : 1);
}

bool
CCefClientDelegate::getScreenPoint(CefRefPtr<CefBrowser> browser, int viewX, int viewY, int& screenX, int& screenY)
{
  if (!IsValidBrowser(browser))
    return false;

  QPoint ptScreen = pCefViewPrivate_->q_ptr->mapToGlobal(QPoint(viewX, viewY));
  screenX = ptScreen.x();
  screenY = ptScreen.y();
  return true;
}

bool
CCefClientDelegate::getScreenInfo(CefRefPtr<CefBrowser> browser, CefScreenInfo& screen_info)
{
  if (!IsValidBrowser(browser))
    return false;

    // get the screen which the view is currently residing in
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
  QScreen* currentScreen = pCefViewPrivate_->q_ptr->screen();
#else
  QWidget* ancestorWidget = pCefViewPrivate_->q_ptr->window();
  QWindow* ancestorWindow = ancestorWidget ? ancestorWidget->windowHandle() : nullptr;
  QScreen* currentScreen = ancestorWindow ? ancestorWindow->screen() : nullptr;
#endif

  if (!currentScreen) {
    // the view is not visible so we retrieve the main screen info
    currentScreen = QApplication::screens().at(0);
  }

  QRect rect = currentScreen->geometry();
  QRect availableRect = currentScreen->availableGeometry();
  screen_info.Set(currentScreen->devicePixelRatio(),                                                      //
                  currentScreen->depth(),                                                                 //
                  0,                                                                                      //
                  false,                                                                                  //
                  { rect.x(), rect.y(), rect.width(), rect.height() },                                    //
                  { availableRect.x(), availableRect.y(), availableRect.width(), availableRect.height() } //
  );

  // qDebug() << "CCefClientDelegate::GetScreenInfo: pixel ratio:" << screen_info.device_scale_factor << "rect:" << rect
  //           << "availableRect:" << availableRect;

  return true;
}

void
CCefClientDelegate::onPopupShow(CefRefPtr<CefBrowser> browser, bool show)
{
  if (!IsValidBrowser(browser))
    return;

  pCefViewPrivate_->onOsrShowPopup(show);
}

void
CCefClientDelegate::onPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect)
{
  if (!IsValidBrowser(browser))
    return;

  pCefViewPrivate_->onOsrResizePopup(QRect{ rect.x, rect.y, rect.width, rect.height });
}

void
CCefClientDelegate::onPaint(CefRefPtr<CefBrowser> browser,
                            CefRenderHandler::PaintElementType type,
                            const CefRenderHandler::RectList& dirtyRects,
                            const void* buffer,
                            int width,
                            int height)
{
  if (!IsValidBrowser(browser))
    return;

  QImage frame;
  QRegion region;

  frame = QImage(static_cast<const uchar*>(buffer), width, height, QImage::Format_ARGB32_Premultiplied);
  for (auto& rect : dirtyRects) {
    region += QRect{ rect.x, rect.y, rect.width, rect.height };
  }

  if (PET_VIEW == type) {
    pCefViewPrivate_->onOsrUpdateViewFrame(frame, region);
  } else if (PET_POPUP == type) {
    pCefViewPrivate_->onOsrUpdatePopupFrame(frame, region);
  } else {
  }
}

void
CCefClientDelegate::onAcceleratedPaint(CefRefPtr<CefBrowser> browser,
                                       CefRenderHandler::PaintElementType type,
                                       const CefRenderHandler::RectList& dirtyRects,
                                       void* shared_handle)
{}

QPixmap
cefImageToPixmap(CefRefPtr<CefImage> cefImage)
{
  int width = cefImage->GetWidth();
  int height = cefImage->GetHeight();

  CefRefPtr<CefBinaryValue> imageData = cefImage->GetAsPNG(1.0,true,width,height);
  size_t imageSize = imageData->GetSize();
  uchar* buff = new uchar[imageSize];
  imageData->GetData(buff, imageSize, 0);

  QPixmap pixmap;

  bool success = pixmap.loadFromData(static_cast<const uchar*>(buff), static_cast<int>(imageSize), "PNG");

  delete buff;
  buff = nullptr;

  return pixmap;
}


/// <summary>
/// Called when the user starts dragging content in the web view. Contextual information about the dragged content is
/// supplied by dragData. OS APIs that run a system message loop may be used within the StartDragging call.
/// Don't call any of the IBrowserHost.DragSource*Ended* methods after returning false.
/// Return true to handle the drag operation. Call <see cref="IBrowserHost.DragSourceEndedAt"/> and <see
/// cref="IBrowserHost.DragSourceSystemDragEnded"/> either synchronously or asynchronously to inform the web view that
/// the drag operation has ended.
/// </summary>
/// <param name="dragData">drag data</param>
/// <param name="mask">operation mask</param>
/// <param name="x">combined x and y provide the drag start location in screen coordinates</param>
/// <param name="y">combined x and y provide the drag start location in screen coordinates</param>
/// <returns>Return false to abort the drag operation.</returns>
bool
CCefClientDelegate::startDragging(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefDragData> drag_data,
                                  CefRenderHandler::DragOperationsMask allowed_ops,
                                  int x,
                                  int y)
{
  // Implemented by Totvs
    CefString html = drag_data->GetFragmentHtml();
    CefString text = drag_data->GetFragmentText();

    CefRefPtr<CefDragData> dragDataClone = drag_data->Clone();
    dragDataClone->ResetFileContents();

    CefMouseEvent* event = new CefMouseEvent();
    event->x = x;
    event->y = y;


    QMetaObject::invokeMethod(
    pCefViewPrivate_, [=]() {
        // main thread

        QPoint originalPoint(x, y);

        QByteArray itemData;
        QDataStream dataStream(&itemData, QIODevice::ReadWrite);
                
        QDrag* drag = new QDrag(pCefViewPrivate_);
        drag->setHotSpot(QPoint(0, 0));

        if (drag_data->HasImage()) {
            CefRefPtr<CefImage> img = drag_data->GetImage();
            QPixmap qp = cefImageToPixmap(img);
            dataStream << qp << originalPoint << html.ToString().c_str() << text.ToString().c_str();
            drag->setPixmap(qp);
        } else {
            dataStream << originalPoint << html.ToString().c_str() << text.ToString().c_str();
        }

        QMimeData* mimeData = new QMimeData;
        mimeData->setData("application/x-dnditemdata", itemData);
        drag->setMimeData(mimeData);   

        pCefViewPrivate_->setDragData(dragDataClone);
                        
        Qt::DropAction da = drag->exec();
             
        CefRenderHandler::DragOperationsMask operationMask = QCefViewPrivate::getDragOperationMask(da);

        browser->GetHost()->DragSourceEndedAt(x, y, operationMask);
        browser->GetHost()->DragSourceSystemDragEnded(); //Abre uma tela do sistema operacional para salvar um

    }, Qt::QueuedConnection);

  //<returns> Return false to abort the drag operation.</ returns>
  return true;
}


void
CCefClientDelegate::updateDragCursor(CefRefPtr<CefBrowser> browser, CefRenderHandler::DragOperation operation)
{
  //qDebug() << "UpdateDragCursor: " << operation;
  QMetaObject::invokeMethod(
    pCefViewPrivate_, [=]() { pCefViewPrivate_->currentDragOperation = operation; }, Qt::DirectConnection);
}

void
CCefClientDelegate::onScrollOffsetChanged(CefRefPtr<CefBrowser> browser, double x, double y)
{}

void
CCefClientDelegate::onImeCompositionRangeChanged(CefRefPtr<CefBrowser> browser,
                                                 const CefRange& selected_range,
                                                 const CefRenderHandler::RectList& character_bounds)
{
  if (!IsValidBrowser(browser))
    return;

  // qDebug() << "OnImeCompositionRangeChanged:" << selected_range.from << " - " << selected_range.to
  //         << ", RC:" << character_bounds.size();

  if (!character_bounds.empty()) {
    auto r = character_bounds[character_bounds.size() - 1];
    QRect rc(r.x, r.y, r.width, r.height);
    QMetaObject::invokeMethod(pCefViewPrivate_,            //
                              "onOsrImeCursorRectChanged", //
                              Q_ARG(const QRect&, rc));
  }

  return;
}

void
CCefClientDelegate::onTextSelectionChanged(CefRefPtr<CefBrowser> browser,
                                           const CefString& selected_text,
                                           const CefRange& selected_range)
{}

void
CCefClientDelegate::onVirtualKeyboardRequested(CefRefPtr<CefBrowser> browser,
                                               CefRenderHandler::TextInputMode input_mode)
{}

#endif
