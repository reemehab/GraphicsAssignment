#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <cmath>
#include <iostream>
using namespace std;
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Code::Blocks Template Windows App"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               544,                 /* The programs width */
               375,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

void draw_8_points(HDC hdc,int Xc, int Yc, int X, int Y )
{
    SetPixel(hdc,Xc+X,Yc+Y, RGB(0,0,0));
    SetPixel(hdc,Xc+X,Yc-Y, RGB(0,0,100));
    SetPixel(hdc,Xc-X,Yc+Y, RGB(0,100,0));
    SetPixel(hdc,Xc-X,Yc-Y, RGB(100,0,0));
///    SetPixel(hdc,Xc+Y,Yc+X, c);
    SetPixel(hdc,Xc+Y,Yc-X, RGB(0,0,0));
    SetPixel(hdc,Xc-Y,Yc+X, RGB(100,0,0));
    SetPixel(hdc,Xc-Y,Yc-X, RGB(0,0,100));
}

void BresenhamCircle(HDC hdc,int Xc, int Yc, int R)
{
    int X=0;
    int Y=R;
    draw_8_points(hdc,Xc,Yc,X,Y);
    int d=1-R;
    while(X<Y)
    {
        if(d<0) // point inside the Circle , x++
        {
            d+= 2*X +3;
            X++;
        }
        else // out the circle , x++ , y--
        {
            d+=2*(X-Y)+5;
            X++;
            Y--;
        }
        draw_8_points(hdc,Xc,Yc,X,Y);

    }
}

/*  This function is called by the Windows function DispatchMessage()  */
int index=1;
HDC hdc;
int Xc, Yc, Y1, Y2, X1, X2,R1,R2;

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    case WM_LBUTTONDOWN:
        hdc = GetDC(hwnd);
        if(index==1)
        {
            Xc= LOWORD(lParam);
            Yc=HIWORD(lParam);
            index++;
        }
        else if(index==2)
        {
            X1= LOWORD(lParam);
            Y1=HIWORD(lParam);
            R1= sqrt((X1-Xc)*(X1-Xc)+(Y1-Yc)*(Y1-Yc));
            BresenhamCircle(hdc,Xc,Yc,R1);
            index++;
        }
        else if(index ==3)
        {
            X2= LOWORD(lParam);
            Y2=HIWORD(lParam);
            R2= sqrt((X2-Xc)*(X2-Xc)+(Y2-Yc)*(Y2-Yc));
            BresenhamCircle(hdc,Xc,Yc,R2);
            index++;
            int Rmin=R1 , Rmax=R2 ;
            if(R1>R2)
            {
                Rmax=R1;
                Rmin=R2;
            }
            for(Rmin; Rmin<Rmax; Rmin++)
            {
                BresenhamCircle(hdc,Xc,Yc,Rmin);

            }
        }
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
