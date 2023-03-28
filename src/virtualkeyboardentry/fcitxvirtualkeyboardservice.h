#ifndef FCITXVIRTUALKEYBOARDSERVICE_H
#define FCITXVIRTUALKEYBOARDSERVICE_H

class FcitxVirtualKeyboardService {
public:
    virtual ~FcitxVirtualKeyboardService() {}

    virtual void showVirtualKeyboard() const = 0;
    virtual void hideVirtualKeyboard() const = 0;

protected:
    FcitxVirtualKeyboardService() = default;
};

#endif // FCITXVIRTUALKEYBOARDSERVICE_H
