//Room: qunyulou.c



#include <ansi.h>



inherit ROOM;



int check_container(object ob);



void create()

{

        set("short", "Ⱥ��¥");

        set("long", 

"Ⱥ��¥�ǳ���������һָ�Ĵ��Ժ�����ﲻ����ͨ�������ĵط���\n"

"����㲻�Ǹ���һ���Ĳ���������㲻���۰�һ���İ������ţ������\n"

"�����������壬�����������Ҳ���롣¥�й͵Ĵ��ֱ��������������\n"

"�ţ�������ͷ�����ļ�������������ƿ���ȥ���������Ÿ�ʾ(gaoshi)\n"

"һ�ţ��ָ߹�����������\n"

HIR"             ��"HIG"              ��\n\n"NOR

HIR"             ��"HIG"              ɫ\n\n"NOR

HIR"             ʴ"HIG"              ��\n\n"NOR

HIR"             ��"HIG"              ��\n"NOR  	

        );

        set("exits", ([ /* sizeof() == 2 */

        	"north" : __DIR__"qixiang4",

        	"south" : __DIR__"qunyulou1",

        ]));

        set("item_desc", ([

        	"gaoshi": HIY @TEXT

�涨:

���������Դ�������¥��

�����������£��������Ը���

TEXT NOR

        ]) );

        set("objects", ([

        	__DIR__"npc/dashou" : 4,

        ]) );



        set("NONPC",1);

        setup();

}



