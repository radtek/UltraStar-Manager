#include "QUAlbumArtExCollector.h"
#include "QUAlbumArtExImageSource.h"
#include "QUAlbumArtExRequestUrl.h"

#include "QUCommunicatorInterface.h"
#include "QUSongInterface.h"
#include "QUSongSupport.h"

#include <QFile>
#include <QBuffer>
#include <QRegExp>
#include <QTextStream>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QVariant>

QUAlbumArtExCollector::QUAlbumArtExCollector(QUSongInterface *song, QUAlbumArtExImageSource *source): QUHttpCollector(song, source) {}

QURequestUrl* QUAlbumArtExCollector::url() const {
	return new QUAlbumArtExRequestUrl(
			source()->host(),
			QStringList() << source()->songDataField("first") << source()->songDataField("second") << source()->songDataField("third"),
			song());
}

void QUAlbumArtExCollector::processSearchResults() {
	song()->log(tr("[QUAlbumArtExCollector] processSearchResults(), state() = ") + QString::number(state()), QU::Help);
	QRegExp rx = QRegExp("<div class=\"img-box\" style=\"background-image: url\\(/coverart/_tn/(.*)\\);\">(.*)<p class=\"image-info\"><span class=\"dimensions\">(\\d+)&times;(\\d+)</span>");

	rx.setMinimal(true);
	rx.setCaseSensitivity(Qt::CaseInsensitive);

	QString text(buffer()->data());
	//song()->log(tr("[albumartex - result] ") + text, QU::Help);
	QStringList allUrls;
	QList<QPair<int, int> > resolutions;
	int pos = 0;

	while ((pos = rx.indexIn(text, pos)) != -1) {
		allUrls << "/coverart/gallery/" + rx.cap(1).trimmed();
		resolutions << QPair<int, int>(QVariant(rx.cap(3)).toInt(), QVariant(rx.cap(4)).toInt());
		pos += rx.matchedLength();
	}

	rx.setPattern("Images (\\d+)\\-(\\d+) of (\\d+)\\.");
	rx.indexIn(text);
	int from = QVariant(rx.cap(1)).toInt();
	int to = QVariant(rx.cap(2)).toInt();
	int last = QVariant(rx.cap(3)).toInt();

	handleOldDownloads();

	ignoredUrls = qMax(0, allUrls.size() - source()->limit());

	if(allUrls.isEmpty()) {
		setState(Idle);
		communicator()->send(tr("No results."));
		communicator()->send(QUCommunicatorInterface::Done);
		return;
	}

	setState(ImageRequest);

	for(int i = 0; i < allUrls.size() and i < source()->limit(); i++) {
		song()->log(tr("[albumartex - result] ") + "http://" + source()->host() + allUrls.at(i), QU::Help);
		manager()->get(QNetworkRequest(QUrl("http://" + source()->host() + allUrls.at(i))));
	}
}

void QUAlbumArtExCollector::processImageResults(QNetworkReply* reply) {
	song()->log(tr("[QUAlbumArtExCollector] processImageResults(), state() = ") + QString::number(state()), QU::Help);
	QUrl url = reply->url();
	QFile *file = openLocalFile(source()->imageFolder(song()).filePath(QFileInfo(url.toString()).fileName()));

	if(file) {
		file->write(reply->readAll());
	}
}
