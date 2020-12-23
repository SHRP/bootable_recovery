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
class ThemeManager{
    private:
        struct color{
            string colorName;
            string colorValue;
        };
        vector<color> colors;
    public:
        //CustomTheme Functions
        bool initCustomTheme(string path);
        bool verifyColor(string hex);
        void removeTempData();
        bool applyCustomTheme();

        //Default Functions
        int hexToInt(string str); //Convert Hex value to int for processing the color val
        bool isColorDark(string str);
        string getColor(string str);
        string get_subBackgroundColor();
        string get_DashboardColor();
        bool syncDyanmicVar();
        static void initialVarProcess();
        bool applyThemeResouces();
        ThemeManager(void){
            color c;
            c.colorName="black";
            c.colorValue="#010101";
            colors.push_back(c);
            c.colorName="dark";
            c.colorValue="#191919";
            colors.push_back(c);
            c.colorName="deepBlue";
            c.colorValue="#0f112f";
            colors.push_back(c);
            c.colorName="darkBlue";
            c.colorValue="#191f22";
            colors.push_back(c);
            c.colorName="darkGreen";
            c.colorValue="#141a19";
            colors.push_back(c);
            c.colorName="darkPurple";
            c.colorValue="#161218";
            colors.push_back(c);
            c.colorName="darkViolate";
            c.colorValue="#242038";
            colors.push_back(c);
            c.colorName="white";
            c.colorValue="#ffffff";
            colors.push_back(c);
            c.colorName="blue";
            c.colorValue="#0088ff";
            colors.push_back(c);
            c.colorName="cyan";
            c.colorValue="#00f2c4";
            colors.push_back(c);
            c.colorName="indigo";
            c.colorValue="#3f51b5";
            colors.push_back(c);
            c.colorName="green";
            c.colorValue="#21ef8b";
            colors.push_back(c);
            c.colorName="teal";
            c.colorValue="#009688";
            colors.push_back(c);
            c.colorName="brown";
            c.colorValue="#795548";
            colors.push_back(c);
            c.colorName="yellow";
            c.colorValue="#ffc000";
            colors.push_back(c);
            c.colorName="orange";
            c.colorValue="#ed6f01";
            colors.push_back(c);
            c.colorName="red";
            c.colorValue="#e53935";
            colors.push_back(c);
            c.colorName="lred";
            c.colorValue="#ff5353";
            colors.push_back(c);
            c.colorName="pink";
            c.colorValue="#ff0095";
            colors.push_back(c);
            c.colorName="rpink";
            c.colorValue="#fd0f60";
            colors.push_back(c);
            c.colorName="purple";
            c.colorValue="#7750fb";
            colors.push_back(c);
        }
};