
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
        s = Strip(s);
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
            while (i < s.size() && j >= 0 && i < j)
            {
                bool hasblank = false;
                if (s.at(i) == ' ' && i + 1 < s.size())
                {
                    ++i;
                    hasblank = true;
                }

                if (s.at(j) == ' ' && j - 1 >= 0)
                {
                    --j;
                    hasblank = true;
                }

                if (!hasblank)
                {
                    break;;
                }
            }
            ret = s.substr(istart, iend - istart);
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
        static DFA normalnumber(20);
        static  DFA realnumber(6);
        static  DFA scinumber(10);
        if (!initialized)
        {
            //normal number dfa
            for (int i = 1; i <= 9; ++i)
            {
                normalnumber.AddTrans(0, i, '0' + i);
                normalnumber.MarkAccept(i);
            }
            
            for (int i = 1; i <= 19; ++i)
            {
                normalnumber.MarkAccept(i);
                for (int j = 10; j <= 19; ++j)
                {
                    normalnumber.AddTrans(i, j, '0' + j - 10);
                }
            }


            //real number dfa
            realnumber.AddTrans(0, 1, EnumChaType_Number);
            realnumber.AddTrans(1, 2, EnumChaType_Number);
            realnumber.AddTrans(2, 2, EnumChaType_Number);
            realnumber.AddTrans(3, 4, EnumChaType_Number);
            realnumber.AddTrans(4, 5, EnumChaType_Number);
            realnumber.AddTrans(5, 5, EnumChaType_Number);
            realnumber.AddTrans(1, 3, '.');
            realnumber.AddTrans(2, 3, '.');
            realnumber.MarkAccept(4);
            realnumber.MarkAccept(5);

            //sci number dfa
            scinumber.AddTrans(0, 1, EnumChaType_Number);
            scinumber.AddTrans(1, 2, EnumChaType_Number);
            scinumber.AddTrans(2, 2, EnumChaType_Number);
            scinumber.AddTrans(3, 4, EnumChaType_Number);
            scinumber.AddTrans(4, 5, EnumChaType_Number);
            scinumber.AddTrans(5, 5, EnumChaType_Number);
            scinumber.AddTrans(1, 3, 'e');
            scinumber.AddTrans(2, 3, 'e');
            scinumber.MarkAccept(4);
            scinumber.MarkAccept(5);

            initialized = true;
        }

        bool isnormalnumber = normalnumber.Run(s);
        bool isrealnumber = realnumber.Run(s);
        bool isscinumber = scinumber.Run(s);
        if (isnormalnumber || isrealnumber || isscinumber)
        {
            return true;
        }
        return false;
    }
};



int main()
{
    Solution s;
    string sstr = "15";
    bool isnum = s.isNumber(sstr);
    cout << sstr << "  isNum? " << isnum << endl;

    sstr = "1.5045654754";
    isnum = s.isNumber(sstr);
    cout << sstr << "  isNum? " << isnum << endl;

    sstr = "10e5045654754";
    isnum = s.isNumber(sstr);
    cout << sstr << "  isNum? " << isnum << endl;
    getchar();
    return 0;
}