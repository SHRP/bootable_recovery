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
#include <unistd.h>
#include <sys/stat.h>
#include <string>
#include <vector>
#include "twcommon.h"
#include "twrp-functions.hpp"
#include "gui/gui.hpp"

#include "SHRPTOOLS.hpp"

//SHRP minUtils
bool minUtils::compare(string str1,string str2){
    transform(str1.begin(),str1.end(),str1.begin(), ::tolower);
    transform(str2.begin(),str2.end(),str2.begin(), ::tolower);
    return (str1==str2) ? true : false;
}

bool minUtils::isFileEditable(string fileExtension){
	vector<string> extensions = {".txt", ".xml", ".prop", ".sh", ".conf", ".json", ".cfg", ".rc", ".d", ".md"};

    if(fileExtension == "" || fileExtension == "none") return true;

	for(auto it = extensions.begin(); it < extensions.end(); it++){
		if(compare(fileExtension, *it)) return true;
	}
    return false;
}

bool minUtils::find(std::string str,std::string sub){
	if(str.find(sub) == string::npos){
		return false;
	}else{
		return true;
	}
}
bool minUtils::find(std::string str,std::string sub,int dummy){
	transform(str.begin(),str.end(),str.begin(), ::tolower);
	transform(sub.begin(),sub.end(),sub.begin(), ::tolower);
	if(str.find(sub) == string::npos){
		return false;
	}else{
		return true;
	}
}

void minUtils::remountSystem(bool display){
	if(PartitionManager.Is_Mounted_By_Path(PartitionManager.Get_Android_Root_Path())){
	  	PartitionManager.UnMount_By_Path(PartitionManager.Get_Android_Root_Path(),false);
	  	unlink("/system");
		mkdir("/system", 0755);
	}
	TWFunc::Exec_Cmd("mount -w "+PartitionManager.Get_Android_Root_Path(),display);
	if(display){
		gui_msg("remount_system_rw=[i] Remounted system as R/W!");
	}
}

string minUtils::getExtension(string str,string arg){
	int dotPos=str.find_last_of('.');
	if(dotPos == -1 || dotPos== (int) str.length()-1){
		return "none";
	}else{
		return (arg+str.substr(dotPos+1, str.length() - dotPos));
	}
}