class Solution {  
public:  
    bool Find(int target, vector<vector<int> > array) {  
        int rowCount=array.size();//顺序表中的每个元素都是一个顺序表，由此构成了二维数组，array.size()就是所行的大小  
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