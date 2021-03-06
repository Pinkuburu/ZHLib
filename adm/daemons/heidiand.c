//商业系统总控程序
//edit by smallfish

#include <ansi.h>
#include <getconfig.h>

/*
本文件为商业系统中黑店的主干程序
本文件提供以下文件调用：
/cmds/wiz/openshop.c			巫师开放黑店指令
/cmds/wiz/closeshop.c		巫师关闭黑店指令
/cmds/std/listshop.c			查询黑店列表指令
/feature/heidian_huoji.c		伙计程序接口文件
/feature/heidian.c			黑店程序接口文件

黑店以及伙计的程序存放路径：/clone/heidian
黑店伙计产用*.o格式的文件存储，存储文件存放于：/data/shop
黑店伙计继承文件：/feature/heidian_huoji.c
黑店伙计主要存储变量：
string owner:			主人ID
string shop_id:		伙计数据存盘文件名
int shop_type			伙计状态（0表示关闭）
mapping vendor_goods:	货物价格，keys为货物的base_name
mapping vendor_goods_num:	单个货物数量，keys为货物的base_name
int all_vendor_goods:	所有货物数量总和
mapping vip:			贵宾的折扣，keys为贵宾的id
string *unwelcome:		黑户列表，keys为黑户id

黑店继承文件：/feature/heidian.c
黑店主要变量：
string owner:			主人ID
int shop_type：		黑店状态（0表示关闭）
*/

public mapping *query_heidian();						//返回黑店列表
public int is_chushihua();							//返回黑店是否全部初始化
private int check_owner(string arg);					//检查黑店主人和黑店伙计主人是否相同
public int change_owner(object me,string arg,string owner);		//修改黑店店主（黑店主人和黑店伙计主人）
public int is_owner(string owner);						//查看玩家owner是否黑店店主
public int close_all(object me);						//关闭所有黑店
public int close_heidian(object me,string arg);				//关闭指定黑店
public int open_all(object me);						//开放所有黑店
public int open_heidian(object me,string arg);				//开放指定黑店
public int reset_all(object me);						//初始化所有黑店
public int reset_heidian(object me,string arg);				//初始化指定黑店
public int list_heidian(object me);					//查询黑店（供listshop指令调用）
public string do_modify(object obj,object me,string arg);		//修改黑店伙计的属性
public string do_stock(object ob,object me,string arg);		//标价并出售货物
public string do_unstock(object ob,object me,string arg);		//取下货物
public string do_list(object ob,object me,string arg);			//查询货物
public int do_buy(object obj,object me,string arg);			//购买货物
private int player_pay(object who, object target, int amount);	//付款，计算商业评价以及税率
public string do_jiezhang(object ob,object me);				//结帐（黑店主人不在线时候的黑店收入）
public string list_vip(object ob,object me);				//查询贵宾列表
public string do_vip(object ob,object me,string arg);			//设置|取消 贵宾
public string list_unwelcome(object ob,object me);			//查询黑户列表
public string do_unwelcome(object ob,object me,string arg);		//设置|取消 黑户
private void destruct_it(object ob);					
public int do_long(object me);						//修改黑店描述
public int do_short(object me,string arg);				//修改黑店名字
private int get_long(string msg,string arg,object me);			//录入黑店描述（供do_long函数调用）
private varargs int do_saveroom(object me,object the_room);		//存储黑店文件
private int save_room_file(string yn, string file, string str, object me);	//将黑店文件写入硬盘
private int update(object me, string file);				//重新编译黑店

string *special_props = ({
    "short", "long" });

static mapping *all_heidian = ({
        ([
                "id":             "beijing",
                "name":		"北京",
                "start_room":     "beijing_heidian",
                "huoji":          "beijing_huoji",
                "type":           "heidian",
                "price":		3000,
        ]),
        ([
                "id":             "dali",
                "name":		"大理",
                "start_room":     "dali_heidian",
                "huoji":          "dali_huoji",
                "type":           "heidian",
                "price":		3000,
        ]),
        ([
                "id":             "yangzhou",
                "name":		"扬州",
                "start_room":     "yangzhou_heidian",
                "huoji":          "yangzhou_huoji",
                "type":           "heidian",
                "price":		3000,
        ]),
        ([
                "id":             "foshan",
                "name":		"佛山",
                "start_room":     "foshan_heidian",
                "huoji":          "foshan_huoji",
                "type":           "heidian",
                "price":		3000,
        ]),
        ([
                "id":             "ruzhou",
                "name":		"汝州",
                "start_room":     "ruzhou_heidian",
                "huoji":          "ruzhou_huoji",
                "type":           "heidian",
                "price":		3000,
        ]),
        ([
                "id":             "chengdu",
                "name":		"成都",
                "start_room":     "chengdu_heidian",
                "huoji":          "chengdu_huoji",
                "type":           "heidian",
                "price":		2000,
        ]),
});

public mapping *query_heidian()
{
        return all_heidian;
}

