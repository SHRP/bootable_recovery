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
#include <fstream>
#include "twcommon.h"
#include "variables.h"
#include "twrp-functions.hpp"
#include "data.hpp"
#include "twinstall.h" // For TWinstall_zip()
#include "gui/gui.hpp"

#include <openssl/sha.h>	// sha hashing
#include <iomanip>		// setw for hashing
#include <random>		// salting

#include "SHRPMAIN.hpp"
#include "SHRPTOOLS.hpp"
#include "SHRPFILETOOLS.hpp"

//JSON_Genarator
string JSON::getVar(string var,string val){
	char exp='"';
	if(val=="true"||val=="false"){
		return exp+var+exp+": "+val;
	}else{
		return exp+var+exp+": "+exp+val+exp;
	}
}
string JSON::getVar(string var,int val){
	char exp='"';
	return exp+var+exp+": "+to_string(val);
}
string JSON::getVar(string var,float val){
	char exp='"';
	return exp+var+exp+": "+to_string(val);
}
string JSON::genarateRAWJson(){
#ifdef SHRP_BUILD_DATE
	string build;
	stringstream date(EXPAND(SHRP_BUILD_DATE));
	date>>build;
#else
	string build="none";
#endif
#ifdef SHRP_EXPRESS
	string express="true";
#else
	string express="false";
#endif
	return getVar("codeName",DataManager::GetStrValue("device_code_name"))+","+getVar("buildNo",build)+","+getVar("isOfficial",DataManager::GetStrValue("is_Official"))+","+getVar("has_express",express)+","+getVar("shrpVer",DataManager::GetStrValue("shrp_ver"));
}

void JSON::storeShrpInfo(){
	if(DataManager::GetIntValue(TW_IS_ENCRYPTED)==0){
		string text="[{"+genarateRAWJson()+"}]";
		//Creating the folder
		TWFunc::Exec_Cmd("mkdir -p /data/shrp/",true);
		//Pushing the json
		fstream file;
		file.open("/data/shrp/shrp_info.json",ios::out);
		file<<text;
		file.close();
	}
}


//Express Functions
#ifdef SHRP_EXPRESS
bool Express::shrpResExp(string inPath,string outPath,bool display){
	LOGINFO("------------\nStarting Express\n");
	bool opStatus;

	// Creating Env for operation
	envRet ret = Express::provideEnvironment(false, inPath);
	if (!ret.envCreated) return false;


	LOGINFO("Inpath - %s \nOutpath - %s \n",inPath.c_str(),outPath.c_str());
	if(TWFunc::Path_Exists(inPath)){
		LOGINFO("Inpath - Exists\n");
		if(!TWFunc::Path_Exists(outPath)){
			LOGINFO("Outpath - Not Exists\nCreating new one\n");
			TWFunc::Exec_Cmd("mkdir -p "+outPath,display,display);
		}
		//Execption
		if(inPath == "/twres/"){
			opStatus = TWFunc::Exec_Cmd("cp -r /twres/images "+outPath,display,display) == 0 ? true : false;
			opStatus = (opStatus && TWFunc::Exec_Cmd("cp -r /twres/dynamic "+outPath,display,display) == 0 && opStatus) ? true : false;
			opStatus = (opStatus && TWFunc::Exec_Cmd("cp -r /twres/slts "+outPath,display,display) == 0 && opStatus) ? true : false;
			opStatus = (opStatus && TWFunc::Exec_Cmd("cp -r /twres/version "+outPath,display,display) == 0 && opStatus) ? true : false;
		}else{
			opStatus = TWFunc::Exec_Cmd("cp -r " + inPath + "* "+outPath,display,display) == 0 ? true : false;
		}

		opStatus ? LOGINFO("Executed Successfully\n") : LOGINFO("Execution Failed\n");
		//END

	}else{
		LOGINFO("Inpath - Not Exists\nExiting....\n");
		opStatus=true;
	}

	Express::cleanup(ret);

	LOGINFO("Express Processing End\n------------\n");
	return opStatus;
}

void Express::flushSHRP(){
	string basePath=DataManager::GetStrValue("shrpBasePath");

	// Creating Env for operation
	envRet ret = Express::provideEnvironment(true);
	if (!ret.envCreated) return;

	if(TWFunc::Path_Exists(basePath + "/etc/shrp")){
		TWFunc::Exec_Cmd("cp -r " + basePath + "/etc/shrp/slts /tmp/", true, true);
		TWFunc::Exec_Cmd("rm -r " + basePath + "/etc/shrp/*", true, true);
		TWFunc::Exec_Cmd("cp -r /tmp/slts " + basePath + "/etc/shrp/", true, true);
	}
	if(TWFunc::Path_Exists("/tmp/shrp")){
		TWFunc::Exec_Cmd("rm -rf /tmp/shrp",true,true);
	}
	
	Express::cleanup(ret);
}

