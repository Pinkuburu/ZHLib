#include <room.h>
#include <ansi.h>
#include <getconfig.h>
inherit ROOM;

#define HEIDIAN "yangzhou"
#define HEIDIAN_NAME "扬州黑店"

int do_look(string arg);

void create()
{
    set("short", HIY"金色走廊"NOR);
    set("long", @LONG
这里是一条金碧辉煌的走廊，前面通往拍卖休息室，墙壁上
画着一块黑底金字的大匾(dabian)，整个走廊静谧严肃，令人不
敢大声喧哗。
LONG );

    set("no_fight",1);
    set("no_beg",1);
    set("no_steal",1);

    set("exits", ([
        "south"  : __DIR__"pm_road5",
        "north"  : __DIR__"pm_restroom",
    ]));

    set("no_clean_up", 0);
    setup();
}

void init()
{
    add_action("do_look","look");
}

int do_look(string arg)
{
    int i;
    object pms;
    string msg;
    mapping *heidian_record;

    if (arg != "dabian") return 0;
    pms = load_object("/adm/npc/fjtx");
    heidian_record = pms->query(HEIDIAN);
    destruct(pms);
    msg = HIC +LOCAL_MUD_NAME()+ HIY HEIDIAN_NAME HIC " 历代店主：\n"NOR;
    msg += HIC "-------------------------------------------------------------------------\n" NOR;
    if (!sizeof(heidian_record))
        msg += HIY "目前还没有数据。\n";
    else
    {
        msg += sprintf(HIW"%s\t\t\t%-30s%-10s%s\n"NOR,"姓名","拍卖时间","拍卖师","价格");
        for (i = 0;i < sizeof(heidian_record);i++)
            msg += sprintf(HIY"%s(%s)\t\t%-30s%-10s%d\n"NOR,
                   heidian_record[i]["name"],
                   heidian_record[i]["id"],
                   ctime(heidian_record[i]["time"]),
                   heidian_record[i]["pms"],
                   heidian_record[i]["value"]
                   );
    }
    msg += HIC "-------------------------------------------------------------------------\n" NOR;
    write(msg);
    return 1;
}

int valid_leave(object me, string dir)
{
       if (dir != "north") return 1;
       if (wizardp(me)) return 1;
	if (!me->query("is_vendor"))
               return notify_fail("对不起，你并不是商人，不能入内！\n");
	if (me->query("balance")<100000000)
               return notify_fail("对不起，你的存款不足一万两黄金，还是别进去了吧。\n");
	if (me->query("vendor_score")<5000)
               return notify_fail("对不起，你的商业评价还不足5000，不能入内！\n");

	return 1;
}