public int is_chushihua()
{
        int i,is_ok;
        object heidian,huoji;

        is_ok = 1;
        for (i = 0;i < sizeof(all_heidian);i++)
        {
                heidian = load_object(HEIDIAN_DIR + all_heidian[i]["start_room"]);
                huoji = load_object(HEIDIAN_DIR + all_heidian[i]["huoji"]);
                if (!heidian || !huoji || heidian->query("shop_type") || huoji->query("shop_type"))
                {
                        is_ok = 0;
                        break;
                }
                else
                {
                        if ((huoji->query("vendor_goods") && sizeof(huoji->query("vendor_goods")) > 0) ||
                             huoji->query("all_vendor_goods") ||
                             heidian->query("owner") != "VOID_HEIDIAN" ||
                             huoji->query("owner") != "VOID_HEIDIAN")
                        {
                                is_ok = 0;
                                break;
                        }
                }
        }
        return is_ok;
}

private int check_owner(string arg)
{
        int i,have_it;
        object the_heidian,the_huoji;
        string heidian_owner,huoji_owner;

        have_it = 0;
        for (i = 0;i < sizeof(all_heidian);i++)
        {
                if (all_heidian[i]["id"] == arg)
                {
                        have_it = 1;
                        break;
                }
        }
        if (!have_it) return 0;	//没有找到指定的黑店
        the_heidian = load_object(HEIDIAN_DIR + all_heidian[i]["start_room"]);
        if (!the_heidian) return 0;
        heidian_owner = the_heidian->query("owner");
        the_huoji = load_object(HEIDIAN_DIR + all_heidian[i]["huoji"]);
        if (!the_huoji) return 0;
        huoji_owner = the_huoji->query("owner");
        destruct(the_huoji);	//load_object产生的是个伙计文件的复制对象，为避免存储数据出错，这里摧毁
        if (heidian_owner != huoji_owner)
                return 0;
        else
                return 1;
}

public int change_owner(object me,string arg,string owner)
{
        int i,have_it;
        object the_heidian,the_huoji;

        have_it = 0;
        for (i = 0;i < sizeof(all_heidian);i++)
        {
                if (all_heidian[i]["id"] == arg)
                {
                        have_it = 1;
                        break;
                }
        }
        if (!have_it)
        {
                tell_object(me,"对不起，" + arg + "黑店并不存在。\n");
                return 1;
        }
        if (!check_owner(arg))
        {
                tell_object(me,"警告："+arg+"黑店主人和黑店伙计主人数据异常，请仔细检查！\n");
                log_file("static/heidian",sprintf("%s黑店数据异常，时间：%s。\n",arg,ctime(time())));
                return 1;
        }
        the_huoji = load_object(HEIDIAN_DIR + all_heidian[i]["huoji"]);
        if (the_huoji)
        {
                the_huoji->set("owner",owner);
                the_huoji->save();
                destruct(the_huoji);
                tell_object(me,arg + "黑店伙计主人修改完毕！\n");
        } else
        {
                tell_object(me,arg + "黑店伙计文件不存在，请仔细查看！\n");
                return 1;
        }
        the_heidian = load_object(HEIDIAN_DIR + all_heidian[i]["start_room"]);
        if (the_heidian)
        {
                the_heidian->set("owner",owner);
                do_saveroom(me,the_heidian);
                tell_object(me,arg + "黑店店主修改完毕！\n");
        } else
        {
                tell_object(me,arg + "黑店文件不存在，请仔细查看！\n");
                return 1;
        }
        return 1;
}

public int is_owner(string owner)
{
        int i;
        object heidian;
        for (i = 0;i < sizeof(all_heidian);i ++)
        {
                heidian = load_object(HEIDIAN_DIR + all_heidian[i]["start_room"]);
                if (!objectp(heidian)) return 0;
                if (heidian->query("owner") == owner) return 1;
        }
        return 0; 
}

public int close_all(object me)
{
        int i;
        for (i = 0;i < sizeof(all_heidian);i++)
                close_heidian(me,all_heidian[i]["id"]);
        return 1;
}

//关闭黑店是修改黑店以及伙计的shop_type为0
public int close_heidian(object me,string arg)
{
        int i,have_it;
        object the_heidian,the_huoji;

        have_it = 0;
        for (i = 0;i < sizeof(all_heidian);i++)
        {
                if (all_heidian[i]["id"] == arg)
                {
                        have_it = 1;
                        break;
                }
        }
        if (!have_it)
        {
                tell_object(me,"对不起，" + arg + "黑店并不存在。\n");
                return 1;
        }
        if (!check_owner(arg))
        {
                tell_object(me,"警告："+arg+"黑店主人和黑店伙计主人数据异常，请仔细检查！\n");
                log_file("static/heidian",sprintf("%s黑店数据异常，时间：%s。\n",arg,ctime(time())));
                return 1;
        }
        the_huoji = load_object(HEIDIAN_DIR + all_heidian[i]["huoji"]);
        if (the_huoji)
        {
                the_huoji->set("shop_type",0);
                the_huoji->save();
                destruct(the_huoji);
                tell_object(me,arg + "黑店伙计成功关闭！\n");
        } else
        {
                tell_object(me,arg + "黑店伙计文件不存在，请仔细查看！\n");
                return 1;
        }
        the_heidian = load_object(HEIDIAN_DIR + all_heidian[i]["start_room"]);
        if (the_heidian)
        {
                the_heidian->set("shop_type",0);
                do_saveroom(me,the_heidian);
                tell_object(me,arg + "黑店成功关闭！\n");
        } else
        {
                tell_object(me,arg + "黑店文件不存在，请仔细查看！\n");
                return 1;
        }
        return 1;
}

