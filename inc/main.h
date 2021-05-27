#ifndef __MAIN_H_
#define __MAIN_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/input.h>

#define UP 1   //上一页
#define DOWN 2 //下一页
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

#define MENU "./picture/menu.bmp"
#define UPDATA_SUCCESS "./picture/updata_seccess.bmp"
#define ADMIN_MANGAGE "./picture/admin_manage.bmp"

#define PAY "./picture/pay.bmp"
#define PAY_SECCESS "./picture/pay_success.bmp"
#define NEXT_VISIT "./picture/next_visit.bmp"
#define FAILED_BMP "./picture/failed.bmp"
#define LIMIT4_BMP "./picture/limit4.bmp"
#define LIMIT6_BMP "./picture/limit6.bmp"

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
#define KEY_DOT_ "./picture/10.bmp"
#define KEY_ZERO "./picture/0.bmp"
#define KEY_DELET "./picture/bai.bmp"
#define KEY_STAR "./picture/star.bmp"

#define LCD_WIDTH 800
#define LCD_HEIGHT 480

#define BIT_3 24
#define BIT_2 16
#define BIT_1 8
#define BIT_0 0

/**
 * 用户信息结构体
*/
typedef struct usr
{
  int id;
  char name[20];         //姓名
  int role;              //身份
  char sex[8];           //性别
  char passwd[16];       //密码
  char old_passwd[16];   //旧密码
  struct usr *next;      //下一个结点指针
  struct usr *prev;      //上一个结点指针
  struct usr *user_head; //用户信息头结点指针
} user, *user_p;

/**
 * 坐标结构体
*/
typedef struct index_x_y
{
  int x; //横坐标
  int y; //纵坐标
} xy;

/**
 * 键值 链栈
*/
typedef struct key
{
  int value; //键值
  struct key *next;
  struct key *prev;
} key_node, *key_node_p;

/**
 * 键值 链栈管理
*/
typedef struct stack
{
  int size;        //栈大小
  struct key *top; //栈顶
} link_stack, *link_stack_p;

/**
 * 菜单链表
*/
typedef struct order
{
  int price;      //价格
  int index;      //序号
  char name[30];  //菜名
  int seleced;    //被选中
  char path[100]; //图片地址
  struct order *next;
  struct order *prev;
} link_order, *link_order_p;

/**
 * 用户首指针
*/
//struct usr *user_start;

/***************login.h start***************/

/**
 * 打印图片到屏幕
 * bmp_path : 图片路径
 * x，y:要显示图片的起始位置
 * clean:是否清屏
*/
int display_picture(char *filename, int setx, int sety, bool clean);

/**
 * 登录验证
*/
bool login(int id, char *passwd, int role, user_p usr_list);

/**
 * 进度条
*/
int progress_bar();

/**
 * 用户身份选择
 * p:坐标结构体指针
*/
int selec_user_role(xy *p);

/**
 * 记录id
*/
void record_id(xy *p, int *id, link_stack_p id_stack, int *index_offset);

/**
 * 记录密码
*/
void record_passwd(xy *p, char *passwd, link_stack_p passwd_stack, int *index_offset);

/**
 * 用户登录
*/
bool user_login(int role, xy *p, user_p usr_list);

/**
 * 登录界面
*/
bool login_interface(int *role, xy *index_xy, user_p usr_list);

/**************login.h end****************/

/**************order.c start****************/

/**
 * 创建菜单链表头节点
*/
link_order_p init_order_list();

/**
 * 增加菜品 
*/
bool create_new_order(link_order_p list, int price, int index, char *path, char *name);

/**
 * 删除菜品
*/
bool dele_order(link_order_p list, int index);

/**
 * 菜单总数量
*/
int order_length(link_order_p list);

/**
 * 菜品页码加载
 * pages:页码
 * list:菜单链表头指针
*/
link_order_p page(int pages, link_order_p list);

/**
 * 翻页
 * operation:上下页操作(up,down)
 * current_pages:当前页码
 * list:菜单链表头指针
*/
link_order_p page_turning(int operation, int *current_pages, int max_pages, link_order_p list);

/**
 * 结算
 * list:菜单链表头指针
*/
void settlement(link_order_p list, int role, xy *p, double discount);

/**************order.c end****************/

/**************keyboard.c start****************/

/**
 * 初始化键值链栈
*/
link_stack_p init_stack();

/**
 * 创建键节点
 * values:键值
*/
key_node_p creat_key_node(int values);

/**
 * 入栈
 * p: 链栈管理
 * node: 键值节点
*/
bool push(link_stack_p p, key_node_p node);

/**
 * 出栈
 * p: 链栈管理
 * values: 键值
*/
bool pop(link_stack_p p, int *values);

/**
 * 销毁栈
*/
void destroy_link_stack(link_stack_p p);

/**
 * 是否在触摸键区域
 * (x1,y1),(x2,y2)
 * p: 坐标结构体指针;
*/
bool is_key_area(xy *p, int x1, int y1, int x2, int y2);

/**
 * 键盘输入
*/
int key_input(xy *p);

/**
 * 显示栈数据
*/
void display_stack_data(link_stack_p list);

/**
 * 显示输入框的值
 * option:true为id false为passwd
*/
void display_input_value(bool option, link_stack_p key_list);

/**
 * 整型转字符串
*/
void int2str(int n, char *str);

/**************keyboard.c end****************/

/**************touch_screen.c start****************/

/**
 * 获取坐标
*/
int get_xy(xy *p);

/**************touch_screen.c end****************/

/**************user.c start****************/

/**
 * 查找用户
*/
user_p find(int id, user_p usr_list);

/**
 * 初始化用户链表
*/
user_p init_user_list();

/**
 * 新建用户
 * 
*/
user_p create_user(
    user_p usr_list,
    int id,
    char *name,
    char *sex,
    int role,
    char *passwd,
    char *old_passwd);

/**
 * 删除用户
*/
user_p del_user(user_p usr_list, int id);

/**
 * 修改密码
*/
bool updata_passwd(int id, char *new_passwd, char *old_passwd, user_p usr_list);

/**
 * 显示用户
*/
void display_user(user_p usr_list);

/**************user.c end****************/

/**************read_file.c start****************/

/**
 * 读取所有菜单数据保存在链表
 * order_list: 菜单头结点
 * filepath: 菜单数据文件路径
*/
bool read_order(link_order_p order_list, char *filepath);

/**
 * 读取所需菜单数据保存在链表
 * order_list: 菜单头结点
 * filepath: 菜单数据文件路径
*/
bool read_order_index(int index_num, link_order_p order_list, char *filepath);

/**
 * 从数据文件中读取用户并保存
*/
bool read_usr_data(user_p usr_list, char *filepath);

/**************read_file.c end****************/

/**************admin.c start****************/

/**
 * 管理菜单
*/
void admin_manage(link_order_p order_list, char *filepath, xy *p);

/**
 * 增加菜
*/
bool add_order(int index, link_order_p order_list, char *filepath);

/**
 * 删除菜
*/
bool del_order(int index, link_order_p order_list);

/**************admin.c end****************/

#endif