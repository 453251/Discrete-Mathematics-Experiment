#include <algorithm>
#include"expression.hpp"

string output_1,output_2,output_3;

bool MyCompare::operator()(const pair<char, bool>& a, const pair<char, bool>& b) const{
	return a.first < b.first;
}

set<char> vaild_sign{ '!','&','|','-','$','(',')','#' };

char cmp(const char a, const char b)
{
	if (vaild_sign.find(b) != vaild_sign.end())//判断算符是否合法
	{
		switch (a)
		{
		case '!':
			switch (b)
			{
			case '!': return '=';
			case '&':
			case '|':
			case '-':
			case ')':
			case '#':
			case '$': return '>';
			case '(': return '<';
                default:break;
			}
			break;
		case '&':
			switch (b)
			{
			case '&':
			case '|':
			case '-':
			case '$':
			case '#':
			case ')':return '>';
			case '!':
			case '(':return '<';
                default:break;
			}
			break;
		case '|':
			switch (b)
			{
			case '|':
			case '-':
			case '$':
			case '#':
			case ')': return '>';
			case '(':
			case '!':
			case '&': return '<';
                default:break;
			}
			break;
		case '-':
			switch (b)
			{
			case '-':
			case '$':
			case '#':
			case ')': return '>';
			case '(':
			case '!':
			case '&':
			case '|': return '<';
                default:break;
			}
			break;
		case '$':
			switch (b)
			{
			case '$':
			case '#':
			case ')': return '>';
			case '(':
			case '!':
			case '&':
			case '|':
			case '-': return '<';
                default:break;
			}
			break;
		case '(':
			switch (b)
			{
			case '!':
			case '&':
			case '|':
			case '-':
			case '$':
			case '(':return '<';
			case ')':return '=';
			case '#': return INVAILD;
                default:break;
			}
			break;
		case ')':
			switch (b)
			{
			case '!':
			case '&':
			case '|':
			case '-':
			case '$':
			case '#':
			case ')': return '>';
			case '(': return INVAILD;
                default:break;
			}
			break;
		case '#':
			switch (b)
			{
			case '!':
			case '&':
			case '|':
			case '-':
			case '$':
			case '(':
			case ')': return '<';
			case '#': return '=';
                default:break;
			}
			break;
            default:break;
		}
	}
	return 0;
}
//单个计算
bool operate(bool a, char chr, bool b)
{
	switch (chr)
	{
	case '&':/*a合取b*/
		return a && b;
	case '|':/*a析取b*/
		return a || b;
	case '-':/*a->b*/
		return (!a) || b;
	case '$':/*a<->b*/
		return (a && b) || ((!a) && (!b));
        default:
            break;
	}
	return true;
}
//表达式计算
bool calc(const string& sub_s)
{
    if(sub_s == "##"){
        return false;
    }
	stack<char> optr;/*操作符栈*/
	stack<bool> opnum;/*操作数栈*/
	optr.push('#');

	for (int i = 1; i < sub_s.size(); i++)
	{
		if (sub_s[i] == '0' || sub_s[i] == '1')
			opnum.push(sub_s[i] - '0');
		else
		{
		back:
			char ret = cmp(optr.top(), sub_s[i]);
			if (ret == '>')
			{
				if (optr.top() == '!')/*单目运算符*/
				{
					bool temp = opnum.top();
					opnum.pop();
					opnum.push(!temp);
				}
				else/*双目运算符*/
				{
					bool second = opnum.top();
					opnum.pop();
					bool first = opnum.top();
					opnum.pop();
					opnum.push(operate(first, optr.top(), second));
				}
				optr.pop();
				goto back;
			}
			else if (ret == '<')
				optr.push(sub_s[i]);
			else if (ret == '=')
				optr.pop();
		}
	}

	return opnum.top();
}
//判断表达式是否合法
bool judge_expression_proper(string expr)
{
	for (auto& elem : expr)
	{
		if (isalpha(elem))
			elem = '0';
	}

	/*括号匹配*/
	stack<char> bracket;

	for (int i = 0; i < expr.size(); i++)
	{
		if (expr[i] == '0')
		{
			if (i != expr.size() - 1)
				if (expr[i + 1] == '(' || expr[i + 1] == '!')
					return false;

			if (i != 0)
				if (expr[i - 1] == ')')
					return false;
		}
		else if (expr[i] == '(' || expr[i] == ')')
		{
			if (expr[i] == '(')
				bracket.push('(');
			else if (expr[i] == ')')
			{
				if (bracket.empty())
					return false;
				else
					bracket.pop();
			}
		}
		else if (expr[i] == '!')
		{
			if (i != expr.size() - 1)
			{
				if (expr[i + 1] == '&' || expr[i + 1] == '|' || expr[i + 1] == '$' || expr[i + 1] == '-' || expr[i + 1] == ')')
					return false;
			}
			else
				return false;

			if (i != 0)
				if (expr[i - 1] == ')' || expr[i - 1] == '0')
					return false;
		}
		else if (expr[i] == '&' || expr[i] == '|' || expr[i] == '-' || expr[i] == '$')
		{
			if (i != expr.size() - 1)
			{
				if (expr[i + 1] == '&' || expr[i + 1] == '|' || expr[i + 1] == '$' || expr[i + 1] == '-' || expr[i + 1] == ')')
					return false;
			}
			else
				return false;

			if (i != 0)
			{
				if (expr[i - 1] == '&' || expr[i - 1] == '|' || expr[i - 1] == '$' || expr[i + 1] == '-' || expr[i - 1] == '(' || expr[i - 1] == '!')
					return false;
			}
			else
				return false;
		}
		else
			return false;
	}
	
	if (bracket.empty())
		return true;
	else
		return false;
}