public int open_all(object me)
{
        int i;
        for (i = 0;i < sizeof(all_heidian);i++)
                open_heidian(me,all_heidian[i]["id"]);
        return 1;
}

//开放黑店是修改黑店以及伙计的shop_type为1
public int open_heidian(object me,string arg)
{
        int i,have_it;
        object the_heidian,the_huoji;

        have_it = 0;
        for (i = 0;i < sizeof(all_heidian);i++)
        {
                if (all_heidian[i]["id"] == arg)
                {
                        have_it = 1;
                        break;
                }
        }
        if (!have_it)
        {
                tell_object(me,"对不起，" + arg + "黑店并不存在。\n");
                return 1;
        }
        if (!check_owner(arg))
        {
                tell_object(me,"警告："+arg+"黑店主人和黑店伙计主人数据异常，请仔细检查！\n");
                log_file("static/heidian",sprintf("%s黑店数据异常，时间：%s。\n",arg,ctime(time())));
                return 1;
        }
        the_huoji = load_object(HEIDIAN_DIR + all_heidian[i]["huoji"]);
        if (the_huoji)
        {
                the_huoji->set("shop_type",1);
                the_huoji->save();
                destruct(the_huoji);
                tell_object(me,arg + "黑店伙计成功开放！\n");
        } else
        {
                tell_object(me,arg + "黑店伙计文件不存在，请仔细查看！\n");
                return 1;
        }
        the_heidian = load_object(HEIDIAN_DIR + all_heidian[i]["start_room"]);
        if (the_heidian)
        {
                the_heidian->set("shop_type",1);
                do_saveroom(me,the_heidian);
                tell_object(me,arg + "黑店成功开放！\n");
        } else
        {
                tell_object(me,arg + "黑店文件不存在，请仔细查看！\n");
                return 1;
        }
        return 1;
}

public int reset_all(object me)
{
        int i;
        for (i = 0;i < sizeof(all_heidian);i++)
                reset_heidian(me,all_heidian[i]["id"]);
        return 1;
}

//初始化黑店包括：
//关闭黑店
//设置黑店和伙计的主人为空值
//清除伙计的货物以及伙计的存款
//清除伙计的贵宾和黑户
public int reset_heidian(object me,string arg)
{
        int i,have_it;
        object the_heidian,the_huoji;

        have_it = 0;
        for (i = 0;i < sizeof(all_heidian);i++)
        {
                if (all_heidian[i]["id"] == arg)
                {
                        have_it = 1;
                        break;
                }
        }
        if (!have_it)
        {
                tell_object(me,"对不起，" + arg + "黑店并不存在。\n");
                return 1;
        }

        the_huoji = load_object(HEIDIAN_DIR + all_heidian[i]["huoji"]);
        if (the_huoji)
        {
                the_huoji->set("shop_type",0);
                the_huoji->set("owner","VOID_HEIDIAN");
                the_huoji->set("balance",0);
                the_huoji->delete("vendor_goods");
                the_huoji->delete("vendor_goods_num");
                the_huoji->delete("all_vendor_goods");
                the_huoji->delete("vip");
                the_huoji->delete("unwelcome");
                the_huoji->save();
                destruct(the_huoji);
                tell_object(me,arg + "黑店伙计成功初始化！\n");
        } else
        {
                tell_object(me,arg + "黑店伙计文件不存在，请仔细查看！\n");
                return 1;
        }
        the_heidian = load_object(HEIDIAN_DIR + all_heidian[i]["start_room"]);
        if (the_heidian)
        {
                the_heidian->set("shop_type",0);
                the_heidian->set("owner","VOID_HEIDIAN");
                do_saveroom(me,the_heidian);
                tell_object(me,arg + "黑店成功初始化！\n");
        } else
        {
                tell_object(me,arg + "黑店文件不存在，请仔细查看！\n");
                return 1;
        }
        return 1;
}

public int list_heidian(object me)
{
        int i;
        string arg,msg;
        object the_heidian;

        msg = HIC + LOCAL_MUD_NAME() + "黑店列表如下：\n" NOR;
        msg += YEL "≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡\n" NOR;
        for (i = 0;i < sizeof(all_heidian);i++)
        {
                arg = all_heidian[i]["id"];
                if (!check_owner(arg)) continue;
                the_heidian = load_object(HEIDIAN_DIR + all_heidian[i]["start_room"]);
                if (!the_heidian) continue;
                msg += sprintf(HIW "黑店ID：" HIY "%-12s" NOR HIW "黑店名称：" HIY "%-10s" NOR HIW "黑店主人：" HIY "%-15s" NOR HIW "黑店状态：" HIY "%s\n" NOR,
                        arg,all_heidian[i]["name"],the_heidian->query("owner"),the_heidian->query("shop_type")?"营业中":"关闭");
        }
        msg += YEL "≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡\n" NOR;
        msg += HIC "总共 " + HIR + chinese_number(sizeof(all_heidian)) + HIC + " 家黑店。\n" NOR;
        tell_object(me,msg);
        return 1;
}

