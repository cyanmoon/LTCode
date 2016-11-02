
#include <iostream>
#include <string>
#include <vector>
#include <set>
using namespace std;
enum EnumChaType
{
    EnumChaType_Invalid = 129,
    EnumChaType_Number,
    EnumChaType_Sign,
    EnumChaType_Number_Except0,
};

class DFA
{
public:
    DFA(int num)
    {
        this->statenum = num;
        transitions.resize(this->statenum*this->statenum);
        for (auto i = 0; i < transitions.size(); ++i)
        {
            transitions[i] = INVALID_CHAR;
        }
    }

    ~DFA()
    {
        vector<int> emptyvec;
        transitions.swap(emptyvec);
        accepts.clear();
    }

    void AddTrans(int i, int j, int c)
    {
        int index = i * this->statenum + j;
        if (index >= 0 && index < transitions.size())
        {
            transitions[index] = c;
        }
    }

    int Convert(int c)
    {
        if (c >= '0' && c <= '9')
        {
            return EnumChaType_Number;
        }
        else if(c == '+' || c == '-')
        {
            return EnumChaType_Sign;
        }

        return c;
    }

    int GetTrans(int i, int c)
    {
        int startindex = i * statenum;
        for (int i = 0; i < statenum; ++i)
        {
            int index = startindex + i;
            if (transitions[index] == c)
            {
                return i;
            }
        }
        return INVALID_STATE;
    }

    void MarkAccept(int i)
    {
        accepts.insert(i);
    }

    bool Run(string s)
    {
        if (s.empty())
        {
            return false;
        }

        int curstate = 0;
        for (auto i = 0; i < s.size(); ++i)
        {
            char c = s.at(i);
            auto tmpstate = GetTrans(curstate, c);
            if (tmpstate == INVALID_STATE)
            {
                tmpstate = GetTrans(curstate, Convert(c));
            }
            if (tmpstate == INVALID_STATE)
            {
                return false;
            }

            curstate = tmpstate;
        }

        bool finded = accepts.find(curstate) != accepts.cend();
        return finded;
    }

    string Strip(string s)
    {
        string ret = "";
        if (s.size() > 0)
        {
            int istart = 0;
            int iend = s.size();
            auto i = 0;
            auto j = s.size() - 1;
            while (i < s.size() && j >= 0 && i <= j)
            {
                bool hasblank = false;
                if (s.at(i) == ' ')
                {
                    ++i;
                    hasblank = true;
                }

                if (s.at(j) == ' ')
                {
                    --j;
                    hasblank = true;
                }

                istart = i;
                iend = j;
                if (!hasblank)
                {
                    break;;
                }
            }
            ret = s.substr(istart, iend - istart + 1);
        }
        
        return ret;
    }
    
    string Strip1(string s)
    {
        string ret = s;
        if(s.find("+") != string::npos && s.size() > 1)
        {
            char* cs = new char[s.size() + 1];
            cs[s.size()] = '\0';
            char replaced = false;
            for(auto i = 0; i < s.size(); ++i)
            {
                if(!replaced && s[i] == '+')
                {
                    cs[i] = '0';
                }
                else
                {
                    cs[i] = s[i];
                }
            }
            ret = string(cs);
            delete cs;
        }
        
        return ret;
    }
    
    int statenum;
    vector<int> transitions;
    set<int> accepts;
    const int INVALID_STATE = -1;
    const char INVALID_CHAR = -128;
};



class Solution 
{
public:
    bool isRealNumber(string s, DFA& normalnumber)
    {
        bool isreal = false;
        int dotnum = 0;
        int numbernum = 0;
        int indexofe = -1;
        int eenum = 0;
        char* newcs = new char[s.size() + 1];
        newcs[s.size()] = '\0';
        for(auto i = 0; i < s.size(); ++i)
        {
            newcs[i] = s[i];
            if(s[i] == '.')
            {
                ++dotnum;
                newcs[i] = '1';
            }
            
            else if(s[i] >= '0' && s[i] <= '9')
            {
                ++numbernum;
            }
            else if(s[i] == 'e')
            {
                indexofe = i;
                ++eenum;
            }
        }
        string news = string(newcs);
        isreal = dotnum == 1 && numbernum > 0 && normalnumber.Run(newcs);
        return isreal;
    }
    
