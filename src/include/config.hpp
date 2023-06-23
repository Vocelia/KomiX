#include <cstdio>
#include <SimpleIni.h>

class Config {
  public:
    CSimpleIniA ini;
    const char** key_names;
    const char** section_names;

    Config(const char* path);
    int get_all_sections();
    int get_all_keys(const char* section);
};