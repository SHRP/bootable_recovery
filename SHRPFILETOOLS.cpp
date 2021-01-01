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
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip> 
#include <fstream> 

//Required local headers
#include "SHRPFILETOOLS.hpp"
#include "SHRPTOOLS.hpp"
#include "data.hpp"
#include "twrp-functions.hpp"
#include "twcommon.h"

//For Checksums
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <openssl/evp.h>
#define BUFFSIZE 16384

//FileSystem Stuff
#include "filesystem.hpp"
namespace fs = ghc::filesystem;

using namespace std;


string FileManager::getFileName(string str){
    if(str.find_last_of('/') == string::npos){
        return str;
    }
    return (str.substr(str.find_last_of('/')+1, (str.length() - str.find_last_of('/'))-1));
}
string FileManager::getFolderName(string str){
    if(str.find_last_of('/') == string::npos){
        return str;
    }

    if(str.find_last_of('/') == str.length()-1){
        str = str.substr(0, str.find_last_of('/'));
    }
    return (str.substr(str.find_last_of('/')+1, (str.length() - str.find_last_of('/'))-1));
}
string FileManager::getPrevFolderPath(string str){
    if(str.find_last_of('/') == str.length()-1){//If Path /sdcard/hello/ then It will remove / from last
        // /sdcard/hello
        str = str.substr(0, str.find_last_of('/'));
    }
    if(str.find_last_of('/') == 0 && str.length() > 1){// /sdcard
        return "";
    }else if(str.find_last_of('/') == 0 || str.find_last_of('/') == string::npos){
        return "";
    }else{
        return (str.substr(0, str.find_last_of('/')));
    }
}

bool FileManager::isFile(string str){
    std::error_code ec;
    return (fs::is_directory(str.c_str(), ec) ? false : true);
}

string FileManager::getSizeStr(string str){
    string sizeStr;
    std::error_code ec;
    long long unsigned int size = 0;
    if(FileManager::isFile(str)){
        size = fs::is_symlink(str, ec) ? 0 : fs::file_size(str, ec);
    }else{
        if(str == "/" || minUtils::find(str, "/proc") || minUtils::find(str, "/sys")){
            size = 0;
        }else{
            for(fs::recursive_directory_iterator it(str, fs::directory_options::skip_permission_denied, ec); it != fs::recursive_directory_iterator(); it.increment(ec)){
                if(!fs::is_directory(*it, ec) && !fs::is_symlink(*it, ec)){
                    size += fs::file_size(*it, ec);
                }
            }
        } 
    }

    string szType = " Bytes";
    long double KB, MB, GB, finalSize;

    if(size < 1024){
        finalSize = size;
    }else{
        KB = size / 1024;
        if(KB < 1024){
            finalSize = KB;
            szType = " KB";
        }else{
            MB = KB / 1024;
            if(MB < 1024){
                MB = (int)(MB * 10);
                finalSize = MB / 10;
                szType = " MB";
            }else{
                GB = MB / 1024;
                GB = (int)(GB * 10);
                finalSize = GB / 10;
                szType = " GB";
            }
        }
    }

    stringstream ss;
    ss<<finalSize;
    sizeStr = ss.str() + szType;
    return sizeStr;
}

bool FileManager::isDirEmpty(string str){
    std::error_code ec;
    return fs::is_empty(str, ec) ? true : false;
}

