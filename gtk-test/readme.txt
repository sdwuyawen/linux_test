1. 依赖库
  1). gtk安装
	sudo apt-get install libgtk2.0-dev
  2). gtk环境
	$ pkg-config --cflags gtk+-2.0
	-I/usr/include/gtk-2.0 -I/usr/lib/gtk-2.0/include -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/freetype2 -I/usr/include/libpng12 -I/usr/include/pixman-1  
	$ pkg-config --libs gtk+-2.0
	-lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lgdk_pixbuf-2.0 -lm -lpangocairo-1.0 -lpango-1.0 -lcairo -lgobject-2.0 -lgmodule-2.0 -ldl -lglib-2.0 

  3). 连接的函数库包括：
    GTK（-lgtk）：基于GDK 的widget函数库。
    GDK（-lgdk）：Xlib封装（Wrapper）。.
    gdk-pixbuf（-lgdk_pixbuf）：影像处理函数库。
    Pango（-lpango）：国际化文字。
    gobject（-lgobject）：GTK用以为基础的类型系统。
    gmodule（-lgmodule）：用来载入执行时期的扩充。
    GLib（-lglib）：包括各种函数，GTK基于GLib，所以你需要用到这些函数。
    Xlib（-lX11）：被GDK所使用。
    Xext（-lXext）：包括共享内存 pixmaps（shared memory pixmaps）与其他X扩充。
    math（-lm）：因各种目的而被GTK所使用的数学函数库。

	GTK基于GDK（Gnome Drawing Kit），GDK就是一个与绘图相关的开发工具箱，GDK将Xlib(X函数库)加于封装简化，像点，线，视窗，色彩等的设定，以及键盘, 鼠标等事件的支持。

2. 类型：
  1). 基本类型：
	在/usr/include/glib-2.0/glib/gtypes.h中
	typedef char   gchar;
	typedef short  gshort;
	typedef long   glong;
	typedef int    gint;
	typedef gint   gboolean;

	typedef unsigned char   guchar;
	typedef unsigned short  gushort;
	typedef unsigned long   gulong;
	typedef unsigned int    guint;

	typedef float   gfloat;
	typedef double  gdouble;

3. GObject
  1). GTK雖然使用C來撰寫，但是透過GObject函式庫，可以支援物件導向的物件封裝、繼承觀念，透過巨集還可以支援多型的觀念（至少概念上達到一些部份），一個GTK物件階層如下所示：
	GObject
	 +--GInitiallyUnowned
		 +-- GtkObject
			   +-- GtkWidget
					 +-- GtkContainer
						   +-- GtkBin
								 +-- GtkWindow

	gtk_window_new()會建立一個GtkWindow，這是一個基本的視窗物件，GtkWindow繼承自GtkBin，GtkBin繼承自 GtkContainer，GtkContainer可以容納其它widget，
	所以在GtkWindow中可以置放其它的widget，而它們全都是 GtkWidget的後代。
	
	结构体的第一个变量是父类型的变量, 类似于c++中的继承

