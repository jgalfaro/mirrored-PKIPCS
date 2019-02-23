/*
Télécom SudParis
2017, February
Pierre-Olivier Rocher
*/

#pragma comment(lib, "DbgHelp")
#define MAX_BUFF_SIZE 1024

#include <Windows.h>
#include <Dbghelp.h>
#include <iostream>
#include <tchar.h>
#include <strsafe.h>
#include <shlobj.h>

#include "stonepine.utilities.dumpCore.h"

namespace Stonepine
{
	namespace Utilities
	{
		class dumpCore::dumpCoreBase final
		{

		public:

			dumpCoreBase(void) noexcept
			{
				SetUnhandledExceptionFilter(unhandledHandlerCallback);
			}

			~dumpCoreBase(void)
			{

			}

			static const void dumpCoreBase::makeMiniDump(EXCEPTION_POINTERS* e)
			{
				TCHAR tszFileName[MAX_BUFF_SIZE] = { 0 };
				TCHAR tszPath[MAX_BUFF_SIZE] = { 0 };

				SYSTEMTIME stTime = { 0 };

				GetSystemTime(&stTime);

				SHGetSpecialFolderPath(NULL, tszPath, CSIDL_APPDATA, FALSE);

				StringCbPrintf(tszFileName, _countof(tszFileName), _T("%s\\%s__%4d%02d%02d_%02d%02d%02d.dmp"), tszPath, _T("CrashDump"), stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);

				HANDLE hFile = CreateFile(tszFileName, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

				if (hFile == INVALID_HANDLE_VALUE)
				{
					return;
				}

				MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;

				exceptionInfo.ThreadId = GetCurrentThreadId();
				exceptionInfo.ExceptionPointers = e;
				exceptionInfo.ClientPointers = FALSE;

				MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MINIDUMP_TYPE(MiniDumpWithIndirectlyReferencedMemory | MiniDumpScanMemory | MiniDumpWithFullMemory), e ? &exceptionInfo : NULL, NULL, NULL);

				if (hFile)
				{
					CloseHandle(hFile);
					hFile = NULL;
				}

				return;
			}

			static LONG CALLBACK dumpCoreBase::unhandledHandlerCallback(EXCEPTION_POINTERS* e)
			{
				makeMiniDump(e);

				return EXCEPTION_CONTINUE_SEARCH;
			}

		};

		dumpCore::dumpCore(void) noexcept
		{
			m_dumpCoreBase = std::make_unique<dumpCoreBase>();
		}

		dumpCore::~dumpCore(void)
		{

		}
	}
}
 