public string do_modify(object obj,object me,string arg)
{
        string item, msg;
        mapping dbase;

        if (!obj->query("shop_type"))
               return "对不起，该黑店目前已经被巫师关闭！\n";
        if(!arg || sscanf(arg,"%s %s",item,msg) != 2)
	        return "SYNTAX: modify 款项 内容\n";
        if(CHINESE_D->check_control(msg))
                return "描述不可有控制符！\n";
        if(CHINESE_D->check_space(msg))
                return "描述必需不含空格！\n";
        if(CHINESE_D->check_return(msg))
                return "描述必需不含回车键！\n";
        msg = replace_string(msg, "$BLK$", BLK);
        msg = replace_string(msg, "$RED$", RED);
        msg = replace_string(msg, "$GRN$", GRN);
        msg = replace_string(msg, "$YEL$", YEL);
        msg = replace_string(msg, "$BLU$", BLU);
        msg = replace_string(msg, "$MAG$", MAG);
        msg = replace_string(msg, "$CYN$", CYN);
        msg = replace_string(msg, "$WHT$", WHT);
        msg = replace_string(msg, "$HIR$", HIR);
        msg = replace_string(msg, "$HIG$", HIG);
        msg = replace_string(msg, "$HIY$", HIY);
        msg = replace_string(msg, "$HIB$", HIB);
        msg = replace_string(msg, "$HIM$", HIM);
        msg = replace_string(msg, "$HIC$", HIC);
        msg = replace_string(msg, "$HIW$", HIW);
        msg = replace_string(msg, "$NOR$", NOR);
        msg = replace_string(msg, "$S$", BLINK);
	switch(item)
	{
        case "desc":
                if(CHINESE_D->check_length(msg) > 100)
	                return "描述太长！\n"; 
	        obj->set("long",msg+ NOR"\n");
	        obj->save();
	        return "ＯＫ！\n";
        case "nickname" :
                if(CHINESE_D->check_length(msg) > 20)
                        return "描述太长！\n";
                obj->set("nickname",msg+NOR);
                obj->save();
                return "ＯＫ！\n";
        case "title" :
                if(CHINESE_D->check_length(msg) > 20)
                        return "描述太长！\n";
                obj->set("title",msg+NOR);
                obj->save();
                return "ＯＫ！\n";
        case "name" :
                if(CHINESE_D->check_length(msg) > 10)
                        return "描述太长！\n";
                dbase = obj->query_entire_dbase();
                dbase["name"] = msg;
                obj->save();
                return "ＯＫ！\n";
        }
        return "你要修改什么？\n";
}

public string do_stock(object ob,object me,string arg)
{
        object obj,goods,obj2;
        int value;
        mapping all_goods,all_goods_num;
        
        if (!ob->query("shop_type"))
               return "对不起，该黑店目前已经被巫师关闭！\n";

        if (!arg || !sscanf(arg, "%s value %d", arg,value) == 2) 
                return "stock <货物> value * (其中 * 是以铜板作为单位的价格)\n";
        if (!value)
                return "stock <货物> value * (其中 * 是以铜板作为单位的价格)\n";
        if (value > 1500000)
                return "黑店最多标价150两黄金，你就别那么心黑了吧。\n";
        if (!(goods = present(arg,me)) || !objectp(goods)) 
                return "你身上并没有这个货物啊！\n";
        if (goods->is_no_clone())
                return "这个宝贝不能拿来卖！\n";
        if (goods->is_item_make())
                return "玩家自己制造的物品不能拿来卖！\n";
        if (goods->is_character()) 
                return "你不能贩卖人口！\n";
        if (goods->query("money_id")) 
                return "你疯了？钱也拿来出售？\n";
        if (ob->query("all_vendor_goods") >= 80) 
                return "你一次只能摆上八十个货物，请首先收起几种货物再摆上这种货物。\n";

        all_goods = ob->query("vendor_goods");
        if (!all_goods) all_goods = ([ ]);
        all_goods_num = ob->query("vendor_goods_num");
        if (!all_goods_num) all_goods_num = ([ ]);

        all_goods[base_name(goods)] = value;
        all_goods_num[base_name(goods)] += 1;

        ob->set("vendor_goods",all_goods);
        ob->set("vendor_goods_num",all_goods_num);
        ob->add("all_vendor_goods",1);

        message_vision(HIW "$N将"+ NOR + goods->name(1) + HIW +"标上" + HIY + MONEY_D->price_str(value) + HIW"的价格，开始出售。\n"NOR,me);
        ob->save();
        if (goods->query_amount() && goods->query_amount() > 1) 
        {
                goods->set_amount((int)goods->query_amount() - 1);
                obj2 = new(base_name(goods));
                obj2->set_amount(1);
                destruct(obj2);
        }
        else destruct(goods);
        return "ＯＫ！\n";
}

