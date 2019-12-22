/**       GNU GENERAL PUBLIC LICENSE, Version 3
 * cpulimit - CPU usage limiter for windows (cygwin)
 * Copyright (C) 2012 Alexandre Quercia <alquerci@email.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// cpulimit.cpp : d閒init le point d'entr閑 pour l'application console.
//

#include "stdafx.h"
#include "ProcessHandler.h"

int TraverseProcesses(int *ids)
{
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);

	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcessSnap == INVALID_HANDLE_VALUE) {
		return 0;
	}

	BOOL bResult =Process32First(hProcessSnap, &pe32);//开始遍历

	int num = 0;

	while(bResult)
	{
		int id = pe32.th32ProcessID;
		if(id != 0) {
			ids[num] = id;
			num++;
		}

		bResult = Process32Next(hProcessSnap,&pe32);//返回值为false，表明进程遍历完
	}

	CloseHandle(hProcessSnap);
	return num;
}

#define maxprocess 1024*128

int main(int argc, char **argv)
{
    Config *rootset = new Config(argc, argv);
	rootset->SetLimit(50);

	int *ids = new int[maxprocess];
	Config **setarray = new Config*[maxprocess];
	ProcessHandler **phs = new ProcessHandler*[maxprocess];
	while(1) {
		int num = TraverseProcesses(ids);

		for(int i = 0 ; i < num - 1 ; i++) {
			if(ids[i] != GetCurrentProcessId()) {
				setarray[i] = new Config(argc, argv);
				setarray[i]->SetProcessId(ids[i]);

				phs[i] = new ProcessHandler(setarray[i]);
			}
		}

		for(int i = 0 ; i < num - 1 ; i++) {
			phs[i]->Suspend();
		}

		int s = rootset->GetTimeOff();
		Sleep(s);

		for(int i = 0 ; i < num - 1 ; i++) {
			phs[i]->Resume();
			delete phs[i];
			delete setarray[i];
		}

		s = rootset->GetTimeOn();
		Sleep(s);
	}
}
