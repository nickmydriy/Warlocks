#include "Settings.h"

Settings::Settings(std::string const &fileName) {
    this->fileName = fileName;
    reload();
}

void Settings::reload() {
    checkFile();
    settingsMap.clear();
    std::ifstream cin (fileName);
    std::string set, value;
    while (cin) {
        getline(cin, set, ' ');
        getline(cin, value);
        settingsMap[set] = value;
    }
    cin.close();
}

std::string const & Settings::get(std::string const & name, std::string const & def) const {
    if (settingsMap.count(name) != 0) {
        return settingsMap.at(name);
    } else {
        return def;
    }

}

void Settings::set(std::string const &name, std::string const &value) {
    checkFile();
    settingsMap[name] = value;
    Settings::save();
}

void Settings::save() {
    checkFile();
    std::ofstream cout(fileName);
    std::map <std::string, std::string>::iterator temp;
    for (temp = settingsMap.begin(); temp != settingsMap.end(); temp++) {
        cout << (*temp).first << " " << (*temp).second << std::endl;
    }
    cout.close();
}

void Settings::reset() {
    checkFile();
    std::ofstream cout (fileName);
    settingsMap.clear();
    cout.close();
}

void Settings::checkFile() {
    if (fileName == "") {
        fileName = "default.cfg";
    }
}