public string do_unstock(object ob,object me,string arg)
{
        object obj,goods;
        int i,amount,value,have_it = 0;
        mapping all_goods,all_goods_num;
        string *goods_key,ob_file;
        
        if (!ob->query("shop_type"))
               return "对不起，该黑店目前已经被巫师关闭！\n";

        if (!arg) return "unstock <货物>\n";

        if (!ob->query("all_vendor_goods")) 
                return "你现在还没有摆上任何货物！\n";

        all_goods = ob->query("vendor_goods");
        all_goods_num = ob->query("vendor_goods_num");

        if (sizeof(all_inventory(me)) >= MAX_ITEM_CARRIED)
                return "你身上的东西太多了，没法从货架上取东西下来。\n";

        all_goods = ob->query("vendor_goods");
        if (!all_goods) all_goods = ([ ]);
        all_goods_num = ob->query("vendor_goods_num");
        if (!all_goods_num) all_goods_num = ([ ]);

        goods_key = keys(all_goods);
        if (arrayp(goods_key))
                for (i = 0; i < sizeof(goods_key); i++) 
                {
                        if (goods_key[i]->id(arg)) 
                                ob_file = goods_key[i];
                        else if (filter_color(goods_key[i]->name(1)) == arg) 
                                ob_file = goods_key[i];
                }

        if (!ob_file)
                return environment(ob)->short() + " 货架上并没有这样货物，你可仔细看清楚哦。\n";

        goods = new(ob_file);

	if (!goods)
		return "对不起，此物品有问题，请联系巫师来处理！\n";

        ob->add("all_vendor_goods",-1);
        all_goods_num[base_name(goods)] -= 1;
        if (all_goods_num[base_name(goods)] == 0)
        {
                map_delete(all_goods,base_name(goods));
                map_delete(all_goods_num,base_name(goods));
        }
        message_vision(HIW "$N将" + NOR + goods->name(1) + HIW + "从货架上取下来不卖了。\n",me);
   
        ob->set("vendor_goods",all_goods);
        ob->set("vendor_goods_num",all_goods_num);
        ob->save();
        if (!goods->move(me))
        {
                goods->move(environment(me));
                tell_object(me,"你已经拿不动这么多东西了，只好把" + goods->name(1) + "放在地上。\n");
        }
        return "ＯＫ！\n";
}

public string do_list(object ob,object me,string arg)
{
        mapping goods,goods_num;
        string *gks;
        object *obs,*inv;
        string msg;
        int i,have_vendor = 0;

        mapping price;
        mapping unit;
        mapping count;
        string  short_name;
        string  prefix;
        string  *dk;

        if (!ob->query("shop_type"))
               return "对不起，该黑店目前已经被巫师关闭！\n";

        if (ob->query("unwelcome") && member_array(me->query("id"),ob->query("unwelcome")) != -1)
               return "对不起，您现在是本黑店不受欢迎的人物！\n";

        if (!ob->query("all_vendor_goods")) 
                return environment(ob)->short() + " 目前并没有出售任何货物。\n";

        goods = ob->query("vendor_goods");
        if (!goods) goods = ([]);
        goods_num = ob->query("vendor_goods_num");
        if (!goods_num) goods_num = ([]);

        count   = ([]);
        unit    = ([]);
        price   = ([]);
       
        gks = keys(goods);
        for (i = 0; i < sizeof(gks); i++)
        {
                object gob;
                call_other(gks[i], "???");
                if (! objectp(find_object(gks[i])))
                {
                        log_file("user_vendor", sprintf("No found vendor good:%s\n", gks[i]));
                        continue;
                }
                gob = find_object(gks[i]);
                short_name = gob->name(1) + "(" + gob->query("id") + ")";
                if (gob->query("base_unit"))
                        prefix = "base_";
                else
                        prefix = "";
                unit  += ([ short_name : gob->query(prefix + "unit") ]);
                price += ([ short_name : goods[gks[i]] ]);
                count += ([ short_name : goods_num[gks[i]] ]);
        }

        msg = environment(ob)->short() + " 目前出售以下物品：\n";
        msg += "-----------------------------------------------------\n";
        dk = sort_array(keys(unit), 1);
        for (i = 0; i < sizeof(dk); i++)
        {
                int p;
                p = price[dk[i]];
                msg += sprintf("%" + sprintf("%d", (30 + color_len(dk[i]))) +
                               "-s：每%s%s" CYN "(现货%s)\n" NOR, 
                               dk[i], unit[dk[i]], MONEY_D->price_str(p),
                               chinese_number(count[dk[i]]) + unit[dk[i]]);
        }
        msg += "-----------------------------------------------------\n";
        if (me->query("id") == ob->query("owner")) 
                msg += "总共" + chinese_number(ob->query("all_vendor_goods")) + "件货物。\n";

        if (ob->query("vip/" + me->query("id")))     
                msg += "您是本店贵宾，购买所有货物均享受" + HIW + chinese_number(ob->query("vip/" + me->query("id"))) + "折" + NOR + "优惠！\n";

        return msg;
}       

