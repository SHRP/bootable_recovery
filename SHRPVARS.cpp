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
#include <sstream>
#include "infomanager.hpp"
#include "data.hpp"
#include "twcommon.h"
#include "variables.h"
#include "SHRPINIT.hpp"


void initSHRPVars(InfoManager* mConst, InfoManager* mData, InfoManager* mPersist){
    //SHRP constant variables
	mConst->SetValue("shrp_ver", "3.1");
	mConst->SetValue("buildNo", EXPAND(SHRP_BUILD_DATE));
	mConst->SetValue("shrp_ver_status", "Stable");
	mConst->SetValue("c_magisk_path", "/_magisk/");
	mConst->SetValue("c_magisk_stock_var", "22.0");

	//SHRP SemiConstant variables
	mData->SetValue("shrpBasePath", "/system");
	mData->SetValue("c_magisk_status", "0");
	mData->SetValue("c_shrp_resource_status", "0");
	mData->SetValue("c_magisk_ver", "0");
	mData->SetValue("c_lock_screen_date", "0");
	mData->SetValue("c_mm_name", "0");
	mData->SetValue("c_mm_ver", "0");
	mData->SetValue("c_mm_author", "0");
	mData->SetValue("c_s_p", "0");
	mData->SetValue("c_s_status", "Not Available");
	mData->SetValue("c_i_p", "0");
	mData->SetValue("c_i_status", "Not Available");
	mData->SetValue("c_e_p", "0");
	mData->SetValue("c_e_status", "Not Available");
	mData->SetValue("c_o_p", "0");
	mData->SetValue("c_o_status", "Not Available");
	mData->SetValue("c_r_p", "0");
	mData->SetValue("c_r_status", "Not Available");
	mData->SetValue("c_magisk_module_name", "0");
	mData->SetValue("c_magisk_name", "0");
	mData->SetValue("core", "0");
	mData->SetValue("c_m_exist", "0");
	mData->SetValue("c_module_disable", "0");
	mData->SetValue("c_target_destination", "main2");
	mData->SetValue("c_queue_enabled", "0");
	mData->SetValue("c_magisk_update", "0");
	mData->SetValue("tw_ls_time", "0");
	mData->SetValue("shrp_zipName", "");
	mData->SetValue("shrp_zipFolderName", "");
	mData->SetValue("shrpUnzipFolder", "");
	mData->SetValue("canBeUnzip", "0");
	mData->SetValue("isThemeFile", "0");
	mData->SetValue("chFolder", "0");
	//SHRP_PERSISTED_VARS
	mPersist->SetValue("c_temp2", "pepe");
	mPersist->SetValue("c_dateview", "0");
	mPersist->SetValue("c_devMode", "0");
	mPersist->SetValue("c_shrpUpdate", "0");
	mPersist->SetValue(REBOOTOTA_DISABLED, "1");
	mPersist->SetValue(INSTALLMAGISK_OTA, "0");
	//SHRP_LOCK_VARS
	mData->SetValue("lock_enabled", "0");
	mData->SetValue("patt_lock_enabled", "0");
	mData->SetValue("main_pass", "1");
	mData->SetValue("c_new", "1");
	mData->SetValue("c_new_pattern", "1");
    mData->SetValue(TW_MKINJECT_AFTER_ZIP, "0");

	//SHRP_BOARDCONFIGS_Variables (type - const)
#ifdef SHRP_MAINTAINER
	mConst->SetValue("device_maintainer_name", EXPAND(SHRP_MAINTAINER));
#else
	mConst->SetValue("device_maintainer_name", "N/A");
#endif

#ifdef SHRP_DEVICE_CODE
	mConst->SetValue("device_code_name", EXPAND(SHRP_DEVICE_CODE));
#else
	mConst->SetValue("device_code_name", "N/A");
#endif

#ifdef SHRP_INTERNAL
	mConst->SetValue("internal_storage_location", EXPAND(SHRP_INTERNAL));
#else
	mConst->SetValue("internal_storage_location", "/sdcard");
#endif

#ifdef SHRP_EXTERNAL
	mConst->SetValue("external_storage_location", EXPAND(SHRP_EXTERNAL));
#else
	mConst->SetValue("external_storage_location", "/");
#endif

#ifdef SHRP_OTG
	mConst->SetValue("usb_otg_location", EXPAND(SHRP_OTG));
#else
	mConst->SetValue("usb_otg_location", "/");
#endif

#ifdef SHRP_EDL_MODE
	mConst->SetValue("has_edl_mode", EXPAND(SHRP_EDL_MODE));
#else
	mConst->SetValue("has_edl_mode", "0");
#endif

#ifdef SHRP_FLASH
	mConst->SetValue("has_flashlight", EXPAND(SHRP_FLASH));
	mData->SetValue("c_flashlight_status", "0");
#else
	mConst->SetValue("has_flashlight", "0");
	mData->SetValue("c_flashlight_status", "0");
#endif

#ifdef SHRP_FONP_1
	mConst->SetValue("c_flashlight_path_1", EXPAND(SHRP_FONP_1));
#endif

#ifdef SHRP_FONP_2
	mConst->SetValue("c_flashlight_path_2", EXPAND(SHRP_FONP_2));
#endif

#ifdef SHRP_FONP_3
	mConst->SetValue("c_flashlight_path_3", EXPAND(SHRP_FONP_3));
#endif

#ifdef SHRP_FLASH_MAX_BRIGHTNESS
	mConst->SetValue("c_flashlight_max_brightness", EXPAND(SHRP_FLASH_MAX_BRIGHTNESS));
#endif

#ifdef SHRP_REC
	mConst->SetValue("shrp_rec", EXPAND(SHRP_REC));
#endif

#ifdef SHRP_REC_TYPE
	mConst->SetValue("shrp_rec_type", EXPAND(SHRP_REC_TYPE));
#else
	mConst->SetValue("shrp_rec_type", "N/A");
#endif

#ifdef SHRP_DEVICE_TYPE
	mConst->SetValue("shrp_device_type", EXPAND(SHRP_DEVICE_TYPE));
#else
	mConst->SetValue("shrp_device_type", "N/A");
#endif

#ifdef SHRP_AB
	mConst->SetValue("shrp_ab", "1");
#else
	mConst->SetValue("shrp_ab", "0");
#endif

#ifdef SHRP_STATUSBAR_LEFT_PADDING
	int val=0;
	stringstream guun(EXPAND(SHRP_STATUSBAR_LEFT_PADDING));
	guun>>val;
	val += 20;
	mConst->SetValue("c_time_x", val);
#else
	mConst->SetValue("c_time_x", 20);
#endif

#ifdef SHRP_STATUSBAR_RIGHT_PADDING
	int valL=0;
	stringstream bappa(EXPAND(SHRP_STATUSBAR_RIGHT_PADDING));
	bappa>>valL;
	mConst->SetValue("c_batteryText1", 913-valL);
	mConst->SetValue("c_batteryText2", 928-valL);
	mConst->SetValue("c_batteryText3", 950-valL);
	mConst->SetValue("c_batteryIco", 1020-valL);
#else
	mConst->SetValue("c_batteryText1", 913);
	mConst->SetValue("c_batteryText2", 928);
	mConst->SetValue("c_batteryText3", 950);
	mConst->SetValue("c_batteryIco", 1020);
#endif

#ifdef SHRP_EXPRESS
	mConst->SetValue("av_express", 1);
#else
	mConst->SetValue("av_express", 0);
#endif

#ifdef SHRP_LITE
    mConst->SetValue("shrp_lite", 1);
#else
    mConst->SetValue("shrp_lite", 0);
#endif

#ifdef MAGISK_IN_REC
    mConst->SetValue("magisk_in_rec", 1);
#else
    mConst->SetValue("magisk_in_rec", 0);
#endif

#ifndef SHRP_EXCLUDE_MAGISK_FLASH
	mConst->SetValue("exclude_magisk_flash", "1");
#endif

#ifndef SHRP_NOTCH
    //Positions variables
    mConst->SetValue("rPos0", "0");
    // mConst->SetValue("rPos1", "15");
    mConst->SetValue("rPos2", "30");
    mConst->SetValue("rPos3", "45");
    mConst->SetValue("rPos4", "60");
    mConst->SetValue("rPos5", "75");
    // mConst->SetValue("rPos6", "90");
    // mConst->SetValue("rPos7", "105");
    mConst->SetValue("rPos8", "120");
    mConst->SetValue("rPos9", "135");
    mConst->SetValue("rPos10", "150");
    // mConst->SetValue("rPos11", "165");
    mConst->SetValue("rPos12", "180");
    // mConst->SetValue("rPos13", "195");
    mConst->SetValue("rPos14", "210");
    mConst->SetValue("rPos15", "225");
    // mConst->SetValue("rPos16", "240");
    mConst->SetValue("rPos17", "255");
    mConst->SetValue("rPos18", "270");
    mConst->SetValue("rPos19", "285");
    mConst->SetValue("rPos20", "300");
    mConst->SetValue("rPos21", "315");
    mConst->SetValue("rPos22", "330");
    // mConst->SetValue("rPos23", "345");
    mConst->SetValue("rPos24", "360");
    mConst->SetValue("rPos25", "375");
    // mConst->SetValue("rPos26", "390");
    mConst->SetValue("rPos27", "405");
    mConst->SetValue("rPos28", "420");
    mConst->SetValue("rPos29", "435");//2
    mConst->SetValue("rPos30", "450");
    mConst->SetValue("rPos31", "465");//3
    // mConst->SetValue("rPos32", "480");
    mConst->SetValue("rPos33", "495");
    mConst->SetValue("rPos34", "510");
    mConst->SetValue("rPos35", "525");
    mConst->SetValue("rPos36", "540");//3
    mConst->SetValue("rPos37", "555");
    mConst->SetValue("rPos38", "570");//2
    // mConst->SetValue("rPos39", "585");
    // mConst->SetValue("rPos40", "600");
    mConst->SetValue("rPos41", "615");
    mConst->SetValue("rPos42", "630");
    mConst->SetValue("rPos43", "645");//2
    // mConst->SetValue("rPos44", "660");
    mConst->SetValue("rPos45", "675");
    // mConst->SetValue("rPos46", "690");
    // mConst->SetValue("rPos47", "705");
    mConst->SetValue("rPos48", "720");
    // mConst->SetValue("rPos49", "735");
    mConst->SetValue("rPos50", "750");//2
    // mConst->SetValue("rPos51", "765");
    // mConst->SetValue("rPos52", "780");
    mConst->SetValue("rPos53", "795");
    // mConst->SetValue("rPos54", "810");
    mConst->SetValue("rPos55", "825");//2
    mConst->SetValue("rPos56", "840");
    // mConst->SetValue("rPos57", "855");
    mConst->SetValue("rPos58", "870");
    // mConst->SetValue("rPos59", "885");
    mConst->SetValue("rPos60", "900");//2
    // mConst->SetValue("rPos61", "915");
    mConst->SetValue("rPos62", "930");
    // mConst->SetValue("rPos63", "945");
    // mConst->SetValue("rPos64", "960");
    mConst->SetValue("rPos65", "975");
    // mConst->SetValue("rPos66", "990");
    mConst->SetValue("rPos67", "1005");
    mConst->SetValue("rPos68", "1020");
    // mConst->SetValue("rPos69", "1035");
    // mConst->SetValue("rPos70", "1050");
    // mConst->SetValue("rPos71", "1065");
    mConst->SetValue("rPos72", "1080");
    mConst->SetValue("rPos73", "1095");
    mConst->SetValue("rPos74", "1110");
    // mConst->SetValue("rPos75", "1125");
    // mConst->SetValue("rPos76", "1140");
    mConst->SetValue("rPos77", "1155");//2
    mConst->SetValue("rPos78", "1170");
    // mConst->SetValue("rPos79", "1185");
    // mConst->SetValue("rPos80", "1200");
    // mConst->SetValue("rPos81", "1215");
    mConst->SetValue("rPos82", "1230");
    mConst->SetValue("rPos83", "1245");
    mConst->SetValue("rPos84", "1260");
    // mConst->SetValue("rPos85", "1275");
    mConst->SetValue("rPos86", "1290");
    // mConst->SetValue("rPos87", "1305");
    mConst->SetValue("rPos88", "1320");
    mConst->SetValue("rPos89", "1335");
    // mConst->SetValue("rPos90", "1350");
    // mConst->SetValue("rPos91", "1365");
    // mConst->SetValue("rPos92", "1380");
    // mConst->SetValue("rPos93", "1395");
    // mConst->SetValue("rPos94", "1410");
    // mConst->SetValue("rPos95", "1425");
    mConst->SetValue("rPos96", "1440");
    // mConst->SetValue("rPos97", "1455");
    // mConst->SetValue("rPos98", "1470");
    // mConst->SetValue("rPos99", "1485");
    // mConst->SetValue("rPos100", "1500");
    // mConst->SetValue("rPos101", "1515");
    // mConst->SetValue("rPos102", "1530");
    // mConst->SetValue("rPos103", "1545");
    // mConst->SetValue("rPos104", "1560");
    // mConst->SetValue("rPos105", "1575");
    // mConst->SetValue("rPos106", "1590");
    // mConst->SetValue("rPos107", "1605");
    // mConst->SetValue("rPos108", "1620");
    // mConst->SetValue("rPos109", "1635");
    // mConst->SetValue("rPos110", "1650");
    // mConst->SetValue("rPos111", "1665");
    // mConst->SetValue("rPos112", "1680");
    // mConst->SetValue("rPos113", "1695");
    // mConst->SetValue("rPos114", "1710");
    // mConst->SetValue("rPos115", "1725");
    // mConst->SetValue("rPos116", "1740");
    // mConst->SetValue("rPos117", "1755");
    // mConst->SetValue("rPos118", "1770");
    // mConst->SetValue("rPos119", "1785");
    // mConst->SetValue("rPos120", "1800");
    // mConst->SetValue("rPos121", "1815");
    // mConst->SetValue("rPos122", "1830");
    // mConst->SetValue("rPos123", "1845");
    // mConst->SetValue("rPos124", "1860");
    // mConst->SetValue("rPos125", "1875");
    // mConst->SetValue("rPos126", "1890");
    // mConst->SetValue("rPos127", "1905");
    // mConst->SetValue("rPos128", "1920");




        // mConst->SetValue("revPos0", "0");
    // mConst->SetValue("revPos1", "15");
    // mConst->SetValue("revPos2", "30");
    // mConst->SetValue("revPos3", "45");
    // mConst->SetValue("revPos4", "60");
    // mConst->SetValue("revPos5", "75");
    // mConst->SetValue("revPos6", "90");
    // mConst->SetValue("revPos7", "105");
    // mConst->SetValue("revPos8", "120");
    // mConst->SetValue("revPos9", "135");
    // mConst->SetValue("revPos10", "150");
    // mConst->SetValue("revPos11", "165");
    // mConst->SetValue("revPos12", "180");
    // mConst->SetValue("revPos13", "195");
    // mConst->SetValue("revPos14", "210");
    // mConst->SetValue("revPos15", "225");
    // mConst->SetValue("revPos16", "240");
    // mConst->SetValue("revPos17", "255");
    // mConst->SetValue("revPos18", "270");
    // mConst->SetValue("revPos19", "285");
    // mConst->SetValue("revPos20", "300");
    // mConst->SetValue("revPos21", "315");
    // mConst->SetValue("revPos22", "330");
    // mConst->SetValue("revPos23", "345");
    // mConst->SetValue("revPos24", "360");
    // mConst->SetValue("revPos25", "375");
    // mConst->SetValue("revPos26", "390");
    // mConst->SetValue("revPos27", "405");
    // mConst->SetValue("revPos28", "420");
    // mConst->SetValue("revPos29", "435");
    // mConst->SetValue("revPos30", "450");
    // mConst->SetValue("revPos31", "465");//2
    // mConst->SetValue("revPos32", "480");
    // mConst->SetValue("revPos33", "495");
    // mConst->SetValue("revPos34", "510");
    // mConst->SetValue("revPos35", "525");
    // mConst->SetValue("revPos36", "540");
    // mConst->SetValue("revPos37", "555");
    // mConst->SetValue("revPos38", "570");
    // mConst->SetValue("revPos39", "585");
    // mConst->SetValue("revPos40", "600");
    // mConst->SetValue("revPos41", "615");
    // mConst->SetValue("revPos42", "630");
    // mConst->SetValue("revPos43", "645");
    // mConst->SetValue("revPos44", "660");
    // mConst->SetValue("revPos45", "675");
    // mConst->SetValue("revPos46", "690");
    mConst->SetValue("revPos47", "705");
    // mConst->SetValue("revPos48", "720");
    // mConst->SetValue("revPos49", "735");
    // mConst->SetValue("revPos50", "750");
    // mConst->SetValue("revPos51", "765");
    // mConst->SetValue("revPos52", "780");
    // mConst->SetValue("revPos53", "795");
    // mConst->SetValue("revPos54", "810");
    // mConst->SetValue("revPos55", "825");
    // mConst->SetValue("revPos56", "840");
    // mConst->SetValue("revPos57", "855");
    // mConst->SetValue("revPos58", "870");
    // mConst->SetValue("revPos59", "885");
    // mConst->SetValue("revPos60", "900");
    // mConst->SetValue("revPos61", "915");
    // mConst->SetValue("revPos62", "930");
    // mConst->SetValue("revPos63", "945");
    mConst->SetValue("revPos64", "960");
    // mConst->SetValue("revPos65", "975");
    // mConst->SetValue("revPos66", "990");
    // mConst->SetValue("revPos67", "1005");
    // mConst->SetValue("revPos68", "1020");
    // mConst->SetValue("revPos69", "1035");
    // mConst->SetValue("revPos70", "1050");
    // mConst->SetValue("revPos71", "1065");
    //mConst->SetValue("revPos72", "1080");
    mConst->SetValue("revPos73", "1095");
    // mConst->SetValue("revPos74", "1110");
    // mConst->SetValue("revPos75", "1125");
    // mConst->SetValue("revPos76", "1140");
    // mConst->SetValue("revPos77", "1155");
    // mConst->SetValue("revPos78", "1170");
    // mConst->SetValue("revPos79", "1185");
    // mConst->SetValue("revPos80", "1200");
    // mConst->SetValue("revPos81", "1215");
    // mConst->SetValue("revPos82", "1230");
    // mConst->SetValue("revPos83", "1245");
    mConst->SetValue("revPos84", "1260");
    // mConst->SetValue("revPos85", "1275");
    // mConst->SetValue("revPos86", "1290");
    // mConst->SetValue("revPos87", "1305");
    mConst->SetValue("revPos88", "1320");
    // mConst->SetValue("revPos89", "1335");
    // mConst->SetValue("revPos90", "1350");
    // mConst->SetValue("revPos91", "1365");
    // mConst->SetValue("revPos92", "1380");
    mConst->SetValue("revPos93", "1395");
    // mConst->SetValue("revPos94", "1410");
    // mConst->SetValue("revPos95", "1425");
    // mConst->SetValue("revPos96", "1440");
    mConst->SetValue("revPos97", "1455");
    // mConst->SetValue("revPos98", "1470");
    // mConst->SetValue("revPos99", "1485");
    mConst->SetValue("revPos100", "1500");
    // mConst->SetValue("revPos101", "1515");
    // mConst->SetValue("revPos102", "1530");
    // mConst->SetValue("revPos103", "1545");
    // mConst->SetValue("revPos104", "1560");
    // mConst->SetValue("revPos105", "1575");
    // mConst->SetValue("revPos106", "1590");
    // mConst->SetValue("revPos107", "1605");
    // mConst->SetValue("revPos108", "1620");
    // mConst->SetValue("revPos109", "1635");
    // mConst->SetValue("revPos110", "1650");
    // mConst->SetValue("revPos111", "1665");
    // mConst->SetValue("revPos112", "1680");
    // mConst->SetValue("revPos113", "1695");
    // mConst->SetValue("revPos114", "1710");
    // mConst->SetValue("revPos115", "1725");
    // mConst->SetValue("revPos116", "1740");
    // mConst->SetValue("revPos117", "1755");
    // mConst->SetValue("revPos118", "1770");
    // mConst->SetValue("revPos119", "1785");
    // mConst->SetValue("revPos120", "1800");
    // mConst->SetValue("revPos121", "1815");
    // mConst->SetValue("revPos122", "1830");
    // mConst->SetValue("revPos123", "1845");
    // mConst->SetValue("revPos124", "1860");
    // mConst->SetValue("revPos125", "1875");
    // mConst->SetValue("revPos126", "1890");
    // mConst->SetValue("revPos127", "1905");
    // mConst->SetValue("revPos128", "1920");

#else
    mConst->SetValue("rPos0", "55");
    mConst->SetValue("rPos2", "85");
    mConst->SetValue("rPos3", "100");
    mConst->SetValue("rPos4", "115");
    mConst->SetValue("rPos5", "130");
    mConst->SetValue("rPos8", "175");
    mConst->SetValue("rPos9", "190");
    mConst->SetValue("rPos10", "205");
    mConst->SetValue("rPos12", "235");
    mConst->SetValue("rPos14", "265");
    mConst->SetValue("rPos15", "280");
    mConst->SetValue("rPos17", "310");
    mConst->SetValue("rPos18", "325");
    mConst->SetValue("rPos19", "340");
    mConst->SetValue("rPos20", "355");
    mConst->SetValue("rPos21", "370");
    mConst->SetValue("rPos22", "385");
    mConst->SetValue("rPos24", "415");
    mConst->SetValue("rPos25", "430");
    mConst->SetValue("rPos27", "460");
    mConst->SetValue("rPos28", "475");
    mConst->SetValue("rPos29", "490");
    mConst->SetValue("rPos30", "505");
    mConst->SetValue("rPos31", "520");
    mConst->SetValue("rPos33", "550");
    mConst->SetValue("rPos34", "565");
    mConst->SetValue("rPos35", "580");
    mConst->SetValue("rPos36", "595");
    mConst->SetValue("rPos37", "610");
    mConst->SetValue("rPos38", "625");
    mConst->SetValue("rPos41", "670");
    mConst->SetValue("rPos42", "685");
    mConst->SetValue("rPos43", "700");
    mConst->SetValue("rPos45", "730");
    mConst->SetValue("rPos48", "775");
    mConst->SetValue("rPos50", "805");
    mConst->SetValue("rPos53", "850");
    mConst->SetValue("rPos55", "880");
    mConst->SetValue("rPos56", "895");
    mConst->SetValue("rPos58", "925");
    mConst->SetValue("rPos60", "955");
    mConst->SetValue("rPos62", "985");
    mConst->SetValue("rPos65", "1030");
    mConst->SetValue("rPos67", "1060");
    mConst->SetValue("rPos68", "1075");
    mConst->SetValue("rPos72", "1135");
    mConst->SetValue("rPos73", "1150");
    mConst->SetValue("rPos74", "1165");
    mConst->SetValue("rPos77", "1210");
    mConst->SetValue("rPos78", "1225");
    mConst->SetValue("rPos82", "1285");
    mConst->SetValue("rPos83", "1300");
    mConst->SetValue("rPos84", "1315");
    mConst->SetValue("rPos86", "1345");
    mConst->SetValue("rPos88", "1375");
    mConst->SetValue("rPos89", "1390");
    mConst->SetValue("rPos96", "1495");

    mConst->SetValue("revPos47", "650");
    mConst->SetValue("revPos64", "905");
    mConst->SetValue("revPos73", "1040");
    mConst->SetValue("revPos84", "1205");
    mConst->SetValue("revPos88", "1265");
    mConst->SetValue("revPos93", "1340");
    mConst->SetValue("revPos97", "1400");
    mConst->SetValue("revPos100", "1445");
#endif

        // mConst->SetValue("rndPos0", "0");
    // mConst->SetValue("rndPos1", "15");
    // mConst->SetValue("rndPos2", "30");
    // mConst->SetValue("rndPos3", "45");
    // mConst->SetValue("rndPos4", "60");
    // mConst->SetValue("rndPos5", "75");
    // mConst->SetValue("rndPos6", "90");
    // mConst->SetValue("rndPos7", "105");
    // mConst->SetValue("rndPos8", "120");
    // mConst->SetValue("rndPos9", "135");
    // mConst->SetValue("rndPos10", "150");
    // mConst->SetValue("rndPos11", "165");
    // mConst->SetValue("rndPos12", "180");
    // mConst->SetValue("rndPos13", "195");
    // mConst->SetValue("rndPos14", "210");
    // mConst->SetValue("rndPos15", "225");
    // mConst->SetValue("rndPos16", "240");
    mConst->SetValue("rndPos17", "255");
    // mConst->SetValue("rndPos18", "270");
    // mConst->SetValue("rndPos19", "285");
    // mConst->SetValue("rndPos20", "300");
    // mConst->SetValue("rndPos21", "315");
    // mConst->SetValue("rndPos22", "330");
    // mConst->SetValue("rndPos23", "345");
    // mConst->SetValue("rndPos24", "360");
    // mConst->SetValue("rndPos25", "375");
    // mConst->SetValue("rndPos26", "390");
    // mConst->SetValue("rndPos27", "405");
    // mConst->SetValue("rndPos28", "420");
    // mConst->SetValue("rndPos29", "435");
    // mConst->SetValue("rndPos30", "450");
    // mConst->SetValue("rndPos31", "465");
    // mConst->SetValue("rndPos32", "480");
    // mConst->SetValue("rndPos33", "495");
    // mConst->SetValue("rndPos34", "510");
    // mConst->SetValue("rndPos35", "525");
    // mConst->SetValue("rndPos36", "540");
    mConst->SetValue("rndPos37", "555");//2
    mConst->SetValue("rndPos38", "570");
    // mConst->SetValue("rndPos39", "585");
    // mConst->SetValue("rndPos40", "600");
    // mConst->SetValue("rndPos41", "615");
    // mConst->SetValue("rndPos42", "630");
    // mConst->SetValue("rndPos43", "645");
    //mConst->SetValue("rndPos44", "660");
    mConst->SetValue("rndPos45", "675");
    // mConst->SetValue("rndPos46", "690");
    mConst->SetValue("rndPos47", "705");
    // mConst->SetValue("rndPos48", "720");
    // mConst->SetValue("rndPos49", "735");
    mConst->SetValue("rndPos50", "750");
    // mConst->SetValue("rndPos51", "765");
    // mConst->SetValue("rndPos52", "780");
    // mConst->SetValue("rndPos53", "795");
    mConst->SetValue("rndPos54", "810");
    // mConst->SetValue("rndPos55", "825");
    // mConst->SetValue("rndPos56", "840");
    // mConst->SetValue("rndPos57", "855");
    mConst->SetValue("rndPos58", "870");
    // mConst->SetValue("rndPos59", "885");
    mConst->SetValue("rndPos60", "900");
    // mConst->SetValue("rndPos61", "915");
    // mConst->SetValue("rndPos62", "930");
    // mConst->SetValue("rndPos63", "945");
    // mConst->SetValue("rndPos64", "960");
    // mConst->SetValue("rndPos65", "975");
    // mConst->SetValue("rndPos66", "990");
    // mConst->SetValue("rndPos67", "1005");
    // mConst->SetValue("rndPos68", "1020");
    mConst->SetValue("rndPos69", "1035");
    // mConst->SetValue("rndPos70", "1050");
    // mConst->SetValue("rndPos71", "1065");
    mConst->SetValue("rndPos72", "1080");
    // mConst->SetValue("rndPos73", "1095");
    // mConst->SetValue("rndPos74", "1110");
    // mConst->SetValue("rndPos75", "1125");
    // mConst->SetValue("rndPos76", "1140");
    //mConst->SetValue("rndPos77", "1155");
    mConst->SetValue("rndPos78", "1170");
    // mConst->SetValue("rndPos79", "1185");
    // mConst->SetValue("rndPos80", "1200");
    mConst->SetValue("rndPos81", "1215");
    // mConst->SetValue("rndPos82", "1230");
    mConst->SetValue("rndPos83", "1245");
    mConst->SetValue("rndPos84", "1260");
    // mConst->SetValue("rndPos85", "1275");
    // mConst->SetValue("rndPos86", "1290");
    // mConst->SetValue("rndPos87", "1305");
    mConst->SetValue("rndPos88", "1320");
    // mConst->SetValue("rndPos89", "1335");
    mConst->SetValue("rndPos90", "1350");
    // mConst->SetValue("rndPos91", "1365");
    // mConst->SetValue("rndPos92", "1380");
    mConst->SetValue("rndPos93", "1395");
    mConst->SetValue("rndPos94", "1410");//2
    // mConst->SetValue("rndPos95", "1425");
    // mConst->SetValue("rndPos96", "1440");
    // mConst->SetValue("rndPos97", "1455");
    // mConst->SetValue("rndPos98", "1470");
    //mConst->SetValue("rndPos99", "1485");
    mConst->SetValue("rndPos100", "1500");
    // mConst->SetValue("rndPos101", "1515");
    mConst->SetValue("rndPos102", "1530");
    // mConst->SetValue("rndPos103", "1545");
    // mConst->SetValue("rndPos104", "1560");
    mConst->SetValue("rndPos105", "1575");
    // mConst->SetValue("rndPos106", "1590");
    // mConst->SetValue("rndPos107", "1605");
    // mConst->SetValue("rndPos108", "1620");
    // mConst->SetValue("rndPos109", "1635");
    mConst->SetValue("rndPos110", "1650");
    // mConst->SetValue("rndPos111", "1665");
    // mConst->SetValue("rndPos112", "1680");
    // mConst->SetValue("rndPos113", "1695");
    // mConst->SetValue("rndPos114", "1710");
    mConst->SetValue("rndPos115", "1725");
    // mConst->SetValue("rndPos116", "1740");
    // mConst->SetValue("rndPos117", "1755");
    // mConst->SetValue("rndPos118", "1770");
    // mConst->SetValue("rndPos119", "1785");
    // mConst->SetValue("rndPos120", "1800");
    // mConst->SetValue("rndPos121", "1815");
    // mConst->SetValue("rndPos122", "1830");
    // mConst->SetValue("rndPos123", "1845");
    // mConst->SetValue("rndPos124", "1860");
    // mConst->SetValue("rndPos125", "1875");
    // mConst->SetValue("rndPos126", "1890");
    // mConst->SetValue("rndPos127", "1905");
    // mConst->SetValue("rndPos128", "1920");


    mData->SetValue("testOperationToogle", "0");
    mData->SetValue("testOperation", "None");
    mData->SetValue("testLoadToggle", "0");
    mData->SetValue("mSelectedPathList", "");
    mData->SetValue("extenPathList", "");
    mData->SetValue("selectionHandle", "0");

    mData->SetValue("passNotMatched", "0");
    mData->SetValue("autoOverlayHandler", "0");

    mData->SetValue("shrpTweakName", "");
    mData->SetValue("shrpTweakInfo", "");
    mData->SetValue("shrpTweakFileName", "");
    mData->SetValue("shrpTweakConfirm", "");
    mData->SetValue("shrpTweakFinish", "");

    mData->SetValue("shrpMagiskRoot", "");
    mData->SetValue("shrpMagiskUnroot", "");
    mData->SetValue("shrpMagiskUpdate", "");
    mData->SetValue("magiskModule", "");

    mData->SetValue("modulePath", "");
    mData->SetValue("moduleName", "");
    mData->SetValue("moduleVersion", "");
    mData->SetValue("moduleAuthor", "");
    mData->SetValue("moduleDesc", "");
    mData->SetValue("refreshElement", "0");

    mData->SetValue("moduleEnabled", "1");
    //Security
    mData->SetValue("recLockStatus", "0");

    mData->SetValue("lockVal_notMatched", "0");
    mData->SetValue("lockType", "1");
    mData->SetValue("lockReset", "0");


    mData->SetValue("themeProcessErr", "-1");
    mData->SetValue("processHead", "");
    mData->SetValue("processDetails", "");


    //Color Properties
    mConst->SetValue("clrBlack", "#010101");
    mConst->SetValue("clrDark", "#191919");
    mConst->SetValue("clrDeepBlue", "#0f112f");
    mConst->SetValue("clrDarkBlue", "#191f22");
    mConst->SetValue("clrDarkGreen", "#141a19");
    mConst->SetValue("clrDarkPurple", "#161218");
    mConst->SetValue("clrDarkViolate", "#242038");
    mConst->SetValue("clrWhite", "#ffffff");
    mConst->SetValue("clrBlue", "#0088ff");
    mConst->SetValue("clrCyan", "#00f2c4");
    mConst->SetValue("clrIndigo", "#3f51b5");
    mConst->SetValue("clrGreen", "#21ef8b");
    mConst->SetValue("clrTeal", "#009688");
    mConst->SetValue("clrBrown", "#795548");
    mConst->SetValue("clrYellow", "#ffc000");
    mConst->SetValue("clrOrange", "#ed6f01");
    mConst->SetValue("clrRed", "#e53935");
    mConst->SetValue("clrLred", "#ff5353");
    mConst->SetValue("clrPink", "#ff0095");
    mConst->SetValue("clrRpink", "#fd0f60");
    mConst->SetValue("clrPurple", "#7750fb");

    //Color Panel Handler Vars
    mData->SetValue("assignVar", "none");
    mData->SetValue("colorHolder", "none");


    //Dublicate Var for theme
    mData->SetValue("extern_primaryColor", "");
    mData->SetValue("extern_secondaryColor", "");
    mData->SetValue("extern_accentColor", "");
    mData->SetValue("extern_backgroundColor", "");
    mData->SetValue("extern_navbarColor", "");
    mData->SetValue("extern_dashboardIconType", "");
    mData->SetValue("extern_roundedcornerType", "");


#ifdef SHRP_OFFICIAL
		mConst->SetValue("is_Official", "true");
        mConst->SetValue("shrpBuild", 1);
#else
        mConst->SetValue("is_Official", "false");
        mConst->SetValue("shrpBuild", 0);
#endif


    //Permission
    mData->SetValue("permOwnerR", 0);
    mData->SetValue("permOwnerW", 0);
    mData->SetValue("permOwnerX", 0);
    mData->SetValue("permGroupR", 0);
    mData->SetValue("permGroupR", 0);
    mData->SetValue("permGroupR", 0);
    mData->SetValue("permGlobalR", 0);
    mData->SetValue("permGlobalW", 0);
    mData->SetValue("permGlobalX", 0);

    mData->SetValue("NewpermOwnerR", 0);
    mData->SetValue("NewpermOwnerW", 0);
    mData->SetValue("NewpermOwnerX", 0);
    mData->SetValue("NewpermGroupR", 0);
    mData->SetValue("NewpermGroupR", 0);
    mData->SetValue("NewpermGroupR", 0);
    mData->SetValue("NewpermGlobalR", 0);
    mData->SetValue("NewpermGlobalW", 0);
    mData->SetValue("NewpermGlobalX", 0);
    mData->SetValue("permText", "");
    mData->SetValue("permChanged", 0);

    mData->SetValue("mSelection", 0);
    mData->SetValue("selectEnabled", 0);
    mData->SetValue("searchText", "");
    mData->SetValue("sideBarDes", "tw_file_location1");

    mData->SetValue("isTextFile", 0);
    mData->SetValue("isTheme", 0);
    mData->SetValue("isZip", 0);
    mData->SetValue("isRepackable", 0);

    //FileManager
    mData->SetValue("newRenameName", "");
    mData->SetValue("newFolderName", "");
    mData->SetValue("newChmod", "");
    mData->SetValue("archiveName", "");
    mData->SetValue("fManagerAction", "");
    mData->SetValue("fAction", "0");
    mData->SetValue("fActionName", "N/A");
    mData->SetValue("isFile", 0);
    mData->SetValue("fPerm", "");

    //TextEditor
    mData->SetValue("refresh", 0);
    mData->SetValue("selectedLine", "");
    mData->SetValue("selectedLineNo", 0);
    mData->SetValue("maxLine", 0);
    mData->SetValue("textFileName", "");

    //Infos
    mData->SetValue("objectSize", "");

    //FActionSettingVariables
    mPersist->SetValue("overwriteExisting", 0);
    mPersist->SetValue("useHighCompression", 0);

    //CustomTheme
    mData->SetValue("themeName", "Theme");
    mData->SetValue("themeBackgroundColor", "");
    mData->SetValue("themeAccentColor", "");
    mData->SetValue("themeTextColor", "");
    mData->SetValue("themeSecondaryTextColor", "");
    mData->SetValue("themeNavColor", "");




}