    bool isScinum(string s, DFA& normalnumber)
    {
        bool issci = false;
        int dotnum = 0;
        int dotindex = -1;
        int numbernum = 0;
        int indexofe = -1;
        int leftnumbernum = 0;
        int eenum = 0;
        char* newcs = new char[s.size() + 1];
        newcs[s.size()] = '\0';
        for(auto i = 0; i < s.size(); ++i)
        {
            newcs[i] = s[i];
            if(s[i] == '.')
            {
                dotindex = i;
                ++dotnum;
                newcs[i] = '1';
            }
            else if(s[i] >= '0' && s[i] <= '9')
            {
                ++numbernum;
                
                if(indexofe < 0 )
                {
                    ++leftnumbernum;
                }
            }
            else if(s[i] == 'e')
            {
                indexofe = i;
                ++eenum;
            }
        }
        
        if(leftnumbernum == 0 || dotnum > 1 || dotindex > indexofe || numbernum == 0)
        {
            return false;
        }
        
        s = string(newcs);
        string lefts = s.substr(0, indexofe);
        string rights = "";
        if(indexofe < s.size() - 1)
        {
            rights = s.substr(indexofe + 1, s.size() - indexofe + 2);
        }
        cout << "lefts:"<<lefts <<" rights:"<<rights<<endl;
        issci = normalnumber.Run(lefts) && normalnumber.Run(rights);
        return issci;
    }
    
    bool isNumber(string s) 
    {
        static bool initialized = false;
        static DFA normalnumber(3);
        s = normalnumber.Strip(s);
        if(s.empty())
        {
            return false;
        }
        if (!initialized)
        {
            //normal number dfa
            normalnumber.AddTrans(0, 1, EnumChaType_Sign);
            normalnumber.AddTrans(1, 2, EnumChaType_Number);
            normalnumber.AddTrans(0, 2, EnumChaType_Number);
            normalnumber.AddTrans(2, 2, EnumChaType_Number);
            normalnumber.MarkAccept(2);
            initialized = true;
        }

        //normal number
        bool isnormalnumber = normalnumber.Run(s);
        
        
        //real number
        bool isreal = isRealNumber(s, normalnumber);
        
        //sci number
        bool isscinum = isScinum(s, normalnumber);
        if (isnormalnumber || isreal|| isscinum)
        {
            
            return true;
        }
        return false;
    }
};



int main()
{
    Solution s;
    
    string testStrs[] = {
"+4",
"-4",
"++4",
"--4",
"4000",
"4.0",
"+4.0",
"+4..0",
"-4..0",
"+4e+1",
        
"+4e-1",
"4.0e1.0",
"+4.0e3.0",
"+4..0e3.0",
"-4..0",
"-.3",
".3",
"3.",
"-3.",
".",
        
" .",
". ",
".e1",
".-4",
"-.4",
"+.8",
"+.8",
"+2",
"+2e+8",
"+2e-7",
"+2.3e-7",
"+2.3e-7.0",
".3e6",
"3.e6",

"1 .",
"+++",
"...",
    };
    for(auto i = 0; i < 37; ++i)
    {
        cout << "index :" << i << endl;
        string sstr = testStrs[i];
        bool isnum = s.isNumber(sstr);
        cout << sstr << "  isNum? " << isnum << endl;
        cout << endl;
        cout<<endl;
    }
    
    DFA a(1);
    string oris = " 123 ";
    oris = "   .";
    cout << "oris:_" << oris << "_stripped:_" << a.Strip(oris) << "_" << endl;
    getchar();
    return 0;
}