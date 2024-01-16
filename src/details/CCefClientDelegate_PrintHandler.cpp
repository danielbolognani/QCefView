#include <QHash>

#include "CCefClientDelegate.h"

#include "QCefViewPrivate.h"

void
CCefClientDelegate::printStart(CefRefPtr<CefBrowser> browser)
{
  if (!IsValidBrowser(browser))
    return;

  pCefViewPrivate_->q_ptr->printStart();
}

void
CCefClientDelegate::printSettings(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefPrintSettings> settings,
                                  bool get_defaults)
{
  if (!IsValidBrowser(browser))
    return;

  QHash<QString, QString> configs;
  configs.insert("DEVICE_NAME", QString(""));
  configs.insert("COLLATE", QString(""));
  configs.insert("COLOR_MODEL", QString(""));
  configs.insert("COPIES", QString(""));
  configs.insert("DPI", QString(""));
  configs.insert("DUPLEX_MODE", QString(""));
  configs.insert("ORIENTATION", QString(""));
  configs.insert("SELECTION_ONLY", QString(""));
  configs.insert("PAGE_RANGES", QString(""));
  configs.insert("PRINTER_PRINTABLE_AREA", QString(""));

  pCefViewPrivate_->q_ptr->printSettings(configs, get_defaults);

  if(!configs["COLLATE"].isEmpty())
  {
    settings->SetCollate(configs["COLLATE"].toInt());
  }
  if(!configs["COLOR_MODEL"].isEmpty())
  {
    int colorMode = configs["COLOR_MODEL"].toInt();
    switch(colorMode)
    {
      case 1:
        settings->SetColorModel(COLOR_MODEL_COLOR);
        break;
      case 0:
        settings->SetColorModel(COLOR_MODEL_GRAY);
        break;
      default:
        settings->SetColorModel(COLOR_MODEL_UNKNOWN);
        break;
    }
  }
  if(!configs["COPIES"].isEmpty())
  {
    settings->SetCopies(configs["COPIES"].toInt());
  }
  if(!configs["DEVICE_NAME"].isEmpty())
  {
    settings->SetDeviceName(configs["DEVICE_NAME"].toStdString());
  }
  if(!configs["DPI"].isEmpty())
  {
    settings->SetDPI(configs["DPI"].toInt());
  }
  if(!configs["DUPLEX_MODE"].isEmpty())
  {
    int duplexMode = configs["DUPLEX_MODE"].toInt();
    switch(duplexMode)
    {
      case 0:
        settings->SetDuplexMode(DUPLEX_MODE_SIMPLEX);
        break;
      case 1:
        settings->SetDuplexMode(DUPLEX_MODE_UNKNOWN);
        break;
      case 2:
        settings->SetDuplexMode(DUPLEX_MODE_LONG_EDGE);
        break;
      case 3:
        settings->SetDuplexMode(DUPLEX_MODE_SHORT_EDGE);
        break;
      default:
        settings->SetDuplexMode(DUPLEX_MODE_UNKNOWN);
        break;
    }
  }
  if(!configs["ORIENTATION"].isEmpty())
  {
    settings->SetOrientation(configs["ORIENTATION"].toInt());
  }
  if(!configs["SELECTION_ONLY"].isEmpty())
  {
    settings->SetSelectionOnly(configs["SELECTION_ONLY"].toInt());
  }
  if(!configs["PRINTER_PRINTABLE_AREA"].isEmpty())
  {
    QStringList printableAreaList = configs["PRINTER_PRINTABLE_AREA"].split(";");
    if(printableAreaList.size() >= 6)
    {
      settings->SetPrinterPrintableArea(CefSize(printableAreaList.at(0).toInt(), printableAreaList.at(1).toInt()), CefRect(printableAreaList.at(2).toInt(), printableAreaList.at(3).toInt(), printableAreaList.at(4).toInt(), printableAreaList.at(5).toInt()), false);
    }
  }
}

bool
CCefClientDelegate::printDialog(CefRefPtr<CefBrowser> browser,
                                    bool has_selection,
                                    CefRefPtr<CefPrintDialogCallback> callback)
{
  return false;

  /*if (!IsValidBrowser(browser))
    return false;

  pCefViewPrivate_->q_ptr->printDialog(has_selection);

  return true;*/
}

bool
CCefClientDelegate::printJob(CefRefPtr<CefBrowser> browser,
                                 const std::string& document_name,
                                 const std::string& pdf_file_path,
                                 CefRefPtr<CefPrintJobCallback> callback)
{
  if (!IsValidBrowser(browser))
    return false;

  QString documentName = QString::fromStdString(document_name);
  QString pdfFilePath = QString::fromStdString(pdf_file_path);

  pCefViewPrivate_->q_ptr->printJob(documentName, pdfFilePath);

  return true;
}

void
CCefClientDelegate::printReset(CefRefPtr<CefBrowser> browser)
{
  if (!IsValidBrowser(browser))
    return;

  pCefViewPrivate_->q_ptr->printReset();
}
