#include "expression.hpp"

using namespace std;
static HWND hEdit_1,hEdit_2;

bool START=false;

pthread_mutex_t mutex_dater;
pthread_t thread_Dater;

HWND hwnd;

HWND hChildWnd1;
HWND hChildWnd2;
HWND hChildWnd3;


string old_1,old_2;
void * T_Dater([[maybe_unused]] void *pVoid);

LRESULT CALLBACK WindowProcedure (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CREATE:
            break;
        case WM_DESTROY:
            PostQuitMessage (0);
            break;
        default:
            return DefWindowProc (hWnd, message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nFunsterStil)
{
    MSG messages;
    WNDCLASSEX wincl;
    const char *LS_Runtime = "命题公式计算器";

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = LS_Runtime;

    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);

    wincl.hIcon = LoadIcon (nullptr, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (nullptr, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (nullptr, IDC_ARROW);

    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    //注册窗口类
    RegisterClassEx (&wincl);
    //创建窗口
    hwnd = CreateWindowEx (
            0,
            LS_Runtime,
            "命题公式计算器 \t&:∧\t|:∨\t!:┐\t=>:=>\t-:->\t$:<->",
            WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX),
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            1285,905,
            HWND_DESKTOP,
            nullptr,
            hThisInstance,
            nullptr
    );
    //显示窗口

    FreeConsole();
    ShowWindow (hwnd, nFunsterStil);
    UpdateWindow(hwnd);
    {
        hChildWnd3 = CreateWindow(
                "EDIT", "", WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL| ES_AUTOHSCROLL  | ES_READONLY,
                2, 669, 1274, 200, hwnd, nullptr, nullptr, nullptr
        );
        ShowWindow(hChildWnd3, SW_SHOW); // 显示窗口
        UpdateWindow(hChildWnd3);
    }// 创建第三个子窗口
    {
        hChildWnd1 = CreateWindow(
                "EDIT", "表达式 I 真值表", WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL| ES_AUTOHSCROLL  | ES_READONLY,
                2, 66, 636, 600, hwnd, nullptr, nullptr, nullptr
        );
        ShowWindow(hChildWnd1, SW_SHOW); // 显示窗口
        UpdateWindow(hChildWnd1);
    }// 创建第一个子窗口
    {
        hChildWnd2 = CreateWindow(
                "EDIT", "表达式 II 真值表", WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL| ES_AUTOHSCROLL  | ES_READONLY,
                640, 66, 636, 600, hwnd, nullptr, nullptr, nullptr
        );
        ShowWindow(hChildWnd2, SW_SHOW); // 显示窗口
        UpdateWindow(hChildWnd2);
    }// 创建第二个子窗口
    {
        hEdit_1 = CreateWindow("EDIT",
                               nullptr,
                               WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                               2, 2, 1275, 30,
                               hwnd, nullptr, nullptr, nullptr);


        hEdit_2 = CreateWindow("EDIT",
                               nullptr,
                               WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                               2, 34, 1275, 30,
                               hwnd, nullptr, nullptr, nullptr);

    }//创建两个编辑框
    {
        HFONT hFont = CreateFontA(
                27,     // 字体大小
                0,      // 字体宽度
                0,      // 文字倾斜
                0,      // 角度
                FW_NORMAL, // 字体粗细
                FALSE,  // 斜体
                FALSE,  // 下划线
                FALSE,  // 删除线
                ANSI_CHARSET, // 字符集
                OUT_DEFAULT_PRECIS, // 输出精度
                CLIP_DEFAULT_PRECIS, // 剪辑精度
                DEFAULT_QUALITY,     // 字体质量
                DEFAULT_PITCH | FF_SWISS, // 字体族
                "Arial" // 字体名称
        );

        SendMessageA(hEdit_1, WM_SETFONT, (WPARAM) hFont, TRUE);

        SendMessageA(hEdit_2, WM_SETFONT, (WPARAM) hFont, TRUE);

        SendMessageA(hChildWnd1, WM_SETFONT, (WPARAM) hFont, TRUE);

        SendMessageA(hChildWnd2, WM_SETFONT, (WPARAM) hFont, TRUE);

        SendMessageA(hChildWnd3, WM_SETFONT, (WPARAM) hFont, TRUE);
    }// 该代码块更换字体
    pthread_mutex_init(&mutex_dater, nullptr);
    pthread_create(&thread_Dater, nullptr, T_Dater, nullptr);

    START=true;

    //运行消息循环
    while (GetMessage (&messages, nullptr, 0, 0))
    {
        //把虚拟按键消息翻译成字符消息
        TranslateMessage(&messages);
        //把消息发送到WindowProcedure函数
        DispatchMessage(&messages);
    }
    // 程序的返回值，由PostQuitMessage()提供。
    return messages.wParam; // NOLINT(*-narrowing-conversions)
}

void * T_Dater([[maybe_unused]] void *pVoid) {
    char Cbuffer[1024];
    while(START){
        //上锁
        pthread_mutex_lock(&mutex_dater);
        //从编辑框读入字符串
        GetWindowText(hEdit_1, Cbuffer, sizeof(Cbuffer));
        std::string text_1(Cbuffer);
        GetWindowText(hEdit_2, Cbuffer, sizeof(Cbuffer));
        std::string text_2(Cbuffer);
        //开始计算

        if(old_1!=text_1||old_2!=text_2){
            output_1="";
            output_2="";
            output_3="";
            if(!text_1.empty()){
                if(!fifth(text_1,1)){
                    if (judge_expression_proper(text_1)) {
                        second(text_1);
                        third(text_1, 1);
                        output_1 += "\r\n";
                        first(text_1, 1);
                    }
                }
            }
            if(!text_2.empty()){
                if(!fifth(text_2,2)){
                    if (judge_expression_proper(text_2)) {
                        second(text_2);
                        output_3 += "\r\n";
                        third(text_2, 2);
                        output_2 += "\r\n";
                        first(text_2, 2);
                        if (!text_1.empty()) {
                            if (judge_expression_proper(text_1)) {
                                fourth(text_1,text_2);
                            }
                        }
                    }
                }
            }
            SetWindowText(hChildWnd1, output_1.c_str());
            SetWindowText(hChildWnd2, output_2.c_str());
            SetWindowText(hChildWnd3, output_3.c_str());
            old_1=text_1;
            old_2=text_2;
        }
        pthread_mutex_unlock(&mutex_dater);
        Sleep(200);
    }
    return nullptr;
}