#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/input.h>
#define UP 1   //上一页
#define DOWN 2 //下一页
#define MAX_PAGES 8
#define LCD_PATH "/dev/fb0"
#define TOUCH_SCREENT_PATH "/dev/input/event0"
#define WELCOME_PICTRUE "./picture/welcome.bmp"
#define ORDER_BG "./picture/order_bg.bmp"
#define SELEC_USER_PICTRUE "./picture/selectuser.bmp"
#define LOGIN_BG "./picture/login_background.bmp"
#define SELECED_PICTRUE "./picture/seleced.bmp"
#define LEFT "./picture/left.bmp"
#define RIGHT "./picture/right.bmp"
#define ORDER_BAR "./picture/order_bar.bmp"
#define ORDER_BOTTOM "./picture/order_bottom.bmp"
#define COMPLETE "./picture/complete.bmp"
#define PAY "./picture/pay.bmp"
#define PAY_SECCESS "./picture/pay_success.bmp"
#define NEXT_VISIT "./picture/next_visit.bmp"
#define MENU "./picture/menu.bmp"
#define UPDATA_SUCCESS "./picture/updata_seccess.bmp"
#define ADMIN_MANGAGE "./picture/admin_manage.bmp"

#define MENU1 "./picture/menu1.bmp"
#define MENU2 "./picture/menu2.bmp"
#define MENU3 "./picture/menu3.bmp"
#define MENU4 "./picture/menu4.bmp"
#define MENU5 "./picture/menu5.bmp"
#define MENU6 "./picture/menu6.bmp"
#define MENU7 "./picture/menu7.bmp"
#define MENU8 "./picture/menu8.bmp"

#define KEYBOARD "./picture/keyboard.bmp"
#define KEY_ONE "./picture/1.bmp"
#define KEY_TWO "./picture/2.bmp"
#define KEY_THREE "./picture/3.bmp"
#define KEY_FOUR "./picture/4.bmp"
#define KEY_FIVE "./picture/5.bmp"
#define KEY_SIX "./picture/6.bmp"
#define KEY_SEVEN "./picture/7.bmp"
#define KEY_EIGHT "./picture/8.bmp"
#define KEY_NINE "./picture/9.bmp"
#define KEY_ZERO "./picture/0.bmp"
#define KEY_DELET "./picture/bai.bmp"
#define KEY_STAR "./picture/star"

#define LCD_WIDTH 800
#define LCD_HEIGHT 480

#define BIT_3 24
#define BIT_2 16
#define BIT_1 8
#define BIT_0 0
typedef struct index_x_y
{
  int x; //横坐标
  int y; //纵坐标
} xy;

int get_xy(xy *p)
{
  int touch_fd = open(TOUCH_SCREENT_PATH, O_RDWR);
  if (touch_fd < 0)
  {
    printf("Failed to open touch screent\n");
    return 0;
  }

  struct input_event ts;

  while (1)
  {
    read(touch_fd, &ts, sizeof(ts));

    if (ts.type == EV_ABS)
    {
      if (ts.code == ABS_X)
      {
        p->x = (ts.value) * 800 / 1024;
      }
      if (ts.code == ABS_Y)
      {
        p->y = (ts.value) * 480 / 600;
      }
    }
    if (ts.type == EV_KEY && ts.code == BTN_TOUCH && ts.value == 0)
      break;
  }
  close(touch_fd);
  return 1;
}

int display_picture(char *filename, int setx, int sety, bool clean)
{
  //打开lcd屏幕设备
  int lcdfd = open("/dev/fb0", O_RDWR);
  if (lcdfd == -1)
  {
    perror("lcdfd open failed");
    return -1;
  }

  //申请映射内存---假定lcd屏幕就是800*480的分辨率，每个像素点4个字节
  int *lcdmem = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, lcdfd, 0);
  if (lcdmem == MAP_FAILED)
  {
    perror("mmap lcdmem failed");
    return -1;
  }

  //清屏
  if (clean)
  {
    bzero(lcdmem, LCD_WIDTH * LCD_HEIGHT * 4);
  }
  //从图片中获取颜色数据
  int bmpfd = open(filename, O_RDONLY);
  if (bmpfd == -1)
  {
    perror("open bmp failed");
    return -1;
  }

  //从图片中读取头文件
  char headinfo[54];
  read(bmpfd, headinfo, 54);

  //从图片的头文件中获取图片的宽度和高度
  int bmp_w = headinfo[18] | headinfo[19] << 8 | headinfo[20] << 16 | headinfo[21] << 24;
  int bmp_h = headinfo[22] | headinfo[23] << 8 | headinfo[24] << 16 | headinfo[25] << 24;
  printf("bmp_w=%d,bmp_h=%d\n", bmp_w, bmp_h);

  int win_size = (4 - bmp_w * 3 % 4) % 4; //windows补齐的字节数
  int bmp_lenth = bmp_w * 3 + win_size;   //图片1行的字节数= 像素点个数*3 + windows补齐的字节数

  char bmpbuf[bmp_lenth * bmp_h];         //bmp图片总共的颜色数据,单位字节
  read(bmpfd, bmpbuf, bmp_lenth * bmp_h); //从图片文件中读取所有数据读到缓冲区里面

  int x, y;
  char lcdbuf[bmp_w * bmp_h * 4]; //只定义图片大小的缓冲区
  for (x = 0; x < bmp_w; x++)
    for (y = 0; y < bmp_h; y++)
    {
      lcdbuf[4 * (x + bmp_w * y)] = bmpbuf[3 * x + bmp_lenth * y];         //图片的蓝色对齐lcd的蓝色
      lcdbuf[4 * (x + bmp_w * y) + 1] = bmpbuf[3 * x + bmp_lenth * y + 1]; //图片的绿色对齐lcd的绿色
      lcdbuf[4 * (x + bmp_w * y) + 2] = bmpbuf[3 * x + bmp_lenth * y + 2]; //图片的红色对齐lcd的红色
      lcdbuf[4 * (x + bmp_w * y) + 3] = 0;                                 //图片没有透明度，所以lcd的透明度不用对齐
    }

  //确定每一行需要绘制的字节个数
  //如果图片宽度小于 800-setx,则绘制图片的宽度
  //如果图片宽度大于 800-setx,
  int draw_lenth = bmp_w < 800 - setx ? bmp_w : 800 - setx;

  for (y = 0; y < 480 - sety && y < bmp_h; y++)
    memcpy(lcdmem + setx + (y + sety) * 800, &lcdbuf[4 * (bmp_w * (bmp_h - 1 - y))], draw_lenth * 4);

  munmap(lcdmem, 800 * 480 * 4);
  close(lcdfd);

  return 0;
}