void Express::init(){
	uint64_t version=0;
	unsigned long long buildNo=1;
	string basePath = DataManager::GetStrValue("shrpBasePath");
	DataManager::GetValue("buildNo",buildNo);
	LOGINFO("Welcome to SHRP -----------\n");
	
	// Creating Env for operation
	envRet ret = Express::provideEnvironment(true);
	if (!ret.envCreated) return;


	if(TWFunc::Path_Exists(basePath+"/etc/shrp/version")){
		TWFunc::read_file(basePath+"/etc/shrp/version", version);
	}

	if(version != (uint64_t)buildNo){
		LOGINFO("Resource Version Not Matched\n");
		
		if(TWFunc::Path_Exists(basePath + "/etc/shrp")){
			LOGINFO("Deleting Old Resources\n");
			TWFunc::Exec_Cmd("cp -r " + basePath + "/etc/shrp/slts /tmp/", true, true);
			TWFunc::Exec_Cmd("rm -r " + basePath + "/etc/shrp/*", true, true);
			TWFunc::Exec_Cmd("cp -r /tmp/slts " + basePath + "/etc/shrp/", true, true);
			TWFunc::Exec_Cmd("cp -r /twres/version " + basePath + "/etc/shrp/", true, true);
		}
		if(TWFunc::Path_Exists("/tmp/shrp")){
			TWFunc::Exec_Cmd("rm -rf /tmp/shrp", true, true);
		}
	}

	//Fetching the saved resources if available
	if(TWFunc::Path_Exists(basePath + "/etc/shrp")){
		LOGINFO("Fetching Saved Resources\n");
		TWFunc::Exec_Cmd("cp -r " + basePath + "/etc/shrp/* /twres/", true, true);
	}

	Express::cleanup(ret);
	return;
}



bool Express::expBackup(){
	string storePath = DataManager::GetStrValue("shrpBasePath") + "/etc/shrp";
	string tmpPath = "/tmp/shrp";
	string prevFolder = FileManager::getPrevFolderPath(tmpPath);

	LOGINFO("ExpressBackup Func() Started --------\n");
	
	
	// Creating Env for operation
	envRet env_ret = Express::provideEnvironment(true);
	if (!env_ret.envCreated) return false;

	LOGINFO("StorePath - %s \nTmpPath - %s \n",storePath.c_str(),tmpPath.c_str());


	if(!TWFunc::Path_Exists(storePath)){
		LOGINFO("No Backupped up data found\nExiting ... \n");
		return false;
	}
	if(FileManager::isDirEmpty(storePath)){
		FileManager::removeFolder(storePath);
		return false;
	}

	if(TWFunc::Path_Exists(tmpPath)){
		FileManager::removeFolder(tmpPath);
	}

	bool ret = FileManager::copy(storePath, prevFolder, true);
	
	if(ret && TWFunc::Path_Exists(tmpPath)){
		FileManager::removeFolder(storePath);
	}else if(TWFunc::Path_Exists(tmpPath)){
		FileManager::removeFolder(tmpPath);
	}
	

	Express::cleanup(env_ret);

	LOGINFO("ExpressBackup Func() Ended.--------\nExiting...\n");

	return ret;
}


void Express::expRestore(bool f){
	if(!f) return;

	string storePath = DataManager::GetStrValue("shrpBasePath") + "/etc/shrp";
	string tmpPath = "/tmp/shrp";
	string prevFolder = FileManager::getPrevFolderPath(storePath);

	LOGINFO("ExpressRestore Func() Started --------\n");
	
	// Creating Env for operation
	envRet env_ret = Express::provideEnvironment(true);
	if (!env_ret.envCreated) return;


	if(TWFunc::Path_Exists(storePath)){
		FileManager::removeFolder(storePath);
	}
	FileManager::createFolder(prevFolder);

	if(!TWFunc::Path_Exists(tmpPath)) return;

	FileManager::copy(tmpPath, prevFolder, true);
	
	

	Express::cleanup(env_ret);
	LOGINFO("ExpressRestore Func() Ended.--------\nExiting...\n");
}

#endif

envRet Express::provideEnvironment(bool forceMount, string inPath) {
	string storePath = DataManager::GetStrValue("shrpBasePath") + "/etc/shrp";
	envRet ret;
#ifdef SHRP_EXPRESS_USE_DATA
	bool data_mounted = PartitionManager.Is_Mounted_By_Path(storePath) ? true : false;
	
	if (!data_mounted) return ret;

	ret.mountStatus = true;
	ret.envCreated = true;
#else
	LOGINFO("Mount status of system partition - ");
	//To Check If the System is Mounted or not for a decision parameter which helpes us to back normal state of system mountation before Express call
	if(PartitionManager.Is_Mounted_By_Path(PartitionManager.Get_Android_Root_Path())){
		LOGINFO("Mounted\n");
		ret.mountStatus = true;
	}else{
		LOGINFO("Not Mounted\n");
		ret.mountStatus = false;
	}

	if (forceMount) {
		minUtils::remountSystem(false);
	} else if (!(ret.mountStatus && minUtils::find(inPath, DataManager::GetStrValue("shrpBasePath")))){
		minUtils::remountSystem(false);
	}

	ret.envCreated = true;
#endif
	return ret;
}


