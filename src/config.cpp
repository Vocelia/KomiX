#include "include/config.hpp"

Config::Config(const char* path) {
  ini.SetUnicode();
  SI_Error rc = ini.LoadFile(path);
  if (rc < 0) printf("[ERR]: Failed to read from the configuration file!\n");
}

int Config::get_all_sections() {
  unsigned int i = 0;
  CSimpleIniA::TNamesDepend all_sections;
  ini.GetAllSections(all_sections);
  if (!all_sections.empty()) {
    sections.size = all_sections.size();
    sections.names = new const char*[all_sections.size()];
    for (const auto &section: all_sections) {
      sections.names[i] = section.pItem; i++;
    }
    return 0;
  }
  return -1;
}

int Config::get_all_keys(const char* section) {
  unsigned int i = 0;
  CSimpleIniA::TNamesDepend all_keys;
  ini.GetAllKeys(section, all_keys);
  if (!all_keys.empty()) {
    keys.size = all_keys.size();
    keys.names = new const char*[all_keys.size()];
    for (const auto &key: all_keys) {
      keys.names[i] = key.pItem; i++;
    }
    return 0;
  }
  return -1;
}