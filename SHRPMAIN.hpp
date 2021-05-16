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
//For FlashManager
struct funcRet{
	bool injectSHRP_ret = false;
	bool expressBACKUP_ret = false;
};

// For Express
struct envRet {
	bool mountStatus = false;
	bool envCreated = false;
};

//JSON_Genarator
class JSON{
	public:
		static string getVar(string,string);
		static string getVar(string,int);
		static string getVar(string,float);
		static string genarateRAWJson();
		static void storeShrpInfo();
};

//EXPRESS Class
class Express{
	public:
#ifdef SHRP_EXPRESS
		static bool shrpResExp(string inPath,string outpath,bool display = true);
		static void flushSHRP();
		static void init();
		static bool expBackup();
		static void expRestore(bool f);
		static int funcInit(string funcName);	
#endif
		static envRet provideEnvironment(bool forceMount = true, string inPath = "");
		static void cleanup(envRet ret);
		static bool is_backupped();
		static void updateSHRPBasePath();
};


//FlashManager
class FlashManager{
	public:
		static int active_slot;
	public:
#ifdef TW_INCLUDE_DEVICEAB
		static bool injectSHRP_Pre();
		static void injectSHRP_Post(bool preConfirm);
#endif
		static int injectMagisk(string var);
		static funcRet funcInit(string f);
		static void funcPost(string f, funcRet F);
};

#define BUFFER_SIZE_SLT 16
#define BUFFER_SIZE_PW 64

//Hasher Class
class Hasher{
	private:
		std::string arg,fsalt,fhash,chash;
		char getlp[1];
		char getfs[BUFFER_SIZE_SLT];
		char gethp[BUFFER_SIZE_PW];
	public:
		string lock_pass;
		static string create_sha256(const string str);
		static string create_salt( size_t length );
		bool LockPassInit(string str);
		bool isPassCorrect();
		static string doHash(string str);
};

//SIG Helpers
struct storageInfo{
	string storageLocation;
	string freeStrVar;
	string freePercentageVar;
};

void process_space(int size,int free,storageInfo storage);