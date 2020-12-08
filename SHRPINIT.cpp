/*
Copyright 2019 - 2020 SKYHAWK RECOVERY PROJECT

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include "gui/gui.hpp"
#include "data.hpp"
#include "partitions.hpp"
#include <list>
#include "twcommon.h"
#include "cutils/properties.h"

#include "SHRPINIT.hpp"

void SHRP::INIT(){
    printRecDetails();
    genarateDate();
    handleLock();
}


void SHRP::printRecDetails(){
	string tmp;
	gui_msg(Msg("|SKYHAWK RECOVERY PROJECT",0));
	DataManager::GetValue("shrp_ver",tmp);
	tmp="|Version - "+tmp;
	gui_msg(Msg(tmp.c_str(),0));
	if(DataManager::GetStrValue("is_Official") == "true"){
		tmp="|Status - Official";
	}else{
		tmp="|Status - Unofficial";
	}
	gui_msg(Msg(tmp.c_str(),0));
	DataManager::GetValue("device_code_name",tmp);
	tmp="|Device - "+tmp;
	gui_msg(Msg(tmp.c_str(),0));
#ifdef SHRP_BUILD_DATE
	tmp="|Build - "+DataManager::GetStrValue("buildNo");
	gui_msg(Msg(tmp.c_str(),0));
#endif
}


void SHRP::genarateDate(){
	stringstream day;
	string Current_Date,month,week,main_result,day_s;
	time_t seconds = time(0);
	struct tm *t = localtime(&seconds);
	{
		string time;
		DataManager::GetValue("tw_ls_time",time);
		DataManager::SetValue("tw_ls_time",time.c_str());
	}
	int m=t->tm_mon+1;
	int y=t->tm_year+1900;
	int d=t->tm_mday;
	static int tmp[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
	y -= m < 3;
	int w=( y + y / 4 - y / 100 + y / 400 + tmp[m - 1] + d) % 7;
	switch(t->tm_mon+1){
		case 1:month=" Jan";
		break;
		case 2:month=" Feb";
		break;
		case 3:month=" Mar";
		break;
		case 4:month=" Apr";
		break;
		case 5:month=" May";
		break;
		case 6:month=" Jun";
		break;
		case 7:month=" Jul";
		break;
		case 8:month=" Aug";
		break;
		case 9:month=" Sep";
		break;
		case 10:month=" Oct";
		break;
		case 11:month=" Nov";
		break;
		case 12:month=" Dec";
		break;
	}
	switch(w){
		case 0:week="Sun, ";
		break;
		case 1:week="Mon, ";
		break;
		case 2:week="Tue, ";
		break;
		case 3:week="Wed, ";
		break;
		case 4:week="Thu, ";
		break;
		case 5:week="Fri, ";
		break;
		case 6:week="Sat, ";
		break;
	}
	day<<t->tm_mday;
	day>>day_s;
	main_result=week+day_s+month;
	DataManager::SetValue("c_lock_screen_date",main_result);
}


void SHRP::handleLock(){
	FILE *f;
	char hello[50];
	f = fopen("/sdcard/SHRP/data/slts","r");
	if(f == NULL){
		f = fopen("/twres/slts","r");
	}
	if(f != NULL){
		fgets(hello, 50, f);
		fclose(f);
		if(hello[0] == '1'){
			//Password Protected Recovery
			DataManager::SetValue("c_target_destination","c_pass_capture");
			DataManager::SetValue("recLockStatus",1);
			property_set("shrp.lock","1");
			PartitionManager.Disable_MTP();
		}else if(hello[0] == '2'){
			//Pattern Protected Recovery
			DataManager::SetValue("c_target_destination","c_patt_capture");
			DataManager::SetValue("recLockStatus",2);
			property_set("shrp.lock","1");
			PartitionManager.Disable_MTP();
		}else{
			//Unprotected Recovery
			DataManager::SetValue("c_target_destination","main2");
			DataManager::SetValue("recLockStatus",0);
			property_set("shrp.lock","0");
		}
	}else{
		DataManager::SetValue("c_target_destination","c_recBlocked");
		DataManager::SetValue("recLockStatus",69);
		property_set("shrp.lock","1");
		PartitionManager.Disable_MTP();
	}
}