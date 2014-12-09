#include <glib.h>

void for_callback(GString *string, gpointer user_data) {
    if(string) {
        printf("%s\n", string->str);
    }
}

int main(int argc, char *argv[]) {
    GString *string;
    GSList *list;
    int select; 
    char input[10];

    list = NULL; // 一開始是沒有節點的

    while(TRUE) { 
        printf(
            "\n請輸入選項(-1結束)： (1)新增至堆疊 (2)刪除頂端值 (3)顯示所有內容"); 
        printf("\n$c>"); 
        scanf("%d", &select); 
        
        if(select == -1) {
            break; 
        }

        switch(select) { 
            case 1: 
                printf("\n輸入值："); 
                scanf("%s", &input);
                string = g_string_new(input);
                list = g_slist_prepend(list, string);
                break; 
            case 2:
                string = list->data;
                list = g_slist_remove(list, string);
                printf("\n頂端值移除：%s", string->str); 
                break; 
            case 3: 
                g_slist_foreach(list, (GFunc) for_callback, NULL); 
                break; 
            default: 
                printf("\n選項錯誤！"); 
        } 
    } 

    printf("\n"); 
    
    g_slist_free(list);

    return 0; 
}