void Express::cleanup(envRet ret) {
#ifndef SHRP_EXPRESS_USE_DATA
	string storePath = DataManager::GetStrValue("shrpBasePath") + "/etc/shrp";
	if (!ret.mountStatus && ret.envCreated) {
		PartitionManager.UnMount_By_Path(PartitionManager.Get_Android_Root_Path(),false);
		LOGINFO("System unmounted\n");
	}
#endif
	return;
}

bool Express::is_backupped(){
	string tmpPath = "/tmp/shrp";
	return TWFunc::Path_Exists(tmpPath) && !FileManager::isDirEmpty(tmpPath) ? true : false;
}



void Express::updateSHRPBasePath(){
#ifdef SHRP_EXPRESS_USE_DATA
	DataManager::SetValue("shrpBasePath", "/data/unencrypted");
#else
	string rootPath = PartitionManager.Get_Android_Root_Path();
	DataManager::SetValue("shrpBasePath", rootPath + "/system");
#endif
	LOGINFO("SHRP CURRENT BASEPATH : %s\n", DataManager::GetStrValue("shrpBasePath").c_str());
}


//Hasher Class
bool Hasher::LockPassInit(string str){
	std::ifstream f;
	arg=str;
	if(TWFunc::Path_Exists("/sdcard/SHRP/data/slts")){
		f.open("/sdcard/SHRP/data/slts", ios::in);
	}else{
		f.open("/twres/slts", ios::in);
	}
	if(!f){
		return false;
	}
	f.read(getlp,1);
	lock_pass.append(getlp,1);

	f.seekg(1,ios::beg);
	f.read(getfs,BUFFER_SIZE_SLT);
	fsalt.append(getfs,BUFFER_SIZE_SLT);

	f.seekg(BUFFER_SIZE_SLT+1,ios::beg);
	f.read(gethp,BUFFER_SIZE_PW);
	fhash.append(gethp,BUFFER_SIZE_PW);
  	f.close();
	return true;
}

bool Hasher::isPassCorrect(){
	chash = create_sha256(arg.c_str() + fsalt);
	std::string givpw=lock_pass+fsalt+chash; // reconstructed type + reconstructed salt + generated hash
	std::string recpw=lock_pass+fsalt+fhash; // reconstructed type + reconstructed salt + reconstructed hash
	return givpw==recpw ? true : false;
}

string Hasher::doHash(string str){
	string salt = create_salt(BUFFER_SIZE_SLT);
	return (salt + create_sha256(str + salt));
}


string Hasher::create_sha256(const string str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

string Hasher::create_salt( size_t length ){
    static std::string chrs = "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    thread_local static std::mt19937 rg{std::random_device{}()};
    thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);

    std::string s;

    s.reserve(length);

    while(length--)
        s += chrs[pick(rg)];

    return s;
}



//SIG Helpers
float roundSize(float var) {
	float value = (int)(var * 100 + .5);
	return(float)value / 100;
}
void process_space(int size,int free,storageInfo storage){
	string partition,temp,tmp;
	int p_val_usage=0;
	float size_g,free_g;
	int used = size-free;
	if(size>0){
		if(free>=1024){
			free_g=(float)free/1024;
			free_g=roundSize(free_g);
			{
			stringstream buff;
			buff<<free_g;
			buff>>temp;
			tmp=temp+" GB free of ";
			}
		}else{
			{
			stringstream buff;
			buff<<free;
			buff>>temp;
			tmp=temp+" MB free of ";
			}
		}
		if(size>=1024){
			size_g=(float)size/1024;
			size_g=roundSize(size_g);
			{
			stringstream buff;
			buff<<size_g;
			buff>>temp;
			tmp=tmp+temp+" GB";
			}
		}else{
			{
			stringstream buff;
			buff<<size;
			buff>>temp;
			tmp=temp;
			tmp=tmp+temp+" MB";
			}
		}
		p_val_usage=used*100/size;
	}
	DataManager::SetValue(storage.freeStrVar,(size<=0 ? "Unavailable" : tmp.c_str()));
	//DataManager::SetValue(storage.freePercentageVar,(size<=0 ? 0 : p_val_usage));
	DataManager::SetValue(storage.freePercentageVar,(size<=0 ? 0 : (440 * p_val_usage /100)));//440 is the width of bar
}