4. GDK事件架构
  1). 根據GdkEventType來決定對所有事件作過濾處理，方 式就如上面的程式碼片段所示範的，一個GdkEvent可能對應多個GdkEventType，
	例如GdkButtonEvent可以對應的 GdkEventType有GDK_BUTTON_PRESS、GDK_2BUTTON_PRESS、GDK_3BUTTON_PRESS與GDK_BUTTON_RELEASE。

  2). GdkEvent傳播給GtkWidget，引發一個與事件相 對應的事件Signal，您可以使用g_signal_connect()函式連結該Signal與Callback函式來處理事件，
	事件類型與對應的事 件Signal名稱，基本上就是去掉GDK_名稱，轉為小寫並加上_event名稱

	如果是"event"，則代表所有的事件。多個事件類型可能會對應同一個事件Signal，GDK_BUTTON、GDK_2BUTTON、GDK_3BUTTON，在事件的Signal連結時，
	都是使用button_press_event，在Callback函式中，再根據GdkEventType加以處理，部份的事件則有特殊的方式處理，例如滑鼠拖曳事件。
	
	gtk_event_demo.c

  3). 
	事件遮罩決定了一個Widget會接收到哪些事件，不同的Widget會有不同預設的事件遮罩，您可以使用gtk_widget_set_events()來設定事件遮罩：
	void gtk_widget_set_events(GtkWidget *widget, gint events);
	
	gint gtk_widget_get_events(GtkWidget *widget);

	使用gtk_widget_set_events()要特別小心，因為您直接改變了Widget的事件遮罩，不小心的話有可能破壞了Widget的功能，您可以使用gtk_widget_events()增加一些特別事件的事件遮罩：
	void gtk_widget_add_events(GtkWidget *widget, gint events);

	gtk_widget_set_events()或gtk_widget_events()必須在Widget實現之前呼叫才有作用。

	GtkWindow預設是不接收滑鼠移動事件，您要使用gtk_widget_events()增加GDK_POINTER_MOTION遮罩，才可以捕捉滑鼠移動事件
	gtk_motion_demo.c

5. GtkHBox 與 GtkVBox
  GObject
   +----GInitiallyUnowned
         +----GtkObject
               +----GtkWidget
                     +----GtkContainer
                           +----GtkBox
                                 +----GtkVBox
                                 +----GtkHBox
                                 +----GtkButtonBox
  
  1). GtkHBox                               
    可以如下撰寫：
	GtkWidget *hbox = gtk_hbox_new(TRUE, 5);
	第一個參數決定GtkHBox中所有的元件是否平均分配空間，第二個參數則設定兩個元件之間的空間，單位是像素（Pixel）。若要將元 件加入GtkHBox中，
	則要使用gtk_box_pack_start()或gtk_box_pack_end()，前者將元件從GtkHBox的左邊 開始加入（如果是GtkVBox就是從上面），後者則加至右邊（如果是GtkVBox就是從下面），
	例如：
		GtkWidget *spinButton = gtk_spin_button_new_with_range(0.0, 100.0, 1.0);
		gtk_box_pack_start(GTK_BOX(hbox), spinButton, TRUE, TRUE, 5);
		
	第一個布局參數設定Widget是否使用所有的可用空間，設定為TRUE時，Widget的可用空間會隨著GtkHBox大小改變而改變（但Widget元件本身不變）。
	第二個參數只有在第一個參數為TRUE時才有用，可設定Widget是否填滿可用空間，設定為TRUE時，Widget的大小會隨GtkHBox大小改變而改變	
	
	gtk_hbox_demo.c
	
  2). GtkVBox		
	函数类似于GtkHBox的定义
	
	gtk_vbox_demo.c