bool FileManager::copy(string from, string to, bool overwrite){
    LOGINFO("Copying From - %s To %s\n",from.c_str(),to.c_str());
    std::error_code ec;
    const auto copyOptionsOverwrite = fs::copy_options::recursive | fs::copy_options::overwrite_existing | fs::copy_options::copy_symlinks;
    const auto copyOptionsDefault = fs::copy_options::recursive | fs::copy_options::skip_existing | fs::copy_options::copy_symlinks;
    string objName = isFile(from) ? getFileName(from) : getFolderName(from);
    to = to == "/" ? to + objName : to + "/" + objName;

    fs::copy(from, to, overwrite ? copyOptionsOverwrite : copyOptionsDefault, ec);
    return (bool)!ec;
}
bool FileManager::copy(string from, string to, int multiple, bool overwrite){
    if(multiple){
        vector<string> fromAdv = TWFunc::split_string(from, '|', true);
        bool ret;
        for(auto it = fromAdv.begin(); it < fromAdv.end(); it++){
            ret = copy(*it, to, overwrite);
        }
        return ret;
    }else{
        return copy(from, to, overwrite);
    }
}
bool FileManager::move(string from, string to, bool overwrite){
    string objName = isFile(from) ? getFileName(from) : getFolderName(from);
    to = to == "/" ? to + objName : to + "/" + objName;
    LOGINFO("Moving From - %s To %s\n",from.c_str(),to.c_str());
    string command = overwrite ? "mv " : "mv -n ";
    if(!TWFunc::Path_Exists(to) || overwrite){
        return TWFunc::Exec_Cmd(command + string(1,'"') + from + string(1,'"') + " " + string(1,'"') + to + string(1,'"'), true, true) == 0 ? true : false;
    }else{
        return false;
    }
}
bool FileManager::move(string from, string to, int multiple, bool overwrite){
    if(multiple){
        vector<string> fromAdv = TWFunc::split_string(from, '|', true);
        bool ret;
        for(auto it = fromAdv.begin(); it < fromAdv.end(); it++){
            ret = move(*it, to, overwrite);
        }
        return ret;
    }else{
        return move(from, to, overwrite);
    }
}
bool FileManager::rename(string from, string objName, bool overwrite){
    string str = getPrevFolderPath(from);
    string command = overwrite ? "mv " : "mv -n ";
    str = (str == "" || str == "/") ? str + objName : str + "/" + objName;
    if(!TWFunc::Path_Exists(str)){
        return TWFunc::Exec_Cmd(command + string(1,'"') + from + string(1,'"') + " " + string(1,'"') + str + string(1,'"'), true, true) == 0 ? true : false;
    }else{
        return false;
    }
}
bool FileManager::removeFile(string path){
    std::error_code ec;
    return fs::remove(path, ec);
}

int FileManager::removeFolder(string path){
    std::error_code ec;
    return fs::remove_all(path, ec);
}

bool FileManager::remove(string path){
    LOGINFO("Removing - %s\n",path.c_str());
    return (FileManager::isFile(path) ? removeFile(path) ? true : false : (removeFolder(path) <= 0) ? false : true);
}

bool FileManager::remove(string path, int multiple){
    if(multiple){
        vector<string> fromAdv = TWFunc::split_string(path, '|', true);
        bool ret;
        for(auto it = fromAdv.begin(); it < fromAdv.end(); it++){
            ret = remove(*it);
        }
        return ret;
    }else{
        return remove(path);
    }
}

bool FileManager::createFolder(string path){
    string cmd = "mkdir -p " + string(1, '"') + path + string(1, '"') + ";";
    return (TWFunc::Exec_Cmd(cmd) == 0 ? true : false);
}

string FileManager::setPermission(string path, string chmod){
    return "chmod " + chmod + " " + string(1,'"') + path + string(1,'"') + ";";
}
string FileManager::setPermission(string path, int ownerR, int ownerW, int ownerX, int groupR, int groupW, int groupX, int globalR, int globalW, int globalX){
    int chmod = 0;
    chmod += ownerR ? 400 : 0;
    chmod += ownerW ? 200 : 0;
    chmod += ownerX ? 100 : 0;

    chmod += groupR ? 40 : 0;
    chmod += groupW ? 20 : 0;
    chmod += groupX ? 10 : 0;

    chmod += globalR ? 4 : 0;
    chmod += globalW ? 2 : 0;
    chmod += globalX ? 1 : 0;

    stringstream ss;
    ss<<chmod;
    return "chmod " + ss.str() + " " + path + ";";
}
string FileManager::getStrPermission(string path){
    string ret;
    stringstream ss;
    Perm P;
    std::error_code ec;
    P.calculatePerm(path);
    ret = fs::is_symlink(path.c_str(), ec) ? "l" : isFile(path) ? "-" : "d";
    ss<<P.getPermInt();
    ret += P.getPermStr() + ss.str();

    return ret;
}

