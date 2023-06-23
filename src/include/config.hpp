#include <cstdio>
#include <SimpleIni.h>

class Config {
  public:
    CSimpleIniA ini;
    const char** key_names;
    const char** section_names;

    Config(const char* path) {
      ini.SetUnicode();
      SI_Error rc = ini.LoadFile(path);
      if (rc < 0) printf("[ERR]: Failed to read from the configuration file!\n");
    }

    int get_all_sections() {
      unsigned int i = 0;
      CSimpleIniA::TNamesDepend sections;
      ini.GetAllSections(sections);
      if (!sections.empty()) {
        section_names = new const char*[sections.size()];
        for (const auto &section: sections) {
          section_names[i] = section.pItem; i++;
        }
        return 0;
      }
      return -1;
    }

    int get_all_keys(const char* section) {
      unsigned int i = 0;
      CSimpleIniA::TNamesDepend keys;
      ini.GetAllKeys(section, keys);
      if (!keys.empty()) {
        key_names = new const char*[keys.size()];
        for (const auto &key: keys) {
          key_names[i] = key.pItem; i++;
        }
        return 0;
      }
      return -1;
    }
};