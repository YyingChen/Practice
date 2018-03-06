class Solution {
public:
	void replaceSpace(char *str,int length) {
		//length为最大长度
		if(str==NULL||length<0)
		{
			return ;
		}
		int oldLength=0;
		int num=0;//空白字符的个数
		int i=0;
		while(str[i]!='\0')
		{
			++oldLength;
			if(str[i]==' ')
			{
				++num;
			}
			++i;
		}
		int newLength=oldLength+2*num;
		if(newLength>length)
		{
			return;
		}
		int p1=oldLength;
		int p2=newLength;1
			while(p1>=0&&p1<p2)
			{
				if(str[p1]==' ')
				{
					str[p2--]='0';
					str[p2--]='2';
					str[p2--]='%';
				}
				else{
					str[p2--]=str[p1];
				}
				--p1;
			}

	}
};