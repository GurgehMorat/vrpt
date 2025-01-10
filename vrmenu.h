#pragma once

#include "f4se/GameMenus.h"
#include "f4se/GameEvents.h"
#include "f4se/ScaleformCallbacks.h"
#include "f4se/ScaleformManager.h"

extern RelocPtr<BSScaleformManager*> g_scaleformManager;

class VRLevelUpMenu : public GameMenuBase {
public:
    VRLevelUpMenu();
    virtual ~VRLevelUpMenu();

    // GameMenuBase override
    virtual void RegisterFunctions() override;

    // Static creation method for menu registration
    static IMenu* Create() { return new VRLevelUpMenu(); }

private:
    struct BGSCodeObjInterface {
    public:
        struct PlaySoundHandler : public GFxFunctionHandler {
            virtual void Invoke(Args* args);
        };
        struct PlayPerkSoundHandler : public GFxFunctionHandler {
            virtual void Invoke(Args* args);
        };
        struct StopPerkSoundHandler : public GFxFunctionHandler {
            virtual void Invoke(Args* args);
        };
        struct SelectPerkHandler : public GFxFunctionHandler {
            virtual void Invoke(Args* args);
        };
        struct GetPerkInfoHandler : public GFxFunctionHandler {
            virtual void Invoke(Args* args);
        };
        struct CloseMenuHandler : public GFxFunctionHandler {
            virtual void Invoke(Args* args);
        };
        struct GetXPInfoHandler : public GFxFunctionHandler {
            virtual void Invoke(Args* args);
        };
        struct RegisterGridHandler : public GFxFunctionHandler {
            virtual void Invoke(Args* args);
        };
        struct GridAddedHandler : public GFxFunctionHandler {
            virtual void Invoke(Args* args);
        };
        struct VRConfirmationHandler : public GFxFunctionHandler {
            virtual void Invoke(Args* args);
        };
    };

    void SetupBGSCodeObj();
};