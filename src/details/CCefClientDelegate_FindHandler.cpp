#include "CCefClientDelegate.h"

#include "QCefViewPrivate.h"

void
CCefClientDelegate::findResult(CefRefPtr<CefBrowser> browser,
                               int identifier,
                               int count,
                               const CefRect& selectionRect,
                               int activeMatchOrdinal,
                               bool finalUpdate)
{
  if (!IsValidBrowser(browser))
    return;

  QRect qSelectionRect(selectionRect.x, selectionRect.y, selectionRect.width, selectionRect.height);
  pCefViewPrivate_->q_ptr->findResult(identifier, count, qSelectionRect, activeMatchOrdinal, finalUpdate);
}