int first(const string& exp,int order)
{
    const char* expCStr = exp.c_str();
    // 使用常量字符指针来创建一个新的字符串
    string expr(expCStr);
	/*字符串预拼接  expression = '#'+expr+'#'*/
	string expression("#" + expr + "#");
	/*判断表达式是否合法*/
 /*用于存储命题变元的容器*/
    set<char> alpha;

    for (auto elem: expr)
        if (isalpha(elem))
            alpha.insert(elem);
    for (auto elem: alpha) {
        if (order == 1) {
            output_1+=elem;
            output_1+="\t";
        } else {
            output_2+=elem;
            output_2+="\t";
        }
    }
    if (order == 1) {
        output_1+="Result";
        output_1+="\r\n";
    } else {
        output_2+="Result";
        output_2+="\r\n";
    }
    /*将pow返回的浮点数类型强制转换成整型*/
    int max{static_cast<int>(pow(2, alpha.size()))};

    for (int i{}; i < max; i++) {
        string sub_s = expression;
        int num = i;
        for (auto elem: alpha) {
            /*命题变元的索引pos*/
            std::string::size_type pos;
            /*利用while循环找出elem的所有位置并进行真值指派（替换）*/
            while ((pos = sub_s.find(elem)) != std::string::npos)
                sub_s.replace(pos, 1, to_string(num % 2));
            if (order == 1) {
                output_1+=(to_string(num % 2)+"\t");
            } else {
                output_2+=(to_string(num % 2)+"\t");
            }
            num /= 2;
        }
        if (order == 1) {
            output_1+=(to_string(calc(sub_s))+"\r\n");
        } else {
            output_2+=(to_string(calc(sub_s))+"\r\n");
        }
    }
    return 0;
} //对全局数组 直接在此处调用绘图函数

