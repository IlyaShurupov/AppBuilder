#pragma once 


class DeviceManager {
    public:
        DeviceManager();
        ~DeviceManager();
        void Inputs(class UInputs* uinputs);
        void Output(class UIItem* UIroot);
};