void FileManager::UpdateGuiPerms(string path){
    Perm P;
    P.calculatePerm(path);
    DataManager::SetValue("permOwnerR", P.Owner.R ? 1 : 0);
    DataManager::SetValue("permOwnerW", P.Owner.W ? 1 : 0);
    DataManager::SetValue("permOwnerX", P.Owner.X ? 1 : 0);
    DataManager::SetValue("permGroupR", P.Group.R ? 1 : 0);
    DataManager::SetValue("permGroupW", P.Group.W ? 1 : 0);
    DataManager::SetValue("permGroupX", P.Group.X ? 1 : 0);
    DataManager::SetValue("permGlobalR", P.Global.R ? 1 : 0);
    DataManager::SetValue("permGlobalW", P.Global.W ? 1 : 0);
    DataManager::SetValue("permGlobalX", P.Global.X ? 1 : 0);

    DataManager::SetValue("NewpermOwnerR", P.Owner.R ? 1 : 0);
    DataManager::SetValue("NewpermOwnerW", P.Owner.W ? 1 : 0);
    DataManager::SetValue("NewpermOwnerX", P.Owner.X ? 1 : 0);
    DataManager::SetValue("NewpermGroupR", P.Group.R ? 1 : 0);
    DataManager::SetValue("NewpermGroupW", P.Group.W ? 1 : 0);
    DataManager::SetValue("NewpermGroupX", P.Group.X ? 1 : 0);
    DataManager::SetValue("NewpermGlobalR", P.Global.R ? 1 : 0);
    DataManager::SetValue("NewpermGlobalW", P.Global.W ? 1 : 0);
    DataManager::SetValue("NewpermGlobalX", P.Global.X ? 1 : 0);
    DataManager::SetValue("newChmod", P.getPermInt());
}

bool FileManager::compress(string path, string fileName, string compressionLvl){
    string folderName = FileManager::getFolderName(path);
    string prevPath = FileManager::getPrevFolderPath(path);
    if(prevPath == ""){return false;}
    string command = "cd " + string(1,'"')  + prevPath + string(1,'"') + "; zip -r -" + compressionLvl + " ";

    return (TWFunc::Exec_Cmd(command + string(1,'"') + prevPath + "/" + fileName + string(1,'"') + " " + string(1,'"') + folderName + string(1,'"'), true, true) == 0 ? true : false);
}

bool FileManager::compressEx(string path, string fileName, string to, string compressionLvl){
    string folderName = FileManager::getFolderName(path);
    string prevPath = FileManager::getPrevFolderPath(path);
    if(prevPath == ""){return false;}
    string command = "cd " + string(1,'"')  + prevPath + string(1,'"') + "; zip -r -" + compressionLvl + " ";

    return (TWFunc::Exec_Cmd(command + string(1,'"') + to + "/" + fileName + string(1,'"') + " " + string(1,'"') + folderName + string(1,'"'), true, true) == 0 ? true : false);
}


bool FileManager::extract(string filePath){
    string extn = minUtils::getExtension(filePath);
    string folderName = filePath.substr(0, filePath.length() - extn.length());
    string path = FileManager::getPrevFolderPath(filePath);
    if(TWFunc::Path_Exists(folderName)){
        std::error_code ec;
        if(!fs::is_empty(folderName, ec)){
            return false;
        }
    }else{
        TWFunc::Exec_Cmd("mkdir -p " + string(1,'"') + folderName + string(1,'"'), true, true);
    }
    return (TWFunc::Exec_Cmd("unzip -d " + string(1,'"') + folderName + string(1,'"') + " " + string(1,'"') + filePath + string(1,'"'), true, true) == 0 ? true : false);
}

