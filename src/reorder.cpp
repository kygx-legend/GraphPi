#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <cstdio>
#include <cstdlib>

using namespace std;

unordered_map<unsigned long, unsigned long> nodes;
vector<pair<unsigned long, unsigned long>> edges;

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("Please input file name!!!\n");
    return 0;
  }
  string filename(argv[1]);

  FILE* stream;
  char* line = NULL;
  size_t len = 0;
  ssize_t read;
  unsigned long size = 0;

  stream = fopen(filename.c_str(), "r");
  if (stream == NULL)
    exit(EXIT_FAILURE);

  while ((read = getline(&line, &len, stream)) != -1) {
    istringstream l(line);
    string start, end;
    l >> start >> end;
		if (start == "#")
			continue;
    unsigned long s = stoi(start);
    unsigned long e = stoi(end);
    if (nodes.find(s) == nodes.end()) {
      pair<unsigned long, unsigned long> p(s, size);
      nodes.insert(p);
      size++;
    }
    if (nodes.find(e) == nodes.end()) {
      pair<unsigned long, unsigned long> p(e, size);
      nodes.insert(p);
      size++;
    }
    pair<unsigned long, unsigned long> p(s, e);
    edges.push_back(p);
    //cout << s << e << endl;
  }

  free(line);
  fclose(stream);

  cout << nodes.size() << " " << edges.size() << endl;

  for (unsigned long i = 0; i < edges.size(); i++) {
    cout << nodes[edges[i].first] << " " << nodes[edges[i].second] << endl;
  }
  return 0;
}
