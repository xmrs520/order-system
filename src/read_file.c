#include "../inc/main.h"

/**
 * 读取所有菜单数据保存在链表
 * order_list: 菜单头结点
 * filepath: 菜单数据文件路径
*/
bool read_order(link_order_p order_list, char *filepath)
{
  FILE *order_list_file;
  char buf[1024];
  char name[20];
  int price;
  char path[30];
  int index;
  char *res;
  order_list_file = fopen(filepath, "r");
  if (order_list_file != NULL)
  {
    while (1)
    {
      res = fgets(buf, sizeof(buf), order_list_file);
      if (res == NULL)
      {
        break;
      }
      // ./picture/menu1.bmp 特色烤鱼 48 1
      sscanf(buf, "%s %s %d %d", path, name, &price, &index);
      create_new_order(order_list, price, index, path, name); // 保存在链表中
      memset(buf, 0, sizeof(buf));
    }

    fclose(order_list_file);
    return true;
  }
  else
  {
    perror("order_list.txt 打开失败\n");
    return false;
  }
}

/**
 * 读取所需菜单数据保存在链表
 * order_list: 菜单头结点
 * filepath: 菜单数据文件路径
*/
bool read_order_index(int index_num, link_order_p order_list, char *filepath)
{
  FILE *order_list_file;
  char buf[1024];
  char name[20];
  int price;
  char path[30];
  int index;
  char *res;
  bool flag = false;
  order_list_file = fopen(filepath, "r");
  if (order_list_file != NULL)
  {
    while (1)
    {
      res = fgets(buf, sizeof(buf), order_list_file);
      if (res == NULL)
      {
        break;
      }
      // ./picture/menu1.bmp 特色烤鱼 48 1
      sscanf(buf, "%s %s %d %d", path, name, &price, &index);
      if (index_num == index)
      {
        create_new_order(order_list, price, index, path, name); // 保存在链表中
        flag = true;
      }
      memset(buf, 0, sizeof(buf));
    }

    fclose(order_list_file);
    if (!flag)
    {
      return false;
    }
    return true;
  }
  else
  {
    perror("order_list.txt 打开失败\n");
    return false;
  }
}

/**
 * 从数据文件中读取用户并保存
 * usr_lists: 用户头结点
 * filepath: 用户数据文件路径
*/
bool read_usr_data(user_p usr_list, char *filepath)
{
  char *res;
  char buf[1024];
  int id;
  char passwd[10];
  int role;
  char name[30];
  char sex[4];
  char old_passwd[10];
  FILE *usr_data_fp;

  usr_data_fp = fopen(filepath, "r");
  if (usr_data_fp != NULL)
  {
    while (1)
    {
      res = fgets(buf, sizeof(buf), usr_data_fp);
      if (res == NULL)
      {
        break;
      }
      //id:1234 passwd:123456789 role:3 name:xmrs sex:男 old_passwd:
      sscanf(buf, "id:%d passwd:%s role:%d name:%s sex:%s old_passwd:%s",
             &id, passwd, &role, name, sex, old_passwd);
      create_user(usr_list, id, name, sex, role, passwd, old_passwd);
      // display_user(usr_list);
      memset(buf, 0, sizeof(buf));
    }

    fclose(usr_data_fp);
    return true;
  }
  else
  {
    perror("order_list.txt 打开失败\n");
    return false;
  }
}