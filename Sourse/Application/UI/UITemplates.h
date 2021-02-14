#pragma once

#include "LinkedList.h"

struct UIItem;
struct DataBlock;
struct Operator;

void ui_add_region(UIItem* region, struct Operators* ops);
void ui_template_button(UIItem* button, struct Operators* ops, DataBlock* db);
void ui_template_group(UIItem* uii, DataBlock* db);
void ui_template_root(UIItem* uii);