bool FileManager::extract(string filePath, string to){
    return (TWFunc::Exec_Cmd("unzip -d " + string(1,'"') + to + string(1,'"') + " " + string(1,'"') + filePath + string(1,'"'), true, true) == 0 ? true : false);
}

void getHex(unsigned char* a, unsigned char* b){
    int i, j;
    i = j = 0;
    while(a[i] != '\0'){
        sprintf((char*) (b+j), "%02x", a[i]);
        i++;
        j+=2;
    }
    b[j++] = '\0';
}

string FileManager::generate_Hash(string path, string hashAlgo){
    // LOGINFO("Stage 0\n");
    // EVP_MD_CTX mdctx;
    // const EVP_MD *md;
    // unsigned char md_value[EVP_MAX_MD_SIZE];
    // unsigned int md_len;
    // EVP_MD_CTX_init(&mdctx);
    // EVP_DigestInit_ex(&mdctx, md, NULL);
    // LOGINFO("Stage 1\n");

    // stringstream ss;
    // char buffer[BUFFSIZE];
    // ifstream ifs(path, std::ifstream::binary);
    // LOGINFO("Stage 2\n");

    // OpenSSL_add_all_digests();

    // md = EVP_get_digestbyname("md5");
    // LOGINFO("Stage 3\n");

    // while (ifs.good()) {
    //     ifs.read(buffer, BUFFSIZE);
    //     EVP_DigestUpdate(&mdctx, buffer, strlen(buffer));
    // }
    // ifs.close();
    // LOGINFO("Stage 4\n");

    // int res = EVP_DigestFinal_ex(&mdctx, md_value, &md_len);
    // EVP_MD_CTX_cleanup(&mdctx);
    // LOGINFO("Stage 5\n");

    // if (res == 0){
    //     return "null";
    // }

    // ss << std::hex << std::uppercase << std::setfill('0');
    // for(unsigned char uc : md_value){
    //     ss << std::setw(2) << (int)uc;
    // }
    // LOGINFO("Stage 6\n");

    stringstream ss;
    ss << std::hex << std::uppercase << std::setfill('0');
    ifstream ifs(path, std::ifstream::binary);

    char buffer[BUFFSIZE];
    unsigned char digest[MD5_DIGEST_LENGTH];
    unsigned char digest2[SHA_DIGEST_LENGTH];
    unsigned char digest3[SHA256_DIGEST_LENGTH];
    MD5_CTX md5Context;
    SHA_CTX sha1Context;
    SHA256_CTX sha256Context;
    
    if(hashAlgo == "md5"){
        MD5_Init(&md5Context);
        while (ifs.good()){
            ifs.read(buffer, BUFFSIZE);
            MD5_Update(&md5Context, buffer, ifs.gcount());
        }
        if(!MD5_Final(digest, &md5Context)) return "null";
        for(unsigned char uc : digest){
            ss << std::setw(2) << (int)uc;
        }
    }else if(hashAlgo == "sha1"){
        SHA1_Init(&sha1Context);
        while (ifs.good()){
            ifs.read(buffer, BUFFSIZE);
            SHA1_Update(&sha1Context, buffer, ifs.gcount());
        }
        if(!SHA1_Final(digest2, &sha1Context)) return "null";
        for(unsigned char uc : digest2){
            ss << std::setw(2) << (int)uc;
        }
    }else if(hashAlgo == "sha256"){
        SHA256_Init(&sha256Context);
        while (ifs.good()){
            ifs.read(buffer, BUFFSIZE);
            SHA256_Update(&sha256Context, buffer, ifs.gcount());
        }
        if(!SHA256_Final(digest3, &sha256Context)) return "null";
        for(unsigned char uc : digest3){
            ss << std::setw(2) << (int)uc;
        }

    }
    ifs.close();

    return ss.str();
}
string FileManager::genarate_SHA1(string path){
    stringstream ss;
    ifstream ifs(path, std::ifstream::binary);

    char buffer[BUFFSIZE];
    unsigned char digest[SHA_DIGEST_LENGTH];
    SHA_CTX sha1Context;
    SHA1_Init(&sha1Context);

    while (ifs.good()) {
        ifs.read(buffer, BUFFSIZE);
        SHA1_Update(&sha1Context, buffer, ifs.gcount());
    }
    ifs.close();

    int res = SHA1_Final(digest, &sha1Context);
    if (res == 0){
        return "null";
    }

    ss << std::hex << std::uppercase << std::setfill('0');
    for(unsigned char uc : digest){
        ss << std::setw(2) << (int)uc;
    }

    return ss.str();
}
string FileManager::genarate_SHA256(string path){
    stringstream ss;
    ifstream ifs(path, std::ifstream::binary);
    
    char buffer[BUFFSIZE];
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256Context;
    SHA256_Init(&sha256Context);

    while (ifs.good()) {
        ifs.read(buffer, BUFFSIZE);
        SHA256_Update(&sha256Context, buffer, ifs.gcount());
    }
    ifs.close();

    int res = SHA256_Final(digest, &sha256Context);
    if (res == 0){
        return "null";
    }

    ss << std::hex << std::uppercase << std::setfill('0');
    for(unsigned char uc : digest){
        ss << std::setw(2) << (int)uc;
    }

    return ss.str();
}









