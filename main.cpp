#include "f4se/PluginAPI.h"
#include "f4se_common/f4se_version.h"
#include "f4se/GameMenus.h"
#include "f4se/GameEvents.h"
#include "vrmenu.h"

#include <ShlObj.h>
#include "version.h"

static PluginHandle g_pluginHandle = kPluginHandle_Invalid;
static F4SEMessagingInterface* g_messaging = nullptr;

void OnF4SEMessage(F4SEMessagingInterface::Message* msg) {
    if (!msg) return;

    switch (msg->type) {
        case F4SEMessagingInterface::kMessage_GameLoaded:
            _MESSAGE("\n=== Game Loaded - Initializing VaultGirlPerkTreeVR ===");
            if (*g_ui) {
                BSFixedString menuName("LevelUpMenu");
                
                // First unregister vanilla menu if it exists
                if ((*g_ui)->IsMenuRegistered(menuName)) {
                    _MESSAGE("Unregistering vanilla LevelUpMenu");
                    (*g_ui)->UnregisterMenu(menuName, true);
                }

                // Register our VR version
                _MESSAGE("Registering VR LevelUpMenu");
                (*g_ui)->Register("LevelUpMenu", VRLevelUpMenu::Create);
                _MESSAGE("Menu registration complete - Ready for perk selection");
            } else {
                _ERROR("UI system not available - Failed to register menu");
            }
            break;
    }
}

extern "C" {
    bool F4SEPlugin_Query(const F4SEInterface* f4se, PluginInfo* info) {
        gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Fallout4VR\\F4SE\\VaultGirlPerkTreeVR.log");
        
        info->infoVersion = PluginInfo::kInfoVersion;
        info->name = "VaultGirlPerkTreeVR";
        info->version = VGPTVR_VERSION_MAJOR;

        if (f4se->isEditor) {
            _FATALERROR("Editor is not supported");
            return false;
        }

        if (f4se->runtimeVersion < RUNTIME_VR_VERSION_1_2_72) {
            _FATALERROR("Unsupported runtime version %08X", f4se->runtimeVersion);
            return false;
        }

        return true;
    }

    bool F4SEPlugin_Load(const F4SEInterface* f4se) {
        _MESSAGE("VaultGirlPerkTreeVR loading");

        g_pluginHandle = f4se->GetPluginHandle();
        g_messaging = (F4SEMessagingInterface*)f4se->QueryInterface(kInterface_Messaging);

        if (g_messaging) {
            g_messaging->RegisterListener(g_pluginHandle, "F4SE", OnF4SEMessage);
            return true;
        }

        return false;
    }
};