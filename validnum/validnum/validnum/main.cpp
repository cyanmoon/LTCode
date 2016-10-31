
#include <iostream>
#include <string>
#include <vector>
#include <set>
using namespace std;
enum EnumChaType
{
    EnumChaType_Invalid = 129,
    EnumChaType_Number,
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

        
//        if(s.find("+") != string::npos)
//        {
//            char* cs = new char[s.size() + 1];
//            cs[s.size()] = '\0';
//            char replaced = false;
//            for(auto i = 0; i < s.size(); ++i)
//            {
//                if(!replaced && s[i] == '+')
//                {
//                    cs[i] = '0';
//                }
//                else
//                {
//                    cs[i] = s[i];
//                }
//            }
//            ret = string(cs);
//            delete cs;
//        }
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
    bool isNumber(string s) 
    {
        static bool initialized = false;
        static DFA normalnumber(3);
        static DFA normalposnumber(2);
//        static DFA negnormalnumber(21);
//        static DFA realnumber(8);
//        static DFA scinumber(8);
//        static DFA scirealnumber(11);
        s = normalnumber.Strip(s);
        if (!initialized)
        {
            //normal number dfa
            normalnumber.AddTrans(0, 1, '-');
            normalnumber.AddTrans(1, 2, EnumChaType_Number);
            normalnumber.AddTrans(0, 2, EnumChaType_Number);
            normalnumber.AddTrans(2, 2, EnumChaType_Number);
            normalnumber.MarkAccept(2);
            
            normalposnumber.AddTrans(0, 1, EnumChaType_Number);
            normalposnumber.AddTrans(1, 1, EnumChaType_Number);
            normalposnumber.MarkAccept(1);
            initialized = true;
        }

        bool isnormalnumber = normalnumber.Run(s);
        bool isscinum = false;
        bool isreal = false;
        auto epos1 = s.find('.');
        if(epos1 != string::npos)
        {
            string s2 = s.substr(0, epos1);
            s2 = normalnumber.Strip1(s2);
            string s3 = s.substr(epos1 + 1, s.size());
            if(s3.find('.') != string::npos || (s2.empty() && s3.empty()))
            {
                isreal = false;
            }
            else
            {
                isreal = normalnumber.Run(s2) || ((s2.empty() || s2.at(0) == '-') && !s3.empty());
                isreal = isreal && (normalposnumber.Run(s3) || (s3.empty() && !s2.empty()));
            }
        }
        
        auto epos = s.find('e');
        if(epos != string::npos)
        {
            string s0 = s.substr(0, epos - 0);
            string s1 = s.substr(epos + 1, s.size() - epos - 1);
            s1 = normalnumber.Strip1(s1);
            if(s1.find('e') != string::npos || (s0.empty() || s1.empty()))
            {
                isscinum = false;
            }
            else
            {
                //isscinum = realnumber.Run(s0);
                auto epos1 = s0.find('.');
                if(epos1 != string::npos)
                {
                    string s2 = s0.substr(0, epos1);
                    s2 = normalnumber.Strip1(s2);
                    string s3 = s0.substr(epos1 + 1, s0.size());
                    if(s3.find('.') != string::npos || (s2.empty() && s3.empty()))
                    {
                        isscinum = false;
                    }
                    else
                    {
                        isscinum = normalnumber.Run(s2) || ((s2.empty() || s2.at(0) == '-') && !s3.empty());
                        isscinum = isscinum && (normalposnumber.Run(s3) || (s3.empty() && !s2.empty()));
                    }
                }
                
                isscinum = isscinum || normalnumber.Run(s0);
                isscinum = isscinum && normalnumber.Run(s1);
            }
        }

        if (isnormalnumber || isscinum || isreal)
        {
            
            return true;
        }
        return false;
    }
};



int main()
{
    /* Test Examples
    "-.3"
    ".3"
    "3."
    "-3."
    "."
    " ."
    ". "
    ".e1"
    ""
    */
    Solution s;
    string sstr = "4e+";
    bool isnum = s.isNumber(sstr);
    cout << sstr << "  isNum? " << isnum << endl;

    sstr = "-1.5045654754";
    isnum = s.isNumber(sstr);
    cout << sstr << "  isNum? " << isnum << endl;

    sstr = "10e-5045654754";
    isnum = s.isNumber(sstr);
    cout << sstr << "  isNum? " << isnum << endl;

    sstr = "10.0e-5045654754";
    isnum = s.isNumber(sstr);
    cout << sstr << "  isNum? " << isnum << endl;
    
    sstr = "-10.0e-5045654754";
    isnum = s.isNumber(sstr);
    cout << sstr << "  isNum? " << isnum << endl;

    sstr = "-10.0e-5045654754.0";
    isnum = s.isNumber(sstr);
    cout << sstr << "  isNum? " << isnum << endl;
    
    sstr = "-.3";
    isnum = s.isNumber(sstr);
    cout << sstr << "  isNum? " << isnum << endl;
    
    sstr = "-3.";
    isnum = s.isNumber(sstr);
    cout << sstr << "  isNum? " << isnum << endl;
    
    sstr = "-3.e1";
    isnum = s.isNumber(sstr);
    cout << sstr << "  isNum? " << isnum << endl;
    sstr = "-3.e1.0";
    isnum = s.isNumber(sstr);
    cout << sstr << "  isNum? " << isnum << endl;
    
    sstr = "-3.e-1";
    isnum = s.isNumber(sstr);
    cout << sstr << "  isNum? " << isnum << endl;
    
    sstr = "+3.e-1";
    isnum = s.isNumber(sstr);
    cout << sstr << "  isNum? " << isnum << endl;
    
    sstr = "+3.e+1";
    isnum = s.isNumber(sstr);
    cout << sstr << "  isNum? " << isnum << endl;
    sstr = "+3";
    isnum = s.isNumber(sstr);
    cout << sstr << "  isNum? " << isnum << endl;

    DFA a(1);
    string oris = " 123 ";
    oris = "   .";
    cout << "oris:_" << oris << "_stripped:_" << a.Strip(oris) << "_" << endl;
    getchar();
    return 0;
}


/*
"-.3"
".3"
"3."
"-3."
"."
" ."
". "
".e1"
".-4"
"-.4"
"+.8"
"+.8"
"+2"
"+2e+8"
"+2e-7"
"1 ."
*/