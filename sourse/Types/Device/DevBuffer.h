# pragma once 


#include "Geometry/Rect.h"
#include "BitMap/Color.h"

void init_device_texture(void* holder_p);

class DevBuffer {

    void* devtx = nullptr;
    void Create(int width, int height);
    void Resize_int(int width, int height);
    
    public:

        vec2<int> size;

        template <typename SizeType>
        DevBuffer(SizeType width, SizeType height) {
            Create((int)width, (int)height);
        }

        ~DevBuffer();

		void DrawRect(const Rect<int>& rect, const Color& col);
		void Project(DevBuffer* srs, const vec2<int>& pos);

		void draw_tex_ro_rend(const vec2<int>& pos);


        template <typename SizeType>
        void resize(SizeType width, SizeType height) {
            Resize_int((int)width, (int)height);
        }

        void Assign(const Color& col) {
            DrawRect(Rect<int>(0, 0, size.x, size.y), col);
        }

        void DrawBounds(Rect<int>& rect, const Color& col, short thickness);
        void DrawLine(const vec2<SCR_INT>& head, const vec2<SCR_INT>& tail, const Color& col, short thickness);
    
};