//Permission related functions
void Perm::calculatePerm(int val){
	int tmp;
    chmod = val;
	if(val >= 100){
		tmp = val /100;
		val = val % 100;
		setPerm(1,tmp);
	}else{
        setPerm(1,0);
    }
    
	if(val >= 10){
		tmp = val /10;
		val = val % 10;
		setPerm(2,tmp);
	}else{
        setPerm(2,0);
    }

	if(val >= 1){
		tmp = val /1;
		val = val % 1;
		setPerm(3,tmp);
	}else{
		setPerm(3,0);
	}
}
void Perm::calculatePerm(string path){
    std::error_code ec;
    fs::perms p = fs::status(path,ec).permissions();
    chmod = 0;

	Owner.R = ((p & fs::perms::owner_read) != fs::perms::none) ? true : false;
    chmod = Owner.R ? chmod + 400 : chmod + 0;
    Owner.W = ((p & fs::perms::owner_write) != fs::perms::none) ? true : false;
    chmod = Owner.W ? chmod + 200 : chmod + 0;
    Owner.X = ((p & fs::perms::owner_exec) != fs::perms::none) ? true : false;
    chmod = Owner.X ? chmod + 100 : chmod + 0;

    Group.R = ((p & fs::perms::group_read) != fs::perms::none) ? true : false;
    chmod = Group.R ? chmod + 40 : chmod + 0;
    Group.W = ((p & fs::perms::group_write) != fs::perms::none) ? true : false;
    chmod = Group.W ? chmod + 20 : chmod + 0;
    Group.X = ((p & fs::perms::group_exec) != fs::perms::none) ? true : false;
    chmod = Group.X ? chmod + 10 : chmod + 0;

    Global.R = ((p & fs::perms::others_read) != fs::perms::none) ? true : false;
    chmod = Global.R ? chmod + 4 : chmod + 0;
    Global.W = ((p & fs::perms::others_write) != fs::perms::none) ? true : false;
    chmod = Global.W ? chmod + 2 : chmod + 0;
    Global.X = ((p & fs::perms::others_exec) != fs::perms::none) ? true : false;
    chmod = Global.X ? chmod + 1 : chmod + 0;
}


void Perm::setPerm(int mode,int set){
    bool R[8]={false,false,false,false,true,true,true,true};
    bool W[8]={false,false,true,true,false,false,true,true};
    bool X[8]={false,true,false,true,false,true,false,true};
	PermMode P;

	P.R = R[set];
	P.W = W[set];
	P.X = X[set];

	if(mode == 1){
		Owner = P;
	}else if(mode == 2){
		Group = P;
	}else{
		Global = P;
	}
}


