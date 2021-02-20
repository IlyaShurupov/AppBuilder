#pragma once

#include "Containers/List.h"

struct UIItem;
struct DataBlock;
class Operator;

void ui_add_region(UIItem* region, struct Operators* ops);
void ui_template_button(UIItem* button, struct Operators* ops, DataBlock* db);
void ui_template_group(UIItem* uii, DataBlock* db);
void ui_template_root(UIItem* uii);