6. 
  1). GtkFrame
	GtkContainer
	 +----GtkBin
		   +----GtkFrame  
	GtkFrame也是GtkContainer的子類，可以容納其它Widget
	要建立GtkFrame，並將元件置於其中，基本上是如下的程式碼片段：
		GtkWidget *gtkFrame = gtk_frame_new("GtkHButtonBox");
		gtk_container_add(GTK_CONTAINER(gtkFrame), hbuttonBox);
		
  2). GtkButtonBox
	GtkButtonBox則是一個可以容納按鈕群組的不可視版面配置元件，您可以使用gtk_hbutton_box_new()函式建立一個水平置放按 鈕的GtkButtonBox，或是使用gtk_vbutton_box_new()建立一個垂直建立按鈕的GtkButtonBox，在建立 GtkButtonBox之後，您可以使用gtk_button_box_set_layout()來設置按鈕的排列方式，依給定的 GtkButtonBoxStyle進行設定：
	typedef enum
	{
	  GTK_BUTTONBOX_DEFAULT_STYLE,
	  GTK_BUTTONBOX_SPREAD,
	  GTK_BUTTONBOX_EDGE,
	  GTK_BUTTONBOX_START,
	  GTK_BUTTONBOX_END,
	  GTK_BUTTONBOX_CENTER
	} GtkButtonBoxStyle;  
  
	gtk_layout_demo.c
	
  3). GtkTable
	在進行元件排版的時候，將元件以類似表格的方式排放也是很常見的排版方式，在GTK中，您可以使用GtkTable，您可以使用gtk_table_new()函式來建立：
		GtkWidget *table = gtk_table_new(3, 3, TRUE);
	上面的程式碼片段也建立3列（row）、3行（column）的表格，第三個參數則設定表格中的空間是否平均分配，所以若設定為TRUE，每一格的空間將取決於置於其中最大的元件。
	
	要將元件置於表格之中，可以使用gtk_table_attach()函式：
	void gtk_table_attach( GtkTable         *table,
						   GtkWidget        *child,
						   guint            left_attach,
						   guint            right_attach,
						   guint            top_attach,
						   guint            bottom_attach,
						   GtkAttachOptions xoptions,
						   GtkAttachOptions yoptions,
						   guint            xpadding,
						   guint            ypadding );

	這個函式中的left_attach、right_attach、top_attach、bottom_attach，決定了元件將佔據的空間	
	若要讓元件佔據左上格空間，則left_attach為0、right_attach為1、top_attach為0、bottom_attach為1，
	若要讓元件佔據右下格空間，則left_attach為1、right_attach為2、top_attach為1、bottom_attach為2，
	若要讓元件佔據底下兩格空間，則left_attach為0、right_attach為2、top_attach為1、bottom_attach為2，依此類推。
		
    gtk_table_demo.c

  4). GtkToggleButton
	gtk_toggle_button_new_with_label()、 gtk_toggle_button_new_with_mnemonic()函式來建立元件，雙態按鈕有停駐及非停駐兩種狀態，
	可以使用 gtk_toggle_button_get_active()取得雙態按鈕的狀態，也可以透過 gtk_toggle_button_set_active()直接設定按鈕為停駐或非停駐狀態，
	雙態按鈕所引發的Signal是"toggled"，若是使用gtk_toggle_button_set_active()改變了停駐狀態，則會同時引發"clicked"及"toggled"兩個 Signal。
  
  5). GtkCheckButton 與 GtkRadioButton
	GtkToggleButton
	    +----GtkCheckButton
	    +----GtkRadioButton  
	
	如：
	GtkWidget *radio1 = gtk_radio_button_new_with_label(NULL, "Linux");
	GtkWidget *radio2 = gtk_radio_button_new_with_label(
             gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio1)) , "Windows");

	或
	GtkWidget *radio1 = gtk_radio_button_new_with_label(NULL, "Linux");
	GtkWidget *radio2 = gtk_radio_button_new_with_label_from_widget(
                       GTK_RADIO_BUTTON(radio1) , "Windows");
                       
	gtk_check_radio_demo.c
	
  6). GtkEntry
	GtkEntry提供一個文字輸入欄位，可以輸入文字或是設定為一般顯示、密碼顯示或搭配 GtkEntryCompletion 製作自動完成功能等，自動完成的範例稍微複雜						
	gtk_entry_set_visibility()可以設定輸入字元是否可見，設定為FALSE的話，密碼顯示會使用遮罩字元（像是*）來回應使用者的輸入，
	您可以使用gtk_entry_set_invisible_char()設定遮罩字元。
	
	GtkEntry還可以使用gtk_entry_set_alignment()設定文字對齊方式，可設定的值為0到1，表示水平方向由左到右的對齊位置，
	也可以使用gtk_entry_set_editable()設定GtkEntry的欄位是否可編輯。
	
	gtk_entry_demo.c

  7). GtkSpinButton
	GtkWidget
	 +----GtkEntry
		   +----GtkSpinButton

	您可以使用gtk_spin_button_new()函式來新增一個GtkSpinButton：
	GtkWidget* gtk_spin_button_new(
				  GtkAdjustment *adjustment, gdouble climb_rate, guint digits);

	climb_rate是設定按下按鈕時，數值改變的加速度，為一個0.0到1.0的設定，digits則是設定顯示數值的小數位數，這個函式還需要一個GtkAdjustment，
	這個物件用以控制數值的範圍、上下限、遞增（減）值等：
	GtkObject* gtk_adjustment_new(gdouble value,          // 初始值
								  gdouble lower,          // 下界值
								  gdouble upper,          // 上界值
								  gdouble step_increment, // 遞增（減）值
								  gdouble page_increment, // 對GtkSpinButton較無意義
								  gdouble page_size);     // 對GtkSpinButton較無意義

	GtkAdjustment也可以用於其它的Widget，page_increment是按下PageDown、PageUp時的端增（減）量， page_size是元件可以顯示的大小，
	但就GtkSpinButton而言，僅step_increment的設定有意義。

	您也可以使用gtk_spin_button_new_with_range()，直接指定最小值、最大值與遞增（減）值即可，這個函式會自動產生GtkAdjustment：
	GtkWidget* gtk_spin_button_new_with_range(gdouble min,
											  gdouble max,
											  gdouble step);
  8). GtkTextView
	GtkTextView可用於顯示文字與編輯文字，其本身並不具備捲軸功能，您可以使用GtkScrolledWindow套於其上，為其增加捲軸功能，
	GtkScrolledWindow可以簡單的使用gtk_scrolled_window_new()來建立：
	GtkWidget *scrolledWindow = gtk_scrolled_window_new(NULL, NULL);

	gtk_scrolled_window_new()的兩個參數為GtkAdjustment，如果您給定為NULL，則會自動生成預設的兩個GtkAdjustment。
	要將GtkTextView加入GtkScrolledWindow可以如下：
	GtkWidget *textView = gtk_text_view_new();
	gtk_container_add(GTK_CONTAINER(scrolledWindow), textView);

	若要取得或設定GtkTextView中的文字，則要取得其內部GtkTextBuffer緩衝區物件，一個例子如下：
	GtkTextBuffer *buffer;
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));
	gtk_text_buffer_set_text(buffer, "Hello! World!", -1);

	設定文字是使用gtk_text_buffer_set_text()函式，想取得文字則是使用gtk_text_buffer_get_text()。

