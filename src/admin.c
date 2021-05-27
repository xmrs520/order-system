#include "../inc/main.h"

/**
 * 删除菜
*/
bool del_order(int index, link_order_p order_list)
{
  bool flag = false;
  flag = dele_order(order_list, index);
  return flag;
}

/**
 * 增加菜
*/
bool add_order(int index, link_order_p order_list, char *filepath)
{
  bool flag = false;
  flag = read_order_index(index, order_list, filepath);
  return flag;
}

/**
 * 管理菜单
*/
void admin_manage(link_order_p order_list, char *filepath, xy *p)
{
  int index;
  display_picture(ADMIN_MANGAGE, 0, 0, false);
  int key_value; // 键值
  int index_offset = 0;
  while (1)
  {
    display_picture(ADMIN_MANGAGE, 0, 0, false);
    get_xy(p);
    if (is_key_area(p, 30, 8, 108, 64)) //返回
    {
      return;
    }

    if (is_key_area(p, 423, 268, 575, 364))
    {
      display_picture(KEYBOARD, 100, 480 - 180, false);

      while (1)
      {
        get_xy(p);

        key_value = key_input(p);
        switch (key_value) //显示输入的值
        {
        case 1:
          display_picture(KEY_ONE, 346 + index_offset, 197, false);
          index_offset += 20;
          break;
        case 2:
          display_picture(KEY_TWO, 346 + index_offset, 197, false);
          index_offset += 20;
          break;
        case 3:
          display_picture(KEY_THREE, 346 + index_offset, 197, false);
          index_offset += 20;
          break;
        case 4:
          display_picture(KEY_FOUR, 346 + index_offset, 197, false);
          index_offset += 20;
          break;
        case 5:
          display_picture(KEY_FIVE, 346 + index_offset, 197, false);
          index_offset += 20;
          break;
        case 6:
          display_picture(KEY_SIX, 346 + index_offset, 197, false);
          index_offset += 20;
          break;
        case 7:
          display_picture(KEY_SEVEN, 346 + index_offset, 197, false);
          index_offset += 20;
          break;
        case 8:
          display_picture(KEY_EIGHT, 346 + index_offset, 197, false);
          index_offset += 20;
          break;
        case 9:
          display_picture(KEY_NINE, 346 + index_offset, 197, false);
          index_offset += 20;
          break;
        case 0:
          display_picture(KEY_ZERO, 346 + index_offset, 197, false);
          index_offset += 20;
          break;
        case 11:
          if (index_offset >= 0)
          {
            index_offset -= 20; //成功删除，向前偏移
            display_picture(KEY_DELET, 346 + index_offset, 197, false);
          }

          break;
        default:
          break;
        }
        if (key_value < 10 && key_value > -1)
        {
          index = key_value;
        }
        if (key_value == 12) // 输入完成
        {
          // push(id_stack, creat_key_node(key_value)); // 保存输入数据
          break;
        }
      }
      printf("add_order-- index = %d \n", index);
      if (add_order(index, order_list, filepath))
      {
        display_picture(UPDATA_SUCCESS, 305, 380, false);
        sleep(4);
      }
      else
      {
        printf("-------增加失败---------\n");
      }
    }
    else if (is_key_area(p, 206, 265, 357, 361))
    {
      display_picture(KEYBOARD, 100, 480 - 180, false);

      while (1)
      {
        get_xy(p);

        key_value = key_input(p);
        switch (key_value) //显示输入的值
        {
        case 1:
          display_picture(KEY_ONE, 346 + index_offset, 197, false);
          index_offset += 20;
          break;
        case 2:
          display_picture(KEY_TWO, 346 + index_offset, 197, false);
          index_offset += 20;
          break;
        case 3:
          display_picture(KEY_THREE, 346 + index_offset, 197, false);
          index_offset += 20;
          break;
        case 4:
          display_picture(KEY_FOUR, 346 + index_offset, 197, false);
          index_offset += 20;
          break;
        case 5:
          display_picture(KEY_FIVE, 346 + index_offset, 197, false);
          index_offset += 20;
          break;
        case 6:
          display_picture(KEY_SIX, 346 + index_offset, 197, false);
          index_offset += 20;
          break;
        case 7:
          display_picture(KEY_SEVEN, 346 + index_offset, 197, false);
          index_offset += 20;
          break;
        case 8:
          display_picture(KEY_EIGHT, 346 + index_offset, 197, false);
          index_offset += 20;
          break;
        case 9:
          display_picture(KEY_NINE, 346 + index_offset, 197, false);
          index_offset += 20;
          break;
        case 0:
          display_picture(KEY_ZERO, 346 + index_offset, 197, false);
          index_offset += 20;
          break;
        case 11:
          if (index_offset >= 0)
          {
            index_offset -= 20; //成功删除，向前偏移
            display_picture(KEY_DELET, 346 + index_offset, 197, false);
          }

          break;
        default:
          break;
        }
        if (key_value < 10 && key_value > -1)
        {
          index = key_value;
        }
        if (key_value == 12) // 输入完成
        {
          // push(id_stack, creat_key_node(key_value)); // 保存输入数据
          break;
        }
      }
      printf("order_list-- index = %d \n", index);

      if (del_order(index, order_list))
      {
        display_picture(UPDATA_SUCCESS, 305, 380, false);
        sleep(4);
      }
      else
      {
        printf("-------删除失败---------\n");
      }
    }
  }
}