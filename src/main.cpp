#include <wchar.h>

#include "imgui_base_d3d9.cpp"

#include "fix.cpp"

enum {
    Status_Begin, 
    Status_ReadyToFix, 
    Status_FailRead, 
    Status_FailBackup, 
    Status_FailFix, 
    Status_Done, 
};

typedef struct {
    wchar_t sfPath[MAX_PATH];
    wchar_t mfPath[MAX_PATH];
    wchar_t bkPath[MAX_PATH];
    
    char *sfUTF8;
    char *mfUTF8;
    char *bkUTF8;
    
    int   sfUTF8Bytes;
    int   mfUTF8Bytes;
    int   bkUTF8Bytes;
    
    bool saveBackup;
    
    bool forceSizeNotMatch;
    bool forceCRC;
    
    validation_err lastVerification;
    
    u32 status;
} persistent_data;

static void
WideToUTF8(wchar_t *wide, char **buf, int *bufBytes){
    int zeroBuffer = 0;
    int bytes = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wide, -1, 0, 0, 0, 0);
    if(bytes){
        if(bytes > (*bufBytes)){
            mfree((void **)buf);
            *bufBytes = 0;
            
            *buf = (char *) malloc(bytes);
            if(*buf){
                *bufBytes = bytes;
            }
        }
        
        if(*buf){
            memset(*buf, 0, *bufBytes);
            if(WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wide, -1, *buf, bytes, 0, 0)){
                // NOTE(furkan): Successful, do nothing
            } else {
                // NOTE(furkan): Contents of *buf is unknown (i.e. docs say 
                // nothing about that case) when WideCharToMultiByte fails.
                // Make sure it is empty.
                zeroBuffer = 1;
            }
        }
    } else {
        zeroBuffer = 1;
    }
    
    if(zeroBuffer){
        if(*buf){
            memset(*buf, 0, *bufBytes);
        }
    }
}

static validation_err 
ValidationSuppressed(persistent_data *pd){
    validation_err verr = pd->lastVerification;
    
    if(verr & ValidationErr_FailSizeNotMatch){
        if(pd->forceSizeNotMatch){
            verr = (validation_err)(verr & ~(ValidationErr_FailSizeNotMatch));
        }
    }
    
    if(verr & ValidationErr_FailCRC){
        if(pd->forceCRC){
            verr = (validation_err)(verr & ~(ValidationErr_FailCRC));
        }
    }
    
    return verr;
}

static void
OnClickBrowse(persistent_data *pd){
    
    wchar_t ofdPath[MAX_PATH];
    ZeroMemory(ofdPath, sizeof(wchar_t)*MAX_PATH);
    
    OPENFILENAMEW OFDArgs;
    ZeroMemory(&OFDArgs, sizeof(OFDArgs));
    
    OFDArgs.lStructSize = sizeof (OPENFILENAMEW);
    OFDArgs.hwndOwner = GetActiveWindow();
    OFDArgs.lpstrFilter = L"2K11 My Player Save File (.CMG)\0*.CMG\0\0";
    OFDArgs.lpstrFile = ofdPath;
    OFDArgs.nMaxFile = MAX_PATH;
    OFDArgs.Flags = OFN_FILEMUSTEXIST;
    if(GetOpenFileNameW(&OFDArgs)){
        
        ZeroMemory(pd->sfPath, sizeof(wchar_t)*MAX_PATH);
        wmemcpy(pd->sfPath, ofdPath, MAX_PATH);
        
        ZeroMemory(pd->mfPath, sizeof(wchar_t)*MAX_PATH);
        wmemcpy(pd->mfPath, pd->sfPath, OFDArgs.nFileOffset);
        
        SYSTEMTIME SystemTime;
        GetLocalTime(&SystemTime);
            
        if(MAX_PATH - OFDArgs.nFileOffset > 15){
            swprintf(pd->mfPath+OFDArgs.nFileOffset, MAX_PATH-OFDArgs.nFileOffset, 
                    L"%d%d%d%d%d%d%d%d%d%d.CMG", 
                    (SystemTime.wMonth /10), (SystemTime.wMonth %10), 
                    (SystemTime.wDay   /10), (SystemTime.wDay   %10), 
                    (SystemTime.wHour  /10), (SystemTime.wHour  %10), 
                    (SystemTime.wMinute/10), (SystemTime.wMinute%10), 
                    (SystemTime.wSecond/10), (SystemTime.wSecond%10));
        }
        
        ZeroMemory(pd->bkPath, sizeof(wchar_t)*MAX_PATH);
        swprintf(pd->bkPath, 
                 OFDArgs.nFileExtension, 
                 L"%ls", pd->sfPath);
        swprintf(pd->bkPath+OFDArgs.nFileExtension-1, 
                 5, 
                 L".bak");
        
        WideToUTF8(pd->sfPath, &pd->sfUTF8, &pd->sfUTF8Bytes);
        WideToUTF8(pd->mfPath, &pd->mfUTF8, &pd->mfUTF8Bytes);
        WideToUTF8(pd->bkPath, &pd->bkUTF8, &pd->bkUTF8Bytes);
        
        pd->lastVerification = ValidateSaveFile(pd->sfPath);
        
        pd->status = Status_ReadyToFix;
        pd->forceSizeNotMatch = false;
        pd->forceCRC = false;
    }
}