int progress_bar()
{
  int lcd_fd = open(LCD_PATH, O_RDWR);
  if (lcd_fd < 0)
  {
    printf("Failed to open lcd\n");
    return 0;
  }
  int x, y;
  int color = 0x00ff0066;
  int *share_addr = NULL;
  share_addr = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd_fd, 0);
  if (share_addr == MAP_FAILED)
  {
    printf("mmap failed\n");
  }
  for (x = 0; x < 800; x++)
  {
    usleep(500);
    for (y = 0; y < 15; y++)
      memcpy(share_addr + x + (y + 465) * 800, &color, 4);
  }

  munmap(share_addr, 800 * 480 * 4);
  close(lcd_fd);
}

bool is_key_area(xy *p, int x1, int y1, int x2, int y2)
{
  if ((p->x > x1 && p->x < x2) &&
      (p->y > y1 && p->y < y2))
  {
    return true;
  }
  else
  {
    return false;
  }
}

int selec_user_role(xy *p)
{
  int role;
  while (1)
  {
    get_xy(p);
    // admin role 3
    if (is_key_area(p, 90, 250, 250, 320))
    {
      role = 3;
      break;
    }
    else if ((p->x > 330 && p->x < 480) &&
             (p->y > 250 && p->y < 320))
    {
      role = 2;
      break;
    }
    else if ((p->x > 550 && p->x < 700) &&
             (p->y > 250 && p->y < 320))
    {
      role = 1;
      break;
    }
  }
  display_picture(LOGIN_BG, 0, 0, false);
  return role;
}

void page(int pages)
{
  switch (pages)
  {
  case 1:
    display_picture(ORDER_BG, 0, 0, false);
    display_picture(MENU1, 21, 115, false);
    display_picture(MENU2, 421, 115, false);
    display_picture(LEFT, 100, 430, false);
    display_picture(RIGHT, 519, 430, false);
    break;
  case 2:
    display_picture(ORDER_BG, 0, 0, false);
    display_picture(MENU3, 21, 115, false);
    display_picture(MENU4, 421, 115, false);
    display_picture(LEFT, 100, 430, false);
    display_picture(RIGHT, 519, 430, false);
    break;
  case 3:
    display_picture(ORDER_BG, 0, 0, false);
    display_picture(MENU5, 21, 115, false);
    display_picture(MENU6, 421, 115, false);
    display_picture(LEFT, 100, 430, false);
    display_picture(RIGHT, 519, 430, false);
    break;
  case 4:
    display_picture(ORDER_BG, 0, 0, false);
    display_picture(MENU7, 21, 115, false);
    display_picture(MENU8, 421, 115, false);
    display_picture(LEFT, 100, 430, false);
    display_picture(RIGHT, 519, 430, false);
    break;
  default:
    break;
  }
}

/**
 * 翻页
 * operation:上下页操作(up,down)
 * current_pages:当前页码
*/
void page_turning(int operation, int *current_pages)
{

  if (operation == UP && (*current_pages) < MAX_PAGES)
  {
    page(*current_pages + 1);
  }
  else if (operation == DOWN && (*current_pages) > 1)
  {
    page(*current_pages - 1);
  }
}
int main(int argc, char const *argv[])
{
  xy p;

  //int x, y;
  //printf("请输入x y 的值\n");
  //scanf("%d %d", &x, &y);

  display_picture(WELCOME_PICTRUE, 0, 0, false);
  progress_bar();
  display_picture(SELEC_USER_PICTRUE, 0, 0, false);
  int role = selec_user_role(&p);
  printf("role =%d \n", role);
  get_xy(&p);
  //display_picture(KEYBOARD, 100, 480 - 180, false);
  display_picture(KEY_ONE, 305, 165, false);
  display_picture(KEY_TWO, 325, 165, false);
  display_picture(KEY_THREE, 345, 165, false);
  display_picture(KEY_FIVE, 365, 165, false);
  display_picture(KEY_SIX, 385, 165, false);
  display_picture(KEY_SEVEN, 405, 165, false);

  display_picture(ORDER_BG, 0, 0, false);
  display_picture(MENU1, 50, 50, false);
  display_picture(MENU2, 460, 50, false);
  display_picture(LEFT, 100, 430, false);
  display_picture(RIGHT, 519, 430, false);
  display_picture(COMPLETE, 600, 425, false);
  display_picture(SELECED_PICTRUE, 240, 170, false);
  display_picture(SELECED_PICTRUE, 640, 170, false);
  //display_picture(ADMIN_MANGAGE, 0, 0, false);

  display_picture(ORDER_BOTTOM, 0, 423, false);
  display_picture(ORDER_BAR, 0, 0, false);
  while (1)
  {
    get_xy(&p);
    printf("x=%d y=%d\n", p.x, p.y);
  }

  return 0;
}