int third(const string& exp,int order)
{
    string HO,XO;
    const char* expCStr = exp.c_str();
    // 使用常量字符指针来创建一个新的字符串
    string expr(expCStr);
    /*字符串预拼接  expression = '#'+expr+'#'*/
    string expression("#" + expr + "#");


    set<pair<char, char>, MyCompare> temp;
    vector<pair<char, char>> alpha;
    vector<vector<pair<char, char>>> hequ;
    vector<vector<pair<char, char>>> xiqu;
    //取出所有的命题变元名，使用set容器去重

    for (auto i: expr) {
        if (('a' <= i && i <= 'z') || ('A' <= i && i <= 'Z')) {
            temp.insert(make_pair(i, '0'));
        }
    }

    //压入vector容器中
    for (auto elem: temp) {
        alpha.push_back(elem);
    }
    int max{static_cast<int>(pow(2, alpha.size()))};

    for (int i{}; i < max; i++) {
        string sub_s = expression;
        int num = i;
        //为字母赋值
        for (auto &j: alpha) {
            j.second = static_cast<char>(num % 2 + '0');
            num /= 2;
        }
        //替换表达式
        for (auto elem: alpha) {
            std::string::size_type pos;
            while ((pos = sub_s.find(elem.first)) != std::string::npos)
                sub_s.replace(pos, 1, string{elem.second});
        }

        if (calc(sub_s))
            xiqu.push_back(alpha);
        else
            hequ.push_back(alpha);
    }
    int i, j;

    /*前n-1个大项，需要输出/\ 抽象！ 为什么 一行 也要用 这样的 注释 啊 符号*/
    for (i = 0; i < hequ.size() - 1; i++) {
        HO += "(";
        for (j = 0; j < hequ[i].size() - 1; j++) {
            if (hequ[i][j].second == '1')
                HO += "┐";
            HO += hequ[i][j].first;
            HO += "∨";
        }
        if (hequ[i][j].second == '1')
            HO += "┐";
        HO += hequ[i][j].first;
        HO += ")";
        HO += "∧";
    }
    HO += "(";
    /*最后一项单独讨论，无需输出/\符号*/
    for (j = 0; j < hequ[i].size() - 1; j++) {
        if (hequ[i][j].second == '1')
            HO += "┐";
        HO += hequ[i][j].first;
        HO += "∨";
    }
    if (hequ[i][j].second == '1')
        HO += "┐";
    HO += hequ[i][j].first;
    HO += ")";
    for (i = 0; i < xiqu.size() - 1; i++) {
        XO += '(';
        for (j = 0; j < xiqu[i].size() - 1; j++) {
            if (xiqu[i][j].second == '0')
                XO += "┐";
            XO += xiqu[i][j].first;
            XO += "∧";
        }
        if (xiqu[i][j].second == '0')
            XO += "┐";
        XO += xiqu[i][j].first;
        XO += ')';
        XO += "∨";
    }

    XO += '(';
    for (j = 0; j < xiqu[i].size() - 1; j++) {
        if (xiqu[i][j].second == '0')
            XO += "┐";
        XO += xiqu[i][j].first;
        XO += "∧";
    }
    if (xiqu[i][j].second == '0')
        XO += "┐";
    XO += xiqu[i][j].first;
    XO += ')';

    if (order == 1) {
        output_1+=("主合取范式为 "+HO+"\r\n");
        output_1+=("主合析取式为 "+XO+"\r\n");
    } else {
        output_2+=("主合取范式为 "+HO+"\r\n");
        output_2+=("主合析取式为 "+XO+"\r\n");
    }
    return 0;
} //将表达式赋值给X_,H_

int second(const string& exp)
{
    const char* expCStr = exp.c_str();
    // 使用常量字符指针来创建一个新的字符串
    string expr(expCStr);
    /*字符串预拼接  expression = '#'+expr+'#'*/
    string expression("#" + expr + "#");

    /*存储真的个数和假的个数*/
    int cnt_true = 0, cnt_false = 0;
    /*判断表达式是否合法*/

    /*用于存储命题变元的容器*/
    set<char> alpha;

    for (auto elem : expr)
        if (isalpha(elem))
            alpha.insert(elem);

    /*将pow返回的浮点数类型强制转换成整型*/
    int max{ static_cast<int>(pow(2, alpha.size())) };

    for (int i{}; i < max; i++)
    {
        string sub_s = expression;
        int num = i;

        for (auto elem : alpha)
        {
            /*命题变元的索引pos*/
            std::string::size_type pos;
            while ((pos = sub_s.find(elem)) != std::string::npos)
                sub_s.replace(pos, 1, to_string(num % 2));
            num /= 2;
        }
        if (calc(sub_s)) cnt_true++;
        else cnt_false++;
    }


    if (cnt_true == static_cast<int>(pow(2, alpha.size()))) output_3 += (expr + "永真" +"\r\n");   //若全为真，则为永真式
    else if (cnt_false == static_cast<int>(pow(2, alpha.size())))  output_3 += (expr + "永假"+"\r\n");
    else output_3 += (expr + "既不是永真式也不是永假式"+"\r\n");//若全为假，则为永假式
    return 0;
} //判断永真永假 输出数字

