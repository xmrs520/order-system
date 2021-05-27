#include "./inc/main.h"

int main(int argc, char const *argv[])
{
  int role;               //用户身份
  double discount = 0.85; //八五折
  xy xy_p;                //记录坐标

  user_p usr_list = init_user_list(); // 初始化用户头结点
  link_order_p order_list = init_order_list();

  read_order(order_list, "./data/order_list.txt"); //读取数据保存到链表
  read_usr_data(usr_list, "./data/user_data.txt");

  link_order_p p = order_list->next;
  while (p != NULL)
  {
    printf("name=%s  path=%s \n", p->name, p->path);
    p = p->next;
  }
  display_user(usr_list);
loop:
  if (login_interface(&role, &xy_p, usr_list))
  {
    printf("登录成功\n");
    if (role == 3)
    {
      admin_manage(order_list, "./data/order_list.txt", &xy_p);
      goto loop;
    }
    settlement(order_list, role, &xy_p, discount); //选购结算
  }
  else
  {
    printf("登录失败\n");
  }
  return 0;
}