public int do_buy(object obj,object me,string arg)
{
        mapping goods,goods_num;
        int amount;
        int value, val_factor;
        string ob_file,*goods_key;
        object *obs,ob,env;
        string my_id;
        int i;
        
        if (!obj->query("shop_type"))
        {
               tell_object(me,"对不起，该黑店目前已经被巫师关闭！\n");
               return 1;
        }

        if (obj->query("unwelcome") && member_array(me->query("id"),obj->query("unwelcome")) != -1)
        {
               tell_object(me,"对不起，您现在是本黑店不受欢迎的人物！\n");
               return 1;
        }

        if(me->is_busy())
        {
                tell_object(me,"什么事都得等你忙完再说吧！\n");
                return 1;
        }
        if (! arg)
        {
                tell_object(me,"指令格式：buy <thing>\n");
                return 1;
        }

        if (sizeof(all_inventory(me)) >= MAX_ITEM_CARRIED)
        {
                tell_object(me,"你身上的东西太多了，先处理一下再买东西吧。\n");
                return 1;
        }

        goods = obj->query("vendor_goods");
        if (!goods) goods = ([]);
        goods_num = obj->query("vendor_goods_num");
        if (!goods_num) goods_num = ([]);

        goods_key = keys(goods);
        if (arrayp(goods_key))
                for (i = 0; i < sizeof(goods_key); i++) 
                {
                        if (goods_key[i]->id(arg)) 
                                ob_file = goods_key[i];
                        else if (filter_color(goods_key[i]->name(1)) == arg) 
                                ob_file = goods_key[i];
                }

        if (!ob_file)
        {
                tell_object(me,environment(obj)->short() + " 并没有出售这样货物。\n");
                return 1;
        }

        value = goods[ob_file];

        //如果是贵宾，则有优惠
        if (obj->query("vip/" + me->query("id")))
                value = value * obj->query("vip/" + me->query("id")) / 10;

        ob = new(ob_file);

	if (!ob)
	{
		tell_object(me,"对不起，此物品有问题，请联系巫师来处理！\n");
		return 1;
	}

        call_out("destruct_it", 0, ob);
 
        switch (player_pay(me,obj,value))
        {
        case 0:
                tell_object(me,CYN + "穷光蛋，一边呆着去！\n" NOR);
                return 1;
        case 2:
                tell_object(me,CYN + "您还有没有零钱啊？银票"+NOR+obj->name(1)+CYN+"可找不开。\n" NOR);
                return 1;
        default:
                if (ob->query_amount())
                {
                        message_vision("$N从$n那里买下了" + ob->short() + "。\n",
                                        me, obj);
                }
                else
                {
                        message_vision("$N从$n那里买下了一" + ob->query("unit") + 
                	               ob->query("name") + "。\n",
			               me, obj);
                }
                ob->move(me, 1);
                goods_num[ob_file] -= 1;
                if (goods_num[ob_file] == 0) 
                {
                        map_delete(goods,ob_file);
                        map_delete(goods_num,ob_file);
                }
                obj->set("vendor_goods",goods);
                obj->set("vendor_goods_num",goods_num);
                obj->add("all_vendor_goods",-1);
        }
        me->start_busy(1);
        obj->save();
        return 1;
}

private int player_pay(object who, object target, int amount)
{
       object t_ob, g_ob, s_ob, c_ob, owner;
       int tc, gc, sc, cc, left;
       int v;
       int pay_amount;

	seteuid(getuid());

	if (amount >= 100000 && t_ob = present("cash_money", who))
		tc = t_ob->query_amount();
	else
	{
		tc = 0;
		t_ob = 0;
	}

	if (g_ob = present("gold_money", who))
		gc = g_ob->query_amount();
	else
		gc = 0;

	if (s_ob = present("silver_money", who))
		sc = s_ob->query_amount();
	else
		sc = 0;

	if (c_ob = present("coin_money", who))
		cc = c_ob->query_amount();
	else
		cc = 0;
	
	v = cc + sc * 100 + gc * 10000;

	if (amount < 100000 && v < amount)
	{
		if (present("cash_money", who))
			return 2;
		else
			return 0;
	}

	v += tc * 100000;

	if (v < amount) 
		return 0;
	else {
		left = v - amount;
		if (tc)
		{
			tc = left / 100000;
			left %= 100000;
		}
		gc = left / 10000;
		left = left % 10000;
		sc = left / 100;
		cc = left % 100;

		if (t_ob && ! g_ob && gc)
		{
			g_ob = new(GOLD_OB);
			g_ob->move(who, 1);
		}

		if (t_ob)
			t_ob->set_amount(tc);
		if (g_ob)
			g_ob->set_amount(gc);
		else sc += (gc * 100);
		if (s_ob)
		 	s_ob->set_amount(sc);
		else if (sc) {
			s_ob = new(SILVER_OB);
			s_ob->set_amount(sc);
			s_ob->move(who, 1);
		}
		if (c_ob)
			c_ob->set_amount(cc);
		else if (cc) {
			c_ob = new(COIN_OB);
			c_ob->set_amount(cc);
			c_ob->move(who, 1);
		}

              //计算税率，每30两黄金加收5%的税
              if (amount > 1200000) 
                     pay_amount = amount*3/4;
              else if (amount > 900000)
                     pay_amount = amount*4/5;
              else if (amount > 600000)
                     pay_amount = amount*17/20;
              else if (amount > 300000)
                     pay_amount = amount*9/10;
              else pay_amount = amount*19/20;
  
              if (owner = find_player(target->query("owner")))
              {
                     owner->add("balance",pay_amount);
                     if (query_ip_number(owner) != query_ip_number(who))
                             owner->add("vendor_score",pay_amount/10000);
              } else target->add("balance",pay_amount);
              return 1;
	}
}

