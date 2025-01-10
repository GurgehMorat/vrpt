#include "vrmenu.h"

VRLevelUpMenu::VRLevelUpMenu() {
    _MESSAGE("\n=== VRLevelUpMenu Constructor ===");

    menuName = BSFixedString("LevelUpMenu");
    flags = 0x040C8049;  // Same flags as vanilla LevelUpMenu
    depth = 1;

    _MESSAGE("Menu properties - Name: %s, Flags: %08X, Depth: %d",
             menuName.c_str(), flags, depth);

    if (!*g_scaleformManager) {
        _ERROR("No Scaleform manager available!");
        return;
    }

    _MESSAGE("Loading movie...");
    CALL_MEMBER_FN(*g_scaleformManager, LoadMovie)(this, movie, menuName.c_str(), "root1", 0);
    
    if (!movie) {
        _ERROR("Failed to load movie!");
        return;
    }
    _MESSAGE("Movie loaded: %p", movie);

    if (!movie->movieRoot) {
        _ERROR("No movie root!");
        return;
    }
    _MESSAGE("Movie root: %p", movie->movieRoot);

    GFxValue root;
    if (!movie->movieRoot->GetVariable(&root, "root1")) {
        _ERROR("Failed to get root1!");
        return;
    }
    _MESSAGE("Got root1 variable");

    SetupBGSCodeObj();
    _MESSAGE("Menu initialization complete");
}

VRLevelUpMenu::~VRLevelUpMenu() {
    _MESSAGE("VRLevelUpMenu destructor");
}

void VRLevelUpMenu::RegisterFunctions() {
    _MESSAGE("RegisterFunctions called");
}

void VRLevelUpMenu::SetupBGSCodeObj() {
    _MESSAGE("\n=== Setting up BGSCodeObj ===");

    if (!movie || !movie->movieRoot) {
        _ERROR("Invalid movie state during BGSCodeObj setup");
        return;
    }

    GFxValue root;
    if (!movie->movieRoot->GetVariable(&root, "root1")) {
        _ERROR("Failed to get root1 for BGSCodeObj setup");
        return;
    }
    _MESSAGE("Got root1 for BGSCodeObj setup");

    // Create BGSCodeObj
    GFxValue codeObj;
    movie->movieRoot->CreateObject(&codeObj);
    _MESSAGE("Created BGSCodeObj");

    _MESSAGE("Registering callback functions:");
    RegisterFunction<BGSCodeObjInterface::PlaySoundHandler>(&codeObj, movie->movieRoot, "PlaySound");
    _MESSAGE("  Registered: PlaySound");
    RegisterFunction<BGSCodeObjInterface::PlayPerkSoundHandler>(&codeObj, movie->movieRoot, "PlayPerkSound");
    _MESSAGE("  Registered: PlayPerkSound");
    RegisterFunction<BGSCodeObjInterface::StopPerkSoundHandler>(&codeObj, movie->movieRoot, "StopPerkSound");
    _MESSAGE("  Registered: StopPerkSound");
    RegisterFunction<BGSCodeObjInterface::SelectPerkHandler>(&codeObj, movie->movieRoot, "SelectPerk");
    _MESSAGE("  Registered: SelectPerk");
    RegisterFunction<BGSCodeObjInterface::GetPerkInfoHandler>(&codeObj, movie->movieRoot, "GetPerkInfoByRank");
    _MESSAGE("  Registered: GetPerkInfoByRank");
    RegisterFunction<BGSCodeObjInterface::CloseMenuHandler>(&codeObj, movie->movieRoot, "CloseMenu");
    _MESSAGE("  Registered: CloseMenu");
    RegisterFunction<BGSCodeObjInterface::GetXPInfoHandler>(&codeObj, movie->movieRoot, "GetXPInfo");
    _MESSAGE("  Registered: GetXPInfo");
    RegisterFunction<BGSCodeObjInterface::RegisterGridHandler>(&codeObj, movie->movieRoot, "RegisterPerkGridComponents");
    _MESSAGE("  Registered: RegisterPerkGridComponents");
    RegisterFunction<BGSCodeObjInterface::GridAddedHandler>(&codeObj, movie->movieRoot, "onGridAddedToStage");
    _MESSAGE("  Registered: onGridAddedToStage");
    RegisterFunction<BGSCodeObjInterface::VRConfirmationHandler>(&codeObj, movie->movieRoot, "OnVRConfimationStart");
    _MESSAGE("  Registered: OnVRConfimationStart");

    // Set BGSCodeObj on root
    root.SetMember("BGSCodeObj", &codeObj);
    _MESSAGE("Attached BGSCodeObj to root");

    // Initialize ActionScript side
    GFxValue retVal;
    root.Invoke("onCodeObjCreate", &retVal, nullptr, 0);
    _MESSAGE("Called ActionScript onCodeObjCreate");
    
    _MESSAGE("BGSCodeObj setup complete");
}

