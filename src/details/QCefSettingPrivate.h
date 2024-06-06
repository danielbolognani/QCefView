#pragma once

#pragma region stl_headers
#include <list>
#include <string>
#pragma endregion stl_headers

#pragma region cef_headers
#include <include/cef_app.h>
#pragma endregion cef_headers

#pragma region qt_headers
#include <QVariant>
#pragma endregion qt_headers

#include <QCefSetting.h>

class QCefSettingPrivate
{
  Q_DECLARE_PUBLIC(QCefSetting)
  QCefSetting* q_ptr;

  friend class CCefClientDelegate;

public:
  static void CopyFromCefBrowserSettings(QCefSetting* qs, const CefBrowserSettings* cs);

  static void CopyToCefBrowserSettings(const QCefSetting* qs, CefBrowserSettings* cs);

public:
  explicit QCefSettingPrivate();

  std::string standardFontFamily_;
  std::string fixedFontFamily_;
  std::string serifFontFamily_;
  std::string sansSerifFontFamily_;
  std::string cursiveFontFamily_;
  std::string fantasyFontFamily_;
  std::string defaultEncoding_;
  std::string acceptLanguageList_;

  /* int */ QVariant windowlessFrameRate_;
  /* int */ QVariant defaultFontSize_;
  /* int */ QVariant defaultFixedFontSize_;
  /* int */ QVariant minimumFontSize_;
  /* int */ QVariant minimumLogicalFontSize_;

  /* bool */ QVariant remoteFonts_;
  /* bool */ QVariant javascript_;
  /* bool */ QVariant javascriptCloseWindows_;
  /* bool */ QVariant javascriptAccessClipboard_;
  /* bool */ QVariant javascriptDomPaste_;
  /* bool */ QVariant plugins_;
  /* bool */ QVariant imageLoading_;
  /* bool */ QVariant imageShrinkStandaloneToFit_;
  /* bool */ QVariant textAreaResize_;
  /* bool */ QVariant tabToLinks_;
  /* bool */ QVariant localStorage_;
  /* bool */ QVariant databases_;
  /* bool */ QVariant webgl_;

  /* QColor */ QVariant backgroundColor_;
};

class QCefPdfPrintSettingPrivate
{
  Q_DECLARE_PUBLIC(QCefPdfPrintSetting)
  QCefPdfPrintSetting* q_ptr;

  friend class CCefClientDelegate;

public:
  static void CopyFromCefPdfPrintSettings(QCefPdfPrintSetting* qs, const CefPdfPrintSettings* cs);

  static void CopyToCefPdfPrintSettings(const QCefPdfPrintSetting* qs, CefPdfPrintSettings* cs);

  explicit QCefPdfPrintSettingPrivate();

  bool display_header_footer_;
  std::string footer_template_ = "";
  std::string header_template_ = "";
  int landscape_ = -1;
  double margin_bottom_ = -1;
  double margin_left_ = -1;
  double margin_right_ = -1;
  double margin_top_ = -1;
  QCefPdfPrintMarginType_t margin_type_;
  std::string page_ranges_;
  double paper_height_ = 297;
  double paper_width_ = 210;
  int prefer_css_page_size_ = -1;
  int print_background_ = -1;
  double scale_ = -1;

private:
  static QCefPdfPrintMarginType_t to_QCef_MarginType(cef_pdf_print_margin_type_t margin_type);
  static cef_pdf_print_margin_type_t to_Cef_MarginType(QCefPdfPrintMarginType_t margin_type);
};
