#ifndef __TOKENIZE_H__
#define __TOKENIZE_H__

#include <string>
#include <vector>
#include <algorithm>

#ifdef WIN32
#define string std::string
#define vector std::vector
#endif

vector<string> GetTokens(string theString, char separator);

#endif