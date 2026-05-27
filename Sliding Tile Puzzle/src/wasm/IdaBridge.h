#ifndef IDA_BRIDGE_H
#define IDA_BRIDGE_H

#include <vector>
#include <string>

// Bridge functions that WebGame can safely call
void initIda(const std::string& dbName, int width, int height, const std::vector<std::vector<int>>& patterns);
std::vector<int> runIda(const std::vector<int>& grid, int width, int height);

#endif