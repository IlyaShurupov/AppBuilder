#pragma once

#include "LinkedList.h"

struct UIItem;
struct DataBlock;
struct Operator;

void ui_add_region(UIItem* region, List<Operator>* operators);
void ui_template_button(UIItem* button, List<Operator>* operators, DataBlock* db);
void ui_template_group(UIItem* uii, DataBlock* db);
void ui_template_root(UIItem* uii);