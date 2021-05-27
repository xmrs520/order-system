#include "../inc/main.h"

/**
 * 初始化键值链栈
*/
link_stack_p init_stack()
{
  link_stack_p stack = (link_stack_p)malloc(sizeof(link_stack));
  if (stack == NULL)
  {
    printf("初始化栈失败\n");
    return NULL;
  }
  stack->size = 0;
  stack->top = NULL;
  return stack;
}

/**
 * 创建键节点
 * values:键值
*/
key_node_p creat_key_node(int values)
{
  key_node_p node = (key_node_p)malloc(sizeof(key_node));
  if (node == NULL)
  {
    printf("创建栈成员失败\n");
    return NULL;
  }
  node->value = values;
  node->next = NULL;
  node->prev = NULL;
  return node;
}

/**
 * 入栈
 * p: 链栈管理
 * node: 键值节点
*/
bool push(link_stack_p p, key_node_p node)
{
  node->next = p->top;
  if (node->next != NULL) //只有一个数据
  {
    p->top->prev = node;
  }
  node->prev = NULL;
  p->top = node;
  p->size++;
  return true;
}

/**
 * 出栈
 * p: 链栈管理
 * values: 键值
*/
bool pop(link_stack_p p, int *values)
{
  if (p == NULL || p->top == NULL)
  {
    printf("栈为空或无效栈\n");
    return false;
  }
  key_node_p tmp = NULL;
  tmp = p->top;
  *values = tmp->value;
  p->top = p->top->next;
  free(tmp);
  if (p->top != NULL)
  {
    p->top->prev = NULL;
  }
  p->size--;
  return true;
}
/**
 * 显示栈数据
*/
void display_stack_data(link_stack_p list)
{
  if (list == NULL || list->top == NULL)
  {
    printf("栈为空或无效栈\n");
    return;
  }
  key_node_p p = list->top;
  printf("key value= ");
  while (p != NULL)
  {
    printf("%d ", p->value);
    p = p->next;
  }
  printf("\n");
}

/**
 * 显示输入框的值
 * option:true为id false为passwd
*/
void display_input_value(bool option, link_stack_p key_list)
{
  if (key_list == NULL || key_list->top == NULL)
  {
    printf("栈为空或无效栈\n");
    return;
  }
  key_node_p p = key_list->top;
  while (p->next != NULL)
  {
    p = p->next;
  }

  int key_value = 0;
  int index_offset = 0;

  while (p != NULL)
  {
    key_value = p->value;
    if (option)
    {
      switch (key_value) //显示输入id的值
      {
      case 1:
        display_picture(KEY_ONE, 305 + index_offset, 165, false);
        index_offset += 20;
        break;
      case 2:
        display_picture(KEY_TWO, 305 + index_offset, 165, false);
        index_offset += 20;
        break;
      case 3:
        display_picture(KEY_THREE, 305 + index_offset, 165, false);
        index_offset += 20;
        break;
      case 4:
        display_picture(KEY_FOUR, 305 + index_offset, 165, false);
        index_offset += 20;
        break;
      case 5:
        display_picture(KEY_FIVE, 305 + index_offset, 165, false);
        index_offset += 20;
        break;
      case 6:
        display_picture(KEY_SIX, 305 + index_offset, 165, false);
        index_offset += 20;
        break;
      case 7:
        display_picture(KEY_SEVEN, 305 + index_offset, 165, false);
        index_offset += 20;
        break;
      case 8:
        display_picture(KEY_EIGHT, 305 + index_offset, 165, false);
        index_offset += 20;
        break;
      case 9:
        display_picture(KEY_NINE, 305 + index_offset, 165, false);
        index_offset += 20;
        break;
      case 0:
        display_picture(KEY_ZERO, 305 + index_offset, 165, false);
        index_offset += 20;
        break;
      default:
        break;
      }
    }
    else
    {
      switch (key_value) //显示输入密码的值
      {
      case 0 ... 9:
        display_picture(KEY_STAR, 305 + index_offset, 243, false);
        index_offset += 20;
        break;
      default:
        break;
      }
    }

    printf("p->value =%d\n", p->value);
    p = p->prev;
  }
}

/**
 * 销毁栈
*/
void destroy_link_stack(link_stack_p p)
{
  if (p == NULL)
  {
    printf("无效栈\n");
    return;
  }
  key_node_p tmp = NULL;
  key_node_p q = NULL;
  q = p->top;
  while (q != NULL)
  {
    tmp = q;
    q = q->next;
    free(tmp);
  }
  free(p);
}
/**
 * 是否在触摸键区域
 * (x1,y1),(x2,y2)
 * p: 坐标结构体指针;
*/
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

/**
 * 键盘输入
*/
int key_input(xy *p)
{
  int value;
  while (1)
  {
    get_xy(p);
    if (is_key_area(p, 100, 315, 247, 350))
    {
      value = 1;
      break;
    }
    else if (is_key_area(p, 247, 315, 394, 350))
    {
      value = 2;
      break;
    }
    else if (is_key_area(p, 394, 315, 541, 350))
    {
      value = 3;
      break;
    }
    else if (is_key_area(p, 100, 350, 247, 385))
    {
      value = 4;
      break;
    }
    else if (is_key_area(p, 247, 350, 394, 385))
    {
      value = 5;
      break;
    }
    else if (is_key_area(p, 394, 350, 541, 385))
    {
      value = 6;
      break;
    }
    else if (is_key_area(p, 100, 385, 247, 415))
    {
      value = 7;
      break;
    }
    else if (is_key_area(p, 247, 385, 394, 415))
    {
      value = 8;
      break;
    }
    else if (is_key_area(p, 394, 385, 541, 415))
    {
      value = 9;
      break;
    }
    else if (is_key_area(p, 100, 415, 541, 478))
    {
      value = 0;
      break;
    }
    else if (is_key_area(p, 541, 315, 700, 350))
    {
      value = 10; // 隐藏
      break;
    }
    else if (is_key_area(p, 541, 350, 700, 385))
    {
      value = 11; // 删除
      break;
    }
    else if (is_key_area(p, 541, 385, 700, 478))
    {
      value = 12; // 完成
      break;
    }
  }

  return value;
}

/**
 * 整型转字符串
*/
void int2str(int n, char *str)
{
  char buf[10] = "";
  int i = 0;
  int len = 0;
  // temp为n的绝对值
  int temp = n < 0 ? -n : n;

  // 如果str为空，直接返回
  if (str == NULL)
  {
    return;
  }
  //把tmp的每一位的数存入buf
  // 逆序存入，如1234 -> 在buf中为4321
  while (temp)
  {
    buf[i++] = (temp % 10) + '0';
    temp = temp / 10;
  }
  //如果是负数多留一位放符串
  len = n < 0 ? ++i : i;
  str[i] = 0;
  //反转字符串4321- 转成 -1234
  while (1)
  {
    i--;
    if (buf[len - i - 1] == 0)
    {
      break;
    }
    str[i] = buf[len - i - 1];
  }
  // 如果是符号，加回去
  if (i == 0)
  {
    str[i] = '-';
  }
}