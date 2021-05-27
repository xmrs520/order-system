#include "../inc/main.h"

/**
 * 打印图片到屏幕
 * bmp_path : 图片路径
 * x，y:要显示图片的起始位置
 * clean:是否清屏
*/
int display_picture(char *filename, int setx, int sety, bool clean)
{
  printf("filename%s \n", filename);
  //打开lcd屏幕设备
  int lcdfd = open("/dev/fb0", O_RDWR);
  if (lcdfd == -1)
  {
    perror("Failed to open lcd");
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
    perror("Failed to open image");
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

/**
 * 登录验证
*/
bool login(int id, char *passwd, int role, user_p usr_list)
{
  user_p user = find(id, usr_list);
  if (user == NULL)
  {
    printf("用户名或密码错误\n");
    return false;
  }
  printf("user->role %d\n", user->role);
  printf("role %d \n", role);
  printf("user->passwd %s \n", user->passwd);
  printf("passwd %s \n", passwd);
  printf("strcmp(user->passwd, passwd) =%d \n", strcmp(user->passwd, passwd));
  if ((strcmp(user->passwd, passwd) == -1 || strcmp(user->passwd, passwd) == 0) && user->role == role)
  {
    printf("登录成功\n");
    return true;
  }
  else
  {
    printf("用户名或密码错误\n");
    return false;
  }
}
/**
 * 进度条
*/
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

/**
 * 用户身份选择
 * p:坐标结构体指针
*/
int selec_user_role(xy *p)
{
  int role;
  while (1)
  {
    get_xy(p);
    if (is_key_area(p, 90, 250, 250, 320))
    {
      role = 1; // 普通用户admin role 3
      break;
    }
    else if (is_key_area(p, 330, 250, 480, 320))
    {
      role = 2; //VIP用户
      break;
    }
    else if (is_key_area(p, 550, 250, 700, 320))
    {
      role = 3; // admin role 3
      break;
    }
  }
  display_picture(LOGIN_BG, 0, 0, false);
  return role;
}

/**
 * 记录id
*/
void record_id(xy *p, int *id, link_stack_p id_stack, int *index_offset)
{
  int key_value; // 键值
  int del_value; //删除的值
  while (1)      // 记录id
  {
    key_value = key_input(p);
    //判断 上限
    if (key_value < 10 && key_value > -1)
    {
      if (*index_offset / 20 >= 4)
      {
        display_picture(LIMIT4_BMP, 305, 225, false);
        usleep(3000);
        break;
      }
    }
    switch (key_value) //显示输入的值
    {
    case 1:
      display_picture(KEY_ONE, 305 + *index_offset, 165, false);
      *index_offset += 20;
      break;
    case 2:
      display_picture(KEY_TWO, 305 + *index_offset, 165, false);
      *index_offset += 20;
      break;
    case 3:
      display_picture(KEY_THREE, 305 + *index_offset, 165, false);
      *index_offset += 20;
      break;
    case 4:
      display_picture(KEY_FOUR, 305 + *index_offset, 165, false);
      *index_offset += 20;
      break;
    case 5:
      display_picture(KEY_FIVE, 305 + *index_offset, 165, false);
      *index_offset += 20;
      break;
    case 6:
      display_picture(KEY_SIX, 305 + *index_offset, 165, false);
      *index_offset += 20;
      break;
    case 7:
      display_picture(KEY_SEVEN, 305 + *index_offset, 165, false);
      *index_offset += 20;
      break;
    case 8:
      display_picture(KEY_EIGHT, 305 + *index_offset, 165, false);
      *index_offset += 20;
      break;
    case 9:
      display_picture(KEY_NINE, 305 + *index_offset, 165, false);
      *index_offset += 20;
      break;
    case 0:
      display_picture(KEY_ZERO, 305 + *index_offset, 165, false);
      *index_offset += 20;
      break;
    case 11:
      if (pop(id_stack, &del_value)) //是否已删除
      {
        *index_offset -= 20; //成功删除，向前偏移
        display_picture(KEY_DELET, 305 + *index_offset, 165, false);
      }
      break;
    default:
      break;
    }

    if (key_value == 12) // 输入完成
    {
      // push(id_stack, creat_key_node(key_value)); // 保存输入数据
      break;
    }

    if (key_value == 11) // 删除
    {
      continue;
      display_stack_data(id_stack);
    }
    push(id_stack, creat_key_node(key_value)); // 保存输入数据
    display_stack_data(id_stack);
  }
  //int id_arr[10];
  // char id_arrc[10];
  //memset(id_arr, 0, sizeof(id_arr));
  //memset(id_arrc, 0, sizeof(id_arrc));
  int i;
  int times = 1; //10的次方
  printf("-----\n");
  display_stack_data(id_stack);
  printf("size= %d\n", id_stack->size);
  key_node_p q = id_stack->top;
  *id = 0;                             //重新赋值(置零)
  for (i = id_stack->size; i > 0; i--) // 保存用户名和密码
  {

    //id_arr[i] = q->value;
    //pop(id_stack, &id_arr[i]); // 保存用户名
    //id_arrc[i] = (char)(q->value);
    //printf("id_arr[i]=%c\n", id_arrc[i]);
    times *= 10;
    *id += ((q->value) * pow(10.0, id_stack->size - i));
    //printf("*id =%d\n", *id);
    q = q->next;
  }
  //*id = atoi(id_arrc);

  //printf("%d\n", *id);
}

/**
 * 记录密码
*/
void record_passwd(xy *p, char *passwd, link_stack_p passwd_stack, int *index_offset)
{
  int key_value; // 键值
  int del_value; // 删除的值
  while (1)      // 记录密码
  {
    key_value = key_input(p);
    //判断 上限
    if (key_value < 10 && key_value > -1)
    {
      if (*index_offset / 20 >= 6)
      {
        display_picture(LIMIT6_BMP, 305, 225, false);
        usleep(3000);
        break;
      }
    }
    switch (key_value)
    {
    case 0 ... 9:
      display_picture(KEY_STAR, 305 + *index_offset, 243, false);
      *index_offset += 20;
      break;
    case 11:
      if (pop(passwd_stack, &del_value)) //是否已删除
      {
        *index_offset -= 20; //成功删除，向前偏移
        display_picture(KEY_DELET, 305 + *index_offset, 243, false);
      }
      break;
    default:
      break;
    }
    if (key_value == 12) // 输入完成
      break;
    if (key_value == 11) // 删除
    {
      continue;
      display_stack_data(passwd_stack);
    }
    push(passwd_stack, creat_key_node(key_value)); // 保存输入数据
    display_stack_data(passwd_stack);
  }

  memset(passwd, 0, sizeof(passwd));
  int i;
  char data[2] = {0};
  char a[10];
  key_node_p q = passwd_stack->top;
  for (i = passwd_stack->size; i > 0; i--) // 保存用户名和密码
  {
    int2str(q->value, data);
    // pop(passwd_stack, &data); // 保存密码
    a[i - 1] = data[0];
    printf("@q->valu = %d\n", q->value);
    printf("int2str= %c \n", a[i - 1]);
    q = q->next;
  }
  //printf("passwd[]%s \n", passwd);
  strcpy(passwd, a);
  printf("a=%s \n", a);
  printf("passw=%s \n", passwd);
}

/**
 * 用户登录
*/
bool user_login(int role, xy *p, user_p usr_list)
{
  char passwd[10] = {0};                    // 密码
  int id = 0;                               // 用户名
  link_stack_p id_stack = init_stack();     // 用户名 栈
  link_stack_p passwd_stack = init_stack(); // 密码 栈
  int index_offset_id = 0;
  int index_offset_passwd = 0;
  int falg = 1; // 密码错误的次数
  int max = 3;  // 密码错误上限

  while (falg != max) // 密码错误上限
  {
    get_xy(p);
    if (is_key_area(p, 171, 160, 580, 200)) //是输入id?
    {
      display_picture(LOGIN_BG, 0, 0, false);
      display_picture(KEYBOARD, 100, 480 - 180, false);

      display_input_value(true, id_stack);
      display_input_value(false, passwd_stack);

      record_id(p, &id, id_stack, &index_offset_id);
    }
    else if (is_key_area(p, 171, 224, 580, 268)) //是输入passwd?
    {
      display_picture(LOGIN_BG, 0, 0, false);
      display_picture(KEYBOARD, 100, 480 - 180, false);

      display_input_value(true, id_stack);
      display_input_value(false, passwd_stack);

      record_passwd(p, passwd, passwd_stack, &index_offset_passwd);
    }
    else if (is_key_area(p, 171, 330, 580, 360)) // 登录button
    {
      if (login(id, passwd, role, usr_list))
      {
        break;
      }
      else
      {
        display_picture(FAILED_BMP, 200, 100, false);
        get_xy(p);
        falg++;
      }
    }
    display_picture(LOGIN_BG, 0, 0, false);
    display_input_value(true, id_stack);
    display_input_value(false, passwd_stack);
    printf("id and passwd\n");
    printf("--id =%d\n", id);
    printf("--passwd  =%s\n", passwd);
  }

  if (falg >= max)
  {
    printf("密码错误超出限制\n");
    destroy_link_stack(id_stack);     // 释放内存
    destroy_link_stack(passwd_stack); // 释放内存
    return false;                     // 登录失败
  }
  else
  {
    destroy_link_stack(id_stack);     // 释放内存
    destroy_link_stack(passwd_stack); // 释放内存
    return true;                      //登录成功
  }
}

/**
 * 登录界面
*/
bool login_interface(int *role, xy *index_xy, user_p usr_list)
{
  display_picture(WELCOME_PICTRUE, 0, 0, false);
  progress_bar();
  display_picture(SELEC_USER_PICTRUE, 0, 0, false);
  *role = selec_user_role(index_xy);
  if (user_login(*role, index_xy, usr_list)) //用户登录
  {
    display_picture(WELCOME_PICTRUE, 0, 0, false);
    return true;
  }
  else
  {
    return false;
  }
}