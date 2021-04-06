#pragma once 


class Device {

    friend class DevBuffer;

    private:
        class SDL_Window*	holder_root = nullptr;
	    class SDL_Renderer* holder = nullptr;

    public:
        Device();
        int GetKeyState(int ascii_code) {}
        ~Device();

};