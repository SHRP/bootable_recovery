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
using namespace std;
class FileManager{
    public:
        //Helper functions
        static bool isFile(string str);
        static bool isValidOp(string str);
        static string getFileName(string str);
        static string getFolderName(string str);
        static string getPrevFolderPath(string str);
        static bool isDirEmpty(string str);
        //Main Operations
        static string getSizeStr(string str);
        static bool copy(string from, string to, bool overwrite);
        static bool copy(string from, string to, int multiple, bool overwrite);
        static bool move(string from, string to, bool overwrite);
        static bool move(string from, string to, int multiple, bool overwrite);
        static bool rename(string from, string objName, bool overwrite);
        static bool removeFile(string path);
        static int removeFolder(string path);
        static bool remove(string path);
        static bool remove(string path, int multiple);
        static bool createFolder(string path);

        static string setPermission(string path, string chmod);
        static string setPermission(string path, int ownerR, int ownerW, int ownerX, int groupR, int groupW, int groupX, int globalR, int globalW, int globalX);
        static void UpdateGuiPerms(string path);
        static string getStrPermission(string path);

        //Utility Functions
        static string generate_Hash(string path, string hashAlgo);//hashAlgo can be md5, sha1, sha256
        static string genarate_SHA1(string path);
        static string genarate_SHA256(string path);

        static bool compress(string path, string fileName, string compressionLvl = "0");
        static bool compressEx(string path, string fileName, string to, string compressionLvl = "0");
        static bool extract(string filePath);
        static bool extract(string filePath, string to);
};

struct PermMode{
	bool R,W,X;
};

class Perm{
	public:
        int chmod;
        PermMode Owner, Group, Global;

		void calculatePerm(int val);
        void calculatePerm(string path);
        int calculatePerm(int oR, int oW, int oX, int grR, int grW, int grX, int glR, int glW, int glX);
		void setPerm(int mode,int set);
        string getPermStr();
        int getPermInt();
};


class TextTool{
    public:
        vector<string>fileData;

        void getFileData(string path, bool addLineNo = false);
        void replaceLine(int lineNo, string lineStr);
        void addLine(int lineNo, string lineStr);
        void removeLine(int lineNo);
        string getLine(int lineNo);
        bool pushString(string path);
        string getDispLine(string str);
        ~TextTool();
};


