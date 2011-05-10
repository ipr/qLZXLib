#include "qlzxlib.h"

#include <exception>

#include "UnLzx.h"


///////////// protected

void QLZXLib::PrepareArchive(QString &szArchive)
{
	if (m_pLZXHandler != nullptr)
	{
		if (m_pLZXHandler->GetArchiveFileName() == szArchive.toStdString())
		{
			// same archive already -> nothing to do?
			return;
		}
		
		delete m_pLZXHandler;
	}

	m_pLZXHandler = new CUnLzx(szArchive.toStdString());

	// status display by signals, errors by exceptions
	//connect(m_pLZXHandler, SIGNAL(message(QString)), this, SIGNAL(message(QString)));
	//connect(m_pLZXHandler, SIGNAL(warning(QString)), this, SIGNAL(warning(QString)));
}

///////////// public

QLZXLib::QLZXLib(QObject *parent)
	: QObject(parent)
	, m_pLZXHandler(nullptr)
{
}

QLZXLib::~QLZXLib()
{
	if (m_pLZXHandler != nullptr)
	{
		delete m_pLZXHandler;
	}
}

///////// various operation flags and options

void QLZXLib::SetArchive(QString szArchive)
{
	PrepareArchive(szArchive);
}

///////// actual operations below

bool QLZXLib::Extract(QString &szExtractPath)
{
	try
	{
		return m_pLZXHandler->Extract(szExtractPath.toStdString());
	}
	catch (std::exception &exp)
	{
		emit fatal_error(exp.what());
	}
	return false;
}

bool QLZXLib::List(QLZXLib::tEntryInfoList &lstArchiveInfo)
{
	try
	{
		tArchiveEntryList lstFiles;
		if (m_pLZXHandler->View(lstFiles) == false)
		{
			return false;
		}
		
		// must make copy for caller
		
		auto it = lstFiles.begin();
		auto itEnd = lstFiles.end();
		while (it != itEnd)
		{
			CArchiveEntry &ArcEntry = it->second;

			unsigned int year, month, day;
			unsigned int hour, minute, second;
			ArcEntry.m_Header.GetTimestampParts(year, month, day, hour, minute, second);
			
			lstArchiveInfo.push_back(CEntryInfo());
			CEntryInfo &Info = lstArchiveInfo.back();
			
			Info.m_bIsMerged = ArcEntry.m_bIsMerged;
			Info.m_bPackedSizeAvailable = ArcEntry.m_bPackedSizeAvailable;
			
			Info.m_uiCrc = ArcEntry.m_uiCrc;
			Info.m_uiDataCrc = ArcEntry.m_uiDataCrc;
			
			Info.m_ulUnpackedSize = ArcEntry.m_ulUnpackedSize;
			Info.m_ulPackedSize = ArcEntry.m_ulPackedSize;
			
			Info.m_Date = QDate(year, month, day);
			Info.m_Time = QTime(hour, minute, second);

			Info.m_szFileName = QString::fromStdString(ArcEntry.m_szFileName);
			Info.m_szComment = QString::fromStdString(ArcEntry.m_szComment);
			
			++it;
		}
		
		return true;
	}
	catch (std::exception &exp)
	{
		emit fatal_error(exp.what());
	}
	return false;
}

QString QLZXLib::GetArchiveFileName()
{
	return QString::fromStdString(m_pLZXHandler->GetArchiveFileName());
}

size_t QLZXLib::GetArchiveFileSize()
{
	return m_pLZXHandler->GetArchiveFileSize();
}

unsigned long QLZXLib::GetTotalSizeUnpacked() 
{ 
	return m_pLZXHandler->GetTotalSizeUnpacked(); 
}

unsigned long QLZXLib::GetTotalSizePacked() 
{ 
	return m_pLZXHandler->GetTotalSizePacked(); 
}

unsigned long QLZXLib::GetTotalFileCount() 
{ 
	return m_pLZXHandler->GetTotalFileCount(); 
}
unsigned long QLZXLib::GetMergeSize()
{
	return m_pLZXHandler->GetMergeSize(); 
}

