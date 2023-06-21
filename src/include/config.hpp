#include <map>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

std::map<std::string, std::map<std::string, std::string>> getConfig(std::string path) {
  std::ifstream fs(path);
  json data = json::parse(fs);
  std::map<std::string, std::map<std::string, std::string>> result;
  for (json::iterator it = data.begin(); it != data.end(); it++) {
    for (json::iterator itn = it.value().begin(); itn != it.value().end(); itn++) {
      result[it.key()] = std::map<std::string, std::string>{{itn.key(), itn.value()}};
    }
  }
  return result;
}