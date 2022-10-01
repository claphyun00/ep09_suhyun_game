#include "pch.h"
#include <stdio.h>
#include "tipsware.h"

#define LEFT_POS_X           30    
#define LEFT_POS_Y           700   
#define LEFT_GUIDE_RADIUS    220   
#define LEFT_START_DEGREE    80    
#define LEFT_SPOT_COUNT      8     
#define LEFT_SPOT_RADIUS     7     
#define LEFT_DEC_DEGREE      4    
#define MAX_IMAGE_COUNT      16     

typedef struct AnimationImageData
{
    UINT16 image_count, index;
    void* p_image[MAX_IMAGE_COUNT];
} AID;

void SetImagePath(AID* ap_data, const char* ap_path, UINT16 a_count)
{
    ap_data->image_count = a_count; 
    ap_data->index = 0;  

    char temp_path[MAX_PATH];

    for (UINT16 i = 0; i < a_count; i++)
    {
        sprintf_s(temp_path, MAX_PATH, "%s%02d.png", ap_path, i);
        ap_data->p_image[i] = LoadImageGP(temp_path);
    }
}

void DeleteImageList(AID* ap_data)
{
    for (UINT16 i = 0; i < ap_data->image_count; i++) DeleteImageGP(ap_data->p_image[i]);
}

UINT16 DrawImage(AID* ap_data, int a_x, int a_y, double a_cx_rate, double a_cy_rate)
{
    void* p_image = ap_data->p_image[ap_data->index];
    int cx = (int)(GetWidthGP(p_image) * a_cx_rate), cy = (int)(GetHeightGP(p_image) * a_cy_rate);
    DrawImageGP(p_image, a_x, a_y, cx, cy);

    return ap_data->index = (ap_data->index + 1) % ap_data->image_count;
}

void DrawImageByIndex(AID* ap_data, int a_x, int a_y, int a_index, double a_cx_rate, double a_cy_rate)
{
    void* p_image = ap_data->p_image[a_index];
    int cx = (int)(GetWidthGP(p_image) * a_cx_rate), cy = (int)(GetHeightGP(p_image) * a_cy_rate);
    DrawImageGP(p_image, a_x, a_y, cx, cy);
}

struct AppData //내부데이터
{
    void* p_bk_image;       
    AID tank_image;         
};

void OnDestroy()
{
    AppData* p_app_data = (AppData*)GetAppData();
    DeleteImageGP(p_app_data->p_bk_image);
    DeleteImageList(&p_app_data->tank_image);
}

MOUSE_MESSAGE_EX(NULL, NULL, NULL, OnDestroy)

int main()
{
    ChangeWorkSize(1524, 800);
    AppData app_data = {NULL,};

    app_data.p_bk_image = LoadImageGP(".\\cannon\\bk.png");
    SetImagePath(&app_data.tank_image, ".\\cannon\\t_", 8);
    //배경,탱크 이미지 읽기

    SetAppData(&app_data, sizeof(app_data));

    DrawImageGP(app_data.p_bk_image, 0, 0);//배경이미지 출력
    DrawImageByIndex(&app_data.tank_image, //탱크이미지 출력
        LEFT_POS_X - 50, LEFT_POS_Y - 130, 0, 0.4, 0.4);

    int dx = 0, dy = 0;
    for (int i = 0; i < LEFT_SPOT_COUNT; i++) 
    {
        GetIntPosFromCircle(LEFT_POS_X, LEFT_POS_Y, LEFT_GUIDE_RADIUS,
            LEFT_START_DEGREE - i * LEFT_DEC_DEGREE, (int*)&dx, (int*)&dy);

        Ellipse(dx - LEFT_SPOT_RADIUS, dy - LEFT_SPOT_RADIUS,
            dx + LEFT_SPOT_RADIUS, dy + LEFT_SPOT_RADIUS, RGB(0, 255, 0));
    }
    ShowDisplay();

    return 0;
}
