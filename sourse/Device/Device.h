#pragma once 


class DeviceManager {

    friend class DevBuffer;

    private:
        class SDL_Window*	holder_root = nullptr;
	    class SDL_Renderer* holder = nullptr;

    public:
        DeviceManager();
        ~DeviceManager();
        void Inputs(class UInputs* uinputs);
        void Output(class UIItem* UIroot);
};