int fourth(const string& exp1,const string& exp2)
{
    const char* expCStr = exp1.c_str();
    // 使用常量字符指针来创建一个新的字符串
    string expression1(expCStr);
    expCStr = exp2.c_str();
    // 使用常量字符指针来创建一个新的字符串
    std::string expression2(expCStr);

    string expr1("#" + expression1 + "#");
	string expr2("#" + expression2 + "#");

	/*用于存储命题变元的容器*/
	set<char> alpha1;
	set<char> alpha2;

	for (auto elem : expr1)
		if (isalpha(elem))
			alpha1.insert(elem);
	for (auto elem : expr2)
		if (isalpha(elem))
			alpha2.insert(elem);

    std::set<char> difference;
    std::set_difference(alpha2.begin(), alpha2.end(), alpha1.begin(), alpha1.end(), std::inserter(difference, difference.begin()));

    // 如果差集为空，表示两个字符串中的字母种类相同
    if (!difference.empty()) {
        output_3 += "命题变元不同 无法比较 \r\n";
        return 1;
    }

	/*将pow返回的浮点数类型强制转换成整型*/
	int max{ static_cast<int>(pow(2, alpha1.size()>alpha2.size()?alpha1.size():alpha2.size()))};

	/*等价还是蕴含关系的判断标志*/
	int dj_flag = 1, yh_flag_1 = 1, yh_flag_2 = 1;  //蕴含关系要看是哪边蕴含哪边
	for (int i{}; i < max; i++)
	{
		string sub_s1 = expr1, sub_s2 = expr2;
		int num = i;

		for (auto elem : alpha1.size() > alpha2.size() ? alpha1 : alpha2)
		{
			/*命题变元的索引pos*/
			std::string::size_type pos1,pos2;
			/*利用while循环找出两个表达式中elem的所有位置（如果有）并进行真值指派（替换）*/
			while ((pos1 = sub_s1.find(elem)) != std::string::npos)
				sub_s1.replace(pos1, 1, to_string(num % 2));
			while ((pos2 = sub_s2.find(elem)) != std::string::npos)
				sub_s2.replace(pos2, 1, to_string(num % 2));
			num /= 2;
		}
		bool a = calc(sub_s1), b = calc(sub_s2);

		if (!(operate(a, '$', b))) dj_flag = 0;  //等价，两个表达式的值满足双条件关系
		if (!(operate(a, '-', b))) yh_flag_1 = 0; //蕴含，两个表达式的值满足条件关系
		if (!(operate(b,'-', a))) yh_flag_2 = 0;
	}

	if (dj_flag) {
        output_3+=(expression1 + "<=>"+expression2+"\n");
    }          //=>
	else if (yh_flag_1) {
        output_3+=(expression1 + "=>"+expression2+"\n");
    }    //=
	else if (yh_flag_2)  {
        output_3+=(expression2 + "=>"+expression1+"\n");
    }   //
	else {
        output_3+="两表达式无关系\n";
    }//无关系
    return 0;
} //如果两个输入框都有输入则开始判断 输出数字

bool fifth(const string& exp,int order){

    const char* expCStr = exp.c_str();
    // 使用常量字符指针来创建一个新的字符串
    string str(expCStr);

	auto check = [](const string& expr1, const string& expr2) -> bool {

        if (!judge_expression_proper(expr1)||!judge_expression_proper(expr2)) {
            return false;
        }

		string expression1("#" + expr1 + "#");
		string expression2("#" + expr2 + "#");

		/*判断表达式是否合法*/

		/*用于存储命题变元的容器*/
		set<char> alpha;

		for (auto elem : expr1)
			if (isalpha(elem))
				alpha.insert(elem);
		for (auto elem : expr2)
			if (isalpha(elem))
				alpha.insert(elem);

		/*将pow返回的浮点数类型强制转换成整型*/
		int max{ static_cast<int>(pow(2, alpha.size())) };

		for (int i{}; i < max; i++)
		{
			string sub_s1 = expression1;
			string sub_s2 = expression2;
			int num = i;

			for (auto elem : alpha)
			{
				/*命题变元的索引pos*/
				std::string::size_type pos1, pos2;
				/*利用while循环找出elem的所有位置并进行真值指派（替换）*/
				while ((pos1 = sub_s1.find(elem)) != std::string::npos)
					sub_s1.replace(pos1, 1, to_string(num % 2));
				while ((pos2 = sub_s2.find(elem)) != std::string::npos)
					sub_s2.replace(pos2, 1, to_string(num % 2));
				num /= 2;
			}
			auto f1 = calc(sub_s1), f2 = calc(sub_s2);
			if (f1 && !f2) {
                return false;
            }
		}

		return true;
	};

	int idx = static_cast<int>(str.find("=>"));
	if (idx == str.size()||idx==-1) {
        return false;
    }
	
	string expr1 = str.substr(0, idx); 
	string expr2 = str.substr(idx + 2, str.size());
	if (!check(expr1, expr2)) {
        if (order == 1) {
            output_1 += ("命题推理错误\r\n");
        } else {
            output_2 += ("命题推理错误\r\n");
        }
        return true;
    }else {
        if (order == 1) {
            output_1 += ("命题推理正确\r\n");
        } else {
            output_2 += ("命题推理正确\r\n");
        }
        return false;
    }

}//窗口字符串检测=> 判断并输出结果 输出数字