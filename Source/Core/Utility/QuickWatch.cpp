#include "CommonHeader.h"
#include "QuickWatch.h"

LRESULT WINAPI QuickWatchWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI QuickWatchWindowProcSlider(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI QuickWatchWindowProcEditBox(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

QuickWatchManager* g_pQuickWatchManager = 0;

QuickWatchManager::QuickWatchManager()
{
    m_WindowHandle = 0;
    m_DefaultProcSlider = 0;
    m_DefaultProcEditBox = 0;

    m_AllowWindowToBeUpdated = true;

    m_NumVariables = 0;
    m_SliderBeingDragged = -1;

    m_EditBoxHandles = new HWND[MAX_QUICKWATCH_VARIABLES];
    m_SliderHandles = new HWND[MAX_QUICKWATCH_VARIABLES];
    m_pVariablePointers = new void*[MAX_QUICKWATCH_VARIABLES];
    m_pVariableRanges = new Vector2[MAX_QUICKWATCH_VARIABLES];

    CreateWatchWindow();
}

QuickWatchManager::~QuickWatchManager()
{
    delete[] ( m_pVariablePointers );
    delete[] ( m_EditBoxHandles );
    delete[] ( m_SliderHandles );
    delete[] ( m_pVariableRanges );
}

void QuickWatchManager::CreateWatchWindow()
{
    WNDCLASSEX wc;
    ATOM atmWndClass;
    
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_OWNDC;
    wc.lpfnWndProc   = QuickWatchWindowProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = NULL;
    wc.hIcon         = NULL;
    wc.hCursor       = NULL;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wc.lpszMenuName  = "";
    wc.lpszClassName = "QuickWatch"; 
    wc.hIconSm       = NULL;
    
    atmWndClass = RegisterClassEx(&wc);

    int w = QUICKWATCH_DEFAULT_WIDTH;//MODIFICATION TO SOURCE CODE-SIZE ORIGINALLY 300
    int h = 1000;
    int x = 1366 - w;
    int y = 0;

    m_WindowHandle = CreateWindow( "QuickWatch", "QuickWatch", WS_EX_MDICHILD | WS_VISIBLE, x, y, w, h, 0, 0, 0, 0 );
    
    assert( m_WindowHandle != 0 );
    if( m_WindowHandle == 0 )
        return;

	SetTimer( m_WindowHandle, 0, 1000/10, 0 );

    return;
}

//void QuickWatchManager::AddInt(const char* name, int* pInt, int min, int max)
//{
//    //AddControl( name );
//
//    //m_pVariablePointers[m_NumVariables] = pInt;
//
//    //m_NumVariables++;
//
//    //g_pQuickWatchManager->Update();
//}

void QuickWatchManager::AddFloat(const char* name, float* pFloat, float min, float max)
{
    assert( m_NumVariables < MAX_QUICKWATCH_VARIABLES );
    if( m_NumVariables >= MAX_QUICKWATCH_VARIABLES )
        return;

    AddControl( name );

    m_pVariablePointers[m_NumVariables] = pFloat;
    m_pVariableRanges[m_NumVariables].Set( min, max );

    m_NumVariables++;

    g_pQuickWatchManager->Update();
}

void QuickWatchManager::AddSpace()
{
    assert( m_NumVariables < MAX_QUICKWATCH_VARIABLES );
    if( m_NumVariables >= MAX_QUICKWATCH_VARIABLES )
        return;

    m_pVariablePointers[m_NumVariables] = 0;

    m_NumVariables++;
}

void QuickWatchManager::AddControl(const char* name)
{
    assert( m_WindowHandle != 0 );
    if( m_WindowHandle == 0 )
        return;

    int PaddingTop = 5;
    int PaddingBottom = 5;
    int PaddingLeft = 2;

    int ControlHeight = PaddingTop + 20 + PaddingBottom;

    int PosX = PaddingLeft;
    int PosY = m_NumVariables*ControlHeight;
    int WindowWidth = QUICKWATCH_DEFAULT_WIDTH;

    int TextHeight = 20;
    int EditBoxHeight = 20;
    int SliderHeight = 20;

    int TextWidth = 200;//100
    int SliderWidth = 120;//120
    int EditBoxWidth = 70;//70

    // create a font
    HFONT font = CreateFont( 15, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, "Arial" );

    if( m_NumVariables )
    {
        CreateWindow( "STATIC", "", WS_CHILD | WS_VISIBLE | SS_BLACKRECT,
                      0, PosY, WindowWidth, 1,
                      m_WindowHandle, (HMENU)m_NumVariables, 0, 0 );
    }

    PosY += PaddingTop;

    // Text label
    {
        HWND controlhandle = CreateWindow( "STATIC", name, WS_CHILD | WS_VISIBLE,
                                           PosX, PosY, TextWidth, TextHeight,
                                           m_WindowHandle, (HMENU)m_NumVariables, 0, 0 );

        assert( controlhandle != 0 );
        if( controlhandle == 0 )
            return;

        SendMessage( controlhandle, WM_SETFONT, (WPARAM)font, MAKELPARAM(FALSE, 0) );

        PosX += TextWidth;
    }

    // Slider
    {
        HWND controlhandle = CreateWindowEx( 0, TRACKBAR_CLASS, "",
            WS_CHILD | WS_VISIBLE | TBS_NOTICKS | WS_TABSTOP,
            PosX, PosY, SliderWidth, SliderHeight,
            m_WindowHandle, (HMENU)m_NumVariables, 0, 0 );

        assert( controlhandle != 0 );
        if( controlhandle == 0 )
            return;

        m_SliderHandles[m_NumVariables] = controlhandle;

        // Set up the slider's message handler.
        m_DefaultProcSlider = (FARPROC)SetWindowLong( controlhandle, GWL_WNDPROC, (DWORD)QuickWatchWindowProcSlider );

        PosX += SliderWidth;
    }

    {
        // Add an edit box
        HWND controlhandle = CreateWindow( "EDIT", "", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
                                           PosX, PosY, EditBoxWidth, EditBoxHeight,
                                           m_WindowHandle, (HMENU)m_NumVariables, 0, 0 );

        assert( controlhandle != 0 );
        if( controlhandle == 0 )
            return;

        m_EditBoxHandles[m_NumVariables] = controlhandle;

        // Set up the edit box's message handler.
        m_DefaultProcEditBox = (FARPROC)SetWindowLong( controlhandle, GWL_WNDPROC, (DWORD)QuickWatchWindowProcEditBox );

        SendMessage( controlhandle, WM_SETFONT, (WPARAM)font, MAKELPARAM(FALSE, 0) );
    }
}

LRESULT WINAPI QuickWatchWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if( uMsg == WM_TIMER )
    {
        if( g_pQuickWatchManager->m_AllowWindowToBeUpdated == true )
            g_pQuickWatchManager->Update();
    }
    
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT WINAPI QuickWatchWindowProcSlider(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WORD id = (WORD)GetMenu( hWnd );

    if( uMsg == WM_SETFOCUS )
    {
        g_pQuickWatchManager->m_AllowWindowToBeUpdated = false;
        g_pQuickWatchManager->m_SliderBeingDragged = id;
    }

    if( uMsg == WM_KILLFOCUS )
    {
        g_pQuickWatchManager->m_AllowWindowToBeUpdated = true;
        g_pQuickWatchManager->m_SliderBeingDragged = -1;
        g_pQuickWatchManager->OnSliderChanged( id );
    }
    
    if( uMsg == WM_MOUSEMOVE )
    {
        if( g_pQuickWatchManager->m_SliderBeingDragged != -1 )
        {
            g_pQuickWatchManager->OnSliderChanged( id );
        }
    }

    return CallWindowProc( (WNDPROC)g_pQuickWatchManager->m_DefaultProcSlider, hWnd, uMsg, wParam, lParam);
}

LRESULT WINAPI QuickWatchWindowProcEditBox(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WORD id = (WORD)GetMenu( hWnd );

    if( uMsg == WM_SETFOCUS )
    {
        g_pQuickWatchManager->m_AllowWindowToBeUpdated = false;
    }

	if( uMsg == WM_KEYDOWN )
    {
		if( wParam == VK_RETURN )
		{
            g_pQuickWatchManager->OnEditBoxChanged( id );
        }
    }

    if( uMsg == WM_KILLFOCUS )
    {
        g_pQuickWatchManager->m_AllowWindowToBeUpdated = true;
    }

    return CallWindowProc( (WNDPROC)g_pQuickWatchManager->m_DefaultProcEditBox, hWnd, uMsg, wParam, lParam);
}

void QuickWatchManager::OnEditBoxChanged(WORD controlid)
{
    char tempstring[50];
    GetWindowText( m_EditBoxHandles[controlid], tempstring, 10 );
    tempstring[9] = 0;

    float value = (float)atof( tempstring );
    *((float*)m_pVariablePointers[controlid]) = value;

    Update( controlid );
}

void QuickWatchManager::OnSliderChanged(WORD controlid)
{
    int perc = SendMessage( m_SliderHandles[controlid], TBM_GETPOS, 0, 0 );

    float range = m_pVariableRanges[controlid].y - m_pVariableRanges[controlid].x;
    float value = range * (perc / 100.0f) + m_pVariableRanges[controlid].x;
    *((float*)m_pVariablePointers[controlid]) = value;

    Update( controlid );
}

void QuickWatchManager::Update(int controltoupdate)
{
    for( int i=0; i<m_NumVariables; i++ )
    {
        if( m_pVariablePointers[i] == 0 )
            continue;

        if( controltoupdate != -1 && controltoupdate != i )
            continue;

        char tempstring[50];

        float value = *(float*)m_pVariablePointers[i];
        sprintf_s( tempstring, 50, "%0.2f", value );

        float temp = value - m_pVariableRanges[i].x;
        float range = m_pVariableRanges[i].y - m_pVariableRanges[i].x;
                
        int perc = (int)( (temp/range) * 100.0f );
        if( perc < 0 ) perc = 0;
        if( perc > 100 ) perc = 100;

        SendMessage( m_EditBoxHandles[i], WM_SETTEXT, 0, (LPARAM)tempstring );
        SendMessage( m_SliderHandles[i], TBM_SETPOS, 1, (LPARAM)perc );
    }
}