public string do_jiezhang(object ob,object me)
{
       int amount;
       if (!ob->query("shop_type"))
               return "对不起，该黑店目前已经被巫师关闭！\n";

       amount = (int)ob->query("balance");
       if (amount < 1)
              return "您不在的时候还没有任何的收入。\n";
       me->add("balance",amount);
       me->add("vendor_score",amount/10000);
       tell_object(me,HIY "您不在线时候的收入：" + MONEY_D->price_str(amount) + "，现已全部转入您的银号。\n");
       ob->set("balance",0);
       ob->save();
       return "ＯＫ\n";
}

public string list_vip(object ob,object me)
{
        int i;
        string msg,*vip_key;
        mapping vip;

        if (!ob->query("shop_type"))
               return "对不起，该黑店目前已经被巫师关闭！\n";

        vip = ob->query("vip");
        if (!vip || sizeof(vip) < 1)
        {
                msg = "您并没有定义任何的贵宾。\n";
                msg += "您可以通过vip <id> <num>来添加贵宾，其中num 1--9表示1--9折，num 10表示取消贵宾资格\n";
                return msg;
        }
        msg = HIY "您所定义的贵宾有如下玩家：\n" NOR;
        msg += "------------------------------\n";
        vip_key = sort_array(keys(vip),1);
        for (i = 0;i < sizeof(vip_key);i++)
        {
                msg += sprintf(HIY"%-20s  " HIW "%s折\n" NOR, vip_key[i], chinese_number(vip[vip_key[i]]));
        }
        msg += "------------------------------\n";
        msg += HIY "总共" + sizeof(vip) + "个贵宾。\n" NOR;
        msg += HIY "您可以通过vip <id> <num>来添加贵宾，其中num 1--9表示1--9折，num 10表示取消贵宾资格\n" NOR;
        return msg;
}

public string do_vip(object ob,object me,string arg)
{
        int num;

        if (!ob->query("shop_type"))
               return "对不起，该黑店目前已经被巫师关闭！\n";

        if (!sscanf(arg,"%s %d",arg,num) == 2)
                return "指令格式：vip <id> <num>来添加贵宾，其中num 1--9表示1--9折，num 10表示取消贵宾资格\n";

        if (num < 1 || num > 10)
                return "指令格式：vip <id> <num>来添加贵宾，其中num 1--9表示1--9折，num 10表示取消贵宾资格\n";

        if (num == 10)
        {
                if (!ob->query("vip/" + arg))
                        return ob->name(1) + "奇怪的看着你说：" + arg + "本来就不是我们店的贵宾啊！\n";
                ob->delete("vip/" + arg);
                ob->save();
                return ob->name(1) + "翻开帐簿用笔划了一下，抬头咯咯咯的奸笑着对你说：老板，我们店已经成功取消了" + arg + "的贵宾资格。\n";
        }
        ob->set("vip/" + arg,num);
        ob->save();
        return ob->name(1) + "翻开帐簿写了点东西，抬头对你说：老板，我们店现在开始对" + arg + "实行" + HIY + chinese_number(num) + "折" + NOR + "优惠！\n";
}

public string list_unwelcome(object ob,object me)
{
        int i;
        string msg,*unwelcome;

        if (!ob->query("shop_type"))
               return "对不起，该黑店目前已经被巫师关闭！\n";

        unwelcome = ob->query("unwelcome");
        if (!unwelcome || sizeof(unwelcome) < 1)
        {
                msg = "您并没有定义任何的黑户。\n";
                msg += "您可以通过unwelcome <id>来添加黑户，也可以通过unwelcome <id> -clear来取消指定黑户。\n";
                return msg;
        }
        msg = HIY "您所定义的黑户有如下玩家：\n" NOR;
        msg += "------------------------------\n";
        for (i = 0;i < sizeof(unwelcome);i++)
        {
                msg += sprintf(HIW "%s\n" NOR, unwelcome[i]);
        }
        msg += "------------------------------\n";
        msg += HIY "总共" + sizeof(unwelcome) + "个黑户。\n" NOR;
        msg += HIY "您可以通过unwelcome <id>来添加黑户，也可以通过unwelcome <id> -clear来取消指定黑户。\n" NOR;
        
        return msg;
}

public string do_unwelcome(object ob,object me,string arg)
{
        string the_id,type,*unwelcome;

        if (!ob->query("shop_type"))
               return "对不起，该黑店目前已经被巫师关闭！\n";
        
        unwelcome = ob->query("unwelcome");
        if (!unwelcome) unwelcome = ({});

        if (sscanf(arg,"%s %s",the_id,type) == 2 && type == "-clear")
        {
                if (member_array(the_id,unwelcome) == -1)
                        return ob->name(1) + "奇怪的看着你说：" + the_id + "并不是我们的黑户啊!\n";
                unwelcome -= ({ the_id });
                ob->set("unwelcome",unwelcome);
                ob->save();
                return ob->name(1) + "翻开帐簿用笔划了一下，抬头呵呵笑着对你说：老板，我们店已经成功取消了" + the_id + "的黑户身份。\n";
        }
        
        if (member_array(arg,unwelcome) != -1)
                return ob->name(1) + "奸笑着对你说：老板，您忘记了？" + arg + "早已经是我们的黑户啦!\n";

        unwelcome += ({ arg });
        ob->set("unwelcome",unwelcome);
        ob->save();
        return ob->name(1) + "翻开帐簿写了点东西，抬头对你说：老板，我们店现在开始将" + arg + "作为黑户处理！\n";
}

