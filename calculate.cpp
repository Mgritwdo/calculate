#include <iostream>
//这个版本是用两个栈实时推算，最终得到一个最简的后缀表达式再进行计算
using namespace std;

typedef enum 
{
    Rank_None,
    Rank_Add,
    Rank_Sub,
    Rank_Mul,
    Rank_Div
}SIGN_Rank;     //符号优先级

template <typename T> class stack
{
    public:
        int opt=0;            //指针
        T data[100];          //数据

        //构造函数：对数组data初始化，不能直接通过T data[100]={0};赋初值
        stack()     
        {
            for(int i=0;i<100;i++)
            {
                data[i] = 0;
            }
        }

        //入栈
        void push(T charx)
        {
            data[opt++] = charx;
        }

        //出栈
        T pop()
        {
            T temp = data[(--opt)];
            data[opt] = 0;
            return temp;
        }

        //获取栈顶数据（不出栈）
        T Top()
        {
            T temp = data[(opt-1)];
            return temp;
        }

        void Print()
        {
            for(int i=0;i<opt;i++){
                cout << data[i] << '\t';
            }
            cout << endl;
        }
};

class numstack : public stack<float>{};
class charstack : public stack<char>{};

//计算两个数字的差值
float num2val(float num1, float num2, char sign)
{
    switch(sign)
    {
        case '+':   return num1+num2;
        case '-':   return num1-num2;
        case '*':   return num1*num2;
        case '/':   return num1/num2;
    }
}

//输入符号返回符号的优先级
int Sign_Rank(char Sign)
{
    switch(Sign)
    {
        case '+':   return Rank_Add;
        case '-':   return Rank_Sub;
        case '*':   return Rank_Mul;
        case '/':   return Rank_Div;
        default:    return Rank_None;
    }
}

//将字符转换成数字
int Str2Num(char* str, int len)
{
    int sum = 0;
    for(int i=0;i<len;i++)
    {
        sum *= 10;
        sum +=  str[i]-'0';
    }
    return sum;
}

int main()
{
    int cnt;                    //遍历输入公式的指针
    char str[100]= "";          //保存输入的公式
    char sign;                  //暂存符号
    int rank = Rank_None;       //符号优先级区分， 优先级从低到高分别为 + - * / 四个等级其中高优先级入栈后低优先级不能入栈，必须先将高优先级出栈
    int numlen=0;               //记录连续的数字字符长度
    int error = 0;              //错误标志

    numstack strnum;            //数字栈
    charstack strsign;          //符号栈

    cout << "请输入运算公式：" << endl;
    cin >> str ;

//数字和符号入栈
    while(str[cnt] != '\0')
    {
        if( str[cnt] >= '0' && str[cnt] <= '9' )    //数字入栈
        {
            numlen = 1;
            while( str[cnt+numlen] >= '0' && str[cnt+numlen] <= '9' ) 
                numlen++;
            //将字符转换成数字,并入栈
            strnum.push( Str2Num(&str[cnt], numlen) );        //数字入栈
            cnt+= numlen;
        }
        else if( str[cnt] == ')' )      //检测到后括号计算括号里面的数值（将括号里面的数据出栈）
        {
            while( ((sign = strsign.pop()) != '(') && (error == 0) )
            {
                //取值
                strnum.push( num2val(strnum.pop(), strnum.pop(), sign ) );
                if( strsign.opt == 0 )
                    error = 1;
            }
            if(error != 0)
                break;
            cnt++;
            rank = Sign_Rank( strsign.Top() );
        }
        else if( str[cnt] == '(' )      //检测到前括号将优先级设置为最低，防止前括号前的符号优先级很高影响后面符号的入栈
        {
            rank = Rank_None;
            strsign.push(str[cnt++]);        //符号入栈
        }
        else if( Sign_Rank( str[cnt] ) != Rank_None )        //有优先级表明这是四则运算符
        {
            if( Sign_Rank( str[cnt+1] ) != Rank_None )      //连续符号检错
            {
                error = 1;
                break;
            }
            if( Sign_Rank( str[cnt] ) <= rank )       //优先级小于栈顶的优先级
            {
                while( Sign_Rank( strsign.Top() )  >= Sign_Rank( str[cnt] ) )
                {
                    //取值
                    strnum.push( num2val(strnum.pop(), strnum.pop(), strsign.pop() ) );
                }
                rank = Sign_Rank( str[cnt] );
                strsign.push(str[cnt++]);
            }
            else
            {
                rank = Sign_Rank( str[cnt] );
                strsign.push(str[cnt++]);        //符号入栈
            }
        }
        else        //符号检错
        {
            error = 1;
            break;
        }
    }

//错误输出
    if( error != 0 )
    {
        cout << "请检查输入是否有误！！！" << endl;
        return -1;
    }

//全部都入栈后计算后缀表达式的值
    while(strsign.opt != 0)
    {
        strnum.push( num2val(strnum.pop(), strnum.pop(), strsign.pop()) );
    }

    cout << "运算结果为：" << endl;
    cout << str <<  " = " << strnum.pop();

    return 0;
}
