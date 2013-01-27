#include "mywindow.h"
#include "input.h"

std::map<HWND, WindowWizard*> WindowWizard::WindowMap;
int WindowWizard::cxSource;
int WindowWizard::cySource;

WindowWizard::WindowWizard(void)
{
}


WindowWizard::WindowWizard(LPSTR WindowName, int width, int height, HINSTANCE hInstance)
{
	WindowCreate(WindowName, width, height, (WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN), 0, hInstance);
}


WindowWizard::WindowWizard(LPSTR WindowName, int width, int height, HINSTANCE hInstance, bool FullScreen, DWORD Style)
{
	WindowCreate(WindowName, width, height, Style, FullScreen, hInstance);
}


WindowWizard::~WindowWizard(void)
{
	if (hrc)
	{
		wglMakeCurrent(NULL, NULL);	// free rendering memory
		wglDeleteContext(hrc);		// Delete our OpenGL Rendering Context
	}

	if (hdc) 
		ReleaseDC(handleToWindow, hdc);			// Release our HDC from memory

	UnregisterClass(wcex.lpszClassName, wcex.hInstance);// Free the window class

	PostQuitMessage (0);		// Post a QUIT message to the window
}

void WindowWizard::SetCursorToCentre()
{
	SetCursorPos(gWidth/2+windowRect.left,gHeight/2+windowRect.top);
}

void WindowWizard::InitializeGraphics(GLdouble fov)
{
	GetClientRect(handleToWindow, &graphicsRect);	//get rect into our handy global rect
	GetWindowRect(handleToWindow, &windowRect);
	InitializeOpenGL(fov);						// initialise openGL

	//OpenGL settings
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	//Also, do any other setting ov variables here for your app if you wish. 
	glEnable(GL_TEXTURE_2D);							//Enable texturing!
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	//specify texture calculation

	//Also, do any other setting ov variables here for your app if you wish. 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);// Linear Filtering
}


void WindowWizard::InitializeOpenGL(GLdouble fov)
{
    hdc = GetDC(handleToWindow);//  sets  global HDC
	fieldOfView = fov;

    if (!SetWindowPixelFormat(hdc))//  sets  pixel format
        PostQuitMessage (0);


    hrc = wglCreateContext(hdc);	//  creates  rendering context from  hdc
    wglMakeCurrent(hdc, hrc);		//	Use this HRC.

	ResizeGLWindow(graphicsRect.right, graphicsRect.bottom);	// Setup the Screen
}

void WindowWizard::FlipBuffers()
{
	SwapBuffers(hdc);
}

void WindowWizard::PrepareForDrawing()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// Clear The Screen And The Depth Buffer
}


void WindowWizard::OnResize(int width, int height)
{
	ResizeGLWindow(width, height);
	GetClientRect(handleToWindow, &graphicsRect);
	GetWindowRect(handleToWindow, &windowRect);
}

void WindowWizard::OnMove()
{
	GetWindowRect(handleToWindow, &windowRect);
}


void WindowWizard::ResizeGLWindow(int width, int height)// Initialize The GL Window
{
	gHeight = height;
	gWidth = width;
	if (height==0)// Prevent A Divide By Zero error
	{
		height=1;// Make the Height Equal One
	}

	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//calculate aspect ratio
	gluPerspective(fieldOfView,(GLfloat)width/(GLfloat)height, 1 ,1000.0f);

	glMatrixMode(GL_MODELVIEW);// Select The Modelview Matrix
	glLoadIdentity();// Reset The Modelview Matrix
}


bool WindowWizard::SetWindowPixelFormat(HDC hdc) 
{ 
    PIXELFORMATDESCRIPTOR pfd = {0}; 
    int pixelformat; 
 
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);	// Set the size of the structure
    pfd.nVersion = 1;							// Always set this to 1
	// Pass in the appropriate OpenGL flags
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
    pfd.dwLayerMask = PFD_MAIN_PLANE;			// standard mask (this is ignored anyway)
    pfd.iPixelType = PFD_TYPE_RGBA;				// RGB and Alpha pixel type
    pfd.cColorBits = COLOUR_DEPTH;				// Here we use our #define for the color bits
    pfd.cDepthBits = COLOUR_DEPTH;				// Ignored for RBA
    pfd.cAccumBits = 0;							// nothing for accumulation
    pfd.cStencilBits = 0;						// nothing for stencil
 
	//Gets a best match on the pixel format as passed in from device
    if ( (pixelformat = ChoosePixelFormat(hdc, &pfd)) == false ) 
    { 
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
        return false; 
    } 
 
	//sets the pixel format if its ok. 
    if (SetPixelFormat(hdc, pixelformat, &pfd) == false) 
    { 
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
        return false; 
    } 
 
    return true;
}


void WindowWizard::WindowCreate(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance)
{


	memset(&wcex, 0, sizeof(WNDCLASS));
	wcex.style			= CS_HREDRAW | CS_VREDRAW;		
	wcex.lpfnWndProc	= &WindowWizard::WndProc;		
	wcex.hInstance		= hInstance;						
	wcex.hIcon			= LoadIcon(NULL, IDI_APPLICATION);; 
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);		
	wcex.hbrBackground	= (HBRUSH) (COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;	
	wcex.lpszClassName	= strWindowName;	

	
	RegisterClass(&wcex);// Register the class

	//dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	//Set the Client area of the window to be our resolution.
	RECT glwindow;
	glwindow.left		= 0;		
	glwindow.right		= width;	
	glwindow.top		= 0;		
	glwindow.bottom		= height;	

	AdjustWindowRect( &glwindow, dwStyle, false);

	//Create the window
	handleToWindow = CreateWindow(	strWindowName, 
							strWindowName, 
							dwStyle, 
							0, 
							0,
							glwindow.right  - glwindow.left,
							glwindow.bottom - glwindow.top, 
							NULL,
							NULL,
							hInstance,
							NULL
							);

	if(!handleToWindow) {
		MessageBox(NULL, "Could Not Get Handle To Window", "Error", MB_OK); // If we couldn't get a handle, return NULL
		PostQuitMessage (0);
	}
	WindowMap[handleToWindow] = this;

	ShowWindow(handleToWindow, SW_SHOWNORMAL);	
	UpdateWindow(handleToWindow);					
	SetFocus(handleToWindow);

}

WindowWizard* WindowWizard::GetWindowReference(HWND hwnd)
{
	return WindowMap[hwnd];
}


LRESULT CALLBACK WindowWizard::WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_CREATE)
    {
		cxSource = GetSystemMetrics (SM_CXSIZEFRAME) + GetSystemMetrics (SM_CXSIZE);
		cySource = GetSystemMetrics (SM_CYSIZEFRAME) + GetSystemMetrics (SM_CYCAPTION);
    }
    else
    {
		WindowWizard* pWindow = WindowMap[hwnd];
		
        switch (message)
        {
            case WM_PAINT:
                break;
            case WM_COMMAND:
				break;
			case WM_SIZE:
				pWindow->OnResize(LOWORD(lParam),HIWORD(lParam));
			case WM_MOVE:
				GetWindowRect(hwnd,&pWindow->windowRect);
            case WM_LBUTTONUP:
            case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
			case WM_MOUSEWHEEL:
            case WM_MOUSEMOVE:
			case WM_KEYDOWN:
			case WM_KEYUP:
				input.Message(message, wParam, lParam);
				break;
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
        }
    }
    return DefWindowProc (hwnd, message, wParam, lParam) ;
}
