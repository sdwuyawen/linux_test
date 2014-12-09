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
  1). GTK雖然使用C來撰寫，但是透過GObject函式庫，可以支援物件導向的物件封裝、繼承觀念，透過巨集(macro)還可以支援多型的觀念（至少概念上達到一些部份），一個GTK物件階層如下所示：
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
	
  9). GtkEntryCompletion
	先前看過 GtkEntry 的範例，主要是作為使用者輸入文字之用，您可以搭配GtkEntryCompletion來讓GtkEntry擁有自動完成功能，
	這需要使用到GtkListStore與GtkTreeIter，這兩個類別在 GtkComboBox 與 GtkListStore 曾經介紹過，只要您會使用 GtkEntry、
	GtkListStore與GtkTreeIter，製作自動完成就不是什麼困難的事。	
    completion = gtk_entry_completion_new();
    gtk_entry_completion_set_model(completion, GTK_TREE_MODEL(store));
    gtk_entry_completion_set_text_column (completion, 0);
    
    gtk_entrycompletion_demo.c

  10). GtkRuler
	GtkRuler 可以是一個垂直或水平外觀的尺規元件，您可以設定它的上下界，也可以讓它上面的游標跟隨滑鼠的移動，您可以使用gtk_hruler_new()或 
	gtk_vruler_new()來建立水平或垂直尺規元件，使用gtk_ruler_set_metric()可以設定度量單位，可以設定的值有 GTK_PIXELS（像素）、
	GTK_INCHES（英吋）、GTK_CENTIMETERS（公釐）。

	您可以設定GtkRuler的範圍：
	void gtk_ruler_set_range(GtkRuler *ruler,
							 gdouble lower,
							 gdouble upper,
							 gdouble position,
							 gdouble max_size);

	lower與upper用來設定尺規的上下界，position設定目前尺規上小游標的顯示位置，max_size則是用來計算尺規上可以顯示刻度及數字的詳細程度時使用，
	設定越小的數字，尺規刻度或數字會越細，設定越大的數字，尺規刻度或數字範圍會越大。

	若要讓尺規上的游標跟隨滑鼠的位置而移動，首先滑鼠移動範圍的元件必須能接受滑鼠移動事件，例如設定GtkWindow接受滑鼠移動事件：
	gtk_widget_set_events(window, GDK_POINTER_MOTION_MASK |
								  GDK_POINTER_MOTION_HINT_MASK);

	而滑鼠移動的motion_notify_event信號，必須連接到GtkRuler的motion_notify_event函式，例如：
	#define EVENT_METHOD(i, x) GTK_WIDGET_GET_CLASS(i)->x

	g_signal_connect_swapped(G_OBJECT(window), "motion_notify_event",
							 G_CALLBACK(EVENT_METHOD(hrule, motion_notify_event)),
							 hrule);

	gtkruler_demo.c      

  11). GtkAssistant
	在應用程式安裝或是使用者註冊、設定時，可以提供使用者「精靈」（Wizard）進行一些選項設定與資訊填寫，在Step by Step的過程中，
	提示使用者完成所有必要的選項設定或資訊填寫，精靈可以使用GtkAssistant類別來提供這個功能。

	GtkAssistant中每一步的畫面，要參考至一個GtkWidget，所以您可 以事先設計好一個GtkWidget，當中進行元件置放、設定版面管理、
	設定圖片、標題等，接著使用gtk_assistant_append_page ()附加為GtkAssistant的一個頁面，接著要使用gtk_assistant_set_page_type()設定好頁面類型：
		GTK_ASSISTANT_PAGE_CONTENT：一般內容頁面
		GTK_ASSISTANT_PAGE_INTRO：簡介頁面，通常是精靈的開始
		GTK_ASSISTANT_PAGE_CONFIRM：確認頁面，通常是精靈的結束
		GTK_ASSISTANT_PAGE_SUMMARY：顯示使用者的變更資訊
		GTK_ASSISTANT_PAGE_PROGRESS：進度頁面，通常是精靈中的某個步驟


	每個頁面預設的「下一步」（例如GTK_ASSISTANT_PAGE_INTRO或GTK_ASSISTANT_PAGE_PROGRESS等）或「套 用」（例如GTK_ASSISTANT_PAGE_CONFIRM）
	預設是無法作用的，您要使用 gtk_assistant_set_page_complete()並設定complete參數為TRUE，「下一步」或「套用」按鈕才會有作用。
	
	gtkassistant_demo.c
	
  12). GtkCalendar
	GtkCalendar 是個可以顯示月曆的元件，只要使用gtk_window_new()建立元件，就可以擁有一個完整的日曆元件顯示，若要設定日期，則可以使用 
	gtk_calendar_select_month()設定年及月份，使用gtk_calendar_select_day()設定日。

	使用gtk_calendar_select_month()設定時要注意的是，月份可設定的數值是從0到11，0表示1月，11表示12月，而使用gtk_calendar_select_day()
	設定值則為1到31，或是設定為0表示不選取日。

	您可以使用gtk_calendar_mark_day()、gtk_calendar_unmark_day()或 gtk_calendar_clear_marks()設定日期標記，若要得知被標記的日期資訊，
	可以透過GtkCalendar結構的成員 num_marked_dates得知有幾天被標記了，marked_date為一個陣列，可用以得知哪一天被標記了，例如：
	if (calendar->marked_date[26-1]) {
		// 日期 26 被標記了
	}

	要注意的是，陣列索引值是從0開始，所以存取marked_date時，日期實際上要減去1，才會是對應的索引。

	另外，Calendar成員中的month、year與selected_day分別表示目前看到的月、年及所選中的日，若要取得選中的年、月、日，則可以使用
	gtk_calendar_get_date()函式，您必須提供三個變數的位址給它，執行過後，三個變數中就會儲存對應的年、月、日：
	void gtk_calendar_get_date(GtkCalendar *calendar,
							   guint *year,
							   guint *month,
							   guint *day);	
    
    gtkcalendar_demo.c
    
  13). GtkDrawingArea
	GtkDrawingArea是一個用來進行繪圖的元件，繪圖的時候，是將之繪製在window成員上，在繪圖時要處理的事件有：
		realize：元件被初始時
		configure_event：元件尺寸改變時
		expose_event：元件需要重繪時


	要在GtkDrawingArea上繪圖，完整的函式內容可以參考 Drawing Primitives，基本上每個繪圖函式都會有一個GdkGC引數，它主要包括了前景色、
	背景色、線寬等資訊，您從GtkDrawingArea的GtkStyle中可以取得對應的GdkGC資訊。    
		
    gtkdrawingarea_demo.c
    
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


