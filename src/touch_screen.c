#include "../inc/main.h"
/**
 * 获取坐标
*/
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