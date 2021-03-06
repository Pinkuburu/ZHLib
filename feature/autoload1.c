// autoload.c
//wsw于2002年12月20日
//目标：连线时保存指定的上次在线时身上的物品
//wsw 2004 3修改

mapping autoload;

void clean_up_autoload() { autoload = 0; }
int is_save(object ob);

int is_save(object ob)
{
	string *item=({  //需要保存的物品文件名列表，也可根据需要设置为不保留的物品列表
						//query("changed")>=1的，保存所有属性，否则只保存文件和数量
             "/d/obj/newitem/baoshi/baoshi",
//             "/d/obj/drug/jinchuang",
//             "/d/obj/drug/hunyuandan",
//             "/d/playercity/obj/xiaomi",
//             "/d/playercity/obj/fan-xiaomi",
            
		});
	if (!ob) return 0;
	if (ob->query("changed")) return 1;
	if (ob->query_autoload()) return 1;
	if ( member_array(base_name(ob),item) >= 0) return 1;
	return 0; 		
}

void save_autoload()
{
	object *inv;
	int i, j,k,param,loop;
	mixed *klist;
	mapping list;
	string filename;


        if( !userp(this_object()) ) return;
	autoload=allocate_mapping(1000); //先预定义为1000的大小
	inv = all_inventory();
	k=0;
	for(i=0; i<sizeof(inv); i++)
	{
	  if (!is_save(inv[i])) continue;  //只保存指定的物品	
		param=inv[i]->query_amount();
		filename = base_name(inv[i])+".c";
		if( param > 0 ) {
			filename += "=" + param;
			autoload +=(["Object"+k:filename]);
		 	k++;
		 	continue;
		} //可重叠物品只保存文件和数量
        else autoload +=(["Object"+k:filename]);
        
        if ((int)inv[i]->query("changed")<1) {k++;continue;}//未经修改的物品只保存文件名
		list = (mapping)inv[i]->query_entire_dbase();//读出物品的dbase
		klist = keys(list);//关键字列表
		klist = sort_array(klist, "sort_keys", this_object());
		for(loop=0; loop<sizeof(klist); loop++)
		autoload["Object"+k+"_"+klist[loop]]=list[ klist[loop]];
		autoload["Object"+k+"_id"]=inv[i]->parse_command_id_list();
		k++;
	}
}

void restore_autoload()
{
	int i,j;
	object ob;
	string file, param, err,*key,a;
	mixed val;

	if( !mapp(autoload) ) return;
	i=0;
	while(autoload["Object"+i])//查找物品，当"Object(0~i)"存在时执行
	{
		a="Object"+i;
		if( sscanf(autoload[a], "%s=%s", file, param)!=2 ) 
		{
			file = autoload[a];
			param = "";
		}
		err = catch(ob = new(file));
          if( err || !ob  ) 
		{
		    //mon 1/20/98
		  	write("你觉得似乎失落了什么重要的东西，最好通知一下巫师。\n");
		  	log_file("AUTOLOAD", sprintf(
		    	"Fail to autoload %s of %s, error %s on %s\n",
		    	file, this_object()->query("id"), err, ctime(time())));
	 	  	i+=1;continue;
		}
		export_uid(ob);
		ob->move(this_object());
		if( sscanf(param, "%d", param)==1 )	ob->set_amount(param);
        //给物品set属性
 		key=keys(autoload);
   		key = sort_array(key, "sort_keys", this_object());
   		if (member_array(a+"_changed",key) == -1) {i++;continue;}
   		if (!autoload[a+"_changed"]) {i++;continue;}
		for(j=0; j<sizeof(key); j++)
		{
			if( sscanf(key[j],a+"_%s", file)==1 ) 
			{
				if (file!="equipped" && file!="id") ob->set(file,autoload[key[j]]);
				if (file=="id") ob->set_name(ob->query("name"),autoload[key[j]]);
			}
		}
		i=i+1;
	}
	
	clean_up_autoload();		// To save memory.
}
