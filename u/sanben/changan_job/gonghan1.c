//发放任务公函1
//配合长安府任务巡城的道具

#include <ansi.h>
inherit ITEM;



void create()
{
        set_name(NOR + HIG "长安府公函" NOR, ({ "gong han", "gonghan", "han" }));
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("unit", "份");
                set("long", "这是一份长安府发放的公函，可以用(read gong han)来阅读。\n");
                set("value", 0);               
        }
        setup();
}

void init()
{
     add_action("do_read","read");     
}

int do_read( string arg )
{
    string msg, space;
    int i, ii;
    msg = "";   
    
    if ( !arg || ( arg != "gong han" && arg != "gonghan" && arg != "han") )
    return notify_fail("你要看什么？\n");

    msg = HIG"                      长安府公函\n\n"NOR;
    msg += HIY"    近日长安城内时有盗贼出没，滋挠百姓。兹派遣民间义士\n"NOR;
    msg += "                        "HIC + query("owner") + NOR HIY
           "\n    协助官府巡城捉盗，以保太平。\n";
    msg += "    需巡视各处地点如下:  \n"HIY;
    for( i=0; i < 6; i++ ) 
    {       
       for( ii=0; ii < 5; ii++ ) 
       { if (ii == 0) msg += "               ";
         if ( query("place/"+(ii+1))[(i*2)..(i*2+1)] != "") 
         msg += query("place/"+(ii+1))[(i*2)..(i*2+1)] + "    "; 
         else
         msg += "      "; 
       }     
         msg += "\n";   
    }
    for( ii=0; ii < 5; ii++ ) 
       { if (ii == 0) msg += "               ";
         if ( query("placeok/"+(ii+1)) ) 
         msg += HIG"已"NOR+ "    "; 
         else
         msg += HIW"未"NOR + "    "; 
       } 
    msg += "\n";
    for( ii=0; ii < 5; ii++ ) 
       { if (ii == 0) msg += "               ";
         msg += HIC"巡"NOR + "    "; 
       } 
    msg += "\n";
    msg += NOR HIR"                   速去速回，不得有误\n";
    msg += "                  长安知府：梁兴禄 签发\n"NOR;
    write( msg  +"\n");  
    return 1;
}