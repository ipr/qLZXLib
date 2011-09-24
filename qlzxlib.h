#ifndef QLZXLIB_H
#define QLZXLIB_H

#include "qLZXLib_global.h"

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QList>


class CUnLzx;

class QLZXLIBSHARED_EXPORT QLZXLib : public QObject
{
	Q_OBJECT

private:
	CUnLzx *m_pLZXHandler;

protected:
	void PrepareArchive(QString &szArchive);
	
public:
    QLZXLib(QObject *parent = 0);
    virtual ~QLZXLib();

	// for each file entry in archive (for caller)
	class CEntryInfo
	{
	public:
		CEntryInfo(void)
			: m_uiCrc(0)
			, m_uiDataCrc(0)
			, m_Date()
			, m_Time()
			, m_ulUnpackedSize(0)
			, m_bPackedSizeAvailable(true)
			, m_ulPackedSize(0)
			, m_bIsMerged(false)
			, m_szFileName()
			, m_szComment()
		{}
		~CEntryInfo(void)
		{}
	
	
		// CRC from file header (32-bit)
		unsigned int m_uiCrc;
	
		// CRC of data (32-bit)
		unsigned int m_uiDataCrc;

		// timestamp in parts
		QDate m_Date;
		QTime m_Time;
		
		// unpacked size from file
		unsigned long m_ulUnpackedSize;

		// for some files, packed size might not be given in archive
		// (merged files only?):
		// merged files are compressed together and no individual packing sizes..
		//
		bool m_bPackedSizeAvailable;
	
		// compressed size from file (if available)
		//
		unsigned long m_ulPackedSize;
	
		// if file is merged with another
		bool m_bIsMerged;
	
		// name of entry from archive
		QString m_szFileName;
	
		// comment of entry from archive (if available)
		QString m_szComment;
	};

	// simplest way to list all files in archive for caller (must be copy)
	typedef QList<CEntryInfo> tEntryInfoList;
	
public slots:
	void SetArchive(QString szArchive);
	void SetAllowBadCrc(const bool bValue);
	void SetSkipUnknownPackMode(const bool bValue);

	//////////////////	
	// actual operations below
	bool Extract(QString &szExtractPath);
	bool ExtractSelected(QString &szExtractPath, QStringList &lstFiles);
	bool ExtractToBuffer(QString &szFileEntry, QByteArray &outArray);
	bool List(QLZXLib::tEntryInfoList &lstArchiveInfo);
	bool Test();

signals:
	void message(QString);
	void warning(QString);
	void error(QString);
	void fatal_error(QString);
	
public:
	// information about archive file itself
	QString GetArchiveFileName();
	size_t GetArchiveFileSize();

	// statistical information access to caller
	unsigned long GetTotalSizeUnpacked();
	unsigned long GetTotalSizePacked();
	unsigned long GetTotalFileCount();
	//unsigned long GetMergeSize(); // per-group only
	
};

#endif // QLZXLIB_H
