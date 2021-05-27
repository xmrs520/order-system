#include "../inc/main.h"

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
 * 删除菜品
*/
bool dele_order(link_order_p list, int index)
{
  bool flag = false;
  link_order_p tmp = NULL;
  link_order_p p = list->next;
  while (p != NULL)
  {
    if (p->index == index)
    {
      tmp = p;
      tmp->prev->next = tmp->next;
      tmp->next->prev = tmp->prev;
      flag = true;
      break;
    }
    p = p->next;
  }
  if (!flag)
  {
    return false;
  }
  free(tmp);
  return true;
}
/**
 * 菜单总数量
*/
int order_length(link_order_p list)
{
  link_order_p p = list;
  int conut = 0;
  while (p != NULL)
  {
    conut++;
    p = p->next;
  }
  return conut;
}
/**
 * 菜品页码加载
 * pages:页码
 * list:菜单链表头指针
*/
link_order_p page(int pages, link_order_p list)
{
  int i = 0;
  int j = 0;
  int offset = 0; //偏移显示下一个菜
  link_order_p p = list->next;
  link_order_p tmp = NULL; //记录当前页的第一个菜品节点地址
  display_picture(ORDER_BG, 0, 0, false);
  display_picture(LEFT, 100, 430, false);
  display_picture(RIGHT, 519, 430, false);
  display_picture(COMPLETE, 600, 425, false);
  while (p != NULL && i < pages * 2)
  {
    if (i > pages * 2 - 3)
    {
      j++;
      display_picture(p->path, 50 + offset, 50, false); //第一个菜
      printf("%s", p->path);
      if (p->seleced && j == 1) //是否被选中
      {
        display_picture(SELECED_PICTRUE, 240, 170, false);
      }
      else if (p->seleced && j == 2)
      {
        display_picture(SELECED_PICTRUE, 640, 170, false);
      }

      offset += 400; //并排显示 相差400px
      if (j == 1)
      {
        tmp = p;
      }
    }
    i++;
    p = p->next;
  }
  return tmp;
}

/**
 * 翻页
 * operation:上下页操作(up,down)
 * current_pages:当前页码
 * list:菜单链表头指针
*/
link_order_p page_turning(int operation, int *current_pages, int max_pages, link_order_p list)
{
  link_order_p p = NULL;
  if (operation == UP && (*current_pages) > 1)
  {
    p = page(*current_pages - 1, list);
    (*current_pages)--;
    return p;
  }
  else if (operation == DOWN && (*current_pages) < max_pages)
  {
    p = page(*current_pages + 1, list);
    (*current_pages)++;
    return p;
  }
  return page(*current_pages, list);
}

/**
 * 挑选
*/

/**
 * 结算
 * list:菜单链表头指针
*/
void settlement(link_order_p list, int role, xy *p, double discount)
{

  //显示菜品
  int pages = 1;
  link_order_p order_p = list->next;        // 菜单
  link_order_p current = page(pages, list); //当前页第一个菜的节点
  //link_order_p current_next = current->next; //当前页第二个菜的节点
  int max_ages = order_length(list) / 2; // 最大页码
  printf("------max_ages=%d\n", max_ages);
  double sum_price = 0.0;
  //挑选
  while (1)
  {
    get_xy(p);
    //是否选中第一个菜
    if (is_key_area(p, 60, 60, 300, 400))
    {
      if (!current->seleced)
      {
        display_picture(SELECED_PICTRUE, 240, 170, false);
        current->seleced = 1;
      }
      else
      {
        current->seleced = 0;
        page(pages, list);
      }
    }
    //是否选中第二个菜
    else if (is_key_area(p, 487, 60, 700, 370))
    {
      if (current->next != NULL)
      {
        if (!current->next->seleced)
        {
          display_picture(SELECED_PICTRUE, 640, 170, false);
          current->next->seleced = 1;
        }
        else
        {
          current->next->seleced = 0;
          page(pages, list);
        }
      }
    }
    //上一页
    else if (is_key_area(p, 100, 400, 140, 470))
    {
      current = page_turning(UP, &pages, max_ages, list);
    }
    //下一页
    else if (is_key_area(p, 520, 410, 560, 470))
    {
      current = page_turning(DOWN, &pages, max_ages, list);
    }
    else if (is_key_area(p, 610, 430, 740, 470)) //完成点菜
    {
      break;
    }
    printf("-----pages =%d \n", pages);
  }

  //显示结算界面
  display_picture(ORDER_BAR, 0, 0, false);
  display_picture(MENU, 0, 43, false);
  printf("===========结算==========\n");
  while (order_p != NULL) //显示清单
  {
    if (order_p->seleced)
    {
      printf("菜名: %s \n", order_p->name);
      printf("价格: %d \n", order_p->price);
      sum_price = sum_price + (double)order_p->price;
      printf("--------sum_price = %lf\n", sum_price);
    }
    order_p = order_p->next;
  }
  if (role == 2)
  {
    printf("--------2---sum_price = %lf\n", sum_price);
    printf("-----discount =%lf\n", discount);
    printf("总价: %.2lf\n", sum_price * discount);
  }
  else
  {
    printf("--------3---sum_price = %lf\n", sum_price);

    printf("总价: %.2lf\n", sum_price);
  }
  printf("========================\n");
  display_picture(ORDER_BOTTOM, 0, 423, false);

  //显示金额
  int index_offset = 0;
  int n;
  char show_price[5];
  gcvt(sum_price * discount, 5, show_price); // 将double 变成字符串
  for (n = 0; n < strlen(show_price); n++)
  {
    switch (show_price[n]) //显示输入的值
    {
    case '1':
      display_picture(KEY_ONE, 566 + index_offset, 436, false);
      index_offset += 15;
      break;
    case '2':
      display_picture(KEY_TWO, 566 + index_offset, 436, false);
      index_offset += 15;
      break;
    case '3':
      display_picture(KEY_THREE, 566 + index_offset, 436, false);
      index_offset += 15;
      break;
    case '4':
      display_picture(KEY_FOUR, 566 + index_offset, 436, false);
      index_offset += 15;
      break;
    case '5':
      display_picture(KEY_FIVE, 566 + index_offset, 436, false);
      index_offset += 15;
      break;
    case '6':
      display_picture(KEY_SIX, 566 + index_offset, 436, false);
      index_offset += 15;
      break;
    case '7':
      display_picture(KEY_SEVEN, 566 + index_offset, 436, false);
      index_offset += 15;
      break;
    case '8':
      display_picture(KEY_EIGHT, 566 + index_offset, 436, false);
      index_offset += 15;
      break;
    case '9':
      display_picture(KEY_NINE, 566 + index_offset, 436, false);
      index_offset += 15;
      break;
    case '0':
      display_picture(KEY_ZERO, 566 + index_offset, 436, false);
      index_offset += 15;
      break;
    case '.':
      display_picture(KEY_DOT_, 566 + index_offset, 436, false);
      index_offset += 15;
    default:
      break;
    }
  }

  while (1)
  {
    get_xy(p);
    // 点击买单
    if (is_key_area(p, 650, 450, 740, 475))
    {
      break;
    }
  }
  display_picture(PAY, 0, 0, false); //显示支付码
  sleep(5);
  display_picture(PAY_SECCESS, 0, 0, false); //显示支付成功
  sleep(5);
  display_picture(NEXT_VISIT, 0, 0, false); //显示欢迎下次光临
}