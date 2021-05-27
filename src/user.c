#include "../inc/main.h"

/**
 * 初始化用户链表
*/
user_p init_user_list()
{
  user_p head = (user_p)malloc(sizeof(user));
  if (head == NULL)
  {
    printf("初始化失败\n");
    return NULL;
  }
  head->id = -1;
  strcpy(head->name, "初始化");
  strcpy(head->sex, "未知");
  strcpy(head->passwd, "0000");
  strcpy(head->old_passwd, "");
  head->role = -1;
  head->user_head = head;
  head->next = NULL;
  head->prev = NULL;
  return head;
}

/**
 * 新建用户(尾插法)
 * 
*/
user_p create_user(
    user_p usr_list,
    int id,
    char *name,
    char *sex,
    int role,
    char *passwd,
    char *old_passwd)
{
  if (usr_list == NULL)
  {
    printf("无效的用户链表\n");
    return NULL;
  }
  user_p node = (user_p)malloc(sizeof(user));
  if (node == NULL)
  {
    printf("新建用户失败\n");
    return NULL;
  }
  user_p p = usr_list;

  while (p->next != NULL)
  {
    p = p->next;
  }

  node->id = id;
  strcpy(node->name, name);
  strcpy(node->sex, sex);
  strcpy(node->passwd, passwd);
  node->role = role;
  strcpy(node->old_passwd, "");

  p->next = node;
  node->prev = p;
  node->next = NULL;
}

/**
 * 插入用户(尾插法)
*/

/**
 * 查找用户
*/
user_p find(int id, user_p usr_list)
{
  user_p p = usr_list->next;
  while (p != NULL)
  {
    if (p->id == id)
    {
      return p;
    }
    p = p->next;
  }
  printf("暂无该用户\n");
  return NULL;
}
/**
 * 删除用户
*/
user_p del_user(user_p usr_list, int id)
{
  user_p p = find(id, usr_list);
  if (p == NULL)
  {
    return NULL;
  }
  p->prev->next = p->next;
  p->next->prev = p->prev;
  free(p);
}

/**
 * 修改密码
*/
bool updata_passwd(int id, char *new_passwd, char *old_passwd, user_p usr_list)
{
  user_p user = find(id, usr_list);
  if (user == NULL)
  {
    return false;
  }
  if (strcmp(old_passwd, user->passwd) == 0)
  {
    memset(user->passwd, 0, sizeof(user->passwd));
    strcpy(user->passwd, new_passwd);
    memset(user->old_passwd, 0, sizeof(user->old_passwd));
    strcpy(user->old_passwd, old_passwd);
  }
  else
  {
    printf("旧密码错误\n");
    return false;
  }
  printf("修改密码成功\n");
  return true;
}

/**
 * 显示用户
*/
void display_user(user_p usr_list)
{
  user_p p = usr_list->next;
  if (p == NULL)
  {
    printf("暂无用户信息\n");
  }
  while (p != NULL)
  {
    printf("==================用户信息=======================\n");
    printf("id: %d\n", p->id);
    printf("姓名：%s\n", p->name);
    printf("性别：%s\n", p->sex);
    printf("用户身份：%d\n", p->role);
    printf("密码：%s\n", p->passwd);
    printf("旧密码：%s\n", p->old_passwd);
    p = p->next;
  }
}