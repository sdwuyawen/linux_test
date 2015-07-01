/*---------------------------------------
*   日期：2015-06-30
*   作者：SJF0115
*   题目：火车进站
*   来源：华为上机
-----------------------------------------*/
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#define _DEBUG_EN_

using namespace std;

void helper(string &inTrain,vector<string> &outTrain,int index){
    if(index == inTrain.size()){
        return;
    }//if
    if(index == 0){
        string outNum("");
        outNum += inTrain[index];
        outTrain.push_back(outNum);
    }//if
    else{
        vector<string> newOutTrain;
        // 出栈序列
        int size = outTrain.size();
        // 第index辆火车进栈
        for(int i = 0;i < size;++i){
            // 第i个出栈序列
            int count = outTrain[i].size();
            // 寻找前一个进栈的火车下标
            int targetIndex;
            for(int j = 0;j < count;++j){
                if(inTrain[index-1] == outTrain[i][j]){
                    targetIndex = j;
                    break;
                }//if
            }//for
            string tmp(outTrain[i]);
            for(int j = targetIndex;j <= count;++j){	//插入到j直到count(最后)的位置
                tmp.insert(tmp.begin()+j,inTrain[index]);
                newOutTrain.push_back(tmp);
                tmp.erase(tmp.begin()+j);
            }//for
        }//for
        swap(outTrain,newOutTrain);
    }//else
    
#ifdef _DEBUG_EN_    
	cout<<"======="<<index<<"======="<<inTrain[index]<<endl;
	for (int i=0; i<outTrain.size(); i++) {
		cout<<outTrain[i]<<endl;
	}
#endif
	
    helper(inTrain,outTrain,index+1);
}

vector<string> TrainLeft(string inTrain){
    vector<string> result;
    int size = inTrain.size();
    if(size <= 0){
        result;
    }//if
    helper(inTrain,result,0);
    sort(result.begin(),result.end());
    return result;
}

int main(){
    int n;
    //freopen("C:\\Users\\Administrator\\Desktop\\c++.txt","r",stdin);
    while(cin>>n){
        string train("");
        int num;
        for(int i = 1;i <= n;++i){
            cin>>num;
            train += num + '0';
        }//for
        vector<string> trainNum = TrainLeft(train);
        // 输出
        int size = trainNum.size();
        for(int i = 0;i < size;++i){
            int count = trainNum[i].size();
            for(int j = 0;j < count;++j){
                if(j == 0){
                    cout<<trainNum[i][j];
                }//if
                else{
                    cout<<" "<<trainNum[i][j];
                }//else
            }//for
            cout<<endl;
        }//for
    }//while
    return 0;
}
