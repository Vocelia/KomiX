#pragma once
#include <cstdio>
#include "SimpleIni.h"

class Config {
  private:
    struct keysT {
      size_t size;
      const char** names;
    };
    struct sectionsT {
      size_t size;
      const char** names;
    };
  public:
    CSimpleIniA ini;
    struct keysT keys;
    struct sectionsT sections;

    Config(const char* path);
    int get_all_sections();
    int get_all_keys(const char* section);
};