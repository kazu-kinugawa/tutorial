#pragma once

#include <string>
#include <unordered_map>
#include <vector>

class Vocabulary{
public:
  Vocabulary(const std::string& trainFile, const int tokenFreqThreshold);
  Vocabulary(const std::string& loadFile);

  const int eosIndex;
  const int bosIndex;
  const int unkIndex;

  std::unordered_map<std::string, int> token2index;
  std::vector<std::string> index2token;

  void save(const std::string& saveFile);
  std::string getStr(const std::vector<int>& array);

  static void check();
};
