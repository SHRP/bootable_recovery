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
#include <string>
#include <iterator>
#include <vector>
#include <algorithm>
#include "gui/gui.h"
#include "gui/pages.hpp"
#include "twcommon.h"
#include "data.hpp"
using namespace std;

#include "SHRPTOOLS.hpp"
#include "SHRPGUI.hpp"

void processRefPlacement(int* var, bool isPlus, int refval){
    int tmp=*var;
    if(isPlus){
        *var = tmp + refval;
    }else{
        *var = tmp - refval;
    }
}

vector<string> fetchExtn(string str){
    vector<string> extensions;
    string tmp;
    int pos;
    if( (int)str.find_first_of(',') != -1){
        do{
            pos=str.find_first_of(',');
            extensions.push_back(str.substr(0,pos));
            str=str.substr(pos+1,str.length());
        }while( (int)str.find_first_of(',') != -1);
        pos=str.find_first_of(',');
        extensions.push_back(str.substr(0,pos));
    }else{
        extensions.push_back(str);
    }
    return extensions;
}


bool isExtnMatched(vector<string> extn,string fileName){
    vector<string>::iterator ptr;
    string fileExtn=minUtils::getExtension(fileName);
    //LOGINFO("File Extension : %s\n",fileExtn.c_str());
    for(ptr=extn.begin();ptr<extn.end();ptr++){
        //LOGINFO("Comparing %s with %c\n",fileExtn.c_str(),*ptr->c_str());
        if(minUtils::compare(fileExtn,*ptr)){
            return true;
        }
    }
    return false;
}