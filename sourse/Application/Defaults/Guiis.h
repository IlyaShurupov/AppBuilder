
#pragma once

#include "UI/uis/GUI.h"

class DataView : public Guii {

    DataView& operator = (const DataView& in);
    DataView(const DataView& in) : Guii(in) {
    } 

    public:

    DataView(Obj* prnt, Rect<float> _rect) : Guii(prnt, _rect) {
        RegisterType(ObjType("DataView"));
    }

    virtual DataView& Instance() {
        return *new DataView(*this);
    }

    void ProcBody(ObList* requests) {
      if (state == GuiiState::ACTIVATE) {
        int i = 0;
      }
      else if (state == GuiiState::CLOSE) {
        int i = 0;
      }
      else if (state == GuiiState::ENTERED) {
        int i = 0;
      }
      else if (state == GuiiState::INSIDE) {
        int i = 0;
      }
    }

    void DrawBody(Obj* root_obj) {
        if (state == GuiiState::INSIDE) {
            buff->DrawRect(Rect<int>(0, 0, rect.size.x, rect.size.y), Color(0, 1, 1, .5));
        } else {
            buff->DrawRect(Rect<int>(0, 0, rect.size.x, rect.size.y), Color(1, 0, 1, .5));
        }
    }

    bool TransformRequest() { 
        return false;
    }

    void Transform() {}

    virtual ~DataView() {}
};