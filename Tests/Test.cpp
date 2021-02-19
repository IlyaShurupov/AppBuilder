
#include "Containers/LinkedList.h"

class P {
 public:
  int i;
  P(int i) : i(i) {}
};

int main() {
  List<P> list;
  list.PushBack(&P(3));
  list.PushBack(&P(2));
  list.PushBack(&P(1));

  list.Sort([](P& p1, P& p2) { return p1.i > p2.i; });

  for (Iterator<P> iter(&list, 0); iter < 3; ++iter) {
    list[iter] = 6;
  }
}