7. 版面元件
  1). GtkNotebook
	在版面配置上，可以會使用GtkNootbook來作功能頁的分類，它提供多個顯示頁，可以藉由上方的標籤來選擇所要的功能頁面
  
	gtk_notebook_demo.c
	
  2). GtkPaned
	GtkPaned是個版面分割元件，可以將視窗版面進行水平切割或垂直切割，您可以使用以下的函式來建立水平或垂直切割的GtkPaned：
	GtkWidget *gtk_hpaned_new(void);
	GtkWidget *gtk_vpaned_new(void);

	一個GtkPaned有左右或上下兩個區域可以加入子元件，您可以使用gtk_paned_pack1()將元件加入左邊或上面，使用gtk_paned_pack2()將元件加入右邊或下面：
	void gtk_paned_pack1(GtkPaned *paned,
						 GtkWidget *child,
						 gboolean resize,
						 gboolean shrink);
	void gtk_paned_pack2(GtkPaned *paned,
						 GtkWidget *child,
						 gboolean resize,
						 gboolean shrink);			

	gtk_paned_demo.c						

	gtk_paned_pack1()與gtk_paned_pack2()可以使用有預設參數的gtk_paned_add1()及gtk_paned_add2()來簡化。若要作較複雜的版面切割，則可以搭配垂直與水平的切割組合

	gtk_hv_paned_demo.c

 3). GtkFixed
	GtkContainer
	+----GtkFixed
	您可以使用gtk_fixed_put()來指定位置擺放元件，使用gtk_fixed_move()來移動元件至指定的位置：
	void gtk_fixed_put(GtkFixed *fixed,
					   GtkWidget *widget,
					   gint x,
					   gint y);

	void gtk_fixed_move(GtkFixed *fixed,
						GtkWidget *widget,
						gint x,
						gint y);	

	gtk_fixed_demo.c