string Perm::getPermStr(){
    string str;
	Owner.R ? str+="r" : str+="-";
	Owner.W ? str+="w" : str+="-";
	Owner.X ? str+="x" : str+="-";
	Group.R ? str+="r" : str+="-";
	Group.W ? str+="w" : str+="-";
	Group.X ? str+="x" : str+="-";
	Global.R ? str+="r" : str+="-";
	Global.W ? str+="w" : str+="-";
	Global.X ? str+="x" : str+="-";
	return str;
}

int Perm::calculatePerm(int oR, int oW, int oX, int grR, int grW, int grX, int glR, int glW, int glX){
    Owner.R = oR ? true : false;
    Owner.W = oW ? true : false;
    Owner.X = oX ? true : false;
    Group.R = grR ? true : false;
    Group.W = grW ? true : false;
    Group.X = grX ? true : false;
    Global.R = glR ? true : false;
    Global.W = glW ? true : false;
    Global.X = glX ? true : false;

    chmod = 0;
    chmod += Owner.R ? 400 : 0;
    chmod += Owner.W ? 200 : 0;
    chmod += Owner.X ? 100 : 0;
    chmod += Group.R ? 40 : 0;
    chmod += Group.W ? 20 : 0;
    chmod += Group.X ? 10 : 0;
    chmod += Global.R ? 4 : 0;
    chmod += Global.W ? 2 : 0;
    chmod += Global.X ? 1 : 0;

    return chmod;
}

inline int Perm::getPermInt(){return chmod;}




//TextFile Functions()

void TextTool::getFileData(string path, bool addLineNo){
    fstream fs;
	string tmp;
    unsigned long long int lineNo = 1;


	fs.open(path.c_str(), ios::in);

	if(fs){
        fileData.clear(); //Clearing prev data if present
		while(getline(fs, tmp)){//Gethering all lines
            fileData.push_back(addLineNo ? to_string(lineNo++) + "  " + getDispLine(tmp) : tmp);
        }
	}else{
        fileData.push_back("This file is empty");
    }
	fs.close();
}


void TextTool::replaceLine(int lineNo, string lineStr){
    if(fileData.empty()){
        return;
    }

    if(lineNo >= (int)fileData.size() || lineNo <= -1){
        return;
    }

    fileData.at(lineNo) = lineStr;
}

void TextTool::addLine(int lineNo, string lineStr){
    if(fileData.empty()){
        return;
    }
    signed int pos = 0;

    if(lineNo >= (int)fileData.size()){
        pos = 1;
    }else if(lineNo <= 0){
        pos = -1;
    }
    switch(pos){
        case -1:
            fileData.insert(fileData.begin(), lineStr);
            break;
        case 0:
            fileData.insert(fileData.begin() + lineNo, lineStr);
            break;

        case 1:
            fileData.push_back(lineStr);
            break;
    }
}

void TextTool::removeLine(int lineNo){
    if(fileData.empty()){
        return;
    }

    if(lineNo >= (int)fileData.size() || lineNo < 0){
        return;
    }
    fileData.erase(fileData.begin() + lineNo);
}

string TextTool::getDispLine(string str){
	int tmp1=str.find_first_of('\t');
	while(tmp1!=-1){
		str[tmp1]=' ';
		str.insert(tmp1,"  ");
		tmp1=str.find_first_of('\t');
	}
	return str;
}

string TextTool::getLine(int lineNo){return fileData[lineNo];}

bool TextTool::pushString(string path){
    fstream file;
	file.open(path.c_str(),ios::out);
    if(file.is_open()){
        for(auto it = fileData.begin(); it < fileData.end(); it++){
            file << *it + "\n";
        }
        file.close();
    }else{
        return false;
    }
    return true;
}

TextTool::~TextTool(){
    fileData.clear();
}