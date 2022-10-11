#include "CCefClientDelegate.h"

#include "QCefViewPrivate.h"

bool
CCefClientDelegate::selectClientCertificate(CefRefPtr<CefBrowser> browser,
                                            bool isProxy,
                                            const CefString& host,
                                            int port,
                                            const CefRequestHandler::X509CertificateList& certificates,
                                            CefRefPtr<CefSelectClientCertificateCallback> callback)
{
  if (!IsValidBrowser(browser))
    return false;

  QString qHost = QString::fromStdString(host.ToString());

  QList<QByteArray> qCertificates;
  for (CefRefPtr<CefX509Certificate> i : certificates) {
    QByteArray ba;
    ba.resize(i->GetPEMEncoded()->GetSize());
    i->GetPEMEncoded()->GetData(ba.data(), ba.size(), 0);
    qCertificates.append(ba);
  }
  pCefViewPrivate_->setClientSelectedCertificate(-1);
  pCefViewPrivate_->q_ptr->selectClientCertificate(isProxy, qHost, port, qCertificates);

  int selectedCertificate = pCefViewPrivate_->getClientSelectedCertificate();

  if (selectedCertificate >= 0 && selectedCertificate < certificates.size()) {
    callback->Select(certificates.at(selectedCertificate));
  }

  return true;
}
