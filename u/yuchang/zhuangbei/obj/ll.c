// 神话世界·西游记·版本４．５０
/* <SecCrypt CPL V3R05> */

#include <ansi.h>

inherit F_CLEAN_UP;

int look_item(object me, object obj);


void create() { seteuid(getuid()); }

int main(object me, string arg)
{
   object obj,target;
   int result;
   string id;

   if( !arg ) return notify_fail("你要察看什么物品？\n");
   obj=present(arg, me);
   if (!obj) obj = present(arg, environment(me));
   if (obj) return result = look_item(me, obj);
   else if (!wizardp(me)) return notify_fail("你要察看什么物品？\n");
   if (sscanf (arg, "%s in %s", arg, id) != 2) return notify_fail("你要察看什么物品？\n");
   target=find_player(id);
   if (!target)  return notify_fail("没有此生物。\n");
   obj=present(arg,target);
   if (obj) result = look_item(me, obj);
   else return notify_fail("该生物身上没有此物品。\n");
   return result;
}



int look_item(object me, object obj)
{
   int i,j;
   mixed *inv;
   string text,*str,str1,msg;
   string *bind_type=({"不绑定","装备绑定","拾取绑定","直接绑定"});
   mapping tianfu=([
        "str":"体    格",
        "con":"根    骨",
        "int":"悟    性",
        "per":"容    貌",
        "dex":"身    法",
        "kar":"福    缘",
                  ]);
   str=keys(tianfu);
   for (i=0;i<sizeof(tianfu);i++) {
                  j=obj->query("weapon_wiel/"+str[i]);
                  if (j>0) {str1=tianfu[str[i]];break;}
          }
                  
   
 //  if (obj->query("changed")<1 && obj->query("change")<1) return notify_fail("这只是一件普通物品，没什么好看的。\n");
    if (living(obj)) return notify_fail("你用一双贼眼在"+obj->query("name")+"身上比划着。。\n");
   if (obj->query("changed")<1 ) return notify_fail("这只是一件普通物品，没什么好看的。\n");
   if (obj->query("armor_prop") && obj->query("type") == "dazao")   {
                  msg=sprintf(HIM+"\n                    ※%22|s"+HIM+"※\n"+NOR,obj->short()); 
                  msg+=sprintf(HIM+repeat_string("※",34)+"\n"+NOR);
                  msg+=sprintf(HIM+"※"+NOR+HIR+"装备等级： "+HIM+chinese_number(obj->query("level"))+HIR+"级。\n"+NOR);
                  msg+=sprintf(HIM+"※"+NOR+HIR+"增加轻功级别数： "+HIM+obj->query("armor_prop/dodge")+"/30\n"+NOR);
                  msg+=sprintf(HIM+"※"+NOR+HIR+"增加物理防御力： "+HIM+obj->query("armor_prop/armor")+"/75\n"+NOR);
                  msg+=sprintf(HIM+"※"+NOR+HIR+"增加魔法攻击力： "+HIM+obj->query("armor_prop/spells")+"/30\n"+NOR);
                  msg+=sprintf(HIM+"※"+NOR+HIR+"增加内功抵抗力： "+HIM+obj->query("armor_prop/armor_vs_force")+"/700\n"+NOR);
                  msg+=sprintf(HIM+"※"+NOR+HIR+"增加魔法抵抗力： "+HIM+obj->query("armor_prop/armor_vs_spells")+"/700\n"+NOR);
                  msg+=sprintf(HIM+repeat_string("※",34)+"\n"+NOR);
   }
   else if (obj->query("weapon_prop"))
   {
                  for (i=0;i<sizeof(tianfu);i++) {
                          j=obj->query("weapon_wiel/"+str[i]);
                          if (j>0) {str1=tianfu[str[i]];break;}
                  }       
          
          msg=sprintf(HIM+"\n                    ※%22|s"+HIM+"※\n"+NOR,obj->short());
          msg+=sprintf(HIM+repeat_string("※",34)+"\n"+NOR);
          msg+=sprintf(HIM+"※"+NOR+HIR+"基本属性： "+HIM+"                                                     ※\n"+NOR); 
          msg+=sprintf(HIM+"※"+NOR+"\t"+HIC+"重    量：%10|d\t\t"+"价    值：%10|d"+HIM+"      ※\n"+NOR,obj->query_weight(),obj->query("value"));
          msg+=sprintf(HIM+"※"+NOR+"\t"+HIC+"耐 久 度：%5|d"+"/%5|d"+"\t\t"+"攻 击 力：%10|d"+HIM+"      ※\n"+NOR,obj->query("durability/now"),obj->query("durability/max"),obj->query("weapon_prop/damage"));
          msg+=sprintf(HIM+"※"+NOR+"\t"+HIC+"绑定类型：%-8s\t\t"+"绑定ＩＤ：%-10s"+HIM+"      ※\n"+NOR,bind_type[obj->query("bind_type")],obj->query("bind_id")?obj->query("bind_id"):"无");
          msg+=sprintf(HIM+"※"+NOR+HIR+"附加属性： "+HIM+"                                                     ※\n"+NOR); 
          msg+=sprintf(HIM+"※"+NOR+"\t"+HIC+"宝石插孔：%3|d"+"/%3|d"+"\t\t"+"幸 运 值：%3|d"+HIM+"             ※\n"+NOR,obj->query("sockets/now"),obj->query("sockets/max"),obj->query("weapon_prop/luck"));
          if (obj->query("cold_attack"))
          {
                  msg+=sprintf(HIM+"※"+NOR+"\t"+HIC+"属性特征：  冰    "+"\t\t最高等级：%3|d"+HIM+"             ※\n"+NOR,obj->query("cold_attack/max_level"));
                  msg+=sprintf(HIM+"※"+NOR+"\t"+HIC+"当前等级：%3|d"+"\t\t\t经 验 值：%10|d"+HIM+"      ※\n"+NOR,obj->query("weapon_prop/cold_attack"),obj->query("cold_attack/exp"));
          }
          else if(obj->query("fire_attack"))
          {
                  msg+=sprintf(HIM+"※"+NOR+"\t"+HIC+"属性特征：  火    "+"\t\t最高等级：%3|d"+HIM+"             ※\n"+NOR,obj->query("fire_attack/max_level"));
                  msg+=sprintf(HIM+"※"+NOR+"\t"+HIC+"当前等级：%3|d"+"\t\t\t经 验 值：%10|d"+HIM+"      ※\n"+NOR,obj->query("weapon_prop/fire_attack"),obj->query("fire_attack/exp"));
          }
          else if(obj->query("lightning_attack"))
          {
                  msg+=sprintf(HIM+"※"+NOR+"\t"+HIC+"属性特征：  电    "+"\t\t最高等级：%3|d"+HIM+"             ※\n"+NOR,obj->query("lightning_attack/max_level"));
                  msg+=sprintf(HIM+"※"+NOR+"\t"+HIC+"当前等级：%3|d"+"\t\t\t经 验 值：%10|d"+HIM+"      ※\n"+NOR,obj->query("weapon_prop/lightning_attack"),obj->query("lightning_attack/exp"));
          }
          else if(obj->query("poison_attack"))
          {
                  msg+=sprintf(HIM+"※"+NOR+"\t"+HIC+"属性特征：  毒    "+"\t\t最高等级：%3|d"+HIM+"             ※\n"+NOR,obj->query("poison_attack/max_level"));
                  msg+=sprintf(HIM+"※"+NOR+"\t"+HIC+"当前等级：%3|d"+"\t\t\t经 验 值：%10|d"+HIM+"      ※\n"+NOR,obj->query("weapon_prop/poison_attack"),obj->query("poison_attack/exp"));
          }
          else if(obj->query("wind_attack"))
          {
                msg+=sprintf(HIM+"※"+NOR+"\t"+HIC+"属性特征：  风    "+"\t\t最高等级：%3|d"+HIM+"             ※\n"+NOR,obj->query("wind_attack/max_level"));
                  msg+=sprintf(HIM+"※"+NOR+"\t"+HIC+"当前等级：%3|d"+"\t\t\t经 验 值：%10|d"+HIM+"      ※\n"+NOR,obj->query("weapon_prop/wind_attack"),obj->query("wind_attack/exp"));
          }
      
          msg+=sprintf(HIM+"※"+NOR+HIR+"装备要求： "+HIM+"                                                     ※\n"+NOR); 
          msg+=sprintf(HIM+"※"+NOR+"\t"+HIC+"经    验：%14s"+"点\t"+str1+"：%4|d"+HIM+"            ※\n"+NOR,chinese_number(obj->query("weapon_wiel/exp")),j);
          msg+=sprintf(HIM+"※"+NOR+"\t"+HIC+to_chinese(obj->query("skill_type",1))+"：%5|d"+"级\t\t"+"最大气血：%5|d"+HIM+"           ※\n"+NOR,obj->query("weapon_wiel/skill"),obj->query("weapon_wiel/max_qi"));   
          msg+=sprintf(HIM+repeat_string("※",34)+"\n"+NOR);
    }

    else if (obj->query("armor_prop")) 
    {
          for (i=0;i<sizeof(tianfu);i++) {
                          j=obj->query("armor_wiel/"+str[i]);
                          if (j>0) {str1=tianfu[str[i]];break;}
                  }
          msg=sprintf(HIM+"\n                   ※%22|s"+HIM+"※\n"+NOR,obj->short());
          msg+=sprintf(HIM+repeat_string("※",34)+"\n"+NOR);
          msg+=sprintf(HIM+"※"+NOR+HIR+"基本属性："+HIM+"                                                      ※\n"NOR);
          msg+=sprintf(HIM+"※"+NOR+"\t"+HIC+"重    量：%10|d"+"\t\t"+"价    值：%10|d"+HIM+"      ※\n"+NOR,obj->query_weight(),obj->query("value"));
          msg+=sprintf(HIM+"※"+NOR+"\t"+HIC+"耐 久 度：%7|d"+"/%7|d"+"\t"+"防 御 力：%7|d"+HIM+"         ※\n"+NOR,obj->query("durability/now"),obj->query("durability/max"),obj->query("armor_prop/armor"));
          msg+=sprintf(HIM+"※"+NOR+"\t"+HIC+"绑定类型：%-8s\t\t"+"绑定ＩＤ：%-10s"+HIM+"      ※\n"+NOR,bind_type[obj->query("bind_type")],obj->query("bind_id")?obj->query("bind_id"):"无");
          msg+=sprintf(HIM+"※"+NOR+HIR+"附加属性："+HIM+"                                                      ※\n"NOR); 
          msg+=sprintf(HIM+"※"+NOR+"\t"+HIC+"宝石插孔：%2|d"+"/%2|d"+"\t\t\t"+"防    风：%3|d"+"％"+HIM+"           ※\n"+NOR,obj->query("sockets/now"),obj->query("sockets/max"),obj->query("armor_prop/wind_defence"));
          msg+=sprintf(HIM+"※"+NOR+"\t"+HIC+"防    冰：%3|d"+"％\t\t\t"+"防    火：%3|d"+"％"+HIM+"           ※\n"+NOR,obj->query("armor_prop/cold_defence"),obj->query("armor_prop/fire_defence"));
          msg+=sprintf(HIM+"※"+NOR+"\t"+HIC+"防    电：%3|d"+"％\t\t\t"+"防    毒：%3|d"+"％"+HIM+"           ※\n"+NOR,obj->query("armor_prop/lightning_defence"),obj->query("armor_prop/poison_defence"));             msg+=sprintf(HIM+"※"+NOR+HIR+"装备要求： "+HIM+"                                                     ※\n"NOR); 
          msg+=sprintf(HIM+"※"+NOR+"\t"+HIC+"经    验：%14s"+"点\t"+str1+"：%4|d"+HIM+"            ※\n"+NOR,chinese_number(obj->query("armor_wiel/exp")),j);
          msg+=sprintf(HIM+repeat_string("※",34)+"\n"+NOR);
          
    }     
    else if (obj->query("prop"))
    {
          msg=sprintf(HIM+"\n                 ※%22|s"+HIM+"※\n"+NOR,obj->short());
          msg+=sprintf(HIM+repeat_string("※",34)+"\n"+NOR);
          msg+=sprintf(HIM+"※"+NOR+HIR+"属性："+HIM+"                                                          ※\n"NOR);
          msg+=sprintf(HIM+"※"+NOR+"\t"+HIC+"加攻击（防御）：%3|d"+"％"+HIM+"                                     ※\n"+NOR,obj->query("prop/damage"));
          msg+=sprintf(HIM+"※"+NOR+"\t"+HIC+"加魔攻（魔防）机率：%3|d"+"％"+HIM+"                                 ※\n"+NOR,obj->query("prop/attack"));
          msg+=sprintf(HIM+"※"+NOR+"\t"+HIC+"加幸运（插入宝石数）机率：%3|d"+"％"+HIM+"                           ※\n"+NOR,obj->query("prop/luck"));
          msg+=sprintf(HIM+repeat_string("※",34)+"\n"+NOR);
    }

   
   inv = all_inventory(obj);
   if( sizeof(inv) ) {
     inv = map_array(inv, "inventory_look", this_object() );
     message("vision", sprintf("里面有：\n  %s\n",
        implode(inv, "\n  ") ), me);
   }
   if(me->query("ll")) me->set_temp("ll",msg);
   tell_object(me,msg);
   return 1;
}



string inventory_look(object obj, int flag)
{
   string str;

   str = obj->short();
   if( obj->query("equipped") )
     str = HIC "  □" NOR + str;
   else if( !flag )
     str = "    " + str;
   else
     return 0;

   return str;
}



int help (object me)
{
   write(@HELP
 指令格式: ll [<物品>|in <生物>
 
这个指令让你查看某件特殊物品的属性，巫师可以查看别人身上的物品。
 
HELP
);
   return 1;
}


