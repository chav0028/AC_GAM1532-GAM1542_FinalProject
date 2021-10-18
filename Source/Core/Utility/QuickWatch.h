#ifndef __QuickWatch_H__
#define __QuickWatch_H__

#include <commctrl.h>

#define MAX_QUICKWATCH_VARIABLES    40
const int QUICKWATCH_DEFAULT_WIDTH = 400;//original size of 300

class Vector2;

class QuickWatchManager;

extern QuickWatchManager* g_pQuickWatchManager;

class QuickWatchManager
{
public:
    HWND m_WindowHandle;
    FARPROC m_DefaultProcSlider;
    FARPROC m_DefaultProcEditBox;
    HWND* m_EditBoxHandles;
    HWND* m_SliderHandles;

    bool m_AllowWindowToBeUpdated;

    int m_NumVariables;
    int m_SliderBeingDragged;

    void** m_pVariablePointers;
    Vector2* m_pVariableRanges;

protected:
    void CreateWatchWindow();
    void AddControl(const char* name);

public:
    void Update(int controltoupdate = -1);
    void OnEditBoxChanged(WORD controlid);
    void OnSliderChanged(WORD controlid);

public:
    QuickWatchManager();
    ~QuickWatchManager();

    //void AddInt(const char* name, int* pInt, int min, int max);
    void AddFloat(const char* name, float* pFloat, float min, float max);
    void AddSpace();
};

#endif // __QuickWatch_H__
