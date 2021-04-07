/*
	Copyright 2013 bigbiff/Dees_Troy TeamWin
	This file is part of TWRP/TeamWin Recovery Project.

	TWRP is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	TWRP is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with TWRP.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <string.h>

extern "C" {
#include "../twcommon.h"
}
#include "../minuitwrp/minui.h"

#include "rapidxml.hpp"
#include "objects.hpp"
#include "../data.hpp"
#include "../partitions.hpp"
#include "../twrp-functions.hpp"
#include "pages.hpp"

//<SHRP>
#include <dirent.h>
//</SHRP>
extern std::vector<language_struct> Language_List;

GUIListBox::GUIListBox(xml_node<>* node) : GUIScrollList(node)
{
	xml_attribute<>* attr;
	xml_node<>* child;
	mIconSelected = mIconUnselected = NULL;
	mUpdate = 0;
	isCheckList = isTextParsed = false;

	// Get the icons, if any
	child = FindNode(node, "icon");
	if (child) {
		mIconSelected = LoadAttrImage(child, "selected");
		mIconUnselected = LoadAttrImage(child, "unselected");
	}
	int iconWidth = 0, iconHeight = 0;
	if (mIconSelected && mIconSelected->GetResource() && mIconUnselected && mIconUnselected->GetResource()) {
		iconWidth = std::max(mIconSelected->GetWidth(), mIconUnselected->GetWidth());
		iconHeight = std::max(mIconSelected->GetHeight(), mIconUnselected->GetHeight());
	} else if (mIconSelected && mIconSelected->GetResource()) {
		iconWidth = mIconSelected->GetWidth();
		iconHeight = mIconSelected->GetHeight();
	} else if (mIconUnselected && mIconUnselected->GetResource()) {
		iconWidth = mIconUnselected->GetWidth();
		iconHeight = mIconUnselected->GetHeight();
	}
	SetMaxIconSize(iconWidth, iconHeight);

	// Handle the result variable
	child = FindNode(node, "data");
	if (child) {
		attr = child->first_attribute("name");
		if (attr)
			mVariable = attr->value();
		attr = child->first_attribute("default");
		if (attr)
			DataManager::SetValue(mVariable, attr->value());
		// Get the currently selected value for the list
		DataManager::GetValue(mVariable, currentValue);
		if (mVariable == "tw_language") {
			std::vector<language_struct>::iterator iter;
			for (iter = Language_List.begin(); iter != Language_List.end(); iter++) {
				ListItem data;
				data.displayName = (*iter).displayvalue;
				data.variableValue = (*iter).filename;
				data.action = NULL;
				data.icon = NULL; //SHRP
				if (currentValue == (*iter).filename) {
					data.selected = 1;
					DataManager::SetValue("tw_language_display", (*iter).displayvalue);
				} else
					data.selected = 0;
				mListItems.push_back(data);
			}
		} else if (mVariable == "tw_crypto_user_id") {
			std::vector<users_struct>::iterator iter;
			std::vector<users_struct>* Users_List = PartitionManager.Get_Users_List();
			for (iter = Users_List->begin(); iter != Users_List->end(); iter++) {
				if (!(*iter).isDecrypted) {
					ListItem data;
					data.displayName = (*iter).userName;
					data.variableValue = (*iter).userId;
					data.action = NULL;
					data.icon = NULL; //SHRP
					DataManager::GetValue("tw_crypto_user_id", currentValue);
					if (currentValue == (*iter).userId || currentValue == "") {
						data.selected = 1;
						DataManager::SetValue("tw_crypto_user_id", (*iter).userId);
						DataManager::SetValue("tw_crypto_pwtype", (*iter).type);
					} else
						data.selected = 0;
					mListItems.push_back(data);
				}
			}
		}
				//<SHRP>
		else if(mVariable == "shrpTweaks"){
#ifndef SHRP_EXCLUDE_DEFAULT_ADDONS
#ifndef SHRP_SKIP_DEFAULT_ADDON_1
			{
				addonInfo aItem;
				aItem.name="Substratum Overlay Disabler [Normal]";
				aItem.info="This will disable substratum overlays.";
				aItem.confirmBtnText="Disable";
				aItem.successfulText="Disabled";
				aItem.fileName="s_oms.zip";
				addons.push_back(aItem);
			}
#endif
#ifndef SHRP_SKIP_DEFAULT_ADDON_2
			{
				addonInfo aItem;
				aItem.name="Substratum Overlay Disabler [Legacy]";
				aItem.info="This will disable substratum overlays. Note - Legacy Only.";
				aItem.confirmBtnText="Disable";
				aItem.successfulText="Disabled";
				aItem.fileName="s_non_oms.zip";
				addons.push_back(aItem);
			}
#endif
#ifndef SHRP_SKIP_DEFAULT_ADDON_3
			{
				addonInfo aItem;
				aItem.name="Clear system lock";
				aItem.info="This addon will remove your android system lock.";
				aItem.confirmBtnText="Clear";
				aItem.successfulText="Removed";
				aItem.fileName="rfp.zip";
				addons.push_back(aItem);
			}
#endif
#ifndef SHRP_SKIP_DEFAULT_ADDON_4
			{
				addonInfo aItem;
				aItem.name="Disable Force Encryption";
				aItem.info="This will disable force encryption of your device.";
				aItem.confirmBtnText="Disable";
				aItem.successfulText="Disabled";
				aItem.fileName="Disable_Dm-Verity_ForceEncrypt.zip";
				addons.push_back(aItem);
			}
#endif
#endif
#ifndef SHRP_EXCLUDE_MAGISK_FLASH
			{
				addonInfo aItem;
				aItem.name = gui_parse_text("{@installMagisk=Install Magisk}");
				aItem.info = gui_parse_text("{@confirm_action=Confirm Action}");
				aItem.confirmBtnText = gui_parse_text("{@install_btn=Install}");
				aItem.successfulText = gui_parse_text("{@successful=Successful}");
				aItem.fileName="c_magisk.zip";
				addons.push_back(aItem);
			}
			{
				addonInfo aItem;
				aItem.name = gui_parse_text("{@c_unroot=Unroot} {@c_magisk=Magisk}");
				aItem.info = gui_parse_text("{@confirm_action=Confirm Action}");
				aItem.confirmBtnText = gui_parse_text("{@c_unroot=Unroot}");
				aItem.successfulText = gui_parse_text("{@successful=Successful}");
				aItem.fileName="unmagisk.zip";
				addons.push_back(aItem);
			}
#endif
			{
#ifdef SHRP_EXTERNAL_ADDON_1_NAME
				addonInfo aItem;
				aItem.name=EXPAND(SHRP_EXTERNAL_ADDON_1_NAME);
#endif
#ifdef SHRP_EXTERNAL_ADDON_1_INFO
				aItem.info=EXPAND(SHRP_EXTERNAL_ADDON_1_INFO);
#endif
#ifdef SHRP_EXTERNAL_ADDON_1_BTN_TEXT
				aItem.confirmBtnText=EXPAND(SHRP_EXTERNAL_ADDON_1_BTN_TEXT);
#endif
#ifdef SHRP_EXTERNAL_ADDON_1_SUCCESSFUL_TEXT
				aItem.successfulText=EXPAND(SHRP_EXTERNAL_ADDON_1_SUCCESSFUL_TEXT);
#endif
#ifdef SHRP_EXTERNAL_ADDON_1_FILENAME
				aItem.fileName=EXPAND(SHRP_EXTERNAL_ADDON_1_FILENAME);
				addons.push_back(aItem);
#endif
			}
			{
#ifdef SHRP_EXTERNAL_ADDON_2_NAME
				addonInfo aItem;
				aItem.name=EXPAND(SHRP_EXTERNAL_ADDON_2_NAME);
#endif
#ifdef SHRP_EXTERNAL_ADDON_2_INFO
				aItem.info=EXPAND(SHRP_EXTERNAL_ADDON_2_INFO);
#endif
#ifdef SHRP_EXTERNAL_ADDON_2_BTN_TEXT
				aItem.confirmBtnText=EXPAND(SHRP_EXTERNAL_ADDON_2_BTN_TEXT);
#endif
#ifdef SHRP_EXTERNAL_ADDON_2_SUCCESSFUL_TEXT
				aItem.successfulText=EXPAND(SHRP_EXTERNAL_ADDON_2_SUCCESSFUL_TEXT);
#endif
#ifdef SHRP_EXTERNAL_ADDON_2_FILENAME
				aItem.fileName=EXPAND(SHRP_EXTERNAL_ADDON_2_FILENAME);
				addons.push_back(aItem);
#endif
			}
			{
#ifdef SHRP_EXTERNAL_ADDON_3_NAME
				addonInfo aItem;
				aItem.name=EXPAND(SHRP_EXTERNAL_ADDON_3_NAME);
#endif
#ifdef SHRP_EXTERNAL_ADDON_3_INFO
				aItem.info=EXPAND(SHRP_EXTERNAL_ADDON_3_INFO);
#endif
#ifdef SHRP_EXTERNAL_ADDON_3_BTN_TEXT
				aItem.confirmBtnText=EXPAND(SHRP_EXTERNAL_ADDON_3_BTN_TEXT);
#endif
#ifdef SHRP_EXTERNAL_ADDON_3_SUCCESSFUL_TEXT
				aItem.successfulText=EXPAND(SHRP_EXTERNAL_ADDON_3_SUCCESSFUL_TEXT);
#endif
#ifdef SHRP_EXTERNAL_ADDON_3_FILENAME
				aItem.fileName=EXPAND(SHRP_EXTERNAL_ADDON_3_FILENAME);
				addons.push_back(aItem);
#endif
			}
			{
#ifdef SHRP_EXTERNAL_ADDON_4_NAME
				addonInfo aItem;
				aItem.name=EXPAND(SHRP_EXTERNAL_ADDON_4_NAME);
#endif
#ifdef SHRP_EXTERNAL_ADDON_4_INFO
				aItem.info=EXPAND(SHRP_EXTERNAL_ADDON_4_INFO);
#endif
#ifdef SHRP_EXTERNAL_ADDON_4_BTN_TEXT
				aItem.confirmBtnText=EXPAND(SHRP_EXTERNAL_ADDON_4_BTN_TEXT);
#endif
#ifdef SHRP_EXTERNAL_ADDON_4_SUCCESSFUL_TEXT
				aItem.successfulText=EXPAND(SHRP_EXTERNAL_ADDON_4_SUCCESSFUL_TEXT);
#endif
#ifdef SHRP_EXTERNAL_ADDON_4_FILENAME
				aItem.fileName=EXPAND(SHRP_EXTERNAL_ADDON_4_FILENAME);
				addons.push_back(aItem);
#endif
			}
			{
#ifdef SHRP_EXTERNAL_ADDON_5_NAME
				addonInfo aItem;
				aItem.name=EXPAND(SHRP_EXTERNAL_ADDON_5_NAME);
#endif
#ifdef SHRP_EXTERNAL_ADDON_5_INFO
				aItem.info=EXPAND(SHRP_EXTERNAL_ADDON_5_INFO);
#endif
#ifdef SHRP_EXTERNAL_ADDON_5_BTN_TEXT
				aItem.confirmBtnText=EXPAND(SHRP_EXTERNAL_ADDON_5_BTN_TEXT);
#endif
#ifdef SHRP_EXTERNAL_ADDON_5_SUCCESSFUL_TEXT
				aItem.successfulText=EXPAND(SHRP_EXTERNAL_ADDON_5_SUCCESSFUL_TEXT);
#endif
#ifdef SHRP_EXTERNAL_ADDON_5_FILENAME
				aItem.fileName=EXPAND(SHRP_EXTERNAL_ADDON_5_FILENAME);
				addons.push_back(aItem);
#endif
			}
			{
#ifdef SHRP_EXTERNAL_ADDON_6_NAME
				addonInfo aItem;
				aItem.name=EXPAND(SHRP_EXTERNAL_ADDON_6_NAME);
#endif
#ifdef SHRP_EXTERNAL_ADDON_6_INFO
				aItem.info=EXPAND(SHRP_EXTERNAL_ADDON_6_INFO);
#endif
#ifdef SHRP_EXTERNAL_ADDON_6_BTN_TEXT
				aItem.confirmBtnText=EXPAND(SHRP_EXTERNAL_ADDON_6_BTN_TEXT);
#endif
#ifdef SHRP_EXTERNAL_ADDON_6_SUCCESSFUL_TEXT
				aItem.successfulText=EXPAND(SHRP_EXTERNAL_ADDON_6_SUCCESSFUL_TEXT);
#endif
#ifdef SHRP_EXTERNAL_ADDON_6_FILENAME
				aItem.fileName=EXPAND(SHRP_EXTERNAL_ADDON_6_FILENAME);
				addons.push_back(aItem);
#endif
			}
			mListItems.clear();
			for(auto ptr=addons.begin(); ptr<addons.end(); ptr++){
				ListItem data;
				data.displayName=ptr->name;
				data.variableName="dummy";
				data.variableValue = "";
				data.action = NULL;
				data.icon = NULL;
				data.selected = 0;
				mListItems.push_back(data);
			}
		}else if(mVariable == "magiskModules"){

			child = FindNode(node,"update");
			if (child){
				attr = child->first_attribute("name");
			}
			if (attr){
				updateVar = attr->value();
				DataManager::GetValue(updateVar,updateVal);
			}

			DIR* d;
			struct dirent* de;
			string fileName,moduleProp;
			d = opendir("/_magisk/");

			if(d!=NULL){
				while((de = readdir(d)) != NULL){
					if(de->d_type == DT_DIR){
						fileName=de->d_name;
						moduleProp="/_magisk/"+fileName+"/module.prop";
						if(TWFunc::Path_Exists(moduleProp)){//If true that means its a module
							int i=0;
							char chr[512];
							string tmp,name,ver,author,desc;
							moduleInfo M;

							M.modulePath="/_magisk/"+fileName;
							FILE *f=std::fopen(moduleProp.c_str(),"r");

							while(i<7){
								fgets(chr,512,f);
								tmp=chr;
								tmp=tmp.substr(0,tmp.find_first_of('='));
								if(tmp == "name"){
									name=chr;
									M.name=name.substr(name.find_first_of('=')+1,name.length());
									M.name[M.name.length()-1]=' ';
								}else if(tmp == "version"){
									ver=chr;
									M.version=ver.substr(ver.find_first_of('=')+1,ver.length());
									M.version[M.version.length()-1]=' ';
								}else if(tmp == "author"){
									author=chr;
									M.author=author.substr(author.find_first_of('=')+1,author.length());
									M.author[M.author.length()-1]=' ';
								}else if(tmp == "description"){
									desc=chr;
									M.description=desc.substr(desc.find_first_of('=')+1,desc.length());
									M.description[M.description.length()-1]=' ';
								}
								i++;
							}
							fclose(f);

							modules.push_back(M);

							ListItem data;
							data.displayName=M.name;
							data.variableName="dummy";
							data.variableValue = "";
							data.action = NULL;
							data.icon = NULL;
							data.selected = 0;
							mListItems.push_back(data);
						}
					}
				}
			}
			DataManager::SetValue("magiskModule", mListItems.empty() ? "0" : "1");
		}else if("colorList"){
			child = FindNode(node, "color");
			while (child) {
				xml_attribute<>* tmp;
				colorInfo C;
				tmp=child->first_attribute("name");
				C.colorName=tmp->value();
				C.mColorIcon=LoadAttrImage(child, "resource");
				colors.push_back(C);
				child = child->next_sibling("color");
			}
			child = FindNode(node, "colorType");
			while (child) {
				ListItem data;
				string colorVar;
				data.displayName=child->first_attribute("name")->value();
				data.info=child->first_attribute("colorVar")->value();
				data.variableName = "";
				data.variableValue = "";
				data.action = NULL;
				data.icon = NULL;
				for(auto it=colors.begin();it<colors.end();it++){
					if(it->colorName == "customColor"){
						data.icon = it->mColorIcon;
					}
					if(DataManager::GetStrValue(data.info) == DataManager::GetStrValue(it->colorName)){
						data.icon = it->mColorIcon;
						break;
					}
				}
				
				data.selected = 0;
				mListItems.push_back(data);
				child = child->next_sibling("colorType");
			}
		}
		//</SHRP>
	} else
		allowSelection = false;  // allows using listbox as a read-only list or menu

	// Get the data for the list
	child = FindNode(node, "listitem");
	if (!child) return;
	while (child) {
		ListItem item;

		attr = child->first_attribute("name");
		if (!attr) continue;
		// We will parse display names when we get page focus to ensure that translating takes place
		item.displayName = attr->value();
		item.variableValue = gui_parse_text(child->value());
		item.selected = (child->value() == currentValue);
		item.action = NULL;
		xml_node<>* action = child->first_node("action");
		if (!action) action = child->first_node("actions");
		if (action) {
			item.action = new GUIAction(child);
			allowSelection = true;
		}
		xml_node<>* variable_name = child->first_node("data");
		if (variable_name) {
			attr = variable_name->first_attribute("variable");
			if (attr) {
				item.variableName = attr->value();
				item.selected = (DataManager::GetIntValue(item.variableName) != 0);
				allowSelection = true;
				isCheckList = true;
			}
		}

		//Fetching icon from xml <SHRP>
		xml_node<>* tmp;
		tmp = FindNode(child, "icon");
		if (tmp){
			item.icon = LoadAttrImage(tmp, "resource");
		}else{
			item.icon = NULL;
		}
		//<SHRP>

		LoadConditions(child, item.mConditions);

		mListItems.push_back(item);
		mVisibleItems.push_back(mListItems.size() - 1);

		child = child->next_sibling("listitem");
	}
}

GUIListBox::~GUIListBox()
{
}

int GUIListBox::Update(void)
{
	if (!isConditionTrue())
		return 0;

	if (mVariable == "tw_crypto_user_id") {
		mListItems.clear();
		std::vector<users_struct>::iterator iter;
		std::vector<users_struct>* Users_List = PartitionManager.Get_Users_List();
		for (iter = Users_List->begin(); iter != Users_List->end(); iter++) {
			if (!(*iter).isDecrypted) {
				ListItem data;
				data.displayName = (*iter).userName;
				data.variableValue = (*iter).userId;
				data.action = NULL;
				DataManager::GetValue("tw_crypto_user_id", currentValue);
				if (currentValue == (*iter).userId || currentValue == "") {
					data.selected = 1;
					DataManager::SetValue("tw_crypto_user_id", (*iter).userId);
					DataManager::SetValue("tw_crypto_pwtype", (*iter).type);
				} else
				data.selected = 0;
				mListItems.push_back(data);
			}
		}
		mVisibleItems.clear();
		for (size_t i = 0; i < mListItems.size(); i++) {
			mVisibleItems.push_back(i);
		}
		mUpdate = 1;
	}
	//<SHRP>
	else if(mVariable == "magiskModules"){
		DIR* d;
		struct dirent* de;
		string fileName,moduleProp;
		d = opendir("/_magisk/");
		mListItems.clear();
		modules.clear();
		if(d!=NULL){
			while((de = readdir(d)) != NULL){
				if(de->d_type == DT_DIR){
					fileName=de->d_name;
					moduleProp="/_magisk/"+fileName+"/module.prop";
					if(TWFunc::Path_Exists(moduleProp)){//If true that means its a module
						int i=0;
						char chr[512];
						string tmp,name,ver,author,desc;
						moduleInfo M;

						M.modulePath="/_magisk/"+fileName;
						FILE *f=std::fopen(moduleProp.c_str(),"r");

						while(i<7){
							fgets(chr,512,f);
							tmp=chr;
							tmp=tmp.substr(0,tmp.find_first_of('='));
							if(tmp == "name"){
								name=chr;
								M.name=name.substr(name.find_first_of('=')+1,name.length());
								M.name[M.name.length()-1]=' ';
							}else if(tmp == "version"){
								ver=chr;
								M.version=ver.substr(ver.find_first_of('=')+1,ver.length());
								M.version[M.version.length()-1]=' ';
							}else if(tmp == "author"){
								author=chr;
								M.author=author.substr(author.find_first_of('=')+1,author.length());
								M.author[M.author.length()-1]=' ';
							}else if(tmp == "description"){
								desc=chr;
								M.description=desc.substr(desc.find_first_of('=')+1,desc.length());
								M.description[M.description.length()-1]=' ';
							}
							i++;
						}
						fclose(f);

						modules.push_back(M);

						ListItem data;
						data.displayName=M.name;
						data.variableName="dummy";
						data.variableValue = "";
						data.action = NULL;
						data.icon = NULL;
						data.selected = 0;
						mListItems.push_back(data);
					}
				}
			}
		}
		DataManager::SetValue("magiskModule", mListItems.empty() ? "0" : "1");
	}else if(mVariable == "colorList"){
		string tmp;
		for(auto it=mListItems.begin(); it<mListItems.end(); it++){
			for(auto ptr=colors.begin(); ptr<colors.end(); ptr++){
				if(DataManager::GetStrValue(it->info.c_str()) == DataManager::GetStrValue(ptr->colorName.c_str())){
					it->icon = ptr->mColorIcon;
				}
			}
		}
	}
	//</SHRP>

	GUIScrollList::Update();

	if (mUpdate) {
		mUpdate = 0;
		if (Render() == 0)
			return 2;
	}
	return 0;
}

int GUIListBox::NotifyVarChange(const std::string& varName, const std::string& value)
{
	GUIScrollList::NotifyVarChange(varName, value);

	if (!isConditionTrue())
		return 0;

	// Check to see if the variable that we are using to store the list selected value has been updated
	if (varName == mVariable) {
		currentValue = value;
		mUpdate = 1;
	}
	//<SHRP>
	else if(varName == updateVar){
		mUpdate = 1;
	}
	//</SHRP>

	std::vector<size_t> mVisibleItemsOld;
	std::swap(mVisibleItemsOld, mVisibleItems);
	for (size_t i = 0; i < mListItems.size(); i++) {
		ListItem& item = mListItems[i];
		// update per-item visibility condition
		bool itemVisible = UpdateConditions(item.mConditions, varName);
		if (itemVisible)
			mVisibleItems.push_back(i);

		if (isCheckList)
		{
			if (item.variableName == varName || varName.empty()) {
				std::string val;
				DataManager::GetValue(item.variableName, val);
				item.selected = (val != "0");
				mUpdate = 1;
			}
		}
		else if (varName == mVariable) {
			if (item.variableValue == currentValue) {
				item.selected = 1;
				SetVisibleListLocation(mVisibleItems.empty() ? 0 : mVisibleItems.size()-1);
			} else {
				item.selected = 0;
			}
		}
	}

	if (mVisibleItemsOld != mVisibleItems) {
		mUpdate = 1; // some item's visibility has changed
		if (firstDisplayedItem >= (int)mVisibleItems.size()) {
			// all items in the view area were removed - make last item visible
			SetVisibleListLocation(mVisibleItems.empty() ? 0 : mVisibleItems.size()-1);
		}
	}

	return 0;
}

void GUIListBox::SetPageFocus(int inFocus)
{
	GUIScrollList::SetPageFocus(inFocus);
	if (inFocus) {
		if (!isTextParsed) {
			isTextParsed = true;
			for (size_t i = 0; i < mListItems.size(); i++) {
				ListItem& item = mListItems[i];
				item.displayName = gui_parse_text(item.displayName);
			}
		}
		DataManager::GetValue(mVariable, currentValue);
		NotifyVarChange(mVariable, currentValue);
	}
}

size_t GUIListBox::GetItemCount()
{
	return mVisibleItems.size();
}

void GUIListBox::RenderItem(size_t itemindex, int yPos, bool selected)
{
	// note: the "selected" parameter above is for the currently touched item
	// don't confuse it with the more persistent "selected" flag per list item used below
	ListItem& item = mListItems[mVisibleItems[itemindex]];
	//ImageResource* icon = item.selected ? mIconSelected : mIconUnselected;
	//Get Icon Resource if available <SHRP>
	ImageResource* icon;
	if(item.icon != NULL){
		icon = item.icon;
	}else{
		icon = item.selected ? mIconSelected : mIconUnselected;
	}
	//<SHRP>
	const std::string& text = item.displayName;

	RenderStdItem(yPos, selected, icon, text.c_str());
}

void GUIListBox::NotifySelect(size_t item_selected)
{
	if (!isCheckList) {
		// deselect all items, even invisible ones
		for (size_t i = 0; i < mListItems.size(); i++) {
			mListItems[i].selected = 0;
		}
	}

	ListItem& item = mListItems[mVisibleItems[item_selected]];

	//<SHRP>
		if(mVariable == "shrpTweaks"){
			for(auto ptr=addons.begin(); ptr<addons.end(); ptr++){
				if(item.displayName == ptr->name){

					DataManager::SetValue("shrpTweakName",ptr->name);
					DataManager::SetValue("shrpTweakInfo",ptr->info == "" ? "Confirm Flash" : ptr->info);
					DataManager::SetValue("shrpTweakConfirm",ptr->confirmBtnText == "" ? "Flash" : ptr->confirmBtnText);
					DataManager::SetValue("shrpTweakFinish",ptr->successfulText == "" ? "Successful" : ptr->successfulText);

					if(TWFunc::Path_Exists("/sdcard/SHRP/addons/"+ptr->fileName)){
						DataManager::SetValue("shrpTweakFileName","/sdcard/SHRP/addons/"+ptr->fileName);
					}else{
						DataManager::SetValue("shrpTweakFileName","/twres/addons/"+ptr->fileName);
					}
					break;
				}
			}
		}else if(mVariable == "magiskModules"){
			for(auto ptr=modules.begin(); ptr<modules.end(); ptr++){
				if(ptr->name == item.displayName){

					DataManager::SetValue("modulePath",ptr->modulePath);
					DataManager::SetValue("moduleName",ptr->name);
					DataManager::SetValue("moduleVersion",ptr->version);
					DataManager::SetValue("moduleAuthor",ptr->author);
					DataManager::SetValue("moduleDesc",ptr->description);
					
					TWFunc::Path_Exists(DataManager::GetStrValue("modulePath")+"/disable") ? DataManager::SetValue("moduleEnabled",0) : DataManager::SetValue("moduleEnabled",1);

					//PageManager::ChangeOverlay("");
					//PageManager::ChangePage("c_magisk_option");

					gui_changeOverlay("");
					gui_changePage("c_magisk_option");
					break;
				}
			}
		}else if(mVariable == "colorList"){
			DataManager::SetValue("assignVar",item.info);
			DataManager::SetValue("colorHolder",DataManager::GetStrValue(DataManager::GetStrValue("assignVar")));
			if(mVisibleItems[item_selected] == 0){
				//PageManager::ChangeOverlay("bgColorPanel");
				gui_changeOverlay("bgColorPanel");
			}else{
				//PageManager::ChangeOverlay("colorPanel");
				gui_changeOverlay("colorPanel");
			}

		}
	//</SHRP>

	if (isCheckList) {
		int selected = 1 - item.selected;
		item.selected = selected;
		DataManager::SetValue(item.variableName, selected ? "1" : "0");
	} else {
		item.selected = 1;
		string str = item.variableValue;	// [check] should this set currentValue instead?
		DataManager::SetValue(mVariable, str);
	}
	if (item.action)
		item.action->doActions();
	mUpdate = 1;
}
