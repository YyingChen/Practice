class Solution {  
public:  
    bool Find(int target, vector<vector<int> > array) {  
        int rowCount=array.size();//˳����е�ÿ��Ԫ�ض���һ��˳����ɴ˹����˶�ά���飬array.size()�������еĴ�С  
        int colCount=array[0].size();  
        int i,j;  
        for(i=rowCount-1,j=0;i>=0&&j<colCount;)  
        {  
            if(array[i][j]>target)  
            {  
                i--;  
                continue;  
            }  
            else if(array[i][j]<target)  
            {  
                j++;  
                continue;  
            }  
            else{  
                return true;  
            }  
              
        }  
        return false;  
    }  
};  