// BGSCodeObj Interface Implementations
void VRLevelUpMenu::BGSCodeObjInterface::PlaySoundHandler::Invoke(Args* args) {
    _MESSAGE("\n=== PlaySound Called ===");
    _MESSAGE("Args count: %d", args->numArgs);
    
    if (args->numArgs > 0) {
        _MESSAGE("Arg type: %d", args->args[0].GetType());
        if (args->args[0].GetType() == GFxValue::kType_String) {
            const char* soundID = args->args[0].GetString();
            _MESSAGE("Playing sound: %s", soundID);
            // TODO: Implement actual sound playing
        } else {
            _ERROR("Invalid argument type for sound ID");
        }
    } else {
        _ERROR("No sound ID provided");
    }
}

void VRLevelUpMenu::BGSCodeObjInterface::PlayPerkSoundHandler::Invoke(Args* args) {
    _MESSAGE("\n=== PlayPerkSound Called ===");
    _MESSAGE("Args count: %d", args->numArgs);
    
    if (args->numArgs > 0) {
        _MESSAGE("Arg type: %d", args->args[0].GetType());
        if (args->args[0].GetType() == GFxValue::kType_String) {
            const char* clipName = args->args[0].GetString();
            _MESSAGE("Playing perk sound for: %s", clipName);
            // TODO: Implement perk sound playing
        } else {
            _ERROR("Invalid argument type for perk clip name");
        }
    } else {
        _ERROR("No perk clip name provided");
    }
}

void VRLevelUpMenu::BGSCodeObjInterface::StopPerkSoundHandler::Invoke(Args* args) {
    _MESSAGE("\n=== StopPerkSound Called ===");
    // TODO: Implement sound stopping
}

void VRLevelUpMenu::BGSCodeObjInterface::SelectPerkHandler::Invoke(Args* args) {
    _MESSAGE("\n=== SelectPerk Called ===");
    _MESSAGE("Args count: %d", args->numArgs);
    
    if (args->numArgs > 1) {
        _MESSAGE("Arg types: %d, %d", args->args[0].GetType(), args->args[1].GetType());
        if (args->args[0].GetType() == GFxValue::kType_String &&
            args->args[1].GetType() == GFxValue::kType_Int) {
            const char* clipName = args->args[0].GetString();
            int rank = args->args[1].GetInt();
            _MESSAGE("Selecting perk: %s, rank %d", clipName, rank);
            // TODO: Implement perk selection
        } else {
            _ERROR("Invalid argument types for perk selection");
        }
    } else {
        _ERROR("Insufficient arguments for perk selection");
    }
}

