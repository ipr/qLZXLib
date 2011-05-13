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

void QLZXLib::SetAllowBadCrc(const bool bValue)
{
	m_pLZXHandler->SetAllowBadCrc(bValue);
}

void QLZXLib::SetSkipUnknownPackMode(const bool bValue)
{
	m_pLZXHandler->SetSkipUnknownPackMode(bValue);
}

///////// actual operations below

bool QLZXLib::Extract(QString &szExtractPath)
{
	try
	{
		m_pLZXHandler->SetExtractPath(szExtractPath.toStdString());
		return m_pLZXHandler->Extract();
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
		if (m_pLZXHandler->View() == false)
		{
			return false;
		}
		m_pLZXHandler->GetEntryList(lstFiles);
		
		// must make copy for caller
		
		auto it = lstFiles.begin();
		auto itEnd = lstFiles.end();
		while (it != itEnd)
		{
			CArchiveEntry *pEntry = it->second;

			lstArchiveInfo.push_back(CEntryInfo());
			CEntryInfo &Info = lstArchiveInfo.back();
			
			Info.m_bIsMerged = pEntry->m_bIsMerged;
			Info.m_bPackedSizeAvailable = pEntry->m_bPackedSizeAvailable;
			
			Info.m_uiCrc = pEntry->m_uiCrc;
			Info.m_uiDataCrc = pEntry->m_uiDataCrc;
			
			Info.m_ulUnpackedSize = pEntry->m_ulUnpackedSize;
			Info.m_ulPackedSize = pEntry->m_ulPackedSize;
			
			Info.m_Date = QDate(pEntry->m_Timestamp.year, pEntry->m_Timestamp.month, pEntry->m_Timestamp.day);
			Info.m_Time = QTime(pEntry->m_Timestamp.hour, pEntry->m_Timestamp.minute, pEntry->m_Timestamp.second);

			Info.m_szFileName = QString::fromStdString(pEntry->m_szFileName);
			Info.m_szComment = QString::fromStdString(pEntry->m_szComment);
			
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

/* // per merge-group only
unsigned long QLZXLib::GetMergeSize()
{
	return m_pLZXHandler->GetMergeSize(); 
}
*/

