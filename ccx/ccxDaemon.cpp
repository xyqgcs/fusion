/***********************************************************************
 ** Copyright (C) 2010 Movid Authors.  All rights reserved.
 **
 ** This file is part of the Movid Software.
 **
 ** This file may be distributed under the terms of the Q Public License
 ** as defined by Trolltech AS of Norway and appearing in the file
 ** LICENSE included in the packaging of this file.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** Contact info@movid.org if any conditions of this licensing are
 ** not clear to you.
 **
 **********************************************************************/


#include <iostream>
#include <fstream>

#include "ccxDaemon.h"
#include "ccxLog.h"
#include "ccxFactory.h"

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif

LOG_DECLARE("Daemon");

void ccxDaemon::init() {
	ccxFactory::init();

#ifdef WIN32
	// initialize network for Win32 platform
	{
		WSADATA wsaData;
		if ( WSAStartup(MAKEWORD(2, 2), &wsaData) == -1 )
			LOG(CCX_CRITICAL, "unable to initialize WinSock (v2.2)");
	}
#endif
}

bool ccxDaemon::detach(std::string pidfilename) {
#ifndef WIN32
	pid_t pid = fork();
	if (pid > 0) {
		LOG(CCX_INFO, "child process created with pid " << pid);
		try {
			std::ofstream pidfile(pidfilename.c_str(), std::ios::out|std::ios::trunc);
			if (pidfile) {
				pidfile << pid << std::endl;
				pidfile.close();
			} else {
				LOG(CCX_ERROR, "Cannot write pidfile " << pidfilename);
			}
		} 
		catch(std::exception x) {
			LOG(CCX_ERROR, "Cannot write pidfile " << pidfilename << ": " << x.what());
		}
	}
	if (pid < 0)
		LOG(CCX_ERROR, "no child process could be created, but this process is still living");
	return(pid <= 0);
#endif
	// TODO implement other platform
	LOG(CCX_INFO, "This platform don't support detach yet.");
	return true;
}

void ccxDaemon::cleanup() {
#ifdef _WIN32
	WSACleanup();
#endif

	ccxFactory::cleanup();
	ccxLog::cleanup();
}