private void destruct_it(object ob)
{
        if (! ob || environment(ob))
                return;
        destruct(ob);
}

public int do_short(object me,string arg)
{
    string prop, str;
    mixed data;

    if (!environment(me)->query("shop_type"))
    {
        tell_object(me,"对不起，该黑店目前已经被巫师关闭！\n");
        return 1;
    }

    if( !arg )
    {
        tell_object(me,"指令格式：short <黑店名字>\n");
        return 1;
    }

    tell_object(me,"设置黑店名字为："+arg);
    environment(me)->set("short", arg);
    tell_object(me,"...Ok.\n");
    do_saveroom(me);
    return 1;
}

public int do_long(object me)
{
    string arg;
    arg="";

    if (!environment(me)->query("shop_type"))
    {
        tell_object(me,"对不起，该黑店目前已经被巫师关闭！\n");
        return 1;
    }

    tell_object(me,"讯息可以有好几行，行首用 . 表示结束。\n");
    tell_object(me,"------------------------------------\n");
    input_to("get_long",arg,me);
    return 1;
}

private int get_long(string msg,string arg,object me)
{
    if( msg=="."){
        environment(me)->set("long", arg);
        tell_object(me,"Ok.\n");
        do_saveroom(me);
        return 1;
    }
    arg+=msg+"\n";
    input_to("get_long",arg,me);
    return 1;
}

private varargs int do_saveroom(object me,object the_room)
{
    object env;
    string str, *name, file;
    mapping prop;
    int i;
    if (the_room) 
            env = the_room;
    else
            env = environment(me);
    file = file_name(env) + ".c";
    str = "// 黑店: " + file +@ROOM_CODE

// 黑店程序由smallfish编写

inherit ROOM;
inherit F_HEIDIAN;

void create()
{
ROOM_CODE
;
    str += sprintf("\tset(\"short\", %O);\n", env->query("short") );
    str += sprintf("\tset(\"long\", @LONG\n%sLONG\n);\n", env->query("long") );
    str += "\n\tset(\"no_fight\",1);\n";
    prop = env->query_entire_dbase();
    name = keys(prop);
    for(i=0; i<sizeof(name); i++) {
        if( member_array(name[i], special_props)!=-1 ) continue;
        str += sprintf("\tset(\"%s\", %O);\n", name[i], prop[name[i]] );
    }
    str += "\n\tsetup();\n}\n";
    // Replace current directory with __DIR__ macro to make relocatable code.
    i = strlen(file)-1;
    while( (i > 0) && (file[i]!='/')) i--;
    if( i>0 ) {
        str = replace_string(str, "\"" + file[0..i], "__DIR__\"");
    }
    str += @ROOM_CODE
 
init()
{
    if (query("owner") == this_player()->query("id"))
    {
        add_action("do_short","short");
        add_action("do_long","long");
    }
}
ROOM_CODE
;
    return save_room_file("y", file, str,me);
}

private int save_room_file(string yn, string file, string str, object me)
{
    if( strlen(yn)<1 || yn[0]!='y' ) return 1;
    seteuid(ROOT_UID);
    rm(file);
    tell_object(me,"存档 " + file + "....");
    if( write_file(file, str) ) {
        tell_object(me,"Ok.\n");
        return update(me,file);
    } else
    {
        tell_object(me,"你没有写入这个档案(" + file + ")的权利。\n");
        return 1;
    }
}

private int update(object me, string file)
{
        int i;
        object obj, *inv;
        string err;

        seteuid( geteuid(me) );

        if( !sscanf(file, "%*s.c") ) file += ".c"; 

        if( file_size(file)==-1 )
        {
                tell_object(me,"黑店档案出错，请联系巫师。\n");
                return 0;
        }

        if (obj = find_object(file)) {
                if( obj==environment(me) ) {
                        if( file_name(obj)==VOID_OB )
                        {
                                tell_object(me,"你不能在 VOID_OB里重新编译 VOID_OB。\n");
                                return 0;
                        }
                        inv = all_inventory(obj);
                        i = sizeof(inv);
                        while(i--)
                                if( userp(inv[i]) ) inv[i]->move(VOID_OB, 1);
                                else inv[i] = 0;
                }
                destruct(obj);
        }

        if (obj) 
        {
                tell_object(me,"无法清除旧程式码。\n");
                return 0;
        }

        tell_object(me,"重新编译 " + file + "：");
        err = catch( call_other(file, "???") );
        if (err)
                tell_object(me,sprintf("发生错误：\n%s\n", err) );
        else {
                tell_object(me,"成功！\n");
                if( (i=sizeof(inv)) && (obj = find_object(file))) {
                        while(i--)
                                if( inv[i] && userp(inv[i])) 
                                        inv[i]->move(obj, 1);
                }
        }
        return 1;
}