//FLashManager
funcRet FlashManager::funcInit(string f){
	funcRet F;

	if(!(f == "restoredefaultsettings" || f == "flash" || f == "wipe" || f == "nandroid" || f == "flashimage" || f == "adbsideload")) return F;

#ifdef TW_INCLUDE_DEVICEAB
	if(f == "flash" || f == "flashimage"){
		F.injectSHRP_ret = FlashManager::injectSHRP_Pre();
	}
#endif

#ifdef SHRP_EXPRESS
	if(f == "flash" || f == "wipe" || f == "nandroid" || f == "flashimage" || f == "adbsideload"){
		F.expressBACKUP_ret = Express::expBackup();
	}

	if(f == "wipe"){
		Express::shrpResExp("/sdcard/SHRP/","/tmp/SDATA/");
	}
	if(f == "restoredefaultsettings") Express::flushSHRP();
#endif

	return F;
}

void FlashManager::funcPost(string f, funcRet F){
	if(!(f == "flash" || f == "wipe" || f == "nandroid" || f == "flashimage" || f == "adbsideloadcancel")) return;

#ifdef TW_INCLUDE_DEVICEAB
	if(f == "flash" || f == "flashimage"){
		FlashManager::injectSHRP_Post(F.injectSHRP_ret);
	}
#endif

	if(f == "flash"){
		FlashManager::injectMagisk(TW_MKINJECT_AFTER_ZIP);
	}

#ifdef SHRP_EXPRESS
	if(f == "flash" || f == "wipe" || f == "nandroid" || f == "flashimage" || f == "adbsideloadcancel"){
		Express::updateSHRPBasePath();
		Express::expRestore(F.expressBACKUP_ret);
	}

	if(f == "wipe"){
		Express::shrpResExp("/tmp/SDATA/","/sdcard/SHRP/");
	}
#endif

	minUtils::remountSystem(false);
}

#ifdef TW_INCLUDE_DEVICEAB
bool FlashManager::injectSHRP_Pre(){
	bool isInjectEnabled = DataManager::GetIntValue(TW_INJECT_AFTER_ZIP) == 1 ? true : false;
	FlashManager::active_slot = DataManager::GetIntValue(TW_ACTIVE_SLOT_INSTALL) == 1 ? 1 : 0;
	bool err = false;

#ifdef TW_USE_TOOLBOX
	if(FlashManager::active_slot == 1) TWFunc::Exec_Cmd("setprop tw_active_slot_install 1");
#endif	

	if(isInjectEnabled){
		//BackupBefore Flash
		err = TWFunc::Exec_Cmd("sh /twres/scripts/backup_ab.sh") == 0 ? false : true;
		err = (TWFunc::Path_Exists("/dev/tmp/shrpinj/old_a/ramdisk.cpio") && TWFunc::Path_Exists("/dev/tmp/shrpinj/old_b/ramdisk.cpio") && err == false) ? true : false;
		gui_msg(Msg( err ? "[!!] One file doesn't exist, exlcluding injection!!" : "[i] Backup of both boot imgs done! Proceeding.",0));
	}
	return !err;
}

void FlashManager::injectSHRP_Post(bool preConfirm){
	if(!preConfirm){
		if(DataManager::GetIntValue(TW_INJECT_AFTER_ZIP)){
			gui_msg(Msg("[!] Please flash a SHRP zip file manually.",0));
			gui_msg(Msg("[!] SHRP injection aborted/failed.",0));
		}
		return;
	}

	if (FlashManager::active_slot) {
		FlashManager::active_slot = 0;
#ifdef TW_USE_TOOLBOX
		TWFunc::Exec_Cmd("setprop tw_active_slot_install 0");
#endif
  	}
	
	gui_msg(Msg(TWFunc::Exec_Cmd("sh /twres/scripts/restore_ab.sh") == 0 ? "[i] SHRP restored successfully!" : "[!!] Restore failed! Please flash manually a SHRP zip file.",0));
#ifdef TW_USE_TOOLBOX
	TWFunc::Exec_Cmd("setprop tw_inject_after_zip 0");
#endif
}

#endif


int FlashManager::injectMagisk(string var){
	if(DataManager::GetIntValue(var) != 1) return 0;
	string zipLoc = TWFunc::Path_Exists("/sdcard/SHRP/addons/c_magisk.zip") ? "/sdcard/SHRP/addons/c_magisk.zip" : "/twres/addons/c_magisk.zip";
	int wipe_cache = 0;

	TWFunc::SetPerformanceMode(true);
	int ret = TWinstall_zip(zipLoc.c_str(), &wipe_cache);
	TWFunc::SetPerformanceMode(false);
	return ret;
}