9. GLib
  1).GLib定義的基本資料型態可概略分為四大類：
    對應C的整數型態：gchar、gint、gshort、glong、gfloat、 gdouble。
    對應C但更易於使用的型態：gpointer、gconstpointer、 guchar、guint、gushort、gulong。
    不是標準C的新型態：gboolean、gsize、gssize。
    保證在各平台長度相同的型態：gint8、guint8、gint16、 guint16、gint32、guint32、gint64、guint64.。

  2). GTimer
	GTimer是個計時器，當您需要量測兩個執行時間點的間隔時就可以使用，例如程式執行的開始與結束時間，您可以使用g_timer_new()建立一個新的GTimer，
	若不再需要時則使用g_timer_destroy()加以毀棄。
	在g_timer_new()之後，會自動標示啟動時間，您也可以使用g_timer_start()再度標示啟動時間，並於g_timer_elapsed()被呼叫時，傳回自啟動後的時間。
	若使用g_timer_start()標示啟動時間，並使用g_timer_end()標示結束時間，則於g_timer_elapsed()被呼叫時，將傳回啟動後時間與結束時間的間隔，
	在使用g_timer_end()標示結束時間之後，您可以使用g_timer_continue()重新繼續GTimer的計時。

	g_timer_demo.c

  3). Timeout 與 Idle
	如果您要定時讓程式去作某件事，則可以使用g_timeout_add()或g_timeout_add_full()，g_timeout_add()的定義如下：
	guint g_timeout_add(guint interval,
						GSourceFunc function,
						gpointer data);

	第一個參數是時間間隔，以毫秒為單位，第二個參數是時間到的回呼函式，第三個參數是傳給回呼函式的資料，以 內建Signal 的發射與停止中的範例來說，
	可以使用g_timeout_add()改寫如下而執行結果相同:
	
	g_timeout_demo.c

	在回呼函式中，若傳回TRUE則繼續下一次計時，計時器的下一次計時，會是在回呼函式執行完畢後開始，傳回FALSE則計時器結束並自動銷毀，若您使用g_timeout_add_full()：
	guint g_timeout_add_full(gint priority,
							 guint interval,
							 GSourceFunc function,
							 gpointer data,
							 GDestroyNotify notify);
	第一個參數為時間到時的執行優先權，可以設定的優先權如下：
		G_PRIORITY_HIGH
		G_PRIORITY_DEFAULT（預設）
		G_PRIORITY_HIGH_IDLE
		G_PRIORITY_DEFAULT_IDLE
		G_PRIORITY_LOW
	最後一個參數則是計時器被移除時要執行的函式。

	相對於計時重複執行某個動作，您可以使用g_idle_add()或g_idle_add_full()函式，讓程式在沒有什麼事情作的時候（例如沒有任何使用者操作，沒有任何需要運算的程式碼時），
	也可以作一些事情，若使用g_idle_add()：
	guint g_idle_add(GSourceFunc function,
					 gpointer data);
	第一個參數是回呼函式，第二個參數是傳遞給回呼函式的資料，例如下面這個範例，在使用者不作任何事時，就會執行指定的idle函式，而按下按鈕時就執行按鈕的回呼函式：

	g_idle_demo.c
		
	同樣的，指定的idle函式若傳回FALSE則會移除idle功能，若是使用g_idle_add_full()：
	guint g_idle_add_full(gint priority,
						  GSourceFunc function,
						  gpointer data,
						  GDestroyNotify notify);

  4). GIOChannel 與 檔案處理
	在 基本檔案讀寫 中使用g_file_get_contents()、g_file_set_contents()函式，會對檔案作 整個讀取與整個寫入的動作，
	若您想要對檔案作一些逐字元、逐行讀取、附加等操作，則可以使用GIOChannel。  
	
	g_io_channel_demo.c
	
	您使用的是g_io_channel_new_file()函式來建立GIOChannel，建立時可以使用"r"、"w"、"a"、"r+"、"w+"、"a+"等檔案模式，其作用與使用 fopen() 時的模式相同。
	程式中使用的是g_io_channel_read_to_end()函式，一次讀取所有的檔案內容，您也可以使用 g_io_channel_read_chars()、g_io_channel_read_line()、 
	g_io_channel_read_line_string()等函式，來對檔案作不同的讀取動作。

  5). GIOChannel 與 Pipe
	在Linux系統中，想要在兩個處理程序之間傳送資料，必須使用pipe，您可以使用pipe()函式來開啟pipe，您要傳入兩個File Descriptor：
	gint parent_to_child[2];
	if(pipe(parent_to_child) == -1) {
		g_error("錯誤: %s\n", g_strerror(errno));
		return 1;
	}

	開啟pipe之後，任何寫入parent_to_child[1]的資料，可以從parent_to_child[1]讀得。
	在 GIOChannel 與 檔案處理 中， 看過如何使用GIOChannel 來處理檔案，在Linux中很多物件或資料都被視作檔案，所以您也可以利用GIOChannel來處理pipe的資料，
	您可以加入watch，監看 GIOChannel中的資料，當有資料進行讀寫時會發出事件，您可利用callback函式予以處理。

	您可以使用g_io_channel_unix_new()函式從pipe的File Descriptor中建立GIOChannel，例如，假設input[0]是pipe中寫出資料的File Descriptior，
	可以如下建立GIOChannel：
	GIOChannel *channel_read = g_io_channel_unix_new(input[0]);
	if(channel_read == NULL) {
		g_error("錯誤: 無法建立 GIOChannels！\n");
	}

	若要對GIOChannel進行監看，可以使用g_io_add_watch()函式，例如：
	if(!g_io_add_watch(channel_read, G_IO_IN | G_IO_HUP,
					  (GIOFunc) iochannel_read, (gpointer) data)) {
		g_error("錯誤: 無法對 GIOChannel 進行監看\n");
	}
	第二個參數是監看的條件：
		G_IO_IN：有待讀取資料
		G_IO_OUT：可寫入資料
		G_IO_PRI：有待讀取的緊急資料
		G_IO_ERR：發生錯誤
		G_IO_HUP：連接掛斷
		G_IO_NVAL：無效請求，File Descriptor沒有開啟


	第三個參數是監看條件發生時的callback函式，第四個參數是傳遞給callback的資料。

	您可以使用g_io_channel_write_chars()函式寫入字元至GIOChannel中，例如：
	GIOStatus ret_value = g_io_channel_write_chars(
							channel, text->str, -1, &length, NULL);
	if(ret_value == G_IO_STATUS_ERROR) {
		g_error("錯誤: 無法寫入 pipe！\n");
	}
	else {
		g_io_channel_flush(channel, NULL);
	}

	可使用g_io_channel_read_line()從GIOChannel中讀入資料：
	ret_value = g_io_channel_read_line(channel, &message, &length, NULL, NULL);

	if(ret_value == G_IO_STATUS_ERROR) {
		g_error("錯誤: 無法讀取！\n");
	}  

	giochannel_pipe_demo.c
	
  6). GString
	GString是GLib所提供的對字串處理的型態，GString保有字串的長度資訊，當您對GString進行插入、附加時，GString會自動調整長度，您也可以搭配一些GLib的函式來方便的處理字串。
	GString的定義如下：
	typedef struct {
	  gchar *str;
	  gsize len;   
	  gsize allocated_len;
	} GString;

	str為null結尾的C字串之參考，len為目前字串不包括null結尾的長度，allocated_len為GString所配置的緩衝區長度，如果字串長度超出這個長度會自動重新配置。
	
	您有三種方式可以建立GString：
	GString* g_string_new(const gchar *init);
	GString* g_string_new_len(const gchar *init, gssize len);
	GString* g_string_sized_new(gsize dfl_size);
	第一個函式依所給的init字串來建立適當len的GString，並保留適當的allocated_len，建立的時候是將init字元複製至 GString中。第二個函式則是指定len來建立GString，
	因為是自行指定，所以len必須超過init的長度。第三個函式則是指定 allocated_len來建立GString。	

	字串的串接可以使用g_string_append()等函式，例如：
	GString *string = g_string_new("哈囉！");
	g_string_append(string, "GTK 程式設計！");
	g_print("%s\n", string->str);

	這一段程式碼會在主控台上顯示 "哈囉！GTK 程式設計！"（以UTF8撰寫程式的話可以顯示中文），若想要在前端附加則使用g_string_prepend()等函式，
	若想要中間插入字元則使用g_string_insert()等函式。

	除了單純的附加、插入字元等函式之外，以下還有幾個常用的操作字串的函式：
	g_string_equal() 	判斷兩個GString的字元內容是否相同
	g_string_ascii_up()或g_utf8_strup() 	轉換GString中的字元為小寫
	g_string_ascii_down()或g_utf8_strdown() 	轉換GString中的字元為大寫
	g_string_printf() 	如printf()一樣的方式，在GString中格式化字串

  7). GArray、GPtrArray、GByteArray
	在處理C的陣列時，您必須處理陣列長度的問題，您可以使用GLib的GArray，並搭配各個所提供的函式，在使用陣列上會更為方便，GArray的定義如下：
	typedef struct {
	  gchar *data;
	  guint len;
	} GArray;

	您有兩種建立GArray的方式：
	GArray* g_array_sized_new(gboolean zero_terminated,
							  gboolean clear_,
							  guint element_size,
							  guint reserved_size);

	GArray* g_array_new(gboolean zero_terminated,
						gboolean clear_,
						guint element_size);

	g_array_sized_new()的第一個參數 zero_terminated設定為TRUE的話，會加入最後一個額外元素，全部的位元都設定為0，clear_設定為TRUE的話，
	陣列的全部元素會 設定為0，element_size則是用來設定每個元素的長度，reserved_size則是用以設定陣列的長度，g_array_new()則是巨集(macro)定義的簡化版本，
	預設長度為0，若加入新的元素，則自動增加陣列長度。  
	
	搭配GArray的函式還有g_array_remove_index()、g_array_sort()等，您可以參考 Arrays 的說明。

	與GArray類似的是GPtrArray，只不過GArray儲存的是數值（若是structs，則會複製至GArray中），而GPtrArray儲存的是指標，GPtrArray的定義如下：
	typedef struct {
	  gpointer *pdata;
	  guint     len;
	} GPtrArray;
	
	GByteArray則允許您儲存guint8的資料，用於儲存位元組資料，為GArray的一個簡化形式，其定義如下：
	typedef struct {
	  guint8 *data;
	  guint len;
	} GByteArray;

  8). GSList、GList
	GSList是一個單向鏈結（Link）的節點，其定義如下：
	typedef struct {
	  gpointer data;
	  GSList *next;
	} GSList;

	data是節點資料（物件）的位址資訊，next是下一個節點資料的位址資訊，搭配 GSList 的相關函式，您可以簡單的進行鏈結節點的附加、插入、刪除等動作，
	例如使用g_slist_append()、g_slist_prepend()附加節點，使用g_slist_sort()進行排序等。
	
	gslist_demo.c

	GList則是雙向鏈結，其定義如下：
	typedef struct {
	  gpointer data;
	  GList *next;
	  GList *prev;
	} GList;

	prev是指向前一個節點

  9). GHashTable
	GHashTable可以讓您以雜湊表的方式來儲存資料，儲存時指定Key演算出Hash值以決定資料儲存位置，要取回資料，也是指定Key演算出資料儲存位置，以快速取得資料。
	簡單的說，您將GHashTable當作一個有很多間房間的房子，每個房間的門有一把鑰匙，您將資料儲存至房間中時，要順便擁有一把鑰匙，下次要取回資料時，就是根據這把鑰匙取得。
	您可以使用g_hash_table_new()來建立GHashTable：
	GHashTable* g_hash_table_new(GHashFunc hash_func,
								 GEqualFunc key_equal_func);

	g_hash_table_new()要指定一個演算Hash值的函式，GLib提供了如g_int_hash()、g_str_hash()函式可以直接使用，您也可以自訂自己的演算Hash值的函式，例如：
	guint hash_func(gconstpointer key) {
		...
		return ...;
	}

	演算出Hash是決定儲存的位置，接下來要確認Key的相等性，GLib提供了如g_int_equal()及g_str_equal()函式可直接使用，同樣的，您也可以自訂函式：
	gboolean key_equal_func(gconstpointer a, gconstpointer b) {
		...
		return ...;
	}

	ghashtable_demo.c
	
  10). GTree 與 GNode
	gtree_demo.c  	

	這個程式會建立一個平衡二元樹，利用指定的key_compare_func比較Key的大小，在這邊利用 g_strcmp0()來比較字串順序，程式中插入三筆資料，
	插入的資料會自動依Key排序，所以取回時會是排序後的結果：
	key     : caterpillar
	value   : caterpillar's message!!

	key     : justin
	value   : justin's message!!

	key     : momor
	value   : momor's message!!

	GNode則是另一種允許您建立任意分枝節點的樹結構，其定義如下：
	typedef struct {
	  gpointer data;
	  GNode *next;
	  GNode *prev;
	  GNode *parent;
	  GNode *children;
	} GNode;

	其中parent、children為父子節點，prev、next是兄弟節點	
  
