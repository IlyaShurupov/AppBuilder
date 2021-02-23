
#include "Containers/List.h"
#include "Containers/Array.h"


int main() {
  Array<uint8> g(5);

  for (uint4 i = 0; i < g.Len(); i++) {
    g[i] = i;
  }

  g.PushBack(5);
  g.Remove(1);

  List<int> fg;
  int d = 5;
  fg.Insert(&d, 0);
}