8. 進階元件
  1). GtkHandleBox
	加入GtkHandleBox的元件，是一個可以被拿下來（torn off）的元件，或稱之為所謂的浮動元件，GtkHandleBox是GtkContainer的子類：
	GtkContainer
	 +----GtkBin
		   +----GtkHandleBox
		   
	gtk_handle_box_demo.c
	
  2). GtkMenuBar, GtkMenu 與 GtkMenuItem
	GtkMenuItem基于GtkMenuBar, 构成菜单栏的主项
	GtkMenuItem基于GtkMenu, 构成菜单栏下的子菜单

    gtk_menu_demo.c

  3). GtkCheckMenuItem、GtkRadioMenuItem 與 GtkTearoffMenuItem
  
  4). GtkUIManager
	使用撰寫程式的方式來建構選單、工具列等使用者介面，過程有時過於繁瑣，您可以使用GtkUIManager從一個或多個使用者介面定義檔讀取介面定義，
	並自動建立相對應的Gtk元件，使用者介面定義檔是一個XML檔案。
	舉個實際的例子來說，可以改寫一下 GtkMenuBar、GtkMenu 與 GtkMenuItem 中的範例，使用GtkUIManager與XML定義檔來作出相同的效果，
	若XML定義檔如下所示：
	gtk_ui_manager.xml
	
	<ui>
	  <menubar name="MenuBar">
		<menu action="File">
		  <menuitem action="Open"/>
		  <menuitem action="Save"/>
		  <separator/>
		  <menuitem action="Close"/>
		</menu>
		<menu action="Edit">
		  <menuitem action="Cut"/>
		  <menuitem action="Copy"/>
		  <menuitem action="Paste"/>
		</menu>
		<menu action="Help">
		  <menuitem action="About"/>
		</menu>
	  </menubar>
	</ui>

	"name"屬性可以讓您在建構程式的時候，依名稱來取得相對應的Gtk元件，而"action"將對應於GtkAction，您可以使用GtkActionEntry來建構GtkAction，
	GtkActionEntry的定義如下：
	typedef struct {
	  const gchar     *name;
	  const gchar     *stock_id;
	  const gchar     *label;
	  const gchar     *accelerator;
	  const gchar     *tooltip;
	  GCallback  callback;
	} GtkActionEntry;

	第一個name成員即對應定義檔中的"name"屬性，其它則為圖示、文字、快速鍵、提示與callback函式，一個設定範例如下：
	static GtkActionEntry entries[] = {
	  { "File", NULL, "_File" },
	  { "Open", GTK_STOCK_OPEN, "Open",
		 "<control>O", "Open File", G_CALLBACK(itemPressed)},
	  { "Save", GTK_STOCK_SAVE, "Save",
		 "<control>S", "Save File", G_CALLBACK(itemPressed)},
	  { "Close", GTK_STOCK_QUIT, "Close",
		 "<control>Q", "Close File", G_CALLBACK(gtk_main_quit)},
	  { "Edit", NULL, "_Edit" },
	  { "Cut", NULL, "Cut"},
	  { "Copy", NULL, "Copy"},
	  { "Paste", NULL, "Paste"},
	  { "Help", NULL, "_Help" },
	  { "About", NULL, "About" }
	};

	GtkAction被組織為GtkActionGrouop，定義了GtkActionEntry之後，您可以使用gtk_action_group_add_actions()函式將之加入GtkActionGroup之中：
	gtk_action_group_add_actions(actionGroup, entries, 10, NULL);

	接著建構GtkUIManager，並使用gtk_ui_manager_insert_action_group()加入GtkActionGroup，然後使用gtk_ui_manager_add_ui_from_file()讀取使用者介面定義檔：
	GtkUIManager *ui = gtk_ui_manager_new();
	gtk_ui_manager_insert_action_group(ui, actionGroup, 0);
	gtk_ui_manager_add_ui_from_file(ui, "gtk_ui_manager.xml", NULL);

	GtkUIManager將會自動建構相對應的Gtk元件，並依"action"設定建立相對應的GtkAction。

	若要從GtkUIManager中取得元件，則可以使用gtk_ui_manager_get_widget()並依"name"屬性之設定來取得，例如取得"MenuBar"並加入GtkVBox中：
	GtkWidget *vbox = gtk_vbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox),
		gtk_ui_manager_get_widget(ui, "/MenuBar"), FALSE, FALSE, 2);    				

  5). GtkProgressBar  
	GtkProgressBar 常用來顯示目前的工作進度，例如程式安裝、檔案複製、下載等，可以使用gtk_progress_bar_new()建立一個 GtkProgressBar，若要設定進度比例，
	可以使用gtk_progress_bar_set_fraction()函式，可設定的值為0.0到 1.0，相對應的要取得目前進度比例，可以使用gtk_progress_bar_get_fraction()函式。
	GtkProgressBar可以使用gtk_progress_bar_set_text()設定文字顯示，使用 gtk_progress_bar_get_text()取得文字，另外，您還可以使用 
	gtk_progress_bar_set_orientation()設定進度列的移動方向，可設定的值有：
		GTK_PROGRESS_LEFT_TO_RIGHT：從左向右（預設）
		GTK_PROGRESS_RIGHT_TO_LEFT：從右向左
		GTK_PROGRESS_BOTTOM_TO_TOP： 從下向上
		GTK_PROGRESS_TOP_TO_BOTTOM：從上向下

	gtk_progress_bar_demo.c
	
  6). GtkToolBar
	GtkToolBar 可以讓您製作工具列，將一些常用指令群組並依使用者需求而顯示於使用介面上，要建立GtkToolBar，只要使用gtk_toolbar_new()函 式，要插入一個項目，
	則使用gtk_toolbar_insert()，所插入的項目是GtkToolItem的實例，而GtkToolItem可以直接 使用gtk_tool_item_set_tooltip_text()設定提示文字，
	但若您要設定影像與文字，則必須知道，GtkToolItem是GtkBin的子類這個事實：
	GtkContainer
	 +----GtkBin
		   +----GtkToolItem

	所以若您要插入影像或文字，或者是其它的元件，例如按鈕等，則要類似 影像及文字按鈕 中介紹的方式自行處理。

	gtk_tool_bar_demo.c

  7). GtkStatusBar
	狀態列通常位於視窗的底部，用以顯示目前視窗操作狀況的一些簡單訊息，在GTK中的狀態列元件是GtkStatusBar，您可以使用gtk_statusbar_new()來建立。

	視窗中各個元件或操作都可以有相對應的狀態訊息，為了讓狀態列區別哪個訊息屬於哪個元件或操作，GtkStatusBar使用Context ID來加以識別，
	您可以使用gtk_statusbar_get_context_id()並給定一個名稱以取得對應的Context ID，例如：
	GtkWidget *statusbar;
	gint contextId;
	statusbar = gtk_statusbar_new();
	contextId = gtk_statusbar_get_context_id(
					GTK_STATUSBAR(statusbar), "Editor Messages");

	取得Context ID之後，若要向GtkStatusBar加入或移除狀態訊息，可以使用以下的幾個函式：
	guint gtk_statusbar_push(GtkStatusbar *statusbar,
							 guint context_id,
							 const gchar *text);

	void gtk_statusbar_pop(GtkStatusbar *statusbar,
						   guint context_id);

	void gtk_statusbar_remove(GtkStatusbar *statusbar,
							  guint context_id,
							  guint message_id);	
