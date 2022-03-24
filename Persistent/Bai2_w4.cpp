
#define _WIN32_DCOM

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <comdef.h>
//  Include the task header file.
#include <taskschd.h>
#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsupp.lib")

using namespace std;
HRESULT CreateCOM()
{
    //  Tạo COM
    HRESULT result = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(result))
    {
        printf("\nCoInitializeEx failed: %x", result);
        exit(3);
    }

    //  Set general COM security levels.
    result = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_PKT_PRIVACY, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, 0, NULL);

    if (FAILED(result))
    {
        printf("\nCoInitializeSecurity failed: %x", result);
        CoUninitialize();
        exit(3);
    }
    return result;
}
BSTR toBSTR(const char* str)
{
    int wslen = MultiByteToWideChar(CP_ACP, 0, str, strlen(str), 0, 0);
    BSTR bstr = SysAllocStringLen(0, wslen);
    MultiByteToWideChar(CP_ACP, 0, str, strlen(str), bstr, wslen);
    return bstr;
    // Use bstr here
    //SysFreeString(bstr);
}

int main()
{ 
    HRESULT hCOM = CreateCOM();
    const char* wstrExecutablePath = "C:\\Users\\ADMIN\\Desktop\\temp1.txt";
    ITaskService* pService = NULL;
    hCOM= CoCreateInstance(CLSID_TaskScheduler, NULL, CLSCTX_INPROC_SERVER, IID_ITaskService, (void**)&pService);
    hCOM = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
  
#pragma region BƯỚC 3: lấy Folder và tạo task trong đó 
    ITaskFolder* pRootFolder = NULL;
    hCOM = pService->GetFolder(_bstr_t(L"\\Microsoft\\Office"), &pRootFolder);   
    LPCWSTR wszTaskName = L"Logon Trigger Test Task";
    pRootFolder->DeleteTask(_bstr_t(wszTaskName), 0);  
    ITaskDefinition* pTask = NULL;
    hCOM = pService->NewTask(0, &pTask);
    pService->Release();  
#pragma endregion
#pragma region BƯỚC 4:get info and Setting
    IRegistrationInfo* pRegInfo = NULL;
    hCOM = pTask->get_RegistrationInfo(&pRegInfo);
    hCOM = pRegInfo->put_Author(toBSTR("Author Name"));
    pRegInfo->Release();

    ITaskSettings* pSettings = NULL;
    hCOM = pTask->get_Settings(&pSettings);
    hCOM = pSettings->put_StartWhenAvailable(VARIANT_TRUE);
    pSettings->Release();
#pragma endregion  
#pragma region BƯỚC 5:tạo trigger

    ITriggerCollection* pTriggerCollection = NULL;
    hCOM = pTask->get_Triggers(&pTriggerCollection);
    ITrigger* pTrigger = NULL;
    hCOM = pTriggerCollection->Create(TASK_TRIGGER_LOGON, &pTrigger);
    pTriggerCollection->Release();
    ILogonTrigger* pLogonTrigger = NULL;
    hCOM = pTrigger->QueryInterface(IID_ILogonTrigger, (void**)&pLogonTrigger);
    pTrigger->Release();
    hCOM = pLogonTrigger->put_Id(_bstr_t(L"Trigger1"));
    //  YYYY//MM/DD    1/1/2021 , 12:05
    hCOM = pLogonTrigger->put_StartBoundary(_bstr_t(L"2021-01-01T12:05:00"));
    hCOM = pLogonTrigger->put_EndBoundary(_bstr_t(L"2021-05-02T08:00:00"));
    hCOM = pLogonTrigger->put_UserId(_bstr_t(L"ADMIN"));
    pLogonTrigger->Release();

#pragma endregion
#pragma region BƯỚC 6: create action
   
    IActionCollection* pActionCollection = NULL;
    hCOM = pTask->get_Actions(&pActionCollection);
    IAction* pAction = NULL;
    hCOM = pActionCollection->Create(TASK_ACTION_EXEC, &pAction);
    pActionCollection->Release();
    IExecAction* pExecAction = NULL;
    hCOM = pAction->QueryInterface(IID_IExecAction, (void**)&pExecAction);
    pAction->Release();
    hCOM = pExecAction->put_Path(toBSTR(wstrExecutablePath));
    pExecAction->Release();
#pragma endregion  
#pragma region BƯỚC 7: Save the task in the root folder
    IRegisteredTask* pRegisteredTask = NULL;
    hCOM = pRootFolder->RegisterTaskDefinition(_bstr_t(wszTaskName), pTask, TASK_CREATE_OR_UPDATE, _variant_t(L"ADMIN"), _variant_t(), TASK_LOGON_GROUP, _variant_t(L""), &pRegisteredTask);
    printf("\n Success! Task successfully registered. ");

    // Clean up
    pRootFolder->Release();
    pTask->Release();
    pRegisteredTask->Release();
    CoUninitialize();
#pragma endregion   

   
    return 0;


}