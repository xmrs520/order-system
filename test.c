
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
 * 增加菜品 
*/
bool create_new_order(link_order_p list, int price, int index, char *path, char *name)
{
  link_order_p p = list;
  link_order_p node = (link_order_p)malloc(sizeof(link_order));
  if (node == NULL)
  {
    printf("增加菜品失败\n");
    return false;
  }
  while (p->next != NULL)
  {
    p = p->next;
  }

  node->price = price;
  node->index = index;
  node->seleced = 0;
  strcpy(node->path, path);
  strcpy(node->name, name);

  node->prev = p;
  p->next = node;
  node->next = NULL;
  return true;
}

/**
 * 创建菜单链表头节点
*/
link_order_p init_order_list()
{
  link_order_p head = (link_order_p)malloc(sizeof(link_order));
  if (head == NULL)
  {
    printf("初始化菜单链表失败\n");
    return NULL;
  }
  head->price = 0;
  head->index = 0;
  head->seleced = 0;
  strcpy(head->path, "");
  strcpy(head->name, "");
  head->next = NULL;
  head->prev = NULL;
  return head;
}

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

int main(int argc, char const *argv[])

{
  link_order_p order_list = init_order_list();
  read_order(order_list, "./data/order_list.txt");
  link_order_p p = order_list->next;
  while (p != NULL)
  {
    printf("name=%s  path=%s \n", p->name, p->path);
    p = p->next;
  }

  return 0;
}
