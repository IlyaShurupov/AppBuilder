
#include "Application.h"

#include "Operators.h"
#include "UInterfaces.h"

UIItem* resolveUiRef(Str* name, Operators* ops, DataBlock* paramsdb, DataBlock* uiidb) {
  UIItem* newuii = nullptr;
  if (*name == Str("Button")) {
    newuii = (UIItem*)NEW(UIIButton)(ops, paramsdb, uiidb);
  } else if (*name == Str("Group")) {
    newuii = (UIItem*)NEW(UIIGroup)(ops, paramsdb, uiidb);
  }
  return newuii;
}

void sdltest();

int main__() {


  Application DrawApp(60);

  DrawApp.AddOperator((Operator*)NEW(OpLogHeap)());
  DrawApp.AddOperator((Operator*)NEW(OpConsoleToggle)());
  DrawApp.AddOperator((Operator*)NEW(OpEndSeance)());
  DrawApp.AddOperator((Operator*)NEW(OpMoveCanvas)());
  DrawApp.AddOperator((Operator*)NEW(OpUIIMove)());
  DrawApp.AddOperator((Operator*)NEW(OpUIIResize)());
  
  Str path("/home/ilusha/dev/AppBuilder/build/bin/Linux64/DrawApp/Configuration/");
  DrawApp.LoadConfiguration(&path, resolveUiRef);

  DrawApp.Launch();
  return 0;
}


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>


void tex_draw(SDL_Texture* tex, SDL_Renderer* window, const vec2<int>& pos){
	SDL_Rect dest;
	dest.x = pos.x;
	dest.y = pos.y;
	SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
	SDL_RenderCopy(window, tex, NULL, &dest);
}

int main___() {

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* holder_head = SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);


	Uint32 render_flags = SDL_RENDERER_ACCELERATED;
	SDL_Renderer* holder = SDL_CreateRenderer(holder_head, -1, render_flags);


	SDL_Surface* surface;
	surface = IMG_Load("jpg.jpg");
	SDL_Texture* tex = SDL_CreateTextureFromSurface(holder, surface);
	SDL_FreeSurface(surface);

	// SDL_RenderDrawRect(SDL_Renderer*   renderer, const SDL_Rect* rect)

	SDL_RenderClear(holder);
	tex_draw(tex, holder, vec2<int>(0, 0));
	// SDL_FillRect(tex, nullptr, SDL_MapRGB(tex->format, 255, 0, 0));

	SDL_RenderPresent(holder); // triggers the double buffers for multiple rendering


	SDL_Delay(2000);

	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(holder);
	SDL_DestroyWindow(holder_head);
	
	SDL_Quit();

	return 0;
}



#define WINDOW_WIDTH			(800)
#define WINDOW_HEIGHT			(600)
#define RENDER_TARGET_WIDTH		(320)
#define RENDER_TARGET_HEIGHT	(240)

class Texture {

	public:
		SDL_Texture* devtx = nullptr;

		Texture(int w, int h, SDL_Renderer* holder){
			devtx =  SDL_CreateTexture(holder, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
		}
		~Texture(){
			SDL_DestroyTexture(devtx);
		}

		void DrawRect(SDL_Renderer*	holder, const Rect<int>& rect, const Color& col){
				SDL_Rect dest;

				dest.x = rect.pos.x;
				dest.y = rect.pos.y;
				dest.w = rect.size.x;
				dest.h = rect.size.y;

				SDL_SetRenderTarget(holder, devtx);
				SDL_SetRenderDrawColor(holder, col.r * 255, col.g * 255, col.b * 255, col.a * 255);
				SDL_RenderFillRect(holder, &dest);
				SDL_SetRenderTarget(holder, nullptr);
		}

		void Project(SDL_Renderer*	holder, Texture* srs, const vec2<int>& pos){
				SDL_Rect dest;

				dest.x = pos.x;
				dest.y = pos.y;
				SDL_QueryTexture(srs->devtx, NULL, NULL, &dest.w, &dest.h);

				SDL_SetRenderTarget(holder, devtx);
				SDL_RenderCopy(holder, srs->devtx, NULL, &dest);
				SDL_SetRenderTarget(holder, nullptr);
		}

		void draw_tex_ro_rend(SDL_Renderer* holder, const vec2<int>& pos) {
			SDL_Rect dest;
			dest.x = pos.x;
			dest.y = pos.y;
			SDL_QueryTexture(devtx, NULL, NULL, &dest.w, &dest.h);

			SDL_RenderCopy(holder, devtx, NULL, &dest);
			SDL_RenderPresent(holder);
		}
};



int main(int argc, char *argv[]) {

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window*	holder_root = SDL_CreateWindow("holder", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 2000, 1200, SDL_WINDOW_SHOWN);
	SDL_Renderer* holder = SDL_CreateRenderer(holder_root, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);


	Texture tex(200, 200, holder);
	Texture tex2(100, 100, holder);
	tex.DrawRect(holder, Rect<int>(0, 0, 200, 200), Color(1, 1, 0, 1));
	tex2.DrawRect(holder, Rect<int>(0, 0, 100, 100), Color(0, 1, 0, 1));
	tex.Project(holder, &tex2, vec2<int>(0, 0));


	tex.draw_tex_ro_rend(holder, vec2<int>(50, 50));
	

	SDL_Delay(2000);
	SDL_DestroyRenderer(holder);
	SDL_DestroyWindow(holder_root);
	SDL_Quit();

	return 0;
}
