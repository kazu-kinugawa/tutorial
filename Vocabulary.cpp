#include "Vocabulary.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>

bool pairCompare(const std::pair<std::string, int>& firstElof, const std::pair<std::string, int>& secondElof)
{
    return firstElof.second > secondElof.second;
}

Vocabulary::Vocabulary(const std::string& trainFile, const int tokenFreqThreshold):
eosIndex(0), bosIndex(1), unkIndex(2)
{
  std::ifstream ifs(trainFile);
  if(!ifs){
    std::cout << trainFile << " does not exist." << std::endl;
    exit(1);
  }
  ifs.unsetf(std::ios::skipws);

  std::vector<std::string> tokens;
  std::unordered_map<std::string, int> tokenCount;

  std::string line;
  std::string token;
  while(std::getline(ifs, line, '\n')){
    std::istringstream stream(line);
    while(std::getline(stream, token, ' ')){
        if(tokenCount.count(token)){
          tokenCount.at(token) += 1;
        }
        else{
          tokenCount.insert(std::make_pair(token,1));
        }
    }
  }

  std::vector< std::pair<std::string, int> > tokenList;

  for (auto it = tokenCount.begin(), itEnd = tokenCount.end(); it != itEnd; ++it){
    if (it->second >= tokenFreqThreshold){
      tokenList.push_back(std::make_pair(it->first, it->second));
    }
  }

  sort(tokenList.begin(), tokenList.end(), pairCompare);

  for (int i = 0, i_end = (int)tokenList.size(); i < i_end; ++i){
    this->token2index.insert(std::make_pair(tokenList[i].first, i + 3));
  }
  this->token2index.insert(std::make_pair("*EOS*", this->eosIndex));
  this->token2index.insert(std::make_pair("*BOS*", this->bosIndex));
  this->token2index.insert(std::make_pair("*UNK*", this->unkIndex));

  this->index2token.resize(this->token2index.size());
  for(auto it = this->token2index.begin(), itEnd = this->token2index.end(); it != itEnd; ++it){
    this->index2token[it->second] = it->first;
  }
}

Vocabulary::Vocabulary(const std::string& loadFile):
eosIndex(0), bosIndex(1), unkIndex(2)
{
  std::ifstream ifs(loadFile);
  if(!ifs){
    std::cout << loadFile << " does not exist." << std::endl;
    exit(1);
  }
  ifs.unsetf(std::ios::skipws);

  int index;
  std::string line, token;
  std::stringstream ss;
  while(std::getline(ifs, line, '\n')){
    std::istringstream stream(line);

    std::getline(stream, token, '\t');

    ss.str(token);
    ss >> index;
    ss.str("");
    ss.clear(std::stringstream::goodbit);

    std::getline(stream, token, '\t');

    this->index2token.push_back(token);
    this->token2index.insert(std::make_pair(token,index));
  }
}

void Vocabulary::save(const std::string& saveFile){
  std::ofstream ofs(saveFile);
  if(!ofs){
    std::cout << saveFile << " does not exist." << std::endl;
    exit(1);
  }

  for (int i = 0, i_end = (int)this->index2token.size(); i < i_end; ++i){
    ofs << i << '\t' << this->index2token[i] << '\t' << std::endl;
  }

  ofs.close();
}

std::string Vocabulary::getStr(const std::vector<int>& array){
  std::string res = "";

  const int i_end = array.size() - 1;
  for(int i = 0, tmp_i_end = i_end - 1; i < tmp_i_end; ++i){
    res += this->index2token[array[i]] + ' ';
  }
  res += this->index2token[array[i_end]];

  return res;
}

void Vocabulary::check(){

  const std::string trainFile = "/data/local/kinugawa/ja_en_subtitle_corpus/tmp.ja";
  const int tokenFreqThreshold = 0;
  const std::string saveFile = "./voc.txt";
  const std::string loadFile = "./voc.txt";

  Vocabulary voc(trainFile, tokenFreqThreshold);

  voc.save(saveFile);
}