static void
OnClickFixIt(persistent_data *pd){
    read_file_32 rf = ReadFile32(pd->sfPath);
    if(rf.err == 0){
        pd->lastVerification = ValidateSaveFile(rf.contents, rf.size);
        validation_err verr = ValidationSuppressed(pd);
        
        if(verr == ValidationErr_None){
            pd->forceSizeNotMatch = false;
            pd->forceCRC = false;
            
            bool cancel = false;
            if(pd->saveBackup){
                write_file_32 wf = WriteFile32(pd->bkPath, rf.contents, rf.size);
                if(wf.err){
                    cancel = true;
                    pd->status = Status_FailBackup;
                }
            }
            
            if(cancel == false){
                fix_result fr = FixSaveFile(rf.contents, rf.size, pd->mfPath);
                if(fr.err == 0){
                    pd->status = Status_Done;
                } else {
                    pd->status = Status_FailFix;
                }
            }
        }
    } else {
        pd->status = Status_FailRead;
    }
}

static void
ShowMainContent(persistent_data *pd){
    
    if(ImGui::Button("Browse")){
        OnClickBrowse(pd);
    }
    
    switch(pd->status){
        case Status_ReadyToFix:{
            
            ImGui::SameLine();
            ImGui::Checkbox("Save Backup File?", &pd->saveBackup);
            
#define __ImGuiTextIfNotNull(fmt, str)   \
            if(str){                     \
                ImGui::Text(fmt, str);   \
            } else {                     \
                ImGui::Text(fmt, "???"); \
            }
            
            __ImGuiTextIfNotNull("Input  path: %s", pd->sfUTF8)
            __ImGuiTextIfNotNull("Output path: %s", pd->mfUTF8);
            __ImGuiTextIfNotNull("Backup path: %s", pd->bkUTF8);
        
#undef __ImGuiTextIfNotNull
            
            validation_err verr = ValidationSuppressed(pd);
            
            if(verr != ValidationErr_None){
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
            }
            
            if(ImGui::Button("Fix It!")){
                OnClickFixIt(pd);
            }
            
            if(verr != ValidationErr_None){
                ImGui::PopItemFlag();
                ImGui::PopStyleVar();
            }
            
            if(verr & ValidationErr_FailRead){
                ImGui::TextWrapped("Input file could not be read");
            }
            
            if(verr & ValidationErr_FailSmallSize){
                ImGui::TextWrapped("Input file is too small. Sorry, we cannot fix that");
            }
            
            if(verr & ValidationErr_FailSizeNotMatch){
                ImGui::TextWrapped("Input file size does not match with the expectation and fix may not work. Do you want to continue?");
                if(ImGui::Button("Ignore file size error")){
                    pd->forceSizeNotMatch = true;
                }
            }
            
            if(verr & ValidationErr_FailCRC){
                ImGui::TextWrapped("CRC check failed. Input file may be corrupted. Do you want to continue?");
                if(ImGui::Button("Ignore CRC failure")){
                    pd->forceCRC = true;
                }
            }
        } break;
    }
}

static void
ShowStatusText(persistent_data *pd){
    switch(pd->status){
        case Status_Begin:{
            ImGui::TextWrapped("Choose your save file");
        } break;
        case Status_ReadyToFix:{
            ImGui::TextWrapped("Check paths and click \"Fix It\"");
        } break;
        case Status_FailRead:{
            ImGui::TextWrapped("Failed to read input file. Wrong path?");
        } break;
        case Status_FailBackup:{
            ImGui::TextWrapped("Failed to copy a backup, cancelled");
        } break;
        case Status_FailFix:{
            ImGui::TextWrapped("Failure");
        } break;
        case Status_Done:{
            ImGui::TextWrapped("Done");
        } break;
        default:{
            ImGui::TextWrapped("Unknown status (%d)", pd->status);
        } break;
    }
}

static void 
FrameUpdate(frame_params *frameParams){
    
    if(frameParams->userData == 0){
        frameParams->userData = (persistent_data *) malloc( sizeof(persistent_data) );
        
        if(frameParams->userData){
            persistent_data *pd = (persistent_data *) frameParams->userData;
            ZeroMemory(pd, sizeof(persistent_data));
            
            pd->saveBackup = true;
            pd->status = Status_Begin;
        }
    }
    
    RECT windowRect;
    if(GetWindowRect(frameParams->windowHandle, &windowRect)){
        ImGui::SetNextWindowPos (ImVec2(0, 0), 0, ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImVec2(windowRect.right-windowRect.left, 
                                        windowRect.bottom-windowRect.top), 
                                 0);
        
        if(ImGui::Begin("untitled", 0, ImGuiWindowFlags_NoDecoration)){
            
            if(ImGui::Button("Quit")){
                frameParams->quitRequested = true;
            }
            
            ImGui::Dummy(ImVec2(0.0f, 20.0f));
            
            if(frameParams->userData){
                persistent_data *pd = (persistent_data *) frameParams->userData;
                
                ShowStatusText(pd);
                ShowMainContent(pd);
                
            } else {
                ImGui::TextWrapped("Unable to start the application");
            }
            
            ImGui::End();
        } else {
            ImGui::End();
        }
    }
}