void VRLevelUpMenu::BGSCodeObjInterface::GetPerkInfoHandler::Invoke(Args* args) {
    _MESSAGE("\n=== GetPerkInfo Called ===");
    _MESSAGE("Args count: %d", args->numArgs);
    
    if (args->numArgs > 1) {
        _MESSAGE("Arg types: %d, %d", args->args[0].GetType(), args->args[1].GetType());
        if (args->args[0].GetType() == GFxValue::kType_String &&
            args->args[1].GetType() == GFxValue::kType_Int) {
            const char* clipName = args->args[0].GetString();
            int rank = args->args[1].GetInt();
            _MESSAGE("Getting info for perk: %s, rank %d", clipName, rank);
            
            // TODO: Return actual perk info
            const char* description = "Test Perk Description\nRequirements: Level 1";
            args->movie->movieRoot->CreateString(args->result, description);
            _MESSAGE("Returned description: %s", description);
        } else {
            _ERROR("Invalid argument types for perk info request");
        }
    } else {
        _ERROR("Insufficient arguments for perk info request");
    }
}

void VRLevelUpMenu::BGSCodeObjInterface::CloseMenuHandler::Invoke(Args* args) {
    _MESSAGE("\n=== CloseMenu Called ===");
    BSFixedString menuName("LevelUpMenu");
    if (*g_uiMessageManager) {
        CALL_MEMBER_FN(*g_uiMessageManager, SendUIMessage)(&menuName, kMessage_Close);
        _MESSAGE("Close message sent to UI manager");
    } else {
        _ERROR("UI message manager not available");
    }
}

void VRLevelUpMenu::BGSCodeObjInterface::GetXPInfoHandler::Invoke(Args* args) {
    _MESSAGE("\n=== GetXPInfo Called ===");
    _MESSAGE("Args count: %d", args->numArgs);
    
    if (args->numArgs > 0 && args->args[0].IsObject()) {
        // TODO: Get actual player XP info
        args->args[0].SetMember("level", &GFxValue(1));
        args->args[0].SetMember("currXP", &GFxValue(0));
        args->args[0].SetMember("maxXP", &GFxValue(100));
        
        // Verify the values were set
        GFxValue verify;
        if (args->args[0].GetMember("level", &verify))
            _MESSAGE("Set level: %d", verify.GetInt());
        if (args->args[0].GetMember("currXP", &verify))
            _MESSAGE("Set currXP: %d", verify.GetInt());
        if (args->args[0].GetMember("maxXP", &verify))
            _MESSAGE("Set maxXP: %d", verify.GetInt());
    } else {
        _ERROR("Invalid or missing object for XP info");
    }
}

void VRLevelUpMenu::BGSCodeObjInterface::RegisterGridHandler::Invoke(Args* args) {
    _MESSAGE("\n=== RegisterPerkGridComponents Called ===");
    _MESSAGE("Args count: %d", args->numArgs);
    
    if (args->numArgs > 0 && args->args[0].IsObject()) {
        _MESSAGE("Got valid grid object");
        // Log initial state
        GFxValue value;
        if (args->args[0].GetMember("visible", &value))
            _MESSAGE("Grid initial visibility: %s", value.GetBool() ? "true" : "false");
        // TODO: Additional grid setup if needed
    } else {
        _ERROR("Invalid or missing grid object");
    }
}

void VRLevelUpMenu::BGSCodeObjInterface::GridAddedHandler::Invoke(Args* args) {
    _MESSAGE("\n=== GridAddedToStage Called ===");
    _MESSAGE("Grid initialization can proceed");
}

void VRLevelUpMenu::BGSCodeObjInterface::VRConfirmationHandler::Invoke(Args* args) {
    _MESSAGE("\n=== VRConfirmation Called ===");
    _MESSAGE("Args count: %d", args->numArgs);
    
    if (args->numArgs > 0) {
        _MESSAGE("Arg type: %d", args->args[0].GetType());
        if (args->args[0].GetType() == GFxValue::kType_Bool) {
            bool state = args->args[0].GetBool();
            _MESSAGE("VR confirmation state: %s", state ? "Starting" : "Ending");
            // TODO: Handle VR confirmation state
        } else {
            _ERROR("Invalid argument type for VR confirmation");
        }
    } else {
        _ERROR("No state provided for VR confirmation");
    }
}