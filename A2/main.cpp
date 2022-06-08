#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <cmath>
#include<iostream>
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


/*  This function is called by the Windows function DispatchMessage()  */
int Round(double x)
{
    return (int)(x + 0.5);
}
struct point
{
    double x,y;

};
double calcDistance(double x1, double y1, double x2, double y2)
{
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}
void DrawHermiteCurve(HDC hdc,point& p1, point& T1, point& p2, point& T2,int R, int xc, int yc)
{
    COLORREF c;
    double alpha0 = p1.x,
           alpha1 = T1.x,
           alpha2 = -3 * p1.x - 2 * T1.x + 3 * p2.x - T2.x,
           alpha3 = 2 * p1.x + T1.x - 2 * p2.x + T2.x;
    double beta0 = p1.y,
           beta1 = T1.y,
           beta2 = -3 * p1.y - 2 * T1.y + 3 * p2.y - T2.y,
           beta3 = 2 * p1.y + T1.y - 2 * p2.y + T2.y;
    for (double t = 0; t <= 1; t += 0.001)
    {
        double t2 = t*t,
               t3 = t2*t;
        double x = alpha0 + alpha1*t + alpha2*t2 + alpha3*t3;
        double y = beta0 + beta1*t + beta2*t2 + beta3*t3;
        double check=calcDistance(x,y,xc,yc);

        if(check<=R) //for clipping if the distance between point and center is smaller than radius then it lies inside circle
            c=RGB(255,0,0);
        else
            c=RGB(0,0,255); // outside the circle

        SetPixel(hdc, Round(x), Round(y), c);
    }
}

void draw_8_points(HDC hdc,int Xc, int Yc, int X, int Y )
{
    SetPixel(hdc,Xc+X,Yc+Y, RGB(0,0,0));
    SetPixel(hdc,Xc+X,Yc-Y, RGB(0,0,0));
    SetPixel(hdc,Xc-X,Yc+Y, RGB(0,0,0));
    SetPixel(hdc,Xc-X,Yc-Y, RGB(0,0,0));
    SetPixel(hdc,Xc+Y,Yc+X, RGB(0,0,0));
    SetPixel(hdc,Xc+Y,Yc-X, RGB(0,0,0));
    SetPixel(hdc,Xc-Y,Yc+X, RGB(0,0,0));
    SetPixel(hdc,Xc-Y,Yc-X, RGB(0,0,0));
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

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    static int R;
    hdc = GetDC(hwnd);
    static point curvePoints[4]; // for the curve
    static point circlePoints[2];  // for the circle window
    static int index = 0;
    static bool finishDrawingCircle = false;
    switch (message)                  /* handle the messages */
    {
    case WM_LBUTTONDOWN:
        if(!finishDrawingCircle) //Draw circle
        {
            if(index==0)
            {
                circlePoints[0].x= LOWORD(lParam);
                circlePoints[0].y=HIWORD(lParam);
                index++;
            }
            else if(index==1)
            {
                circlePoints[1].x= LOWORD(lParam);
                circlePoints[1].y=HIWORD(lParam);
                R= calcDistance(circlePoints[0].x,circlePoints[0].y,circlePoints[1].x,circlePoints[1].y);
                BresenhamCircle(hdc,circlePoints[0].x,circlePoints[0].y,R);
                finishDrawingCircle=true;
                index=0;
            }


        }
        else // curve and clipping
        {
            curvePoints[index].x=LOWORD(lParam);
            curvePoints[index].y=HIWORD(lParam);
            if (index == 3)
            {
                point T1,T2;
                T1.x=3*(curvePoints[1].x - curvePoints[0].x);
                T1.y =3*(curvePoints[1].y - curvePoints[0].y);
                T2.x=3*(curvePoints[3].x - curvePoints[2].x);
                T2.y= 3*(curvePoints[3].y - curvePoints[2].y);
                DrawHermiteCurve(hdc, curvePoints[0], T1, curvePoints[3], T2, R, circlePoints[0].x, circlePoints[0].y);
               /* curvePoints[0].y=curvePoints[0].y+40;
                T1.y+=40;
                curvePoints[3].y=curvePoints[3].y+40;
                T2+=40;
                DrawHermiteCurve(hdc, curvePoints[0], T1, curvePoints[3], T2, R, circlePoints[0].x, circlePoints[0].y);*/.
                ReleaseDC(hwnd, hdc);
                index = 0;
            }
            else
                index++;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
