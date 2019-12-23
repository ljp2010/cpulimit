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
		if(id != 0 && GetCurrentProcessId() != id) {
			ids[num] = id;
			num++;
		}

		bResult = Process32Next(hProcessSnap,&pe32);//返回值为false，表明进程遍历完
	}

	CloseHandle(hProcessSnap);
	return num;
}

int getgcd(int a, int b) {
	if(a > b) {
		int tmp = b;
		b = a;
		a = b;
	}
	for(int i = a ; i > 0 ; i--) {
		if(a % i == 0 && b % i == 0) return i;
	}
	return 0;
}

#define maxprocess 1024*128

int ids[maxprocess];
	
ProcessHandler *phs[maxprocess];
ProcessHandler *phs2[maxprocess];

int main(int argc, char **argv)
{
    Config *rootset = new Config(argc, argv);

	if(rootset->m_nbTimeOn == 0)
		rootset->SetLimit(50);

	
	
	phs[0] = 0;

	while(1) {
		int num = TraverseProcesses(ids);

		// fill phs
		int phs2num = 0;
		for(int i = 0 ; i < num ; i++) {
			for(int j = 0 ; j < maxprocess ; j++) {
				if(phs[j] == 0) {
					// find a new process
					Config *set = (Config*)(new char[sizeof(Config)]);
					memcpy(set, rootset, sizeof(Config));
					set->SetProcessId(ids[i]);
					ProcessHandler *ph = new ProcessHandler(set);
					phs2[phs2num] = ph;
					phs2num++;
					break;
				}
				if(phs[j]->m_Id == ids[i]) {
					phs2[phs2num] = phs[j];
					phs2num++;
					break;
				}
			}
		}
		phs2[phs2num] = 0;

		// clean phs
		for(int i = 0 ; i < maxprocess ; i++) {
			if(phs[i] == 0) break;
			for(int j = 0 ; j < maxprocess ; j++) {
				if(phs2[j] == 0) {
					delete[] (char*)(phs[i]->m_cfg);
					delete (phs[i]);
					break;
				}
				if(phs2[j] == phs[i]) break;
			}
		}
		for(int i = 0 ; i < maxprocess ; i++) {
			if(phs2[i] == 0) {
				phs[i] = 0;
				break;
			}
			phs[i] = phs2[i];
		}

		// stop process
		for(int i = 0 ; i < maxprocess ; i++) {
			if(phs[i] == 0) break;
			phs[i]->Suspend();
		}

		int soff = rootset->GetTimeOff();
		int son = rootset->GetTimeOn();
		int gcd = getgcd(soff, son);

		int s = soff / gcd;
		Sleep(s);

		for(int i = 0 ; i < maxprocess ; i++) {
			if(phs[i] == 0) break;
			phs[i]->Resume();
		}

		s = son / gcd;
		Sleep(s);
	}
}
