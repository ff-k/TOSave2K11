#include "imgui_base_d3d9.cpp"
#include <wchar.h>

#include "fix.cpp"

enum {
    Status_None = 0,  
    Status_Browse, 
    Status_AboutToFix, 
    Status_BackupFailure, 
    Status_Failure, 
    Status_Done, 
};

static void 
FrameUpdate(frame_params *frameParams){
    
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
            
            static u32 status = Status_Browse;
            switch(status){
                case Status_None:{
                    ImGui::TextWrapped("");
                } break;
                case Status_Browse:{
                    ImGui::TextWrapped("Choose your save file");
                } break;
                case Status_AboutToFix:{
                    ImGui::TextWrapped("Check paths and click \"Fix It\"");
                } break;
                case Status_BackupFailure:{
                    ImGui::TextWrapped("Backup failed, cancelled");
                } break;
                case Status_Failure:{
                    ImGui::TextWrapped("Failure");
                } break;
                case Status_Done:{
                    ImGui::TextWrapped("Done");
                } break;
                default:{
                    ImGui::TextWrapped("Unknown status (%d)", status);
                } break;
            }
            
            static wchar_t sfPath[MAX_PATH];
            static wchar_t mfPath[MAX_PATH];
            static wchar_t bkPath[MAX_PATH];
            static OPENFILENAMEW OFDArgs;
            
            static bool sfAvailable = false;
            
            if(ImGui::Button("Browse")){
                
                ZeroMemory(&sfPath, sizeof(wchar_t)*MAX_PATH);
                ZeroMemory(&OFDArgs, sizeof(OFDArgs));
                
                status = 0;
                sfAvailable = false;
                
                OFDArgs.lStructSize = sizeof (OPENFILENAMEW);
                OFDArgs.hwndOwner = GetActiveWindow();
                OFDArgs.lpstrFilter = L"2K11 My Player Save File (.CMG)\0*.CMG\0\0";
                OFDArgs.lpstrFile = sfPath;
                OFDArgs.nMaxFile = MAX_PATH;
                OFDArgs.Flags = OFN_FILEMUSTEXIST;
                if(GetOpenFileNameW(&OFDArgs)){
                    
                    status = Status_AboutToFix;
                    
                    sfAvailable = true;
                    
                    ZeroMemory(&mfPath, sizeof(wchar_t)*MAX_PATH);
                    wmemcpy(mfPath, sfPath, OFDArgs.nFileOffset);
                    
                    SYSTEMTIME SystemTime;
                    GetLocalTime(&SystemTime);
                        
                    if(MAX_PATH - OFDArgs.nFileOffset > 15){
                        swprintf(mfPath+OFDArgs.nFileOffset, MAX_PATH-OFDArgs.nFileOffset, 
                                L"%d%d%d%d%d%d%d%d%d%d.CMG", 
                                (SystemTime.wMonth /10), (SystemTime.wMonth %10), 
                                (SystemTime.wDay   /10), (SystemTime.wDay   %10), 
                                (SystemTime.wHour  /10), (SystemTime.wHour  %10), 
                                (SystemTime.wMinute/10), (SystemTime.wMinute%10), 
                                (SystemTime.wSecond/10), (SystemTime.wSecond%10));
                    }
                    
                    ZeroMemory(&bkPath, sizeof(wchar_t)*MAX_PATH);
                    swprintf(bkPath, OFDArgs.nFileExtension, L"%ls", sfPath);
                    swprintf(bkPath+OFDArgs.nFileExtension-1, 5, L".bak");
                }
            }
            
            static bool saveBackup = true;
            
            if(sfAvailable){
                
                ImGui::SameLine();
                ImGui::Checkbox("Save Backup File?", &saveBackup);
                
                ImGui::Text("Input  path: %ls", sfPath);
                ImGui::Text("Output path: %ls", mfPath);
                ImGui::Text("Backup path: %ls", bkPath);
                
                if(ImGui::Button("Fix It!")){
                    
                    read_file_32 rf = ReadFile32(sfPath);
                    if(rf.err == 0){
                        bool cancel = false;
                        if(saveBackup){
                            write_file_32 wf = WriteFile32(bkPath, rf.contents, rf.size);
                            if(wf.err){
                                cancel = true;
                                status = Status_BackupFailure;
                            }
                        }
                        
                        if(cancel == false){
                            fix_result fr = FixSaveFile(rf.contents, rf.size, mfPath);
                            if(fr.err == 0){
                                status = Status_Done;
                                sfAvailable = false;
                            } else {
                                status = Status_Failure;
                            }
                        }
                    }
                }
            }
                        
            ImGui::End();
        } else {
            ImGui::End();
        }
    }
}